#ifndef DHT22_TYPES_H
#define DHT22_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "notification_handle.h"
#define DHT22_MAX_SUBSCRIBERS 100

typedef struct dht22_data_t {
    float temperature; 
    float humidity; 
} dht22_data_t;

typedef struct dht22_t {
    SemaphoreHandle_t mtx;
    dht22_data_t data;
    notification_handle_t* subscribers[DHT22_MAX_SUBSCRIBERS];
} dht22_t;

typedef struct {
    dht22_t* sensor;          
    TaskHandle_t task_handle;
} observer_ctx_t;

typedef enum dht22_error_t {
    DHT22_OK,
    DHT22_ERROR_TIMEOUT,
    DHT22_ERROR_CHECKSUM,
    DHT22_ERROR_NO_RESPONSE,
    DHT22_ERROR_NULL_POINTER,
    DHT22_ERROR_IN_PROGRESS,
    DHT22_ERROR_MUTEX_TIMEOUT
} dht22_error_t;



#ifdef __cplusplus
}
#endif
#endif