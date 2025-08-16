/**
 * @file http_client.h
 * @brief Wrapper for ESP-IDF HTTP client with observer pattern support.
 */
#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include "esp_http_client.h"
#include "dht22_types.h"
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
    observer_ctx_t client_ctx;              /** Observer context. */
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
void http_client_begin(http_client_t* client, char* url);

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
 * @brief Subscribe HTTP client to sensor notifications.
 * @param client Pointer to HTTP client instance.
 */
void http_client_subscribe(http_client_t* client);

/**
 * @brief Clean up HTTP client resources.
 * @param client Pointer to HTTP client instance.
 */
void http_client_clean(http_client_t* client);
#ifdef __cplusplus
}
#endif

#endif
