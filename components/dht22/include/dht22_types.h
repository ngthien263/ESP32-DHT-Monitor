#ifndef DHT22_TYPES_H
#define DHT22_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "common_subject.h" 

#ifndef DHT22_PORT_NUM
#define DHT22_PORT_NUM 4
#endif

typedef struct dht22_data_t {
    float temperature; 
    float humidity; 
} dht22_data_t;

typedef struct {
    subject_t subject;                         
    SemaphoreHandle_t mtx;                     
    dht22_data_t data;                         
    notification_handle_t* subscribers[MAX_SUBSCRIBERS]; 
} dht22_t;


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