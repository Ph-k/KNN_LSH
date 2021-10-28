#include <chrono>

#include "LSH.h"

using namespace std;

LSH::LSH(
    char const *input_f, char const *query_f, char const *output_f,
    int w, int k, int l, int hash_table_size
/*good practice https://stackoverflow.com/questions/4162021/is-it-ok-to-call-a-function-in-constructor-initializer-list*/
):io_files(input_f, query_f, output_f),L(l){

    this->hash_tables = new HashTable*[l];
    for(int i=0; i<l; i++){
        this->hash_tables[i] = new HashTable(hash_table_size,w,k,io_files.getDimension());
    }

    // We start by reading the input
    Point* p = io_files.ReadPoint();
    while( p != nullptr){

        this->points.push_back(p);
        for(int i=0; i<l; i++){
            this->hash_tables[i]->Insert(p);
        }

        p = io_files.ReadPoint();
    }

}

PD *LSH::kNN_Search(string &id, int L, int k){

    Point *q = io_files.getQuery(id);
    if( q == nullptr ) return nullptr;

    int i;
	PD *b = new PD[k];

    for (i=0; i<k; i++){
        b[i].p = nullptr;
        b[i].distance = 0;
    }

    chrono::steady_clock::time_point startTime = chrono::steady_clock::now();

    for (i=0; i<L; i++){
        hash_tables[0]->knn_search_bucket(k, q, b);
    }

    cout << (chrono::duration_cast<chrono::milliseconds>( chrono::steady_clock::now() - startTime )).count() << "microseconds" << endl;
	return b;
}

LSH::~LSH(){
    for(auto point :points)
        delete point;

    for(int i=0; i<L; i++)
        delete hash_tables[i];
    delete[] hash_tables;
}