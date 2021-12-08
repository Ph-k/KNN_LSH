#pragma once

#include <vector>
#include <string>
#include <unordered_map>

class TimeSeries; // Forward declaration
struct PointPointer;

#define SimpleListItemType PointPointer

typedef struct ListNode ListNode;

class SimpleList{
    public:
        SimpleList();
        int Push(SimpleListItemType item);
        int Pop(SimpleListItemType &item);
        int Pop();
        int Find(SimpleListItemType item);
        void Traverse( void (*fun)(SimpleListItemType* item, void* privateItems ), void* privateItems);
        int knn_search(int k, TimeSeries *q, int Id_q, struct PD* nearest, int M, bool brute_force = false);
        int rangeSearch(int r, TimeSeries *q, std::unordered_map<std::string, TimeSeries*> &r_neighbors, int M);

        // RangeSearch for clustering
        int reverseRangeSearch(int r, std::unordered_map<std::string, TimeSeries*> *Clusters, int k, int k_index, TimeSeries **Medoids, int M);
        TimeSeries* meanVector();
        inline unsigned int size(){ return T;}
        ~SimpleList();
        void empty();
        double averageDistance(TimeSeries *item);
    private:
        ListNode *head;
        unsigned int T;
};