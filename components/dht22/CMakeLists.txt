idf_component_register( SRCS "source/dht22.c" "source/dht22_private.c"
                        INCLUDE_DIRS "include"
                        REQUIRES common
                        PRIV_REQUIRES esp_driver_gpio esp_timer
)