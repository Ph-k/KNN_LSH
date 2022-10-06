#include <chrono>
#include "HyperCube.h"
#include "Utilities.h"

using namespace std;

HyperCube::HyperCube(FileReader &io_files_ref, int w, int k, int M, int probes, int hash_table_size)
:MappingMethod(io_files_ref), hash_table(hash_table_size,150,k,io_files.getDimension(),__H_CUBE_MODE, probes), M(M) {

    // We start by reading the input
    TimeSeries* p = io_files.ReadPoint();
    while( p != nullptr){

        this->points.push_back(p);
        this->hash_table.Insert(p);

        p = io_files.ReadPoint();
    }

}

int HyperCube::kNN_Search(int L, int k, PD **b, std::string &id){

    TimeSeries *q = io_files.getQuery(id);
    if( q == nullptr ) return -1;

	return kNN_Search(L,k,b,q);
}

int HyperCube::kNN_Search(int L, int k, PD **b, TimeSeries* q){

	if(*b == nullptr ) *b = new PD[k];

    int i;
    for (i=0; i<k; i++){
        (*b)[i].p = nullptr;
        (*b)[i].distance = 0;
    }

    chrono::steady_clock::time_point startTime = chrono::steady_clock::now();
    hash_table.knn_search_bucket(k, q, *b, M);

    return (chrono::duration_cast<chrono::__CRHONO_TIME_SCALE>( chrono::steady_clock::now() - startTime )).count();
}

int HyperCube::bruteForceNN(string &id, int L, int k, PD **b){
    TimeSeries *q = io_files.getQuery(id);
    if( q == nullptr ) return -1;

    return bruteForceNN(q,L,k,b);
}

int HyperCube::bruteForceNN(TimeSeries *q, int L, int k, PD **b){    

    if(*b == nullptr ) *b = new PD[k];

    int i;
    for (i=0; i<k; i++){
        (*b)[i].p = nullptr;
        (*b)[i].distance = 0.0;
    }

    chrono::steady_clock::time_point startTime = chrono::steady_clock::now();

    hash_table.bruteForceNN(k, q, *b);

    return (chrono::duration_cast<chrono::__CRHONO_TIME_SCALE>( chrono::steady_clock::now() - startTime )).count();
}

int HyperCube::rangeSearch(int r, std::unordered_map<std::string, TimeSeries*> &r_neighbors, std::string &id){
    TimeSeries *q = io_files.getQuery(id);
    if( q == nullptr ) return -1;

    return rangeSearch(r,r_neighbors,q);
}


int HyperCube::rangeSearch(int r, std::unordered_map<std::string, TimeSeries*> &r_neighbors, TimeSeries *q){

    chrono::steady_clock::time_point startTime = chrono::steady_clock::now();

    hash_table.rangeSearchBucket(r, q, r_neighbors, M);

    return (chrono::duration_cast<chrono::__CRHONO_TIME_SCALE>( chrono::steady_clock::now() - startTime )).count();
}

int HyperCube::reverseRangeSearch(int r, std::unordered_map<std::string, TimeSeries*> *Clusters, int k, int k_index, TimeSeries **Medoids, int M){
    hash_table.reverseRangeSearchBucket(r, Clusters, k, k_index, Medoids, M);
    return 0;
}

HyperCube::~HyperCube(){
    for(auto point: points)
        delete point;
}