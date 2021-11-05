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
        HashTable(unsigned int given_table_size, int w, int k, int vecSize, char type);
        int Insert(HashItem item);
        //int Find(HashItem item);
        //void Traverse( void (*fun)(HashItem *) );
        int knn_search_bucket(int k, Point *q, struct PD* nearest);
        int bruteForceNN(int k, Point *q, struct PD* nearest);
        int rangeSearchBucket(int r, Point *q, std::unordered_map<std::string, Point*> &r_neighbors);
        ~HashTable();
    private:
        SimpleList *bucket;
        unsigned int table_size;
        HashInterface *hashing;
};