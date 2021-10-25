#include "Operator.h"

Operator::Operator(
    char const *input_f, char const *query_f, char const *output_f,
    int w, int k, int hash_table_size
/*good practice https://stackoverflow.com/questions/4162021/is-it-ok-to-call-a-function-in-constructor-initializer-list*/
):io_files(input_f, query_f, output_f), ht(hash_table_size,w,k,io_files.getDimension()){

    // We start by reading the input
    Point* p = io_files.ReadPoint();
    while( p != nullptr){

        ht.Insert(p);

        p = io_files.ReadPoint();
    }

}

Operator::~Operator(){

}