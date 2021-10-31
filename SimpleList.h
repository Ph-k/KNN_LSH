#include <vector>

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
        int rangeSearch(int r, Point *q,  void (*outputFunction)(Point *, void* privateItem), void* outputFunctionItem );
        ~SimpleList();
    private:
        ListNode *head;
};