#include "SimpleList.h"
#include "Utilities.h"
#include "HashLSH.h"

#include <string.h>
#include <iostream>
#include <unordered_map>

#include "TimeSeries.h"

using namespace std;

struct ListNode{
    SimpleListItemType item;
    struct ListNode* next;
};

SimpleList::SimpleList(): head(nullptr), T(0), metric_func(&euclidean_distance){}

int SimpleList::Push(SimpleListItemType item){
    //std::cout << "Listed got a push!" << std::endl;
    ListNode* newNode = new ListNode;
    if( newNode == nullptr ) return -1;

    newNode->next=head;
    newNode->item=item;
    head = newNode;
    T++;
    return 0;
}

int SimpleList::Pop(SimpleListItemType &item){
    if( head == nullptr ) return 1;

    item = head->item;

    ListNode* oldHead = head;
    head = head->next;
    delete oldHead;
    T--;
    return 0;
}

int SimpleList::Pop(){
    if( head == nullptr ) return 1;

    ListNode* oldHead = head;
    head = head->next;
    delete oldHead;
    T--;
    return 0;
}

int SimpleList::Find(SimpleListItemType item){
    ListNode* node = head;
    while(node != nullptr){
        if(node->item.point == item.point)
            return 1;

        node = node->next;
    }
    return 0;
}

void SimpleList::Traverse( void (*fun)(SimpleListItemType *item, void* privateItems ), void* privateItems ){
    ListNode* node = head;
    while(node != nullptr){
        fun( &(node->item), privateItems );
        node = node->next;
    }
}

int SimpleList::knn_search(int k, TimeSeries *q, int Id_q, struct PD* nearest, int M, bool brute_force){
    ListNode* node = head;
    double Lx;
    int i,j,M_i=0;

    while(node != nullptr && ( M==-1 || M_i<M ) ){
        if(brute_force || Id_q == node->item.Id){
            Lx = (*metric_func)(node->item.point,q);

            for(i=0; i<k; i++){
                if( nearest[i].p == nullptr ){
                    nearest[i].p = node->item.point;
                    nearest[i].distance = Lx;
                    break;
                }else if(nearest[i].p == node->item.point){
                    break;
                }else if(nearest[i].distance > Lx){
                    //Shifting arrey
                    for(j=k-1; j>i; j--){
                        nearest[j].p = nearest[j-1].p;
                        nearest[j].distance = nearest[j-1].distance;
                    }
                    //node->item->print();
                    /*cout << nearest[i].p << " vs " << node->item << ' ' << nearest[i].distance << "vs" << l1 << ' ' << endl;
                    nearest[i].p->print(); node->item->print();*/
                    nearest[i].p = node->item.point;
                    nearest[i].distance = Lx;
                    break;
                }
            }
        }

        node = node->next;
        M_i++;
    }

    return 0;
}

int SimpleList::rangeSearch(int r, TimeSeries *q, unordered_map<string, TimeSeries*> &r_neighbors, int M){
    ListNode* node = head;
    int i = 0;
    double Lx;

    while(node != nullptr && ( M==-1 || i<M ) ){
        Lx = (*metric_func)(node->item.point,q);

        if(Lx < r && r_neighbors.find(node->item.point->getId()) == r_neighbors.end() )
            r_neighbors[node->item.point->getId()]=node->item.point;

        node = node->next;
        i++;
    }

    return 0;
}

int SimpleList::reverseRangeSearch(int r, std::unordered_map<std::string, TimeSeries*> *Clusters, int k, int k_index, TimeSeries **Medoids, int M){
    ListNode* node = head;
    int i=0,j;
    bool add;
    double Lx,Lx_t;

    while(node != nullptr && ( M==-1 || i<M ) ){
        Lx = (*metric_func)(node->item.point,Medoids[k_index]);

        if(Lx < r){
            add = true;
            for(j=0; j<k; j++){
                if(j != k_index){
                    if( Clusters[j].find(node->item.point->getId()) != Clusters[j].end() ){
                        Lx_t = (*metric_func)(node->item.point,Medoids[j]);

                        if(Lx_t > Lx){
                            Clusters[j].erase(node->item.point->getId());
                        }else add = false;
                    }
                }
            }
            if(add){
                Clusters[k_index][node->item.point->getId()] = node->item.point;
            }
        }

        node = node->next;
        i++;
    }

    return 0;
}

/*TimeSeries* SimpleList::meanVector(){
    ListNode* node = head;

    int tempVecSize = node->item.point->getXs().size();
    double *tempVec = new double[tempVecSize];

    memset(tempVec,0.0,tempVecSize*sizeof(double));

    const std::vector<__TIMESERIES_X_TYPE>* vec;
    while(node != nullptr){
        vec = &(node->item.point->getXs());
        for(int i=0; i<tempVecSize; i++)
            tempVec[i] +=  ( (__TIMESERIES_X_TYPE)vec->at(i) ) / ((double)T);
        node = node->next;
    }

    std::vector<__TIMESERIES_X_TYPE> *meanVec = new std::vector<__TIMESERIES_X_TYPE>;
    for(int i=0; i<tempVecSize; i++)
        meanVec->push_back((__TIMESERIES_X_TYPE)tempVec[i]);
    std::string *no_s = nullptr;
    TimeSeries *meanP = new TimeSeries(meanVec, no_s);

    delete[] tempVec;
    return meanP;
}*/

void SimpleList::empty(){
    ListNode* temp;
    while(head != nullptr){
        temp = head;
        head = head->next;
        //delete temp->item; for this program, items are points, which are deleted from the LSH class
        delete temp;
    }

    head = nullptr;
    T = 0;
}

SimpleList::~SimpleList(){
    this->empty();
}

double SimpleList::averageDistance(TimeSeries *item){
    ListNode* node = head;

    double tempDist = 0.0;
    while(node != nullptr){
        tempDist += (*metric_func)(node->item.point, item);
        node = node->next;
    }
    tempDist = tempDist/(double)T;
    return tempDist;
}