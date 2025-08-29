#include "esp_netif.h"
#include "esp_wifi.h"
#include "nvs_flash.h"

#define WIFI_AP_DEFAULT_SSID "ESP32"
#define WIFI_AP_DEFAULT_PASS "123456789"
void wifi_init();
void wifi_destroy();

void wifi_start_mode(wifi_mode_t mode, const char* ssid, const char* password);
void wifi_start_config(wifi_config_t* wconfig, wifi_mode_t mode);
#define wifi_start(first,  ...)\
    _Generic((first), \
                int: wifi_start_mode, \
                wifi_mode_t: wifi_start_mode,\
                wifi_config_t*: wifi_start_config\
            )(first, ##__VA_ARGS__)

void wifi_cred_save(const char* ssid, const char* pass);
void wifi_cred_load(char* ssid_out, size_t* ssid_len, char* pass_out, size_t* pass_len);
void wifi_cred_clear(void);
bool wifi_wait_connected(uint32_t timeoutms);