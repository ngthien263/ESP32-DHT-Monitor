#ifndef DHT22_TYPES_H
#define DHT22_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"

typedef struct dht22_data_t {
    float temperature; 
    float humidity; 
} dht22_data_t;

typedef enum dht22_error_t {
    DHT22_OK,
    DHT22_ERROR_TIMEOUT,
    DHT22_ERROR_CHECKSUM,
    DHT22_ERROR_NO_RESPONSE,
    DHT22_ERROR_NULL_POINTER,
    DHT22_ERROR_IN_PROGRESS
} dht22_error_t;

typedef enum dht22_state_t {
    DHT22_STATE_IDLE,
    DHT22_STATE_START_SIGNAL,
    DHT22_STATE_WAIT_REPONSE,
    DHT22_STATE_READ_DATA,
    DHT22_STATE_FINISH
} dht22_state_t;

#ifdef __cplusplus
}
#endif
#endif