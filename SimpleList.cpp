#include "SimpleList.h"

#include <iostream>
#include "Point.h"
#include "Utilities.h"

using namespace std;

struct ListNode{
    SimpleListItemType item;
    struct ListNode* next;
};

SimpleList::SimpleList(): head(nullptr){}

int SimpleList::Push(SimpleListItemType item){
    //std::cout << "A list created!" << std::endl;
    ListNode* newNode = new ListNode;
    if( newNode == nullptr ) return -1;

    newNode->next=head;
    newNode->item=item;
    head = newNode;
    return 0;
}

int SimpleList::Pop(SimpleListItemType &item){
    if( head == nullptr ) return 1;

    item = head->item;

    ListNode* oldHead = head;
    head = head->next;
    delete oldHead;
    return 0;
}

int SimpleList::Pop(){
    if( head == nullptr ) return 1;

    ListNode* oldHead = head;
    head = head->next;
    delete oldHead;
    return 0;
}

int SimpleList::Find(SimpleListItemType item){
    ListNode* node = head;
    while(node != nullptr){
        if(node->item == item)
            return 1;

        node = node->next;
    }
    return 0;
}

void SimpleList::Traverse( void (*fun)(SimpleListItemType *) ){
    ListNode* node = head;
    while(node != nullptr){
        fun( &(node->item) );
        node = node->next;
    }
}

int SimpleList::knn_search(int k, Point *q, struct PD* nearest){
    ListNode* node = head;
    float l1;
    int i,j;

    while(node != nullptr){
        l1 = euclidean_distance(node->item,q);

        for(i=0; i<k; i++){
            if( nearest[i].p == nullptr ){
                nearest[i].p = node->item;
                nearest[i].distance = l1;
                break;
            }else if(nearest[i].p == node->item){
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
                nearest[i].p = node->item;
                nearest[i].distance = l1;
                break;
            }
        }

        node = node->next;
    }

    return 0;
}

SimpleList::~SimpleList(){
    ListNode* temp;
    while(head != nullptr){
        temp = head;
        head = head->next;
        //delete temp->item; for this program, items are points, which are deleted from the LSH class
        delete temp;
    }
}