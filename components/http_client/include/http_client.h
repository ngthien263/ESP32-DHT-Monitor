/**
 * @file http_client.h
 * @brief Wrapper for ESP-IDF HTTP client with observer pattern support.
 */
#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#ifndef UNIT_TEST
#include "esp_http_client.h"


#include "common_observer.h"
#include "dht22.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct http_client_t
 * @brief Encapsulates HTTP client state and config.
 */
typedef struct {
    esp_http_client_config_t client_config; /** HTTP client config. */
    esp_http_client_handle_t client_handle; /** HTTP client handle. */
    observer_t base_observer;               /** Observer context. */
    dht22_data_t data;                      /** Sensor data. */
    char* url;                              /** Target URL. */
    char* header_key;                       /**< HTTP header key (default: Content-Type). */
    char* header_value;                     /**< HTTP header value (default: application/json). */
} http_client_t;

/**
 * @brief Initialize HTTP client with given URL.
 * @param client Pointer to HTTP client instance.
 * @param url Target server URL.
 */
void http_client_init(http_client_t* client, char* url);

/**
 * @brief Perform HTTP POST with sensor data.
 * @param client Pointer to HTTP client instance.
 * @param temperature Sensor temperature value.
 * @param humidity Sensor humidity value.
 */
void http_client_post(http_client_t* client, float temperature, float humidity);

/**
 * @brief Perform HTTP GET and read response.
 * @param client Pointer to HTTP client instance.
 * @param buff Buffer to store response body.
 * @param len Buffer length.
 */
void http_client_get(http_client_t* client, char* buff, size_t len);


/**
 * @brief Set the subject (observable sensor) for the HTTP client.
 * 
 * This assigns the subject_t pointer to the internal observer context.
 * It must be called before subscribing to receive notifications.
 *
 * @param client Pointer to HTTP client instance.
 * @param subject Pointer to observable subject (e.g. a DHT22 sensor).
 */
void http_client_set_subject(http_client_t* client, subject_t* subject);

/**
 * @brief Subscribe HTTP client to subject notifications.
 * 
 * When the subject notifies, the client's observer will be notified via task notify.
 * Requires subject to be set beforehand via http_client_set_subject().
 *
 * @param client Pointer to HTTP client instance.
 */
void http_client_subscribe(http_client_t* client);

/**
 * @brief Unsubscribe HTTP client from subject notifications.
 *
 * This removes the client from the subject's subscriber list, stopping further updates.
 * Does nothing if the client was not previously subscribed.
 *
 * @param client Pointer to HTTP client instance.
 */
void http_client_unsubscribe(http_client_t* client);

/**
 * @brief Clean up HTTP client resources.
 * @param client Pointer to HTTP client instance.
 */
void http_client_cleanup(http_client_t* client);


#ifdef __cplusplus
}
#endif
#endif
#endif
