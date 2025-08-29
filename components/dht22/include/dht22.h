#ifndef DHT22_H
#define DHT22_H

#ifndef UNIT_TEST
#include "freertos/semphr.h"
#else   
#include "mock_freertos.h"
#endif

#include "dht22_types.h"        

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DHT22_PORT_NUM
#define DHT22_PORT_NUM 4
#endif



/** 
 * @brief Create a new DHT22 instance and return its Subject interface.
 *
 *
 * @return subject_t* Pointer to subject interface, or NULL on allocation failure.
 */
subject_t* dht22_create(void);

/**
 * @brief Trigger a sensor measurement and update internal data.
 *
 *
 * @param[in,out] self DHT22 instance handle.
 * @retval DHT22_OK          Success.
 * @retval DHT22_ERROR_TIMEOUT Timeout waiting for sensor or mutex.
 */

 /**
 * @brief Destroy a DHT22 instance.
 *
 *
 * @param[in,out] subject Subject interface pointer returned by ::dht22_create.
 */
void dht22_destroy(subject_t* subject);

// /**
//  * @brief Copy the measured data to the output 
//  *
//  *
//  * @param[in]  self DHT22 instance handle.
//  * @param[out] out  Destination buffer for sensor data (must not be NULL).
//  */
// void dht22_update_data(dht22_t* self, dht22_data_t* out);

dht22_error_t dht22_get_data(dht22_t* self);
#ifdef __cplusplus
}
#endif

#endif // DHT22_H
