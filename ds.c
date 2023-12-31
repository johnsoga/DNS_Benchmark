#include "ds.h"
#include <stdlib.h>
#include <stdio.h>

llist* create_list() {
    
    llist* newList = (llist*)malloc(sizeof(llist));
    newList->size = 0;
    newList->head = NULL;
    newList->tail = NULL;

    return newList;
}
int isEmpty(llist* list) {

    if(list->size == 0) {
        return 1;
    }

    return 0;
}
int destroy(llist* list) {
    if(isEmpty(list))
        return 1;

    node_t* curr;
    node_t* tmp;
    curr = list->head;
    while(!isEmpty(list)) {
        if(curr->data != NULL)
            free(curr->data);
        tmp = curr;
        curr = curr->next;
        free(tmp);
        list->size--;
    }
    list->head = list->tail = NULL;

    return 1;
}
int insertFirst(llist* list, void* newItem) {
    
    node_t* newNode = (node_t*)malloc(sizeof(node_t));

    if(newNode != NULL) {
        newNode->data = newItem;

        if(isEmpty(list)) {
            newNode->next = list->head;
            newNode->prev = NULL;
            list->head = newNode;
            list->tail = newNode;
        } else {
            newNode->next = list->head;
            newNode->prev = NULL;
            list->head->prev = newNode;
            list->head = newNode;
        }
        list->size++;
	    return 1;
    }
    return 0;
}
int insertLast(llist * list, void * newItem) {
   
    node_t* newNode = (node_t*)malloc(sizeof(node_t));

    if(newNode != NULL) {
        newNode->data = newItem;

        if(isEmpty(list)) {
            newNode->next = NULL;
            newNode->prev = list->tail;
            list->tail = newNode;
            list->head = newNode;
        } else {
            newNode->prev = list->tail;
            newNode->next = NULL;
            list->tail->next = newNode;
            list->tail = newNode;
        }
        list->size++;
	    return 1;
    }
    return 0;
}
int insertAt(llist* list, void* newItem, unsigned long pos) {

    if((pos > list->size+1) || (pos <= 0)) {
        return 0;
    }
    if(pos == 1) {
        return insertFirst(list, newItem);
    }
    if(pos == list->size+1) {
        return insertLast(list, newItem);
    }

    node_t* newNode = (node_t*)malloc(sizeof(node_t));
    if(newNode != NULL) {
        newNode->data = newItem;
        node_t* curr = list->head;
	    unsigned long counter = 1;
        while(counter < pos-1) {
            curr = curr->next;
            counter++;
        }
        newNode->next = curr->next;
        newNode->prev = curr;
        curr->next->prev = newNode;
        curr->next = newNode;
        list->size++;

	    return 1;
    }
    return 0;
}

node_t* removeFirst(llist* list) {

    if(isEmpty(list))
        return NULL;

    node_t* returnNode;

    returnNode = list->head;
    list->head = list->head->next;

    if(list->size == 1) {
        list->tail = NULL;
    } else {
        list->head->prev = NULL;
    }

    list->size--;

    return returnNode;
}
node_t* removeLast(llist* list) {
    if(isEmpty(list))
        return NULL;

    node_t* returnNode;
    returnNode = list->tail;
    list->tail = list->tail->prev;
    list->tail->next = NULL;
    list->size--;
    return returnNode;
}
node_t* removeAt(llist* list, unsigned long pos) {

    if((pos > list->size) || (pos <= 0))
        return NULL;

    if(pos == 1) {
	    return removeFirst(list);
    } else if (pos == list->size) {
        return removeLast(list);
    }

    node_t* returnNode;
    node_t* curr = list->head;
    unsigned long counter = 1;
    while(counter < pos-1) {
        curr = curr->next;
        counter++;
    }
    returnNode = curr->next;
    curr->next = returnNode->next;
    returnNode->next->prev = curr;

    list->size--;

    return returnNode;
}
node_t* getFirst(llist* list) {
    return list->head;
}
node_t* getLast(llist* list) {
    return list->tail;
}
node_t* getAt(llist* list, unsigned long pos) {
    if((pos > list->size) || (pos <= 0))
        return NULL;

    if(isEmpty(list))
        return NULL;

    if(pos == 1) {
        return getFirst(list);
    } else if(pos == list->size) {
        return getLast(list);
    }

    node_t* curr = list->head;
    unsigned long counter = 1;
    while(counter < pos) {
        curr = curr->next;
        counter++;
    }

    return curr;
}
