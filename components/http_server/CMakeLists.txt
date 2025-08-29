idf_component_register(SRCS "source/http_server.c" 
                    INCLUDE_DIRS "include"
                    REQUIRES esp_netif  esp_wifi esp_http_server wifi)
target_add_binary_data(${COMPONENT_TARGET} "index.html" TEXT)
