#include "HashTable.h"
#include "Utilities.h"
#include "HashLSH.h"
#include "HashHC.h"
#include <stdio.h>
#include <iostream>
#include "./FredLib/FredLibWrapper.h"

using namespace std;


HashTable::HashTable(unsigned int given_table_size, int w, int k, int vecSize, char type, double delta)
:table_size(given_table_size)
{
    if( type==__LSH_MODE || type==__FRECHET_DISCRETE_MODE || type==__FRECHET_CONTINUOUS_MODE){
        hashing = new HashLSH(w,k,given_table_size,vecSize,type,delta);
    }else if ( type == __H_CUBE_MODE) {
        cerr << "wrong constructor for requested method in hashtable!" << endl; exit(2);
    }else {cerr << "no mode specified to hashtable!" << endl; exit(2);};

    this->bucket = new SimpleList[given_table_size];
    if(this->bucket == nullptr) cerr << "Not enough memory to create hashtable!" << endl;

    if( type == __FRECHET_DISCRETE_MODE )
        for(unsigned int i=0; i<given_table_size; i++)
            (this->bucket[i]).alterDistanceMetric(&dfr_distance);
    else if(type == __FRECHET_CONTINUOUS_MODE){
        for(unsigned int i=0; i<given_table_size; i++)
            (this->bucket[i]).alterDistanceMetric(&cfr_distance);
    }

}

HashTable::HashTable(unsigned int given_table_size, int w, int k, int vecSize, char type, int probes)
:table_size(given_table_size)
{
    if( type==__LSH_MODE || type==__FRECHET_DISCRETE_MODE || type==__FRECHET_CONTINUOUS_MODE){
        cerr << "wrong constructor for requested method in hashtable!" << endl; exit(2);
    }else if ( type == __H_CUBE_MODE) {
        hashing = new HashHC(w,k,given_table_size,vecSize,probes);
    }else {cerr << "no mode specified to hashtable!" << endl; exit(2);};

    this->bucket = new SimpleList[given_table_size];
    if(this->bucket == nullptr) cerr << "Not enough memory to create hashtable!" << endl;

}

int HashTable::Insert(HashItem item){
    int Id_p = hashing->Hash(item->getXs());
    struct PointPointer pp = { item, Id_p };

    int *hash_indexes = hashing->HashIndex(pp.Id,true);
    if( bucket[hash_indexes[0]].Push(pp) != 0) return -1;
    delete[] hash_indexes;
    return 0;
}

int HashTable::knn_search_bucket(int k, TimeSeries *q, struct PD* nearest, int M){
    int Id_q = hashing->Hash(q->getXs());
    int *hash_indexes = hashing->HashIndex( Id_q );
    int probes = hashing->getNumProbes();
    for (int i = 0; i < probes; i++)
        if( bucket[hash_indexes[i]].knn_search(k, q, Id_q, nearest, M) != 0) return -1;

    delete[] hash_indexes;
    return 0;
}

int HashTable::bruteForceNN(int k, TimeSeries *q, struct PD* nearest){
    int res;

    for (unsigned int i=0; i<table_size; i++){
        res = bucket[i].knn_search(k, q, 0, nearest, -1, true);
    }
    return res;
}

int HashTable::rangeSearchBucket(int r, TimeSeries *q, unordered_map<string, TimeSeries*> &r_neighbors, int M){
    r_neighbors.empty();
    int Id_q = hashing->Hash(q->getXs());
    int *hash_indexes = hashing->HashIndex( Id_q );
    int probes = hashing->getNumProbes();
    for (int i = 0; i < probes; i++)
        if( bucket[hash_indexes[i]].rangeSearch(r, q, r_neighbors, M) != 0) return -1;

    delete[] hash_indexes;
    return 0;
}

int HashTable::reverseRangeSearchBucket(int r, std::unordered_map<std::string, TimeSeries*> *Clusters, int k, int k_index, TimeSeries **Medoids, int M){
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
