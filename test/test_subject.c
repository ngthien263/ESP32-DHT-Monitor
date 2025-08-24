#include "test_subject.h"
#include "dht22.h"
static subject_create_fnc create = NULL;
static subject_destroy_fnc destroy = NULL;
subject_t* s = NULL;
static int called = 0;

static void mock_callback(void* instance) { called++; }


void setUp(){
    s = create();
    called = 0;
}

void tearDown(){
    destroy(s);
}

void test_subject_subscribe() {
    void* subscriber[100];
    for(int i = 0; i < MAX_SUBSCRIBERS; i++){
        subscriber[i] = (void*)((uintptr_t)i);
        subject_subscribe(s, subscriber[i], mock_callback);
        dht22_t* dht22 = (dht22_t*)s->self;
        notification_handle_t* temp = (notification_handle_t*)dht22->subject.subscribers[i];
        TEST_ASSERT_NOT_NULL(temp);
        TEST_ASSERT_EQUAL_PTR((void*)(uintptr_t)i, temp->instance);
        TEST_ASSERT_EQUAL_PTR(mock_callback, temp->callback);
    }
}

void test_subject_unsubscribe(){
    for(int i = 0; i < 10; i++)
        subject_subscribe(s, NULL, mock_callback);
    subject_unsubscribe(s, mock_callback);
    subject_notify(s);
    TEST_ASSERT_EQUAL(0, called);
}

void test_subject_notify() {
    for(int i = 0; i < 10; i++)
        subject_subscribe(s, NULL, mock_callback);
    subject_notify(s);
    TEST_ASSERT_EQUAL(10, called);
}

void test_update_data() {
    
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