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

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_crt_bundle.h"
#include "esp_https_ota.h"
#include "nvs_flash.h"
#include "wifi.h"
#include "http_server.h"
#include "http_client.h"
#include "dht22.h"
#include "ssd1306.h"
#define DHT22_PORT_NUM  4

extern const uint8_t server_cert_pem_start[] asm("_binary_cert_pem_start");
extern const uint8_t server_cert_pem_end[] asm("_binary_cert_pem_end");

/**
 * @brief Task to periodically read sensor data.
 *
 * Continuously reads temperature and humidity from DHT22 and
 * triggers notify to subscribers every 2 seconds.
 *
 * @param pvParameters Pointer to dht22_t sensor instance.
 */
void read_sensor_task(void* pvParameters) {
    dht22_t* sensor = (dht22_t*) pvParameters;
    while (1) {
        dht22_get_data(sensor);
        printf("sensor task: %f %f\n", sensor->data.humidity, sensor->data.temperature);
        vTaskDelay(pdMS_TO_TICKS(2000)); 
    }
}

typedef struct logger {
    observer_t base_observer;                   /** Observer context. */
    dht22_data_t data;                          /** Sensor data. */
} logger_t;

void logger_on_notify(void* instance) {
    logger_t* ctx = (logger_t*)instance;
    if (ctx && ctx->base_observer.ctx.task_handle != NULL) {
        xTaskNotifyGive(ctx->base_observer.ctx.task_handle);
    } else {
        printf("Logger notify: task handle NULL!\n");
    }
}

void logger_task(void* pvParameter){
    logger_t* task_ctx = (logger_t*)pvParameter;
    task_ctx->base_observer.ctx.task_handle = xTaskGetCurrentTaskHandle();
    printf("Logger subject:%p ", task_ctx->base_observer.ctx.subject);
    while(1){
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        subject_update_data(task_ctx->base_observer.ctx.subject, &task_ctx->data);
        ESP_LOGI("[Logger task]","Temp: %.2f, Humid: %.2f\n", task_ctx->data.temperature, task_ctx->data.humidity);
    }
}


void ssd1306_display_task(void* pvParameter){
    ssd1306_i2c_module* ssd = (ssd1306_i2c_module*)pvParameter;
    ssd->base_observer.ctx.task_handle = xTaskGetCurrentTaskHandle();

    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        dht22_data_t data = {0};
        subject_update_data(ssd->base_observer.ctx.subject, ssd->);
        ssd1306_drawString(ssd, 0, 0, "50.5");
        ssd1306_drawString(ssd, 0, 10, "20.5");
        ssd1306_display(ssd);
    }
}

void ota_task(void* param) {
    while (1) {
        if (wifi_wait_connected(15000)) {
            ESP_LOGI("OTA", "WiFi connected. Starting OTA...");

            esp_http_client_config_t ota_http_config = {
                .url = "https://ota.ngocthien263.id.vn/latest.bin",
                .cert_pem = (const char*)server_cert_pem_start,
                .cert_len = server_cert_pem_end - server_cert_pem_start,
                .timeout_ms = 10000,
            };
            esp_https_ota_config_t ota_config = {
                .http_config = &ota_http_config,
            };

            esp_err_t ret = esp_https_ota(&ota_config);
            if (ret == ESP_OK) {
                ESP_LOGI("OTA", "Update successful. Restarting...");
                esp_restart();
            } else {
                ESP_LOGE("OTA", "OTA failed: %s", esp_err_to_name(ret));
            }

        } else {
            ESP_LOGW("OTA", "Wi-Fi not connected. Skipping OTA.");
            break;
        }
    }

    vTaskDelete(NULL);
}

void app_main() {  
    ESP_ERROR_CHECK(nvs_flash_init());
    wifi_init();
    char ssid[32] = {0}; size_t ssid_len = sizeof(ssid);
    char pass[64] = {0}; size_t pass_len = sizeof(ssid);
    int has_cred = 0;
    wifi_cred_load(ssid, &ssid_len, pass, &pass_len);   
    if(( ssid_len > 0) && (pass_len > 0)) {
        has_cred = 1;
    }
    if(has_cred) {
        ESP_LOGI("MAIN", "Connecting using saved credentials...");
        wifi_start_mode(WIFI_MODE_STA, ssid, pass);
    } else {
        ESP_LOGI("MAIN", "No credentials, start AP for provisioning...");
        wifi_start_mode(WIFI_MODE_AP, "ESP32", "123456789");
        http_server_init();
    }
    subject_t* sensor = dht22_create();
    ssd1306_i2c_module* ssd1306_module = ssd1306_create(0x3D);
    observer_set_subject(&ssd1306_module->base_observer, sensor);
    ssd1306_subscribe(ssd1306_module);

    static logger_t logger;
    logger.base_observer.self = &logger;
    logger.base_observer.ctx.subject = sensor;
    logger.base_observer.on_notify = logger_on_notify;
    observer_set_subject(&logger.base_observer, sensor);
    observer_subscribe(&logger.base_observer);

    xTaskCreate(logger_task, "logger_task", 2048, &logger, 5, NULL);
    xTaskCreate(ssd1306_display_task, "ssd1306_display_task", 2048, &ssd1306_module, 4, NULL);
    xTaskCreate(read_sensor_task, "read_sensor", 2048, sensor, 5, NULL);
    xTaskCreate(ota_task, "ota", 2048, NULL, 1, NULL);
    while(1){
        vTaskDelay(portMAX_DELAY);
    }

    vTaskDelete(NULL);
}