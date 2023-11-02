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


int main(void) {
    yourtest1();
    yourtest2();
    return 0;
}
