#include <check.h>
#include <stdlib.h>
#include "../ds.h"

START_TEST (test_ds_create) {
    
    llist* list;
    list = create_list();
    ck_assert(isEmpty(list));

} END_TEST
START_TEST (test_ds_insertions) {
    
    llist* list;
    list = create_list();

    char* a = malloc(sizeof(char)*3);
    char* b = malloc(sizeof(char)*3);
    char* c = malloc(sizeof(char)*3);
    char* d = malloc(sizeof(char)*3);
    char* e = malloc(sizeof(char)*3);
    char* f = malloc(sizeof(char)*3);

    strcpy(a, "42");
    strcpy(b, "13");
    strcpy(c, "99");
    strcpy(d, "02");
    strcpy(e, "04");
    strcpy(f, "06");

    insertFirst(list, (void*)a);
    insertFirst(list, (void*)b);
    insertFirst(list, (void*)c);

    ck_assert_pstr_eq((char *)getAt(list, 1)->data, "99");
    ck_assert_pstr_eq((char *)getAt(list, 2)->data, "13");
    ck_assert_pstr_eq((char *)getAt(list, 3)->data, "42");

    insertAt(list, (void*)d, 1);
    insertAt(list, (void*)e, 4);

    ck_assert_pstr_eq((char *)getAt(list, 1)->data, "02");
    ck_assert_pstr_eq((char *)getAt(list, 2)->data, "99");
    ck_assert_pstr_eq((char *)getAt(list, 3)->data, "13");
    ck_assert_pstr_eq((char *)getAt(list, 4)->data, "04");
    ck_assert_pstr_eq((char *)getAt(list, 5)->data, "42");
    
    insertLast(list, (void*)f);

    ck_assert_pstr_eq((char *)getAt(list, 1)->data, "02");
    ck_assert_pstr_eq((char *)getAt(list, 2)->data, "99");
    ck_assert_pstr_eq((char *)getAt(list, 3)->data, "13");
    ck_assert_pstr_eq((char *)getAt(list, 4)->data, "04");
    ck_assert_pstr_eq((char *)getAt(list, 5)->data, "42");
    ck_assert_pstr_eq((char *)getAt(list, 6)->data, "06");

} END_TEST
START_TEST (test_ds_removals) {
    
    llist* list;
    list = create_list();

    char* a = malloc(sizeof(char)*3);
    char* b = malloc(sizeof(char)*3);
    char* c = malloc(sizeof(char)*3);
    char* d = malloc(sizeof(char)*3);
    char* e = malloc(sizeof(char)*3);
    char* f = malloc(sizeof(char)*3);

    strcpy(a, "42");
    strcpy(b, "13");
    strcpy(c, "99");
    strcpy(d, "02");
    strcpy(e, "04");
    strcpy(f, "06");

    insertFirst(list, (void*)a);
    insertFirst(list, (void*)b);
    insertFirst(list, (void*)c);
    insertAt(list, (void*)d, 1);
    insertAt(list, (void*)e, 4);
    insertLast(list, (void*)f);

    removeAt(list, 6);
    ck_assert_pstr_eq((char *)getAt(list, 1)->data, "02");
    ck_assert_pstr_eq((char *)getAt(list, 2)->data, "99");
    ck_assert_pstr_eq((char *)getAt(list, 3)->data, "13");
    ck_assert_pstr_eq((char *)getAt(list, 4)->data, "04");
    ck_assert_pstr_eq((char *)getAt(list, 5)->data, "42");
    ck_assert_int_eq(list->size, 5);

    removeAt(list, 1);
    ck_assert_pstr_eq((char *)getAt(list, 1)->data, "99");
    ck_assert_pstr_eq((char *)getAt(list, 2)->data, "13");
    ck_assert_pstr_eq((char *)getAt(list, 3)->data, "04");
    ck_assert_pstr_eq((char *)getAt(list, 4)->data, "42");
    ck_assert_int_eq(list->size, 4);

    removeAt(list, 3);
    ck_assert_pstr_eq((char *)getAt(list, 1)->data, "99");
    ck_assert_pstr_eq((char *)getAt(list, 2)->data, "13");
    ck_assert_pstr_eq((char *)getAt(list, 3)->data, "42");
    ck_assert_int_eq(list->size, 3);

    clear(list);
    ck_assert(isEmpty(list));
} END_TEST
Suite *ds_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Data Structure Test Suite");
    tc_core = tcase_create("test");

    tcase_add_test(tc_core, test_ds_create);
    tcase_add_test(tc_core, test_ds_insertions);
    tcase_add_test(tc_core, test_ds_removals);

    suite_add_tcase(s, tc_core);

    return s;
}
int main(void) {

    int num_failed = 0;
    Suite *s;
    SRunner *runner;

    s = ds_suite();
    runner = srunner_create(s);

    srunner_run_all(runner, CK_NORMAL);
    num_failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (num_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}