/**
 * @file wifi.c
 * @brief WiFi initialization and event handling implementation.
 */

#include "wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_log.h"
#include "esp_mac.h"

static const char *TAG_AP = "wifi softAP";
static const char *TAG_STA = "wifi softAP";
static int s_retry_num = 0;
 EventGroupHandle_t s_wifi_event_group;

/**
 * @brief WiFi event handler for AP/STA connection events.
 *
 * @param arg 
 * @param event_base Event base (WIFI_EVENT or IP_EVENT).
 * @param event_id Event ID.
 * @param event_data Event data structure.
 */
static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG_AP, 
                 "station "    MACSTR     "join, AID=%d", 
                 MAC2STR(event->mac), event->aid);
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG_AP,
                 "station " MACSTR " leave, AID=%d, reason=%d",
                 MAC2STR(event->mac),
                 event->aid,
                 event->reason);
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < 10) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG_STA, "retry to connect to the AP");
        } else {
            xEventGroupSetBits(s_wifi_event_group, 0);
        }
        ESP_LOGI(TAG_STA,"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG_STA, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, 1);
    }
}

/**
 * @brief Initialize ESP32 WiFi as Access Point.
 *
 * Registers event handler and applies provided configuration.
 *
 * @param wsetup Pointer to WiFi setup structure.
 */
void wifi_init_ap(wifi_setup_t* wsetup){
    esp_netif_create_default_wifi_ap();
    wifi_init_config_t init_config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&init_config);
    esp_event_handler_instance_register(WIFI_EVENT,
                                        ESP_EVENT_ANY_ID,
                                        &wifi_event_handler,
                                        NULL,
                                        NULL);
    esp_wifi_set_mode(wsetup->mode);
    esp_wifi_set_config(WIFI_IF_AP, &wsetup->wconfig);
    esp_wifi_start();
}

/**
 * @brief Initialize ESP32 WiFi as Station.
 *
 * Registers event handler, configures STA, and attempts to connect.
 *
 * @param wsetup Pointer to WiFi setup structure.
 */
void wifi_init_sta(wifi_setup_t* wsetup){
    s_wifi_event_group = xEventGroupCreate();  
    assert(s_wifi_event_group != NULL);        
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t init_config = WIFI_INIT_CONFIG_DEFAULT();
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    esp_wifi_init(&init_config);
    esp_event_handler_instance_register(WIFI_EVENT,
                                        ESP_EVENT_ANY_ID,
                                        &wifi_event_handler,
                                        NULL,
                                        &instance_any_id);
    esp_event_handler_instance_register(IP_EVENT,
                                        IP_EVENT_STA_GOT_IP,
                                        &wifi_event_handler,
                                        NULL,
                                        &instance_got_ip);
    esp_wifi_set_mode(wsetup->mode);
    esp_wifi_set_config(WIFI_IF_STA, &wsetup->wconfig);
    esp_wifi_start();
}


void wifi_app_send_msg(char* msg){
    
}