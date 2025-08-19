idf_component_register(SRCS "main.c"
                        REQUIRES dht22 wifi http_client nvs_flash esp_driver_gpio esp_timer esp_http_client)