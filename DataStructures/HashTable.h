#pragma once

#include "SimpleList.h"
#include "TimeSeries.h"
#include "HashInterface.hpp"

#include <vector>
#include <unordered_map>

// Forward declaration
class TimeSeries;
class Ghash;

#define __L2_DIST 0
#define __DFR_DIST 1
#define HashItem TimeSeries*

class HashTable{
    public:
        HashTable(
            unsigned int given_table_size,
            int w,
            int k,
            int vecSize,
            char type,
            int probes=1
        );
        int Insert(HashItem item);
        int InsertQ(HashItem item, std::vector<__TIMESERIES_X_TYPE> &q_vec);
        int knn_search_bucket(int k, TimeSeries *q, struct PD* nearest, int M = -1);
        int bruteForceNN(int k, TimeSeries *q, struct PD* nearest);
        int rangeSearchBucket(int r, TimeSeries *q, std::unordered_map<std::string, TimeSeries*> &r_neighbors, int M = -1);

        // RangeSearchBucket for clustering
        int reverseRangeSearchBucket(int r, std::unordered_map<std::string, TimeSeries*> *Clusters, int k, int k_index, TimeSeries **Medoids, int M = -1);
        ~HashTable();
    private:
        SimpleList *bucket;
        unsigned int table_size;
        HashInterface *hashing;
};