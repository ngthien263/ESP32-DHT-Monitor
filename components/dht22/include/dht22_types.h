/**
 * @file dht22_types.h
 * @brief Core data types for DHT22 driver and Subject interface glue.
 */
#ifndef DHT22_TYPES_H
#define DHT22_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "common_subject.h"   /**< Defines subject_t, notification_handle_t, MAX_SUBSCRIBERS. */

/** Default GPIO for DHT22 data line. */
#ifndef DHT22_PORT_NUM
#define DHT22_PORT_NUM 4
#endif

/**
 * @brief Struct holds data read from sensor.
 * @note Updated by driver (producer), read by clients (consumers).
 */
typedef struct dht22_data_t {
    float temperature;  /**< Degrees Celsius. */
    float humidity;     /**< Relative humidity in %. */
} dht22_data_t;

/**
 * @brief DHT22 device object (acts as a Subject in the observer pattern).
 *
 * Ownership:
 * - @ref subject : exposes ops + self for polymorphic calls.
 * - @ref mtx     : protects @ref data and @ref subscribers against concurrent access.
 * - @ref data    : last measured values; copied out via update/pull API.
 * - @ref subscribers : fixed-size slots storing client instance + callback.
 */
typedef struct {
    subject_t subject;                           /**< Subject interface (ops + self back-pointer). */
    SemaphoreHandle_t mtx;                       /**< Mutex guarding data/subscribers. */
    dht22_data_t data;                           /**< Cached latest reading. */
    notification_handle_t* subscribers[MAX_SUBSCRIBERS]; /**< Subscriber slots (NULL if free). */
} dht22_t;

/**
 * @brief Driver error codes for sampling/IO and synchronization.
 */
typedef enum dht22_error_t {
    DHT22_OK,                   /**< Operation successful. */
    DHT22_ERROR_TIMEOUT,        /**< Timer timed out. */
    DHT22_ERROR_CHECKSUM,       /**< Frame received but checksum invalid. */
    DHT22_ERROR_NO_RESPONSE,    /**< Sensor did not pull line as expected. */
    DHT22_ERROR_NULL_POINTER,   /**< API received a NULL pointer argument. */
    DHT22_ERROR_IN_PROGRESS,    /**< Operation already running / busy. */
    DHT22_ERROR_MUTEX_TIMEOUT   /**< Could not acquire mutex in time. */
} dht22_error_t;

#ifdef __cplusplus
}
#endif
#endif /* DHT22_TYPES_H */
