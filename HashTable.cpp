#include <stdio.h>
#include <iostream>
#include "HashTable.h"
#include "Point.h"


HashTable::HashTable(unsigned int given_table_size, int w, int k, int vecSize)
:table_size(given_table_size), Ghashing(w,k,given_table_size,vecSize)
{
    this->table = new SimpleList[given_table_size];
    if(this->table == nullptr) perror("Not enough memory to create hashtable!");
}

int HashTable::Insert(HashItem item){
    return table[Ghashing.hash(item->getXs())].Push(item);
}

int HashTable::Find(HashItem item){
    return table[Ghashing.hash(item->getXs())].Find(item);
}

void HashTable::Traverse( void (*fun)(HashItem *) ){
    for(unsigned int i=0; i<table_size; i++){
        table[i].Traverse(fun);
        std::cout << std::endl;
    }
}

HashTable::~HashTable(){
    delete[] table;
}