#include "Operator.h"

Operator::Operator(
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

PD *Operator::kNN_Search(Point *q, HashTable *TableArray, int L, int k){

	PD *b = new PD[k];
	int i;
	for (i=0; i<L; i++){
		TableArray[i].knn_search_bucket(k, q, b);
	}

	return b;

}

Operator::~Operator(){
    for(auto point :points)
        delete point;

    for(int i=0; i<L; i++)
        delete hash_tables[i];
    delete[] hash_tables;
}