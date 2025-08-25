#include "wifi_cred.h"
void wifi_cred_save(const char* ssid, const char* pass) {
    nvs_handle_t nvs;
    esp_err_t err = nvs_open(WIFI_NAMESPACE, NVS_READWRITE, &nvs);
    if(err != ESP_OK)   return ;
    nvs_set_str(nvs, "ssid", ssid);
    nvs_set_str(nvs, "pass", pass);
    nvs_commit(nvs);
    nvs_close(nvs);
}

void wifi_cred_load(char* ssid_out, size_t* ssid_len, char* pass_out, size_t* pass_len)
{
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


void wifi_cred_clear(void)
{
    nvs_handle_t nvs;
    esp_err_t err = nvs_open(WIFI_NAMESPACE, NVS_READWRITE, &nvs);
    if (err != ESP_OK) return ;
    nvs_erase_key(nvs, "ssid");
    nvs_erase_key(nvs, "pass");
    err = nvs_commit(nvs);
    nvs_close(nvs);
}
