#include <chrono>
#include "HyperCube.h"
#include "Utilities.h"

using namespace std;

HyperCube::HyperCube(FileReader &io_files_ref, int w, int k, int probes, int hash_table_size)
:io_files(io_files_ref), hash_table(hash_table_size,150,k,io_files.getDimension(),__H_CUBE_MODE, probes) {

    // We start by reading the input
    Point* p = io_files.ReadPoint();
    while( p != nullptr){

        this->points.push_back(p);
        this->hash_table.Insert(p);

        p = io_files.ReadPoint();
    }

}

int HyperCube::kNN_Search(string &id, int L, int k, PD **b){

    Point *q = io_files.getQuery(id);
    if( q == nullptr ) return -1;

	if(*b == nullptr ) *b = new PD[k];

    int i;
    for (i=0; i<k; i++){
        (*b)[i].p = nullptr;
        (*b)[i].distance = 0;
    }

    chrono::steady_clock::time_point startTime = chrono::steady_clock::now();
    hash_table.knn_search_bucket(k, q, *b);

    return (chrono::duration_cast<chrono::milliseconds>( chrono::steady_clock::now() - startTime )).count();
}

int HyperCube::bruteForceNN(string &id, int L, int k, PD **b){

    Point *q = io_files.getQuery(id);
    if( q == nullptr ) return -1;

    if(*b == nullptr ) *b = new PD[k];

    int i;
    for (i=0; i<k; i++){
        (*b)[i].p = nullptr;
        (*b)[i].distance = 0.0;
    }

    chrono::steady_clock::time_point startTime = chrono::steady_clock::now();

    hash_table.bruteForceNN(k, q, *b);

    return (chrono::duration_cast<chrono::milliseconds>( chrono::steady_clock::now() - startTime )).count();
}

int HyperCube::rangeSearch(string &id, int r, unordered_map<string, Point*> &r_neighbors){
    Point *q = io_files.getQuery(id);
    if( q == nullptr ) return -1;

    chrono::steady_clock::time_point startTime = chrono::steady_clock::now();

    hash_table.rangeSearchBucket(r, q, r_neighbors);

    return (chrono::duration_cast<chrono::milliseconds>( chrono::steady_clock::now() - startTime )).count();
}

int HyperCube::reverseRangeSearch(int r, std::unordered_map<std::string, Point*> *Clusters, int k, int k_index, Point **Medoids){
    hash_table.reverseRangeSearchBucket(r, Clusters, k, k_index, Medoids);
    return 0;
}

HyperCube::~HyperCube(){}