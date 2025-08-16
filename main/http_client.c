/**
 * @file http_client.c
 * @brief Implementation of HTTP client wrapper for ESP-IDF.
 */

#include "http_client.h"
#include "esp_log.h"
#include <inttypes.h>
#include "dht22.h"

static const char *TAG = "HTTP_CLIENT";

void http_client_begin(http_client_t* client, char* url){
    if (!client) return;
    memset(&client->client_config, 0, sizeof(esp_http_client_config_t));
    if(!client->header_key)    client->header_key = "Content-Type";  
    if(!client->header_value)  client->header_value = "application/json";
 
    client->client_config.url = url;
    client->client_handle = esp_http_client_init(&client->client_config);
}


void http_client_post(http_client_t* client, float temperature, float humidity) {
    esp_http_client_set_method(client->client_handle, HTTP_METHOD_POST);
    esp_http_client_set_header(client->client_handle, client->header_key, client->header_value);

    char post_data[64];
    snprintf(post_data, sizeof(post_data), "{\"temp\":%.2f,\"humid\":%.2f}", temperature, humidity);
    esp_http_client_set_post_field(client->client_handle, post_data, strlen(post_data));

    esp_err_t err = esp_http_client_perform(client->client_handle);
    if (err == ESP_OK) {
        int status = esp_http_client_get_status_code(client->client_handle);
        ESP_LOGI(TAG,"[HTTP POST] Status: %d", status);
    } else {
        ESP_LOGE(TAG,"[HTTP POST] Failed: %s", esp_err_to_name(err));
        esp_http_client_cleanup(client->client_handle);
        client->client_handle = NULL;
        memset(&client->client_config, 0, sizeof(esp_http_client_config_t));
        client->client_config.url = client->url; 
        client->client_handle = esp_http_client_init(&client->client_config);
    }
}

void http_client_get(http_client_t* client, char* buff, size_t len) {
    esp_http_client_set_method(client->client_handle, HTTP_METHOD_GET);
    esp_http_client_set_header(client->client_handle, client->header_key, client->header_value);
    esp_err_t err = esp_http_client_perform(client->client_handle);
    if (err == ESP_OK){
        ESP_LOGI(TAG, "[HTTP GET] Status: %d\n, content length = %"PRId64 , 
                       esp_http_client_get_status_code(client->client_handle),
                       esp_http_client_get_content_length(client->client_handle));
        int content_length = esp_http_client_get_content_length(client->client_handle);
        if(content_length > 0 && content_length < len){
            int read_len = esp_http_client_read(client->client_handle, buff, len - 1);
            if(read_len > 0){
                buff[read_len] = '\0';
                ESP_LOGI(TAG, "[HTTP GET] Body: %s", buff);
            }
        }
    } else {
        ESP_LOGE(TAG,"[HTTP GET] Failed: %s", esp_err_to_name(err));
    }
}

static void http_client_on_notify(void* instance){
    http_client_t* ctx = (http_client_t*)instance;
    if (ctx && ctx->client_ctx.task_handle != NULL) {
        xTaskNotifyGive(ctx->client_ctx.task_handle);
    } else {
        ESP_LOGE(TAG, "Task handle NULL");
    }
}

void http_client_subscribe(http_client_t* client) {
    dht22_subscribe(client->client_ctx.sensor, client, http_client_on_notify);
}

void http_client_clean(http_client_t* client) {
    esp_http_client_cleanup(client->client_handle);
}