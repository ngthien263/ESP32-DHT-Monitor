#ifndef WIFI_H
#define WIFI_H

#include "esp_netif.h"
#include "esp_wifi.h"
#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
    wifi_config_t wconfig;
    wifi_mode_t mode;
} wifi_setup_t;


void wifi_init_ap(wifi_setup_t* wsetup);
void wifi_init_sta(wifi_setup_t* wsetup);

#ifdef __cplusplus
}
#endif

#endif // WIFI_H
