#include "HashTable.h"
#include "Utilities.h"
#include "HashLSH.h"
#include "HashHC.h"
#include "Point.h"
#include <stdio.h>
#include <iostream>

using namespace std;


HashTable::HashTable(unsigned int given_table_size, int w, int k, int vecSize, char type)
:table_size(given_table_size)
{
    switch (type){
    case 'l':
        hashing = new HashLSH(w,k,given_table_size,vecSize);
        break;
    default:
        hashing = new HashHC(w,k,given_table_size,vecSize);
        break;
    }

    this->bucket = new SimpleList[given_table_size];
    if(this->bucket == nullptr) perror("Not enough memory to create hashtable!");
}

int HashTable::Insert(HashItem item){
    int Id_p = hashing->Hash(item->getXs());
    struct PointPointer pp = { item, Id_p };
    
    int hash_index = hashing->HashIndex(pp.Id);

    return bucket[hash_index].Push(pp);
}

/*int HashTable::Find(HashItem item){
    int hash_index = Ghashing.Hash(item);
    return table[hash_index].Find(item);
}*/

int HashTable::knn_search_bucket(int k, Point *q, struct PD* nearest){
    int Id_q = hashing->Hash(q->getXs());
    int hash_index = hashing->HashIndex( Id_q );
    return bucket[hash_index].knn_search(k, q, Id_q, nearest);
}

int HashTable::bruteForceNN(int k, Point *q, struct PD* nearest){
    int res;

    for (unsigned int i=0; i<table_size; i++){
        res = bucket[i].knn_search(k, q, 0, nearest, true);
    }
    return res;
}

int HashTable::rangeSearchBucket(int r, Point *q, std::unordered_map<string, Point*> &r_neighbors){
    r_neighbors.empty();
    int Id_q = hashing->Hash(q->getXs());
    int hash_index = hashing->HashIndex( Id_q );
    return bucket[hash_index].rangeSearch(r, q, r_neighbors);
}

/*void HashTable::Traverse( void (*fun)(HashItem *) ){
    for(unsigned int i=0; i<table_size; i++){
        table[i].Traverse(fun);
        std::cout << std::endl;
    }
}*/

HashTable::~HashTable(){
    delete[] bucket;
    delete hashing;
}
