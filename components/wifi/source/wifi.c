#ifndef UNIT_TEST
#include "wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "string.h"
#include "esp_log.h"
#include "esp_mac.h"

#define WIFI_NAMESPACE "wifi"
static const char *TAG_AP = "wifi softAP";
static const char *TAG_STA = "wifi STA";
static int s_retry_num = 0;
EventGroupHandle_t s_wifi_event_group = NULL;
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
        if (s_retry_num < 5) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG_STA, "retry to connect to the AP");
        } else {
            xEventGroupSetBits(s_wifi_event_group, BIT0);
        }
        ESP_LOGI(TAG_STA,"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG_STA, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        
        xEventGroupSetBits(s_wifi_event_group, BIT1);
    }
}


void wifi_init(){
    ESP_ERROR_CHECK(esp_netif_init()); 
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t init_config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&init_config);
    s_wifi_event_group = xEventGroupCreate();
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL);
}

void wifi_destroy(){
    esp_wifi_stop();
    esp_wifi_deinit();
    esp_netif_deinit();
    esp_event_loop_delete_default();
    vEventGroupDelete(s_wifi_event_group);
}

void wifi_start_mode(wifi_mode_t mode, const char* ssid, const char* password){
    wifi_config_t wconfig;
    memset(&wconfig, 0, sizeof(wconfig));
    if(mode == WIFI_MODE_AP) {
        esp_netif_create_default_wifi_ap();
        strcpy((char*)wconfig.ap.ssid, ssid);
        strcpy((char*)wconfig.ap.password, password);
        wconfig.ap.authmode = WIFI_AUTH_WPA2_PSK;
        wconfig.ap.max_connection = 2;
        esp_wifi_set_mode(mode);
        esp_wifi_set_config(WIFI_IF_AP, &wconfig);
    } else if (mode == WIFI_MODE_STA) {
        strcpy((char*)wconfig.sta.ssid, ssid);
        strcpy((char*)wconfig.sta.password, password);
        wconfig.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
        esp_wifi_set_mode(mode);
        esp_wifi_set_config(WIFI_IF_STA, &wconfig);
    }
    esp_wifi_start();
}

void wifi_start_config(wifi_config_t* wconfig, wifi_mode_t mode){
    esp_wifi_set_mode(mode);
    if(mode == WIFI_MODE_AP){
        esp_wifi_set_config(WIFI_IF_AP, wconfig);
    } else if (mode == WIFI_MODE_STA) {
        esp_wifi_set_config(WIFI_IF_STA, wconfig);
    }
    esp_wifi_start();
}

bool wifi_wait_connected(uint32_t timeoutms) {
    if (!s_wifi_event_group) return false;
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group, 
                                           BIT0 | BIT1, 
                                           pdFALSE, pdFALSE, 
                                           pdMS_TO_TICKS(timeoutms));
    return (bits & BIT1);
    
}                                     

void wifi_cred_save(const char* ssid, const char* pass) {
    nvs_handle_t nvs;
    esp_err_t err = nvs_open(WIFI_NAMESPACE, NVS_READWRITE, &nvs);
    if(err != ESP_OK)   return ;
    nvs_set_str(nvs, "ssid", ssid);
    nvs_set_str(nvs, "pass", pass);
    nvs_commit(nvs);
    nvs_close(nvs);
}

void wifi_cred_load(char* ssid_out, size_t* ssid_len, char* pass_out, size_t* pass_len) {
    nvs_handle_t nvs;
    esp_err_t err = nvs_open(WIFI_NAMESPACE, NVS_READONLY, &nvs);
    if (err != ESP_OK) {
        printf( "Open failed: %s", esp_err_to_name(err));
        return;
    }

    err = nvs_get_str(nvs, "ssid", ssid_out, ssid_len);
    if (err != ESP_OK) {
        printf("Get SSID failed: %s", esp_err_to_name(err));
        nvs_close(nvs);
        return;
    }
    printf( "SSID = %s", ssid_out);

    err = nvs_get_str(nvs, "pass", pass_out, pass_len);
    if (err != ESP_OK) {
        printf( "Get PASS failed: %s", esp_err_to_name(err));
        nvs_close(nvs);
        return;
    }
    printf("PASS = %s", pass_out);

    nvs_close(nvs);
}


void wifi_cred_clear(void) {
    nvs_handle_t nvs;
    esp_err_t err = nvs_open(WIFI_NAMESPACE, NVS_READWRITE, &nvs);
    if (err != ESP_OK) return ;
    nvs_erase_key(nvs, "ssid");
    nvs_erase_key(nvs, "pass");
    err = nvs_commit(nvs);
    nvs_close(nvs);
}

#endif