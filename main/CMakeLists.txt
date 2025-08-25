idf_component_register(SRCS "main.c"
                        REQUIRES dht22 wifi http_client http_server nvs_flash esp_driver_gpio esp_timer esp_http_client esp_http_server)