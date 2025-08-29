#include "test_subject.h"

static subject_create_fnc create = NULL;
static subject_destroy_fnc destroy = NULL;
subject_t* s = NULL;
static int callback_called[100];

static void mock_callback(void* ctx) {
    int id = (int)(uintptr_t)ctx;
    callback_called[id]++;
}


void setUp(){
    s = create();
}

void tearDown(){
    destroy(s);
}

void test_subject_subscribe(void) {
    for (int i = 0; i < MAX_SUBSCRIBERS; i++) {
        callback_called[i] = 0;
        void* ctx = (void*)(uintptr_t)i;
        subject_subscribe(s, ctx, mock_callback);
    }

    subject_notify(s);  

    for (int i = 0; i < MAX_SUBSCRIBERS; i++) {
        TEST_ASSERT_EQUAL(1, callback_called[i]); 
    }
}

void test_subject_unsubscribe(void) {
    for (int i = 0; i < 100; i++) callback_called[i] = 0;

    for (int i = 0; i < 10; i++) {
        subject_subscribe(s, (void*)(uintptr_t)i, mock_callback);
    }

    subject_unsubscribe(s, mock_callback);
    subject_notify(s);

    for (int i = 0; i < 10; i++) {
        TEST_ASSERT_EQUAL(0, callback_called[i]);
    }
}


void test_subject_notify(void) {
    for (int i = 0; i < 100; i++) callback_called[i] = 0;

    for (int i = 0; i < 10; i++) {
        void* ctx = (void*)(uintptr_t)i;
        subject_subscribe(s, ctx, mock_callback);
    }

    subject_notify(s);

    for (int i = 0; i < 10; i++) {
        TEST_ASSERT_EQUAL(1, callback_called[i]);
    }
}

int run_subject_test_main(subject_create_fnc create_fnc, subject_destroy_fnc destroy_fnc){
    create = create_fnc;
    destroy  = destroy_fnc;
    UNITY_BEGIN();
    RUN_TEST(test_subject_subscribe);
    RUN_TEST(test_subject_unsubscribe);
    RUN_TEST(test_subject_notify);
    return UNITY_END();
}