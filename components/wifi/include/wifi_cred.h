#ifndef WIFI_CRED_H
#define WIFI__CRED_H
#include "wifi.h"
#include "nvs_flash.h"

void wifi_cred_save(const char* ssid, const char* pass);
void wifi_cred_load(char* ssid_out, size_t* ssid_len, char* pass_out, size_t* pass_len);
void wifi_cred_clear(void);

#endif