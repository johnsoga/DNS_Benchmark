typedef struct node {
    void* data;
    struct node* next;
    struct node* prev;
} node_t;

typedef struct list {
    int size;
    node_t* head;
    node_t* tail;
} llist;

// Queue's and Stack's are just Linked List's with restrictions on how you interact with them
//
// Insertion:
// With a Linked List you can insert arbitrarily at any position n (1 <= n <= size+1) within the size of the list so we have
// insertFirst(), insertLast(), and insertAt(n). With a Queue you insert/enqueue items at the "end/tail" of the list. This means
// to "Enqueue()" is to add items via insertLast()/insertAt(n). With a Stack you equally will always add items, "Push()", at the
// end of list using the same methods described for Enqueue.
//
// Removal:
// With a Linked List you can remove items arbitrarily at any position n (1 <= n <= size) within the size of the list so we have
// removeFirst(), removeLast(), and removeAt(n). With a Queue you remove/dequeue items at the "beginning/front" of the Queue. This
// means to "Dequeue()" is to remove items from the front of Queue via removeFirst()/removeAt(1). With a Stack you will always remove
// items at the "end/tail of the list so to "Pop()" an item off the top of the stack is to removeLast()/removeAt(n). It is common when
// removing at an item from these structures to return the removed item to the caller
//
// Preview:
// With a Linked List you can view/get items arbitrarily at any position n (1 <= n <= size) within the size of the list so we have
// getFirst(), getLast(), and getAt(n). With a Queue this "Peek" behavior is always done on the first item/at the front of the queue
// so a Peek() is just a getFirst()/getAt(1). With a Stack similarily the "Peek" behvaior is instead at the top of the stack/end of the
// list so to Peek() a Stack is a getLast()/getAt(n).

// Because of these restriction Queue's are considered "FIFO" or First In First Out structures. That is the first items in/enqueued are
// the first items out/dequeued. A Stack is considered "FILO" or First In Last Out structures. That is the first items in/pushed are the
// last items out/popped.

/* Creates a new linked list structure */
extern llist* create_list();

/* Returns true(1) if the list has no items */
extern int isEmpty(llist* list);

/* Adds an item at pos(1), returns true(1) on success */
extern int insertFirst(llist* list, void* newItem);

/* Adds an item at pos(size+1), returns true(1) on success */
extern int insertLast(llist* list, void* newItem);

/* Adds an item at pos(n) in the list, returns true(1) on success */
extern int insertAt(llist* list, void* newItem, int pos);

/* Removes an item at pos(1) in the list, returns the removed node */
extern node_t* removeFirst(llist* list);

/* Removes an item at pos(size) in the list, returns the removed node */
extern node_t* removeLast(llist* list);

/* Removes an item at pos(n) in the list, returns the removed node */
extern node_t* removeAt(llist* list, int pos);

/* Returns a copy of the item at pos(1) in the list */
extern node_t* getFirst(llist* list);

/* Returns a copy of the item at pos(size) in the list */
extern node_t* getLast(llist* list);

/* Returns a copy of the item at pos(n) in the list */
extern node_t* getAt(llist* list, int pos);

/* Removes all items from the list */
extern int clear(llist* list);