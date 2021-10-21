#include "SimpleList.h"

#include <iostream>

struct ListNode{
    ItemType item;
    struct ListNode* next;
};

SimpleList::SimpleList(): head(nullptr){}

int SimpleList::Push(ItemType item){
    std::cout << "A list created!" << std::endl;
    ListNode* newNode = new ListNode;
    if( newNode == nullptr ) return -1;

    newNode->next=head;
    newNode->item=item;
    head = newNode;
    return 0;
}

int SimpleList::Pop(ItemType &item){
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

int SimpleList::Find(ItemType item){
    ListNode* node = head;
    while(node != nullptr){
        if(node->item == item)
            return 1;

        node = node->next;
    }
    return 0;
}

void SimpleList::Traverse( void (*fun)(ItemType *) ){
    ListNode* node = head;
    while(node != nullptr){
        fun( &(node->item) );
        node = node->next;
    }
}

SimpleList::~SimpleList(){
    ListNode* temp;
    while(head != nullptr){
        temp = head;
        head = head->next;
        delete temp;
    }
}