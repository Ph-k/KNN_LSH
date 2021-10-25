#include <vector>

class Point; // Forward declaration

#define SimpleListItemType Point*

typedef struct ListNode ListNode;

class SimpleList{
    public:
        SimpleList();
        int Push(SimpleListItemType item);
        int Pop(SimpleListItemType &item);
        int Pop();
        int Find(SimpleListItemType item);
        void Traverse( void (*fun)(SimpleListItemType *) );
        int knn_search(int k, Point *q, struct PD* nearest);
        ~SimpleList();
    private:
        ListNode *head;
};