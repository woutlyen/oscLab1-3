#define _GNU_SOURCE



#include "dplist.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

typedef struct {
    int id;
    char* name;
} my_element_t;

void* element_copy(void * element);
void element_free(void ** element);
int element_compare(void * x, void * y);

void * element_copy(void * element) {
    my_element_t* copy = malloc(sizeof (my_element_t));
    char* new_name;
    asprintf(&new_name,"%s",((my_element_t*)element)->name); //asprintf requires _GNU_SOURCE
    assert(copy != NULL);
    copy->id = ((my_element_t*)element)->id;
    copy->name = new_name;
    return (void *) copy;
}

void element_free(void ** element) {
    free((((my_element_t*)*element))->name);
    free(*element);
    *element = NULL;
}

int element_compare(void * x, void * y) {
    return ((((my_element_t*)x)->id < ((my_element_t*)y)->id) ? -1 : (((my_element_t*)x)->id == ((my_element_t*)y)->id) ? 0 : 1);
}

void ck_assert_msg(bool result, char * msg){
    if(!result) printf("%s\n", msg);
}

void yourtest1()
    {
        // Test free NULL, don't use callback
        dplist_t *list = NULL;
        dpl_free(&list, false);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // Test free NULL, use callback
        list = NULL;
        dpl_free(&list, true);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // Test free empty list, don't use callback
        list = dpl_create(element_copy, element_free, element_compare);
        dpl_free(&list, false);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        // Test free empty list, use callback
        list = dpl_create(element_copy, element_free, element_compare);
        dpl_free(&list, true);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");


        my_element_t *content = (my_element_t *)malloc(sizeof(my_element_t));
        content->id = 5;
        content->name = "foo";

        my_element_t  *element = NULL;

        list = dpl_create(element_copy, element_free, element_compare);
        dpl_insert_at_index(list, content, 0, false);
        element = (my_element_t*)dpl_get_element_at_index(list,0);
        dpl_free(&list, false);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        list = dpl_create(element_copy, element_free, element_compare);
        dpl_insert_at_index(list, content, 0, true);
        element = (my_element_t*)dpl_get_element_at_index(list,0);
        dpl_free(&list, true);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        char *test2 = "123";
        my_element_t *content2 = (my_element_t *)malloc(sizeof(my_element_t));
        content2->id = 6;
        content2->name = test2;
        list = dpl_create(element_copy, element_free, element_compare);

        dpl_insert_at_index(list, content2, 0, false);
        dpl_insert_at_index(list, content, 99, false);
        dpl_free(&list, false);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");


        list = dpl_create(element_copy, element_free, element_compare);

        dpl_insert_at_index(list, content, 0, true);
        dpl_insert_at_index(list, content2, 99, true);
        dpl_free(&list, true);
        ck_assert_msg(list == NULL, "Failure: expected result to be NULL");

        free(content);
        free(content2);
        }

void yourtest2(){
    my_element_t *content = (my_element_t *)malloc(sizeof(my_element_t));
    content->id = 10;
    content->name = "bar";

    dplist_t *list = NULL;

    list = dpl_insert_at_index(NULL, content, -1, false);
    ck_assert_msg(list == NULL, "Failure: expected list to be NULL");

    list = dpl_insert_at_index(NULL, content, 99, true);
    ck_assert_msg(list == NULL, "Failure: expected list to be NULL");

    free(content);
}

/**
 * Tests the dpl_size() function
 *
 * Makes use of:
 *      - dpl_insert_at_index()
 */
