idf_component_register(SRCS "source/http_client.c"
                       INCLUDE_DIRS "include"
                       REQUIRES dht22 common
                       PRIV_REQUIRES esp_http_client)
