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
#include "wifi.h"
#include "wifi_cred.h"
#include "http_client.h"
#include "http_server.h"

#define DHT22_PORT_NUM  4

void app_main() {  
   ESP_ERROR_CHECK(nvs_flash_init());
   ESP_ERROR_CHECK(esp_netif_init()); 
   ESP_ERROR_CHECK(esp_event_loop_create_default());
   subject_t* sensor1 = dht22_create();
   http_client_t http_client = {0}; 
   observer_set_subject(&http_client.base_observer, sensor1);
   printf("%p\n", &http_client);
   wifi_setup_t wsetup = {
      .mode = WIFI_MODE_AP,
      .wconfig = {
         .ap = {
            .ssid = "ESP32",
            .password = "123456789",
            .authmode = WIFI_AUTH_WPA2_PSK,
            .max_connection = 2
         }
      }
   };
   wifi_init_ap(&wsetup);
   httpd_handle_t handle = http_server_init();
   char ssid[32];
   size_t ssid_len = sizeof(ssid);
   char pass[64];
   size_t pass_len = sizeof(pass);
   wifi_cred_load(ssid, &ssid_len, pass, &pass_len);
   
   // http_client_init(&http_client, "http://192.168.4.2:8000/sensor");
   // http_client_subscribe(&http_client);
}