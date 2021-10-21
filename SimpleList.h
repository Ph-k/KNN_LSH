#define ItemType int

typedef struct ListNode ListNode;

class SimpleList{
    public:
        SimpleList();
        int Push(ItemType item);
        int Pop(ItemType &item);
        int Pop();
        int Find(ItemType item);
        void Traverse( void (*fun)(ItemType *) );
        ~SimpleList();
    private:
        ListNode *head;
};