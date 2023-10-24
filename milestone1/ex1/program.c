#define _GNU_SOURCE
/**
 * \author Bert Lagaisse
 *
 * main method that executes some test functions (without check framework)
 */

#include <stdio.h>
#include "dplist.h"

void ck_assert_msg(bool result, char * msg){
    if(!result) printf("%s\n", msg);
}

void dpl_size_test(void){

    ck_assert_msg(dpl_size(NULL) == -1, "List is NULL.");

    dplist_t *numbers = NULL;
    numbers = dpl_create();

    ck_assert_msg(numbers != NULL, "numbers = NULL, List not created");
    ck_assert_msg(dpl_size(numbers) == 0, "Numbers may not contain elements.");

    dpl_insert_at_index(numbers, 'a', 0);
    ck_assert_msg(dpl_size(numbers) == 1, "Numbers must contain 1 element.");

    dpl_insert_at_index(numbers, '\e', -1);
    ck_assert_msg(dpl_size(numbers) == 2, "Numbers must contain 2 elements.");

    dpl_insert_at_index(numbers, 'b', 100);
    ck_assert_msg(dpl_size(numbers) == 3, "Numbers must contain 3 elements.");

    dpl_free(&numbers);
}

void dpl_get_reference_at_index_test(void){

    ck_assert_msg(dpl_get_reference_at_index(NULL,0) == NULL, "List should be NULL.");

    dplist_t *numbers = NULL;
    numbers = dpl_create();

    ck_assert_msg(dpl_get_reference_at_index(numbers,0) == NULL, "List should be empty.");

    dpl_insert_at_index(numbers, 'a', 0);

    /*
    ck_assert_msg(dpl_get_reference_at_index(numbers,0) == numbers->head, "Should return pointer in head of list.");
    ck_assert_msg(dpl_get_reference_at_index(numbers,1) == numbers->head, "Should return pointer in head of list.");
    ck_assert_msg(dpl_get_reference_at_index(numbers,-1) == numbers->head, "Should return pointer in head of list.");
    */

    dpl_free(&numbers);
}

void dpl_get_element_at_index_test(void){

    ck_assert_msg(dpl_get_element_at_index(NULL,0) == 0, "Element should be zero.");

    dplist_t *numbers = NULL;
    numbers = dpl_create();

    ck_assert_msg(dpl_get_element_at_index(numbers,0) == 0, "Element should be zero.");

    dpl_insert_at_index(numbers, 'a', 0);
    ck_assert_msg(dpl_get_element_at_index(numbers,0) == 'a', "Element should be 'a'.");
    ck_assert_msg(dpl_get_element_at_index(numbers,-150) == 'a', "Element should be 'a'.");
    ck_assert_msg(dpl_get_element_at_index(numbers,10) == 'a', "Element should be 'a'.");

    dpl_insert_at_index(numbers, 'b', 1);
    ck_assert_msg(dpl_get_element_at_index(numbers,1) == 'b', "Element should be 'b'.");
    ck_assert_msg(dpl_get_element_at_index(numbers,0) == 'a', "Element should be 'a'.");
    ck_assert_msg(dpl_get_element_at_index(numbers,10) == 'b', "Element should be 'b'.");

    dpl_insert_at_index(numbers, '\e', 0);
    ck_assert_msg(dpl_get_element_at_index(numbers,0) == '\e', "Element should be '\e'.");
    ck_assert_msg(dpl_get_element_at_index(numbers,-150) == '\e', "Element should be '\e'.");
    ck_assert_msg(dpl_get_element_at_index(numbers,1) == 'a', "Element should be 'a'.");
    ck_assert_msg(dpl_get_element_at_index(numbers,2) == 'b', "Element should be 'b'.");
    ck_assert_msg(dpl_get_element_at_index(numbers,10) == 'b', "Element should be 'b'.");

    dpl_free(&numbers);
}

int main(void)
{
    //dpl_size_test();
    //dpl_get_reference_at_index_test();
    dpl_get_element_at_index_test();

    return 0;
}

