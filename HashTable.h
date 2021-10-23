#include "SimpleList.h"

#define HashItem int

class HashTable{
    public:
        HashTable(unsigned int given_table_size, int (*GivenHashFunc)(HashItem item) );
        int Insert(HashItem item);
        HashItem Find(HashItem item);
        void Traverse( void (*fun)(HashItem *) );
        ~HashTable();
    private:
        int (*HashFunc)(HashItem item);
        SimpleList *table;
        unsigned int table_size;
};