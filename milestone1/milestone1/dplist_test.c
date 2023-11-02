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
void dpl_insert_element_at_index_test(){

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
 *      - dpl_insert_element_at_index()
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
 *      - dpl_insert_element_at_index()
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


int main(void) {
    //yourtest1();
    //yourtest2();


    //dpl_insert_element_at_index_test();
    //dpl_get_element_at_index_test();
    dpl_get_index_of_element_test();
    //dpl_size_test();
    return 0;
}
