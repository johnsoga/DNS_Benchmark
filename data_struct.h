// Generic Linked List
// #ifndef DATASTRUCT_H
// #define DATASTRUCT_H
// #endif

typedef struct {
    char *DNSProviderName;
    char *DNSProviderIP;
} dnsserver;

typedef struct {
    dnsserver *DNSServer;
    struct node *next;
} node;

typedef struct {
    int size;
    node *HEAD;
} LinkedList;

extern void insertAtPostion(node *HEAD, int position, dnsserver *DNSServer);
// extern void insertAtPostion(LinkedList *list, int position, dnsserver *DNSServer);
// Create a Linked List
// llist *listCreate(void *data);

// /* llist_free: Free a linked list */
// void llist_free(llist *list);
//
// /* llist_add_inorder: Add to sorted linked list */
// int llist_add_inorder(void *data, llist *list,
//                        int (*comp)(void *, void *));
//
// /* llist_push: Add to head of list */
// void llist_push(llist *list, void *data);
//
// /* llist_pop: remove and return head of linked list */
// void *llist_pop(llist *list);
//
// /* llist_print: print linked list */
// void llist_print(llist *list, void (*print)(void *data));