void dpl_size_test(){

    dplist_t *list = NULL;

    ck_assert_msg(dpl_size(NULL) == -1, "Failure: expected size to be NULL");
    ck_assert_msg(dpl_size(list) == -1, "Failure: expected size to be NULL");

    my_element_t *content1 = (my_element_t *)malloc(sizeof(my_element_t));
    content1->id = 11;
    content1->name = "bar";
    my_element_t *content2 = (my_element_t *)malloc(sizeof(my_element_t));
    content2->id = 12;
    content2->name = "quux";
    my_element_t *content3 = (my_element_t *)malloc(sizeof(my_element_t));
    content3->id = 13;
    content3->name = "gee";

    list = dpl_create(element_copy, element_free, element_compare);
    ck_assert_msg(dpl_size(list)==0, "Failure; expected size to be 0");
    dpl_insert_at_index(list, content1, 0, false);
    ck_assert_msg(dpl_size(list)==1,"Failure: expected size to be 1");
    dpl_insert_at_index(list, content2, 1, false);
    ck_assert_msg(dpl_size(list)==2, "Failure: expected size to be 2");
    dpl_insert_at_index(list, content3, 2, false);
    ck_assert_msg(dpl_size(list)==3, "Failure: expected size to be 3");
    dpl_free(&list, false);

    list = dpl_create(element_copy, element_free, element_compare);
    dpl_insert_at_index(list, content1, -100, false);
    ck_assert_msg(dpl_size(list)==1,"Failure: expected size to be 1");
    dpl_insert_at_index(list, content2, 0, false);
    ck_assert_msg(dpl_size(list)==2, "Failure: expected size to be 2");
    dpl_insert_at_index(list, content3, 0, false);
    ck_assert_msg(dpl_size(list)==3, "Failure: expected size to be 3");
    dpl_free(&list, false);

    list = dpl_create(element_copy, element_free, element_compare);
    dpl_insert_at_index(list, content1, 100, false);
    ck_assert_msg(dpl_size(list)==1,"Failure: expected size to be 1");
    dpl_insert_at_index(list, content2, 0, false);
    ck_assert_msg(dpl_size(list)==2, "Failure: expected size to be 2");
    dpl_insert_at_index(list, content3, 0, false);
    ck_assert_msg(dpl_size(list)==3, "Failure: expected size to be 3");
    dpl_free(&list, false);

    list = dpl_create(element_copy, element_free, element_compare);
    dpl_insert_at_index(list, content1, 0, false);
    ck_assert_msg(dpl_size(list)==1,"Failure: expected size to be 1");
    dpl_insert_at_index(list, content2, 0, false);
    ck_assert_msg(dpl_size(list)==2, "Failure: expected size to be 2");
    dpl_insert_at_index(list, content3, 0, false);
    ck_assert_msg(dpl_size(list)==3, "Failure: expected size to be 3");
    dpl_free(&list, false);

    free(content1);
    free(content2);
    free(content3);
}


/**
 *  Tests the dpl_insert_at_index() function
 *
 *  Makes use of:
 *      - dpl_get_element_at_index()
 */
void dpl_insert_at_index_test(){

    dplist_t *list = NULL;

    my_element_t *content1 = (my_element_t *)malloc(sizeof(my_element_t));
    content1->id = 11;
    content1->name = "bar";
    my_element_t *content2 = (my_element_t *)malloc(sizeof(my_element_t));
    content2->id = 12;
    content2->name = "quux";
    my_element_t *content3 = (my_element_t *)malloc(sizeof(my_element_t));
    content3->id = 13;
    content3->name = "gee";

    ck_assert_msg(dpl_insert_at_index(NULL, content1, 0, false)==NULL, "Failure: expected NULL value returned");
    ck_assert_msg(dpl_insert_at_index(list, content1, 0, false)==NULL, "Failure: expected NULL value returned");

    list = dpl_create(element_copy, element_free, element_compare);
    dpl_insert_at_index(list, content1, 0, false);
    ck_assert_msg(dpl_get_element_at_index(list, 0)==content1, "Failure: expected content1");
    dpl_insert_at_index(list, content2, 1, false);
    ck_assert_msg(dpl_get_element_at_index(list, 1)==content2, "Failure: expected content2");
    dpl_insert_at_index(list, content3, 2, false);
    ck_assert_msg(dpl_get_element_at_index(list, 2)==content3, "Failure: expected content3");
    dpl_free(&list, false);

    list = dpl_create(element_copy, element_free, element_compare);
    dpl_insert_at_index(list, content1, -100, false);
    ck_assert_msg(dpl_get_element_at_index(list, 0)==content1, "Failure: expected content1");
    dpl_insert_at_index(list, content2, 0, false);
    ck_assert_msg(dpl_get_element_at_index(list, 0)==content2, "Failure: expected content2");
    dpl_insert_at_index(list, content3, -100, false);
    ck_assert_msg(dpl_get_element_at_index(list, 0)==content3, "Failure: expected content3");
    dpl_free(&list, false);

    list = dpl_create(element_copy, element_free, element_compare);
    dpl_insert_at_index(list, content1, 100, false);
    ck_assert_msg(dpl_get_element_at_index(list, 0)==content1, "Failure: expected content1");
    dpl_insert_at_index(list, content2, 0, false);
    ck_assert_msg(dpl_get_element_at_index(list, 0)==content2, "Failure: expected content2");
    dpl_insert_at_index(list, content3, 100, false);
    ck_assert_msg(dpl_get_element_at_index(list, 2)==content3, "Failure: expected content3");
    dpl_free(&list, false);

    list = dpl_create(element_copy, element_free, element_compare);
    dpl_insert_at_index(list, content1, 0, false);
    ck_assert_msg(dpl_get_element_at_index(list, 0)==content1, "Failure: expected content1");
    dpl_insert_at_index(list, content2, 0, false);
    ck_assert_msg(dpl_get_element_at_index(list, 0)==content2, "Failure: expected content2");
    dpl_insert_at_index(list, content3, 0, false);
    ck_assert_msg(dpl_get_element_at_index(list, 0)==content3, "Failure: expected content3");
    dpl_free(&list, false);

    free(content1);
    free(content2);
    free(content3);
}

