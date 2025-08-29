#include "test_subject.h"
#include "dht22.h"

void test_dht22_update_data(){
    subject_t* subject = dht22_create();
    dht22_data_t data = {
        .humidity = 50.5, .temperature = 30.5
    };
    dht22_t sensor = { 0 };
    sensor.data = data;
    sensor.mtx = xSemaphoreCreateMutex();
    subject->self = &sensor;
    TEST_ASSERT_NOT_NULL(sensor.mtx);
    dht22_data_t new_data;
    subject_update_data(subject, &new_data);
    TEST_ASSERT_FLOAT_WITHIN(0.01, data.humidity, new_data.humidity);
    TEST_ASSERT_FLOAT_WITHIN(0.01, data.temperature, new_data.temperature);
}

int main(){
    run_subject_test_main(dht22_create, dht22_destroy);
    UNITY_BEGIN();
    RUN_TEST(test_dht22_update_data);
    return UNITY_END();
}