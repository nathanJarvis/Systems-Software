/**
 * Source name     : list.c
 * Version         : 1.0
 * Created date    : 02/08/2017
 * Author          : I-Ting Angelina Lee and <Your name>
 * Description     : This file define the list data type 
 *                   that contains list of int's and supports
 *                   clear, prepend, append, delete, size, and print.
 **/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/* set it to 1 for calling check_list for debugging */
#define LIST_DEBUG 0

#if LIST_DEBUG
#define CHECK_LIST(l) check_list(l)
#else
#define CHECK_LIST(l)
#endif

/* list node data type */
struct node_t {
    struct node_t *prev;
    struct node_t *next;
    int val;
};
typedef struct node_t node_t;

/* A function that checks whether the list invariant is maintained properly */ 
void check_list(list_t *list) {
    assert(list);    
    int count = 0;
  
    /* check head */
    node_t *node = list->head;
    assert(node == NULL || node->prev == NULL); 
    /* traverse the list forward and check count */
    while(node) {
        count++;
        node = node->next;
    }
    assert(count == list->size);

    /* reinit and check tail */
    count = 0; 
    node = list->tail;
    assert(node == NULL || node->next == NULL); 
    /* traverse the list backward and check count */
    while(node) {
        count++;
        node = node->prev;
    }
    assert(count == list->size);
    /* if we didn't fail assert, prev / next didn't skip anything */
}

/**
 * Input: an allocated but uninitialized list
 * This function initializes the list.
 **/ 
int list_init(list_t *list) {
    if(!list) { return 1; }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return 0;
}

/**
 * Input: a list
 * This function clears the list (including deallocating memory used).
 * Return value: 1 if any error is encountered, 0 otherwise.
 **/ 
int list_clear(list_t *list) {
    if(!list) { return 1; }

    CHECK_LIST(list);
    while(list->head) {
        node_t *to_free = list->head;
        list->head = list->head->next;
        if(list->head) {
          list->head->prev = NULL;
        }
        free(to_free);  /* free old list head */
    }
    list_init(list);
    CHECK_LIST(list);

    return 0;
}

/**
 * Input: a list and a value to prepend to the list. 
 * This function prepends the new value onto the list.
 * Return value: 1 if any error occur (such as if the list is not properly
 *    initialized); 0 otherwise. 
 **/ 
int list_prepend(list_t *list, int val) {
    if(!list) { return 1; }

    node_t  *new_node = (node_t *) malloc(sizeof(node_t));
    new_node->val = val;
    new_node->prev = NULL;
    new_node->next = NULL;
 
    if(list->size == 0){
        list->head = new_node;
        list->tail = new_node;
    }
    else{
       new_node->next = list->head;
       list->head->prev = new_node;
       list->head = new_node;
    }
    list->size++;

    return 0;
}

/**
 * Input: a list and a value to append to the list. 
 * This function appends the new value onto the list.
 * Return value: 1 if any error occur (such as if the list is not properly
 *    initialized); 0 otherwise. 
 **/ 
int list_append(list_t *list, int val) {
    if(!list) { return 1; }

    node_t *new_node = (node_t *) malloc(sizeof(node_t));
    new_node->val = val;
    new_node->next = NULL;
    new_node->prev = NULL;

    //empty list
    if(list->size == 0){
       list->head = new_node;
       list->tail = new_node; 

    }
    else{ 
       new_node->prev = list->tail;
       list->tail->next = new_node;
       list->tail = new_node;
    }
    list->size++;  
    return 0;
}

/**
 * Input: a list and a value to delete 
 * This function searches the list to find the given value.
 * If the value is found, delete the value.
 * Return value: 1 if any error occur (such as value not found or if the 
 *    list is not properly initialized); 0 otherwise. 
 **/ 
int list_delete(list_t *list, int val) {
    if(!list) { return 1; }
    node_t * current = list->head;
    node_t * temp;
    int found = 0;
    
    while(current){

	if(current->val == val){
	   found = 1;
		
           //delete head
	   if(current->prev == NULL){
              list->head = current->next;
              current->prev = NULL;
              free(current);
              current = list->head;
	   }
           //delete tail
           else if(current->next == NULL){
              list->tail = current->prev;
	      list->tail->next = NULL;
              free(current);
              current = list->tail;
            }
           //delete arrbitrary middle node
           else{
	      temp = current->prev;
              temp->next = current->next;
              current->next->prev = temp;
              free(current);
              current = temp->next;
           }
           
           list->size--;
	}

      else{
         current = current->next;       

       }
    
    }
    //value not found
    if(!found){
       return 1;

    }
     

    return 0;
}

/**
 * Input: a list and a size_ptr (pointer to an int)
 * This function stores the size of the list in size_ptr. 
 * Return value: 1 if any error occur (if the list is not properly 
 *    initialized); 0 otherwise. 
 **/ 
int list_size(list_t *list, int *size_ptr) {
    if(!list) { return 1; }

    *size_ptr = list->size;

    return 0;
}

/**
 * Input: a list
 * This function prints the content of the list 
 * Return value: 1 if any error occur (if the list is not properly 
 *    initialized); 0 otherwise. 
 **/ 
void list_print(list_t *list) {
    if(!list) { fprintf(stderr, "List not intialized.\n"); return; }

    CHECK_LIST(list);

    if(!list->head) { printf("List empty."); }
    else { printf("List: "); }
    node_t *tmp = list->head;
    while(tmp) {
        printf("%d ", tmp->val);
        tmp = tmp->next;
    }
    printf("\n");

    return;
}