/**
 * Tests the dpl_get_element_at_index() function
 *
 * Makes use of:
 *      - dpl_insert_at_index()
 */
void dpl_get_element_at_index_test(){

    dplist_t *list = NULL;

    my_element_t *content1 = (my_element_t *)malloc(sizeof(my_element_t));
    content1->id = 11;
    content1->name = "bar";
    my_element_t *content2 = (my_element_t *)malloc(sizeof(my_element_t));
    content2->id = 12;
    content2->name = "quux";
    my_element_t *content3 = (my_element_t *)malloc(sizeof(my_element_t));
    content3->id = 13;
    content3->name = "gee";

    ck_assert_msg(dpl_get_element_at_index(NULL, 0)==NULL, "Failure: expected NULL as element");
    ck_assert_msg(dpl_get_element_at_index(list, 0)==NULL, "Failure: expected NULL as element");

    list = dpl_create(element_copy, element_free, element_compare);
    ck_assert_msg(dpl_get_element_at_index(list,0)==NULL, "Failure: expected NULL as element");

    dpl_insert_at_index(list, content1, 0, false);
    ck_assert_msg(dpl_get_element_at_index(list, 0)==content1, "Failure: expected to get content1");
    ck_assert_msg(dpl_get_element_at_index(list, -100)==content1, "Failure: expected to get content1");
    ck_assert_msg(dpl_get_element_at_index(list, 100)==content1, "Failure: expected to get content1");

    dpl_insert_at_index(list, content2, 1, false);
    ck_assert_msg(dpl_get_element_at_index(list, 0)==content1, "Failure: expected to get content1");
    ck_assert_msg(dpl_get_element_at_index(list, 1)==content2, "Failure: expected to get element2");
    ck_assert_msg(dpl_get_element_at_index(list, -100)==content1, "Failure: expected to get content1");
    ck_assert_msg(dpl_get_element_at_index(list, 100)==content2, "Failure: expected to get content2");


    dpl_insert_at_index(list, content3, 2, false);
    ck_assert_msg(dpl_get_element_at_index(list, 0)==content1, "Failure: expected to get content1");
    ck_assert_msg(dpl_get_element_at_index(list, 1)==content2, "Failure: expected to get element2");
    ck_assert_msg(dpl_get_element_at_index(list, 2)==content3, "Failure: expected to get element3");
    ck_assert_msg(dpl_get_element_at_index(list, -100)==content1, "Failure: expected to get content1");
    ck_assert_msg(dpl_get_element_at_index(list, 100)==content3, "Failure: expected to get content3");
    dpl_free(&list, false);

    free(content1);
    free(content2);
    free(content3);
}

/**
 * Tests dpl_get_index_of_element()
 *
 * Makes use of:
 *      - dpl_insert_at_index()
 *      - dpl_get_element_at_index()
 */
