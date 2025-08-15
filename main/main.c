#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "rom/ets_sys.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "dht22.h"
#include "logger.h"
#include "wifi.h"
#define DHT22_PORT_NUM  4

//Sensor task will read sensor then notify subscribers when the data is ready
void read_sensor1_task(void* pvParameters) {
    dht22_t* sensor = (dht22_t*) pvParameters;
    while (1) {
        dht22_get_data(sensor);
        vTaskDelay(pdMS_TO_TICKS(2000)); 
    }
}

//Logger task wait for notify from sensor that the data is ready to get
void logger_task(void* pvParameter){
    printf("[In Logger]\n");
    logger_t* task_ctx = (logger_t*)pvParameter;
    task_ctx->logger_ctx.task_handle = xTaskGetCurrentTaskHandle();
    while(1){
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        dht22_update_data(task_ctx->logger_ctx.sensor, &task_ctx->data);
        printf("[Logger task] Temp: %.2f, Humid: %.2f\n", task_ctx->data.temperature, task_ctx->data.humidity);
    }
}

void app_main() {
    nvs_flash_init();  
    esp_netif_init();
    esp_event_loop_create_default();

    dht22_t* sensor1 = dht22_create();

    logger_t logger1 = {
        .data.humidity = 0,
        .data.temperature = 0,
    };
    logger1.logger_ctx.sensor = sensor1;
    logger1.logger_ctx.task_handle = xTaskGetCurrentTaskHandle();
    logger_subscribe(&logger1);

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
    //Call logger task first to wait for sensor notify 
    //xTaskCreate(logger_task, "logger", 2048, &logger1, 5, NULL);
    //xTaskCreate(read_sensor1_task, "read_sensor1", 2048, sensor1, 5, NULL);
    
    while (1) {
        vTaskDelay(portMAX_DELAY);
    }
    dht22_cleanup(sensor1);
}