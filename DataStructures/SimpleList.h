#pragma once

#include <vector>
#include <string>
#include <unordered_map>

class Point; // Forward declaration
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
        int knn_search(int k, Point *q, int Id_q, struct PD* nearest, int M, bool brute_force = false);
        int rangeSearch(int r, Point *q, std::unordered_map<std::string, Point*> &r_neighbors, int M);

        // RangeSearch for clustering
        int reverseRangeSearch(int r, std::unordered_map<std::string, Point*> *Clusters, int k, int k_index, Point **Medoids, int M);
        Point* meanVector();
        inline unsigned int size(){ return T;}
        ~SimpleList();
        void empty();
        double averageDistance(Point *item);
    private:
        ListNode *head;
        unsigned int T;
};