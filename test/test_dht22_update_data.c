#include "unity.h"
#include "mock_freertos.h"
#include "dht22.h"
#include <stdlib.h>

static int callback_called = 0;

void mock_subscriber_callback(void* instance){

}

void mock_notify_callback(void* instance) {
    callback_called++;
}

void setUp() {callback_called = 0;}
void tearDown() {}

void test_dht22_subscribe(){
    dht22_data_t data = {.temperature = 31.5, .humidity = 50.5};
    dht22_t sensor = {
        .data = data,
        .mtx = xSemaphoreCreateMutex(),
    };
    void* subscribers[5];
    dht22_init(&sensor);
    for(int i = 0; i < 5; i++){
        subscribers[i] = (void*)((uintptr_t)i);
        dht22_subscribe(&sensor, subscribers[i],  mock_subscriber_callback);
    }
        

    for(int i = 0; i < 5; i++){
        notification_handle_t* slot = sensor.subscribers[i];
        TEST_ASSERT_NOT_NULL(slot);
        TEST_ASSERT_EQUAL_PTR((void*)(uintptr_t)i, slot->subscriber_instance);
        TEST_ASSERT_EQUAL_PTR(mock_subscriber_callback, slot->subscriber_update_func);
    }
    
}

void test_dht22_notify(){
    dht22_data_t data = {.temperature = 31.5, .humidity = 50.5};
    dht22_t sensor = {
        .data = data,
        .mtx = xSemaphoreCreateMutex(),
    };
    dht22_init(&sensor);
    dht22_subscribe(&sensor, NULL, mock_notify_callback);
    dht22_notify(&sensor);

    TEST_ASSERT_EQUAL(1, callback_called);
}

void test_update_data_copy_value_correctly(){
    dht22_data_t data = {.temperature = 31.5, .humidity = 50.5};
    dht22_t sensor = {
        .data = data,
        .mtx = xSemaphoreCreateMutex(),
    };

    dht22_update_data(&sensor, &data);

    TEST_ASSERT_FLOAT_WITHIN(0.01, 31.5, data.temperature);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 50.5, data.humidity);
}

int main(){
    UNITY_BEGIN();
    RUN_TEST(test_update_data_copy_value_correctly);
    RUN_TEST(test_dht22_subscribe);
    RUN_TEST(test_dht22_notify);
    return UNITY_END();
}