void dpl_get_index_of_element_test(){

    dplist_t *list = NULL;

    my_element_t *content1 = (my_element_t *)malloc(sizeof(my_element_t));
    content1->id = 11;
    content1->name = "bar";
    my_element_t *content2 = (my_element_t *)malloc(sizeof(my_element_t));
    content2->id = 12;
    content2->name = "quux";
    my_element_t *content3 = (my_element_t *)malloc(sizeof(my_element_t));
    content3->id = 13;
    content3->name = "gee";
    my_element_t *content4 = (my_element_t *)malloc(sizeof(my_element_t));
    content4->id = 14;
    content4->name = "tup";

    ck_assert_msg(dpl_get_index_of_element(NULL, content1)==NULL, "Failure: expected NULL as index");
    ck_assert_msg(dpl_get_index_of_element(list, content1)==NULL, "Failure: expected NULL as index");

    list = dpl_create(element_copy, element_free, element_compare);
    ck_assert_msg(dpl_get_index_of_element(list, content1)==NULL, "Failure: expected NULL as index");

    dpl_insert_at_index(list, content1, 0, false);
    dpl_insert_at_index(list, content2, 1, false);
    dpl_insert_at_index(list, content3, 2, false);
    ck_assert_msg(dpl_get_index_of_element(list, content1)==0, "Failure: expected 0 as index");
    ck_assert_msg(dpl_get_index_of_element(list, content2)==1, "Failure: expected 1 as index");
    ck_assert_msg(dpl_get_index_of_element(list, content3)==2, "Failure: expected 2 as index");
    ck_assert_msg(dpl_get_index_of_element(list, content4)==-1, "Failure: expected -1 as index");
    dpl_free(&list, false);

    free(content1);
    free(content2);
    free(content3);
    free(content4);
}

/**
 * Tests dpl_remove_at_index()
 *
 * Makes use of:
 *      - dpl_insert_at_index()
 */
void dpl_remove_at_index_test(){

    dplist_t *list = NULL;

    my_element_t *content1 = (my_element_t *)malloc(sizeof(my_element_t));
    content1->id = 11;
    content1->name = "bar";
    my_element_t *content2 = (my_element_t *)malloc(sizeof(my_element_t));
    content2->id = 12;
    content2->name = "quux";
    my_element_t *content3 = (my_element_t *)malloc(sizeof(my_element_t));
    content3->id = 13;
    content3->name = "gee";

    ck_assert_msg(dpl_remove_at_index(NULL, 0, false)==NULL, "Failure: expected NULL value");
    ck_assert_msg(dpl_remove_at_index(list, 0, false)==NULL, "Failure: expected NULL value");

    list = dpl_create(element_copy, element_free, element_compare);
    ck_assert_msg(dpl_remove_at_index(list, 0, false)==list, "Failure: expected list");

    dpl_insert_at_index(list, content1, 0, false);
    dpl_remove_at_index(list, 0, false);
    dpl_insert_at_index(list, content1, 0, false);
    dpl_remove_at_index(list, -100, false);
    dpl_insert_at_index(list, content1, 0, false);
    dpl_remove_at_index(list, 100, false);

    dpl_insert_at_index(list, content1, 0, false);
    dpl_insert_at_index(list, content2, 1, false);
    dpl_insert_at_index(list, content3, 2, false);
    dpl_remove_at_index(list, 0, false);
    dpl_remove_at_index(list, 0, false);
    dpl_remove_at_index(list, 0, false);

    dpl_insert_at_index(list, content1, 0, false);
    dpl_insert_at_index(list, content2, 1, false);
    dpl_insert_at_index(list, content3, 2, false);
    dpl_remove_at_index(list, 100, false);
    dpl_remove_at_index(list, 100, false);
    dpl_remove_at_index(list, 100, false);

    dpl_insert_at_index(list, content1, 0, false);
    dpl_insert_at_index(list, content2, 1, false);
    dpl_insert_at_index(list, content3, 2, false);
    dpl_remove_at_index(list, -100, false);
    dpl_remove_at_index(list, -100, false);
    dpl_remove_at_index(list, -100, false);

    dpl_insert_at_index(list, content1, 0, false);
    dpl_insert_at_index(list, content2, 1, false);
    dpl_insert_at_index(list, content3, 2, false);
    dpl_remove_at_index(list, 2, false);
    dpl_remove_at_index(list, 1, false);
    dpl_remove_at_index(list, 0, false);
    dpl_free(&list, false);

    //-----------------------------------------------------------------------

    list = dpl_create(element_copy, element_free, element_compare);
    ck_assert_msg(dpl_remove_at_index(list, 0, false)==list, "Failure: expected list");

    dpl_insert_at_index(list, content1, 0, true);
    dpl_remove_at_index(list, 0, true);
    dpl_insert_at_index(list, content1, 0, true);
    dpl_remove_at_index(list, -100, true);
    dpl_insert_at_index(list, content1, 0, true);
    dpl_remove_at_index(list, 100, true);

    dpl_insert_at_index(list, content1, 0, true);
    dpl_insert_at_index(list, content2, 1, true);
    dpl_insert_at_index(list, content3, 2, true);
    dpl_remove_at_index(list, 0, true);
    dpl_remove_at_index(list, 0, true);
    dpl_remove_at_index(list, 0, true);

    dpl_insert_at_index(list, content1, 0, true);
    dpl_insert_at_index(list, content2, 1, true);
    dpl_insert_at_index(list, content3, 2, true);
    dpl_remove_at_index(list, 100, true);
    dpl_remove_at_index(list, 100, true);
    dpl_remove_at_index(list, 100, true);

    dpl_insert_at_index(list, content1, 0, true);
    dpl_insert_at_index(list, content2, 1, true);
    dpl_insert_at_index(list, content3, 2, true);
    dpl_remove_at_index(list, -100, true);
    dpl_remove_at_index(list, -100, true);
    dpl_remove_at_index(list, -100, true);

    dpl_insert_at_index(list, content1, 0, true);
    dpl_insert_at_index(list, content2, 1, true);
    dpl_insert_at_index(list, content3, 2, true);
    dpl_remove_at_index(list, 2, true);
    dpl_remove_at_index(list, 1, true);
    dpl_remove_at_index(list, 0, true);
    dpl_free(&list, false);

    free(content1);
    free(content2);
    free(content3);
}

