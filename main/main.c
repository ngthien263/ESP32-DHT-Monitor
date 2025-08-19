// /**
//  * @file main.c
//  * @brief Entry point of ESP32 DHT22 monitoring application.
//  *
//  * This application demonstrates:
//  * - Reading data from DHT22 sensor.
//  * - Logging data to console.
//  * - Sending sensor data via HTTP client.
//  * - Running in FreeRTOS tasks with observer–notify pattern.
//  */

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "rom/ets_sys.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "dht22.h"
//#include "logger.h"
#include "wifi.h"
#include "http_client.h"
#define DHT22_PORT_NUM  4
// /**
//  * @brief Task to periodically read sensor data.
//  *
//  * Continuously reads temperature and humidity from DHT22 and
//  * triggers notify to subscribers every 2 seconds.
//  *
//  * @param pvParameters Pointer to dht22_t sensor instance.
//  */
// // void read_sensor1_task(void* pvParameters) {
// //     dht22_t* sensor = (dht22_t*) pvParameters;
// //     while (1) {
// //         dht22_get_data(sensor);
// //         vTaskDelay(pdMS_TO_TICKS(2000)); 
// //     }
// // }
// // /**
// //  * @brief Task to log sensor data when notified.
// //  *
// //  * Waits for notification from DHT22, retrieves latest values,
// //  * and logs them using ESP_LOG.
// //  *
// //  * @param pvParameter Pointer to logger_t context.
// //  */
// // void logger_task(void* pvParameter){
// //     logger_t* task_ctx = (logger_t*)pvParameter;
// //     task_ctx->logger_ctx.task_handle = xTaskGetCurrentTaskHandle();
// //     while(1){
// //         ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
// //         dht22_update_data(task_ctx->logger_ctx.sensor, &task_ctx->data);
// //         ESP_LOGI("[Logger task]","Temp: %.2f, Humid: %.2f\n", task_ctx->data.temperature, task_ctx->data.humidity);
// //     }
// // }
// // /* @brief Task to send sensor data via HTTP POST.
// //  *
// //  * Waits for notify from DHT22, then POSTs temperature and humidity
// //  * to configured HTTP server.
// //  *
// //  * @param pvParameter Pointer to http_client_t context.
// //  */
// // void http_client_task(void* pvParameter){
// //     http_client_t* http_client_ctx =  (http_client_t*)pvParameter;
// //     http_client_ctx->client_ctx.task_handle = xTaskGetCurrentTaskHandle();
// //     while(1){
// //         ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
// //         dht22_update_data(http_client_ctx->client_ctx.sensor, &http_client_ctx->data);
// //         ESP_LOGI("HTTP_CLIENT", "Sending: temp=%.2f, humid=%.2f",
// //                  http_client_ctx->data.temperature,
// //                  http_client_ctx->data.humidity);
// //         http_client_post(http_client_ctx, 
// //                          http_client_ctx->data.temperature, 
// //                          http_client_ctx->data.humidity);
// //     }
// // }

 void app_main() {  
    //Initialize non-volatile storage (NVS). Required by WiFi and other ESP-IDF components to store config.*/
    nvs_flash_init();  
    //Initialize network interface and default event loop. Required for WiFi and TCP/IP stack to work. //
    esp_netif_init();
    esp_event_loop_create_default();
    //Create a DHT22 sensor instance.
    subject_t* sensor1 = dht22_subject(dht22_create());
    sensor1->ops->cleanup(sensor1->self);
    // Configure WiFi in Access Point (AP) mode.
    wifi_setup_t wsetup = {
        .mode = WIFI_MODE_AP,
        .wconfig = {
            .ap = {
                .ssid = "ESP32",
                .password = "123456789",
                .authmode =  WIFI_AUTH_WPA2_PSK,
                .max_connection = 2
            },
        },
    };
    wifi_init_ap(&wsetup);
//     // //Initialize HTTP client and subscribe it to sensor updates.
//     // http_client_t http_client = {0}; 
//     // http_client.client_ctx.sensor = sensor1;
//     // http_client_begin(&http_client, "http://192.168.4.2:8000/sensor");
//     // http_client_subscribe(&http_client);
//     // //Create FreeRTOS Task. Call logger and http task first to wait for sensor notify 
//     // xTaskCreate(http_client_task, "http_client", 4096, &http_client, 5, NULL);
//     // xTaskCreate(logger_task, "logger", 2048, &logger1, 5, NULL);
//     // xTaskCreate(read_sensor1_task, "read_sensor1", 2048, sensor1, 5, NULL);  
//     // while (1) {
//     //     vTaskDelay(portMAX_DELAY);
//     // }
//     // //Cleanup resources
//     // esp_http_client_cleanup(http_client.client_handle);
//     dht22_cleanup(sensor1);
 }