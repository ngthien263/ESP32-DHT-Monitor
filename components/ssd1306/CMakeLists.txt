idf_component_register(SRCS "source/ssd1306.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_driver_i2c common dht22) 
