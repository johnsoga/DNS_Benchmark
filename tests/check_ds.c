#include <check.h>
#include <stdlib.h>
#include "../ds.h"
#include "../dns_types.h"

START_TEST (test_ds_create) {
    
    llist* list;
    list = create_list();
    ck_assert(isEmpty(list));

} END_TEST
START_TEST (test_ds_insertions) {
    
    llist* list;
    list = create_list();

    dnshost* pos_a = malloc(sizeof(dnshost));
    dnshost* pos_b = malloc(sizeof(dnshost));
    dnshost* pos_c = malloc(sizeof(dnshost));
    dnshost* pos_d = malloc(sizeof(dnshost));
    dnshost* pos_e = malloc(sizeof(dnshost));
    dnshost* pos_f = malloc(sizeof(dnshost));
    dnshost* pos_g = malloc(sizeof(dnshost));
    dnshost* pos_h = malloc(sizeof(dnshost));

    strcpy(pos_a->DNSProviderName, "position-a.com");
    strcpy(pos_b->DNSProviderName, "position-b.com");
    strcpy(pos_c->DNSProviderName, "position-c.com");
    strcpy(pos_d->DNSProviderName, "position-d.com");
    strcpy(pos_e->DNSProviderName, "position-e.com");
    strcpy(pos_f->DNSProviderName, "position-f.com");
    strcpy(pos_g->DNSProviderName, "position-g.com");
    strcpy(pos_h->DNSProviderName, "position-h.com");

    insertLast(list, (void*)pos_f);
    insertLast(list, (void*)pos_h);
    insertFirst(list, (void*)pos_b);
    insertFirst(list, (void*)pos_a);
    ck_assert_pstr_eq((char *)getAt(list, 1)->data, "position-a.com");
    ck_assert_pstr_eq((char *)getAt(list, 2)->data, "position-b.com");
    ck_assert_pstr_eq((char *)getAt(list, 3)->data, "position-f.com");
    ck_assert_pstr_eq((char *)getAt(list, 4)->data, "position-h.com");
    ck_assert_int_eq(list->size, 4);

    insertAt(list, (void*)pos_c, 3);
    insertAt(list, (void*)pos_d, 4);
    ck_assert_pstr_eq((char *)getAt(list, 1)->data, "position-a.com");
    ck_assert_pstr_eq((char *)getAt(list, 2)->data, "position-b.com");
    ck_assert_pstr_eq((char *)getAt(list, 3)->data, "position-c.com");
    ck_assert_pstr_eq((char *)getAt(list, 4)->data, "position-d.com");
    ck_assert_pstr_eq((char *)getAt(list, 5)->data, "position-f.com");
    ck_assert_pstr_eq((char *)getAt(list, 6)->data, "position-h.com");
    ck_assert_int_eq(list->size, 6);

    insertAt(list, (void*)pos_e, 5);
    insertAt(list, (void*)pos_g, 7);
    ck_assert_pstr_eq((char *)getAt(list, 1)->data, "position-a.com");
    ck_assert_pstr_eq((char *)getAt(list, 2)->data, "position-b.com");
    ck_assert_pstr_eq((char *)getAt(list, 3)->data, "position-c.com");
    ck_assert_pstr_eq((char *)getAt(list, 4)->data, "position-d.com");
    ck_assert_pstr_eq((char *)getAt(list, 5)->data, "position-e.com");
    ck_assert_pstr_eq((char *)getAt(list, 6)->data, "position-f.com");
    ck_assert_pstr_eq((char *)getAt(list, 7)->data, "position-g.com");
    ck_assert_pstr_eq((char *)getAt(list, 8)->data, "position-h.com");
    ck_assert_int_eq(list->size, 8);

} END_TEST
START_TEST (test_ds_removals) {
    
    llist* list;
    list = create_list();

    dnshost* pos_a = malloc(sizeof(dnshost));
    dnshost* pos_b = malloc(sizeof(dnshost));
    dnshost* pos_c = malloc(sizeof(dnshost));
    dnshost* pos_d = malloc(sizeof(dnshost));
    dnshost* pos_e = malloc(sizeof(dnshost));
    dnshost* pos_f = malloc(sizeof(dnshost));
    dnshost* pos_g = malloc(sizeof(dnshost));
    dnshost* pos_h = malloc(sizeof(dnshost));

    strcpy(pos_a->DNSProviderName, "position-a.com");
    strcpy(pos_b->DNSProviderName, "position-b.com");
    strcpy(pos_c->DNSProviderName, "position-c.com");
    strcpy(pos_d->DNSProviderName, "position-d.com");
    strcpy(pos_e->DNSProviderName, "position-e.com");
    strcpy(pos_f->DNSProviderName, "position-f.com");
    strcpy(pos_g->DNSProviderName, "position-g.com");
    strcpy(pos_h->DNSProviderName, "position-h.com");

    insertLast(list, (void*)pos_f);
    insertLast(list, (void*)pos_h);
    insertFirst(list, (void*)pos_b);
    insertFirst(list, (void*)pos_a);
    insertAt(list, (void*)pos_c, 3);
    insertAt(list, (void*)pos_d, 4);
    insertAt(list, (void*)pos_e, 5);
    insertAt(list, (void*)pos_g, 7);
    ck_assert_pstr_eq((char *)getAt(list, 1)->data, "position-a.com");
    ck_assert_pstr_eq((char *)getAt(list, 2)->data, "position-b.com");
    ck_assert_pstr_eq((char *)getAt(list, 3)->data, "position-c.com");
    ck_assert_pstr_eq((char *)getAt(list, 4)->data, "position-d.com");
    ck_assert_pstr_eq((char *)getAt(list, 5)->data, "position-e.com");
    ck_assert_pstr_eq((char *)getAt(list, 6)->data, "position-f.com");
    ck_assert_pstr_eq((char *)getAt(list, 7)->data, "position-g.com");
    ck_assert_pstr_eq((char *)getAt(list, 8)->data, "position-h.com");
    ck_assert_int_eq(list->size, 8);

    removeAt(list, 8);
    removeAt(list, 1);
    ck_assert_pstr_eq((char *)getAt(list, 1)->data, "position-b.com");
    ck_assert_pstr_eq((char *)getAt(list, 2)->data, "position-c.com");
    ck_assert_pstr_eq((char *)getAt(list, 3)->data, "position-d.com");
    ck_assert_pstr_eq((char *)getAt(list, 4)->data, "position-e.com");
    ck_assert_pstr_eq((char *)getAt(list, 5)->data, "position-f.com");
    ck_assert_pstr_eq((char *)getAt(list, 6)->data, "position-g.com");
    ck_assert_int_eq(list->size, 6);

    removeAt(list, 4);
    ck_assert_pstr_eq((char *)getAt(list, 1)->data, "position-b.com");
    ck_assert_pstr_eq((char *)getAt(list, 2)->data, "position-c.com");
    ck_assert_pstr_eq((char *)getAt(list, 3)->data, "position-d.com");
    ck_assert_pstr_eq((char *)getAt(list, 4)->data, "position-f.com");
    ck_assert_pstr_eq((char *)getAt(list, 5)->data, "position-g.com");
    ck_assert_int_eq(list->size, 5);

    //clear(list);
    removeAt(list, 1);
    removeAt(list, 1);
    removeAt(list, 1);
    removeAt(list, 1);
    removeAt(list, 1);
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