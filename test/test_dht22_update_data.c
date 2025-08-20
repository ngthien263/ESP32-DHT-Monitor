#include "unity.h"
#include "mock_freertos.h"
#include "dht22.h"

void setUp(){}
void tearDown(){}

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
    return UNITY_END();
}