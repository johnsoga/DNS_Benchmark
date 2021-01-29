// struct node {
//     struct dnsserver *DNSServer;
//     struct node *next;
// };
#include "data_struct.h"
#include <stdlib.h>

void insertAtPostion(node *HEAD, int position, dnsserver *DNSServer) {
// void insertAtPostion(LinkedList *list, int position, dnsserver *DNSSERVER) {

    node *HEAD = list->HEAD;

    // Inserting at HEAD, position == 1
    if (HEAD == NULL) {
        HEAD = malloc(sizeof(node));
        HEAD->next = NULL;
        HEAD->DNSServer = DNSSERVER;
        list->HEAD = HEAD;
    }
}
