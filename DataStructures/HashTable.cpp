#include "HashTable.h"
#include "Utilities.h"
#include "HashLSH.h"
#include "HashHC.h"
#include "Point.h"
#include <stdio.h>
#include <iostream>

using namespace std;


HashTable::HashTable(unsigned int given_table_size, int w, int k, int vecSize, char type, int probes)
:table_size(given_table_size)
{
    switch (type){
    case __LSH_MODE:
        hashing = new HashLSH(w,k,given_table_size,vecSize);
        break;
    default:
        hashing = new HashHC(w,k,given_table_size,vecSize,probes);
        break;
    }

    this->bucket = new SimpleList[given_table_size];
    if(this->bucket == nullptr) perror("Not enough memory to create hashtable!");
}

int HashTable::Insert(HashItem item){
    int Id_p = hashing->Hash(item->getXs());
    struct PointPointer pp = { item, Id_p };

    int *hash_indexes = hashing->HashIndex(pp.Id,true);
    if( bucket[hash_indexes[0]].Push(pp) != 0) return -1;
    delete[] hash_indexes;
    return 0;
}

/*int HashTable::Find(HashItem item){
    int hash_index = Ghashing.Hash(item);
    return table[hash_index].Find(item);
}*/

int HashTable::knn_search_bucket(int k, Point *q, struct PD* nearest, int M){
    int Id_q = hashing->Hash(q->getXs());
    int *hash_indexes = hashing->HashIndex( Id_q );
    int probes = hashing->getNumProbes();
    for (int i = 0; i < probes; i++)
        if( bucket[hash_indexes[i]].knn_search(k, q, Id_q, nearest, M) != 0) return -1;

    delete[] hash_indexes;
    return 0;
}

int HashTable::bruteForceNN(int k, Point *q, struct PD* nearest){
    int res;

    for (unsigned int i=0; i<table_size; i++){
        res = bucket[i].knn_search(k, q, 0, nearest, -1, true);
    }
    return res;
}

int HashTable::rangeSearchBucket(int r, Point *q, std::unordered_map<string, Point*> &r_neighbors, int M){
    r_neighbors.empty();
    int Id_q = hashing->Hash(q->getXs());
    int *hash_indexes = hashing->HashIndex( Id_q );
    int probes = hashing->getNumProbes();
    for (int i = 0; i < probes; i++)
        if( bucket[hash_indexes[i]].rangeSearch(r, q, r_neighbors, M) != 0) return -1;

    delete[] hash_indexes;
    return 0;
}

int HashTable::reverseRangeSearchBucket(int r, std::unordered_map<std::string, Point*> *Clusters, int k, int k_index, Point **Medoids, int M){
    int Id_q = hashing->Hash(Medoids[k_index]->getXs());
    int *hash_indexes = hashing->HashIndex( Id_q );
    int probes = hashing->getNumProbes();
    for (int i = 0; i < probes; i++)
        if( bucket[hash_indexes[i]].reverseRangeSearch(r, Clusters, k, k_index, Medoids, M) != 0 ) return -1;;

    delete[] hash_indexes;
    return 0;
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
