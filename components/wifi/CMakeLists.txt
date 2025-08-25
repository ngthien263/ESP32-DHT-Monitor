idf_component_register(SRCS "source/wifi.c" "source/wifi_cred.c" 
                    INCLUDE_DIRS "include"
                    REQUIRES esp_netif  esp_wifi nvs_flash)
