#include "SimpleList.h"
#include "Ghashing.h"

class Point; // Forward declaration
class Ghash;

#define HashItem Point*

class HashTable{
    public:
        HashTable(unsigned int given_table_size, int w, int k, int vecSize);
        int Insert(HashItem item);
        int Find(HashItem item);
        void Traverse( void (*fun)(HashItem *) );
        ~HashTable();
    private:
        SimpleList *table;
        unsigned int table_size;
        Ghash Ghashing;
};