#include "SimpleList.h"
#include "Ghashing.h"

#include <vector>

class Point; // Forward declaration
class Ghash;

#define HashItem Point*

class HashTable{
    public:
        HashTable(unsigned int given_table_size, int w, int k, int vecSize);
        int Insert(HashItem item);
        //int Find(HashItem item);
        //void Traverse( void (*fun)(HashItem *) );
        int knn_search_bucket(int k, Point *q, struct PD* nearest);
        int bruteForceNN(int k, Point *q, struct PD* nearest);
        int rangeSearchBucket(int L, Point *q, void (*outputFunction)(Point *, void* privateItem), void* outputFunctionItem);
        ~HashTable();
    private:
        SimpleList *bucket;
        unsigned int table_size;
        Ghash Ghashing;
};