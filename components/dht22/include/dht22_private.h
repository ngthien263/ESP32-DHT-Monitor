/**
 * @file dht22_private.h
 * @brief Internal low-level functions for DHT22 sensor communication.
 */
#ifndef DHT22_PRIVATE_H
#define DHT22_PRIVATE_H
#include "dht22_types.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Send start signal to the DHT22 sensor
 *
 *
 * @return DHT22_OK: Success
 */
dht22_error_t __start_signal();

/**
 * @brief Wait for response signal from DHT22.
 *
 *
 * @return DHT22_OK: Success
 *         DHT22_ERROR_TIMEOUT: The signal was not received in time
 */
dht22_error_t __wait_respond_signal();

/**
 * @brief Read raw 40-bit data from the DHT22 sensor.
 *
 * 
 * @param[out] data Pointer to output structure storing temp/humidity.
 * @return DHT22_OK: Success
 *         DHT22_ERROR_TIMEOUT: The signal was not received in time
 *         DHT22_ERROR_CHECKSUM: Checksum false.
 */
dht22_error_t __read_data(dht22_data_t* data);

#ifdef __cplusplus
}
#endif

#endif // DHT22_PRIVATE_H