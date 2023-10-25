/**
 * \author Jeroen Van Aken, Bert Lagaisse, Ludo Bruynseels
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "dplist.h"



/*
 * The real definition of struct list / struct node
 */
struct dplist_node {
    dplist_node_t *prev, *next;
    element_t element;
};

struct dplist {
    dplist_node_t *head;
    // more fields will be added later
};

dplist_t *dpl_create() {
    dplist_t *list;
    list = malloc(sizeof(struct dplist));
    list->head = NULL;
  return list;
}

void dpl_free(dplist_t **list) {

    //Do extensive testing with valgrind.
    if(list == NULL){
        return;
    }
    dplist_t *p = *list;
    if(p == NULL){
        return;
    }
    else if(p->head == NULL){
        free(p);
        *list=NULL;
        return;
    }
    else{
        dplist_node_t *node = p->head;
        dplist_node_t *next = node->next;

        while (next != NULL) {
            free(next->prev->element);
            free(next->prev);
            node = next;
            next = next->next;
        }
        free(node->element);
        free(node);

        free(p);
        *list=NULL;
        return;
    }
}

/* Important note: to implement any list manipulation operator (insert, append, delete, sort, ...), always be aware of the following cases:
 * 1. empty list ==> avoid errors
 * 2. do operation at the start of the list ==> typically requires some special pointer manipulation
 * 3. do operation at the end of the list ==> typically requires some special pointer manipulation
 * 4. do operation in the middle of the list ==> default case with default pointer manipulation
 * ALWAYS check that you implementation works correctly in all these cases (check this on paper with list representation drawings!)
 **/


dplist_t *dpl_insert_at_index(dplist_t *list, element_t element, int index) {
    dplist_node_t *ref_at_index, *list_node;
    if (list == NULL) return NULL;

    list_node = malloc(sizeof(dplist_node_t));
    list_node->element = (element_t)malloc(sizeof(element)+1);

    strncpy(list_node->element, element, strlen(element)+1);

    // pointer drawing breakpoint
    if (list->head == NULL) { // covers case 1
        list_node->prev = NULL;
        list_node->next = NULL;
        list->head = list_node;
        // pointer drawing breakpoint
    } else if (index <= 0) { // covers case 2
        list_node->prev = NULL;
        list_node->next = list->head;
        list->head->prev = list_node;
        list->head = list_node;
        // pointer drawing breakpoint
    } else {
        ref_at_index = dpl_get_reference_at_index(list, index);
        assert(ref_at_index != NULL);
        // pointer drawing breakpoint
        if (index < dpl_size(list)) { // covers case 4
            list_node->prev = ref_at_index->prev;
            list_node->next = ref_at_index;
            ref_at_index->prev->next = list_node;
            ref_at_index->prev = list_node;
            // pointer drawing breakpoint
        } else { // covers case 3
            assert(ref_at_index->next == NULL);
            list_node->next = NULL;
            list_node->prev = ref_at_index;
            ref_at_index->next = list_node;
            // pointer drawing breakpoint
        }
    }
    return list;
}

dplist_t *dpl_remove_at_index(dplist_t *list, int index) {

    //TODO: add your code here

    if(list == NULL){
        return NULL;
    }
    else if(list->head == NULL){
        return list;
    }
    else{
        dplist_node_t *node = dpl_get_reference_at_index(list, index);
        if(index <= 0){
            list->head = node->next;
            node->next->prev = NULL;
        }
        else if(index >= dpl_size(list)-1){
            node->prev->next = NULL;
        }
        else{
            node->next->prev = node->prev;
            node->prev->next = node->next;
        }
        free(node);
        return list;
    }
}

int dpl_size(dplist_t *list) {
    int count = 1;

    if(list == NULL){
        return -1;
    }
    else if(list->head == NULL){
        return 0;
    }
    else{
        dplist_node_t *dplist_node = list->head;
        while(dplist_node->next != NULL){
            dplist_node = dplist_node->next;
            count++;
        }
        return count;
    }
}

dplist_node_t *dpl_get_reference_at_index(dplist_t *list, int index) {
    int count;

    if(list == NULL || list->head == NULL){
        return NULL;
    }
    else if(index <= 0){
        return list->head;
    }
    else if(index > dpl_size(list)-1){
        dplist_node_t *dplist_node = list->head;
        while(dplist_node->next != NULL){
            dplist_node = dplist_node->next;
        }
        return dplist_node;
    }
    else{
        dplist_node_t *dplist_node = list->head;
        for(count=0; count < index; count++){
            dplist_node = dplist_node->next;
        }
        return dplist_node;
    }
}

element_t dpl_get_element_at_index(dplist_t *list, int index) {

    dplist_node_t* node = dpl_get_reference_at_index(list, index);

    if(node == NULL){
        return NULL;
    }
    else{
        return node->element;
    }
}

int dpl_get_index_of_element(dplist_t *list, element_t element) {
    int count;

    if(list == NULL || list->head == NULL){
        return -1;
    }
    else{
        dplist_node_t *dplist_node = list->head;
        for(count=0; count < dpl_size(list); count++){
            if(!strcmp(dplist_node->element,element)){
                return count;
            }
            dplist_node = dplist_node->next;
        }
        return -1;
    }
}



