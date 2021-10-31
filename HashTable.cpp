#include "HashTable.h"
#include "Utilities.h"
#include "Point.h"
#include <stdio.h>
#include <iostream>

using namespace std;


HashTable::HashTable(unsigned int given_table_size, int w, int k, int vecSize)
:table_size(given_table_size), Ghashing(w,k,given_table_size,vecSize)
{
    this->bucket = new SimpleList[given_table_size];
    if(this->bucket == nullptr) perror("Not enough memory to create hashtable!");
}

int HashTable::Insert(HashItem item){
    int Id_p = Ghashing.HashID(item->getXs());
    struct PointPointer pp = { item, Id_p };
    
    int hash_index = Ghashing.Hash(pp.Id);

    return bucket[hash_index].Push(pp);
}

/*int HashTable::Find(HashItem item){
    int hash_index = Ghashing.Hash(item);
    return table[hash_index].Find(item);
}*/

int HashTable::knn_search_bucket(int k, Point *q, struct PD* nearest){
    int Id_q = Ghashing.HashID(q->getXs());
    int hash_index = Ghashing.Hash( Id_q );
    return bucket[hash_index].knn_search(k, q, Id_q, nearest);
}

int HashTable::bruteForceNN(int k, Point *q, struct PD* nearest){
    int res;

    for (unsigned int i=0; i<table_size; i++){
        res = bucket[i].knn_search(k, q, 0, nearest, true);
    }
    return res;
}

int HashTable::rangeSearchBucket(int r, Point *q, void (*outputFunction)(Point *, void* privateItem), void* outputFunctionItem){
    int Id_q = Ghashing.HashID(q->getXs());
    int hash_index = Ghashing.Hash( Id_q );
    return bucket[hash_index].rangeSearch(r, q, outputFunction, outputFunctionItem);
}

/*void HashTable::Traverse( void (*fun)(HashItem *) ){
    for(unsigned int i=0; i<table_size; i++){
        table[i].Traverse(fun);
        std::cout << std::endl;
    }
}*/

HashTable::~HashTable(){
    delete[] bucket;
}
