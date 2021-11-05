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
        //void Traverse( void (*fun)(SimpleListItemType *) );
        int knn_search(int k, Point *q, int Id_q, struct PD* nearest, bool brute_force = false);
        int rangeSearch(int r, Point *q, std::unordered_map<std::string, Point*> &r_neighbors);
        ~SimpleList();
    private:
        ListNode *head;
};