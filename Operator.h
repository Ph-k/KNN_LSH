#include "HashTable.h"
#include "FileReader.h"
#include "Utilities.h"

#include <vector>

class Point;

class Operator{
    private:
        FileReader io_files;
        HashTable **hash_tables;
        std::vector<Point*> points;
        int L;
    public:
        Operator(
            char const *input_f, char const *query_f, char const *output_f,
            int w, int k, int l, int hash_table_size=10
        );
        PD *kNN_Search(Point *q, HashTable *TableArray, int L, int k);
        ~Operator();
};


