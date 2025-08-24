#include "mock_freertos.h"
#include "unity.h"
#include "mock_gpio.h"
#include "common_subject.h"

typedef subject_t* (*subject_create_fnc)();
typedef void (*subject_destroy_fnc)(subject_t*);

int run_subject_test_main(subject_create_fnc create, subject_destroy_fnc destroy);