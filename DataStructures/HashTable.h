#pragma once

#include "SimpleList.h"
#include "HashInterface.hpp"

#include <vector>
#include <unordered_map>

class Point; // Forward declaration
class Ghash;

#define HashItem Point*

class HashTable{
    public:
        HashTable(unsigned int given_table_size, int w, int k, int vecSize, char type, int probes=1);
        int Insert(HashItem item);
        //int Find(HashItem item);
        //void Traverse( void (*fun)(HashItem *) );
        int knn_search_bucket(int k, Point *q, struct PD* nearest);
        int bruteForceNN(int k, Point *q, struct PD* nearest);
        int rangeSearchBucket(int r, Point *q, std::unordered_map<std::string, Point*> &r_neighbors);
        int reverseRangeSearchBucket(int r, std::unordered_map<std::string, Point*> *Clusters, int k, int k_index, Point **Medoids);
        ~HashTable();
    private:
        SimpleList *bucket;
        unsigned int table_size;
        HashInterface *hashing;
};