void remove_test_1(){

    //Initialize list
    dplist_t *list = NULL;

    //Initialize the elements
    my_element_t *content1 = (my_element_t *)malloc(sizeof(my_element_t));
    content1->id = 11;
    content1->name = "bar";
    my_element_t *content2 = (my_element_t *)malloc(sizeof(my_element_t));
    content2->id = 12;
    content2->name = "quux";
    my_element_t *content3 = (my_element_t *)malloc(sizeof(my_element_t));
    content3->id = 13;
    content3->name = "gee";

    //------------ Test 1 (insert_copy == FALSE && free_element == TRUE) ----------------

    //Creating empty list
    list = dpl_create(element_copy, element_free, element_compare);

    //Adding the elements to the list
    dpl_insert_at_index(list, content1, 0, false);
    dpl_insert_at_index(list, content2, 1, false);
    dpl_insert_at_index(list, content3, 2, false);

    //Removing the list nodes and elements
    dpl_remove_at_index(list, 2, true);
    dpl_remove_at_index(list, 1, true);
    dpl_remove_at_index(list, 0, true);

    //Removing the list
    dpl_free(&list, false);

    //Not needed because free_element == TRUE
    /*
    free(content1);
    free(content2);
    free(content3);
    */
}

void remove_test_2() {

    //Initialize list
    dplist_t *list = NULL;

    //Initialize the elements
    my_element_t *content1 = (my_element_t *)malloc(sizeof(my_element_t));
    content1->id = 11;
    content1->name = "bar";
    my_element_t *content2 = (my_element_t *)malloc(sizeof(my_element_t));
    content2->id = 12;
    content2->name = "quux";
    my_element_t *content3 = (my_element_t *)malloc(sizeof(my_element_t));
    content3->id = 13;
    content3->name = "gee";

    //------------ Test 2 (insert_copy == TRUE && free_element == TRUE) ----------------

    //Creating empty list
    list = dpl_create(element_copy, element_free, element_compare);

    //Adding the elements to the list
    dpl_insert_at_index(list, content1, 0, true);
    dpl_insert_at_index(list, content2, 1, true);
    dpl_insert_at_index(list, content3, 2, true);

    //Removing the list nodes and elements
    dpl_remove_at_index(list, 2, true);
    dpl_remove_at_index(list, 1, true);
    dpl_remove_at_index(list, 0, true);

    //Removing the list
    dpl_free(&list, false);

    free(content1);
    free(content2);
    free(content3);

}

