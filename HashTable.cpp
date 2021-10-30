#include <stdio.h>
#include <iostream>
#include "HashTable.h"
#include "Point.h"

using namespace std;

HashTable::HashTable(unsigned int given_table_size, int w, int k, int vecSize)
:table_size(given_table_size), Ghashing(w,k,given_table_size,vecSize)
{
    this->table = new SimpleList[given_table_size];
    if(this->table == nullptr) perror("Not enough memory to create hashtable!");
}

int HashTable::Insert(HashItem item){
    int hash_index = Ghashing.hash(item->getXs());
    return table[hash_index].Push(item);
}

int HashTable::Find(HashItem item){
    int hash_index = Ghashing.hash(item->getXs());
    return table[hash_index].Find(item);
}

int HashTable::knn_search_bucket(int k, Point *q, struct PD* nearest){
    int hash_index = Ghashing.hash(q->getXs());
    return table[hash_index].knn_search(k,q, nearest);
}

int HashTable::bruteForceNN(int k, Point *q, struct PD* nearest){
    int res;

    for (unsigned int i=0; i<table_size; i++){
        res = table[i].knn_search(k,q, nearest);
    }
    return res;
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
