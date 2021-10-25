#include "HashTable.h"
#include "FileReader.h"

class Operator{
    private:
        FileReader io_files;
        HashTable ht;
    public:
        Operator(
            char const *input_f, char const *query_f, char const *output_f,
            int w, int k, int hash_table_size=10
        );
        ~Operator();
};