void remove_test_3(){

    //Initialize list
    dplist_t *list = NULL;

    //Initialize the elements
    my_element_t *content1 = (my_element_t *)malloc(sizeof(my_element_t));
    content1->id = 11;
    content1->name = "bar";
    my_element_t *content2 = (my_element_t *)malloc(sizeof(my_element_t));
    content2->id = 12;
    content2->name = "quux";
    my_element_t *content3 = (my_element_t *)malloc(sizeof(my_element_t));
    content3->id = 13;
    content3->name = "gee";

    //------------ Test 3 (insert_copy == FALSE && free_element == TRUE) ----------------

    //Copy of the declared elements above
    my_element_t *content1_copy = element_copy(content1);
    my_element_t *content2_copy = element_copy(content2);
    my_element_t *content3_copy = element_copy(content3);

    //Creating empty list
    list = dpl_create(element_copy, element_free, element_compare);

    //Adding the copies of the elements to the list w/ insert_copy == FALSE
    dpl_insert_at_index(list, content1_copy, 0, false);
    dpl_insert_at_index(list, content2_copy, 1, false);
    dpl_insert_at_index(list, content3_copy, 2, false);

    //Removing the list nodes and elements
    dpl_remove_at_index(list, 2, true);
    dpl_remove_at_index(list, 1, true);
    dpl_remove_at_index(list, 0, true);

    //Removing the list
    dpl_free(&list, false);

    free(content1);
    free(content2);
    free(content3);

}

void dpl_free_test(){

    dplist_t *list = NULL;

    my_element_t *content1 = (my_element_t *)malloc(sizeof(my_element_t));
    content1->id = 11;
    content1->name = "bar";
    my_element_t *content2 = (my_element_t *)malloc(sizeof(my_element_t));
    content2->id = 12;
    content2->name = "quux";
    my_element_t *content3 = (my_element_t *)malloc(sizeof(my_element_t));
    content3->id = 13;
    content3->name = "gee";

    dpl_free(&list, false);
    ck_assert_msg(list==NULL, "Failure: expected NULL value");

    list = dpl_create(element_copy, element_free, element_compare);
    dpl_free(&list, false);
    ck_assert_msg(list==NULL, "Failure: expected NULL value");

    list = dpl_create(element_copy, element_free, element_compare);
    dpl_insert_at_index(list, content1, 0, false);
    dpl_free(&list, false);
    ck_assert_msg(list==NULL, "Failure: expected NULL value");

    list = dpl_create(element_copy, element_free, element_compare);
    dpl_insert_at_index(list, content1, 0, false);
    dpl_insert_at_index(list, content2, 1, false);
    dpl_free(&list, false);
    ck_assert_msg(list==NULL, "Failure: expected NULL value");

    list = dpl_create(element_copy, element_free, element_compare);
    dpl_insert_at_index(list, content1, 0, false);
    dpl_insert_at_index(list, content2, 1, false);
    dpl_insert_at_index(list, content3, 2, false);
    dpl_free(&list, false);
    ck_assert_msg(list==NULL, "Failure: expected NULL value");

    //--------------------------------------------------------------------

    list = dpl_create(element_copy, element_free, element_compare);
    dpl_insert_at_index(list, content1, 0, true);
    dpl_free(&list, true);
    ck_assert_msg(list==NULL, "Failure: expected NULL value");

    list = dpl_create(element_copy, element_free, element_compare);
    dpl_insert_at_index(list, content1, 0, true);
    dpl_insert_at_index(list, content2, 1, true);
    dpl_free(&list, true);
    ck_assert_msg(list==NULL, "Failure: expected NULL value");

    list = dpl_create(element_copy, element_free, element_compare);
    dpl_insert_at_index(list, content1, 0, true);
    dpl_insert_at_index(list, content2, 1, true);
    dpl_insert_at_index(list, content3, 2, true);
    dpl_free(&list, true);
    ck_assert_msg(list==NULL, "Failure: expected NULL value");

    free(content1);
    free(content2);
    free(content3);
}

