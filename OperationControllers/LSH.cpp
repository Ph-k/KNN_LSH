#include <chrono>
#include <unordered_map>

#include "LSH.h"

using namespace std;

LSH::LSH(
    FileReader &io_files_ref,
    int w, int k, int l,double delta, int hash_table_size, char metric
/*good practice https://stackoverflow.com/questions/4162021/is-it-ok-to-call-a-function-in-constructor-initializer-list*/
):MappingMethod(io_files_ref),L(l), metric(metric){

    this->hash_tables = new HashTable*[l];
    for(int i=0; i<l; i++){
        this->hash_tables[i] = new HashTable(hash_table_size,w,k,io_files.getDimension(), metric, delta);
    }

    // We start by reading the input
    TimeSeries* p = io_files.ReadPoint();
    while( p != nullptr){

    this->points.push_back(p);
    for(int i=0; i<l; i++){
        this->hash_tables[i]->Insert(p);
    }

        p = io_files.ReadPoint();
    }

}

int LSH::kNN_Search(int L, int k, PD **b, string &id){

    TimeSeries *q = io_files.getQuery(id);
    if( q == nullptr ) return -1;

    return kNN_Search(L,k,b,q);
}

int LSH::kNN_Search(int L, int k, PD **b, TimeSeries *q){

    int i;
	if(*b == nullptr ) *b = new PD[k];

    for (i=0; i<k; i++){
        (*b)[i].p = nullptr;
        (*b)[i].distance = 0;
    }

    chrono::steady_clock::time_point startTime = chrono::steady_clock::now();

    for (i=0; i<L; i++){
        hash_tables[i]->knn_search_bucket(k, q, *b);
    }

    return (chrono::duration_cast<chrono::__CRHONO_TIME_SCALE>( chrono::steady_clock::now() - startTime )).count();
}

int LSH::bruteForceNN(string &id, int L, int k, PD **b){
    TimeSeries *q = io_files.getQuery(id);
    if( q == nullptr ) return -1;

    return this->bruteForceNN(q,L,k,b);
}

int LSH::bruteForceNN(TimeSeries *q, int L, int k, PD **b){

    int i;
    if(*b == nullptr ) *b = new PD[k];

    for (i=0; i<k; i++){
        (*b)[i].p = nullptr;
        (*b)[i].distance = 0.0;
    }

    chrono::steady_clock::time_point startTime = chrono::steady_clock::now();

    hash_tables[0]->bruteForceNN(k, q, *b);

    return (chrono::duration_cast<chrono::__CRHONO_TIME_SCALE>( chrono::steady_clock::now() - startTime )).count();
}

int LSH::rangeSearch(int r, std::unordered_map<std::string, TimeSeries*> &r_neighbors, std::string &id){
    TimeSeries *q = io_files.getQuery(id);
    if( q == nullptr ) return -1;

    return rangeSearch(r,r_neighbors,q);
}

int LSH::rangeSearch(int r, std::unordered_map<std::string, TimeSeries*> &r_neighbors, TimeSeries *q){
    int i;
    chrono::steady_clock::time_point startTime = chrono::steady_clock::now();

    for (i=0; i<L; i++){
        hash_tables[i]->rangeSearchBucket(r, q, r_neighbors);
    }

    return (chrono::duration_cast<chrono::__CRHONO_TIME_SCALE>( chrono::steady_clock::now() - startTime )).count();
}

int LSH::reverseRangeSearch(int r, std::unordered_map<std::string, TimeSeries*> *Clusters, int k, int k_index, TimeSeries **Medoids){
    for (int i=0; i<L; i++){
        hash_tables[i]->reverseRangeSearchBucket(r, Clusters, k, k_index, Medoids);
    }
    return 0;
}

LSH::~LSH(){
    for(auto point :points)
        delete point;

    for(int i=0; i<L; i++)
        delete hash_tables[i];
    delete[] hash_tables;
}