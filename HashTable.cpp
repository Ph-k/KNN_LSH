#include <stdio.h>
#include <iostream>
#include "HashTable.h"

HashTable::HashTable(unsigned int given_table_size, int (*GivenHashFunc)(int num))
:HashFunc(GivenHashFunc), table_size(given_table_size)
{
    this->table = new SimpleList[given_table_size];
    if(this->table == nullptr) perror("Not enough memory to create hashtable!");
}

int HashTable::Insert(HashItem item){
    return table[HashFunc(item)%table_size].Push(item);
}

HashItem HashTable::Find(HashItem item){
    return table[HashFunc(item)%table_size].Find(item);
}

void HashTable::Traverse( void (*fun)(ItemType *) ){
    for(int i=0; i<table_size; i++){
        table[i].Traverse(fun);
        std::cout << std::endl;
    }
}

HashTable::~HashTable(){
    delete[] table;
}