void dpl_get_element_at_reference_at_index_test(){

    dplist_t *list = NULL;

    my_element_t *content1 = (my_element_t *)malloc(sizeof(my_element_t));
    content1->id = 11;
    content1->name = "bar";
    my_element_t *content2 = (my_element_t *)malloc(sizeof(my_element_t));
    content2->id = 12;
    content2->name = "quux";
    my_element_t *content3 = (my_element_t *)malloc(sizeof(my_element_t));
    content3->id = 13;
    content3->name = "gee";

    ck_assert_msg(dpl_get_reference_at_index(NULL,0)==NULL, "Failure: expected NULL as reference");
    ck_assert_msg(dpl_get_reference_at_index(list,0)==NULL, "Failure: expected NULL as reference");

    list = dpl_create(element_copy, element_free, element_compare);
    ck_assert_msg(dpl_get_reference_at_index(list,0)==NULL, "Failure: expected NULL as reference");

    dpl_insert_at_index(list, content1, 0, false);
    ck_assert_msg(dpl_get_reference_at_index(list,0)!=NULL, "Failure: expected pointer as reference");
    ck_assert_msg(dpl_get_reference_at_index(list,-10)!=NULL, "Failure: expected pointer as reference");
    ck_assert_msg(dpl_get_reference_at_index(list,10)!=NULL, "Failure: expected pointer as reference");

    dpl_insert_at_index(list, content2, 1, false);
    ck_assert_msg(dpl_get_reference_at_index(list,0)==dpl_get_reference_at_index(list,-10), "Failure: expected the same reference");
    ck_assert_msg(dpl_get_reference_at_index(list,1)==dpl_get_reference_at_index(list,10), "Failure: expected the same reference");
    ck_assert_msg(dpl_get_reference_at_index(list,1)!=dpl_get_reference_at_index(list,-2), "Failure: didn't expect the same reference");
    dpl_free(&list, false);


    ck_assert_msg(dpl_get_element_at_reference(NULL,NULL)==NULL, "Failure: expected NULL as element");
    ck_assert_msg(dpl_get_element_at_reference(list,NULL)==NULL, "Failure: expected NULL as element");

    list = dpl_create(element_copy, element_free, element_compare);
    ck_assert_msg(dpl_get_element_at_reference(list,NULL)==NULL, "Failure: expected NULL as element");

    dpl_insert_at_index(list, content1, 0, false);
    ck_assert_msg(dpl_get_element_at_reference(list,NULL)==NULL, "Failure: expected NULL as element");

    dplist_node_t *ref1 = dpl_get_reference_at_index(list, 0);
    ck_assert_msg(dpl_get_element_at_reference(list,ref1)==content1, "Failure: expected content1 as element");
    ref1 = dpl_get_reference_at_index(list, 10);
    ck_assert_msg(dpl_get_element_at_reference(list,ref1)==content1, "Failure: expected content1 as element");
    ref1 = dpl_get_reference_at_index(list, -10);
    ck_assert_msg(dpl_get_element_at_reference(list,ref1)==content1, "Failure: expected content1 as element");

    dpl_insert_at_index(list, content2, 1, false);
    dplist_node_t *ref2 = dpl_get_reference_at_index(list, 1);
    ref1 = dpl_get_reference_at_index(list, 0);
    ck_assert_msg(dpl_get_element_at_reference(list,ref1)==content1, "Failure: expected content1 as element");
    ref1 = dpl_get_reference_at_index(list, -10);
    ck_assert_msg(dpl_get_element_at_reference(list,ref1)==content1, "Failure: expected content1 as element");
    ck_assert_msg(dpl_get_element_at_reference(list,ref2)==content2, "Failure: expected content2 as element");
    ref2 = dpl_get_reference_at_index(list, 10);
    ck_assert_msg(dpl_get_element_at_reference(list,ref2)==content2, "Failure: expected content2 as element");

    dplist_t *list2 = dpl_create(element_copy, element_free, element_compare);
    dpl_insert_at_index(list, content3, 0, false);
    dplist_node_t *ref3 = dpl_get_reference_at_index(list2, 0);
    ck_assert_msg(dpl_get_element_at_reference(list,ref3)==NULL, "Failure: expected NULL as element");
    dpl_free(&list, false);
    dpl_free(&list2, false);

    free(content1);
    free(content2);
    free(content3);

}

int main(void) {
    yourtest1();
    yourtest2();

    dpl_insert_at_index_test();
    dpl_get_element_at_index_test();
    dpl_get_index_of_element_test();
    dpl_size_test();
    dpl_remove_at_index_test();

    //remove_test_1();
    remove_test_2();
    remove_test_3();

    dpl_free_test();
    dpl_get_element_at_reference_at_index_test();
    return 0;
}
