#include "SimpleList.h"
#include "Utilities.h"
#include "HashLSH.h"

#include <string.h>
#include <iostream>
#include <unordered_map>

#include "Point.h"

using namespace std;

struct ListNode{
    SimpleListItemType item;
    struct ListNode* next;
};

SimpleList::SimpleList(): head(nullptr), T(0){}

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

/*void SimpleList::Traverse( void (*fun)(Point *p, void* privateItems ) ){
    ListNode* node = head;
    while(node != nullptr){
        fun( &(node->item) );
        node = node->next;
    }
}*/

int SimpleList::knn_search(int k, Point *q, int Id_q, struct PD* nearest, bool brute_force){
    ListNode* node = head;
    double l1;
    int i,j;

    while(node != nullptr){
        if(brute_force || Id_q == node->item.Id){
            l1 = euclidean_distance(node->item.point,q);

            for(i=0; i<k; i++){
                if( nearest[i].p == nullptr ){
                    nearest[i].p = node->item.point;
                    nearest[i].distance = l1;
                    break;
                }else if(nearest[i].p == node->item.point){
                    break;
                }else if(nearest[i].distance > l1){
                    //Shifting arrey
                    for(j=k-1; j>i; j--){
                        nearest[j].p = nearest[j-1].p;
                        nearest[j].distance = nearest[j-1].distance;
                    }
                    //node->item->print();
                    /*cout << nearest[i].p << " vs " << node->item << ' ' << nearest[i].distance << "vs" << l1 << ' ' << endl;
                    nearest[i].p->print(); node->item->print();*/
                    nearest[i].p = node->item.point;
                    nearest[i].distance = l1;
                    break;
                }
            }
        }

        node = node->next;
    }

    return 0;
}

int SimpleList::rangeSearch(int r, Point *q, unordered_map<string, Point*> &r_neighbors){
    ListNode* node = head;
    double l1;

    while(node != nullptr){
        l1 = euclidean_distance(node->item.point,q);

        if(l1 < r && r_neighbors.find(node->item.point->getId()) == r_neighbors.end() )
            r_neighbors[node->item.point->getId()]=node->item.point;

        node = node->next;
    }

    return 0;
}

int SimpleList::reverseRangeSearch(int r, std::unordered_map<std::string, Point*> *Clusters, int k, int k_index, Point **Medoids){
    ListNode* node = head;
    int i;
    bool add;
    double l1,l1_t;

    while(node != nullptr){
        l1 = euclidean_distance(node->item.point,Medoids[k_index]);

        if(l1 < r){
            add = true;
            for(i=0; i<k; i++){
                if(i != k_index){
                    if( Clusters[i].find(node->item.point->getId()) != Clusters[i].end() ){
                        l1_t = euclidean_distance(node->item.point,Medoids[i]);

                        if(l1_t > l1){
                            Clusters[i].erase(node->item.point->getId());
                        }else add = false;
                    }
                }
            }
            if(add) Clusters[k_index][node->item.point->getId()] = node->item.point;
        }

        node = node->next;
    }

    return 0;
}

Point* SimpleList::meanVector(){
    ListNode* node = head;

    int tempVecSize = node->item.point->getXs().size();
    double *tempVec = new double[tempVecSize];

    memset(tempVec,0.0,tempVecSize*sizeof(double));

    const std::vector<int>* vec;
    while(node != nullptr){
        vec = &(node->item.point->getXs());
        for(int i=0; i<tempVecSize; i++)
            tempVec[i] +=  ( (double)vec->at(i) ) / ((double)T);
        node = node->next;
    }

    std::vector<int> *meanVec = new std::vector<int>;
    for(int i=0; i<tempVecSize; i++)
        meanVec->push_back((int)tempVec[i]);
    std::string *no_s = nullptr;
    Point *meanP = new Point(meanVec, no_s);

    delete[] tempVec;
    return meanP;
}

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