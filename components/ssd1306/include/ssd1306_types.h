#ifndef SSD1306_TYPES_H
#define SSD1306_TYPES_H

#include "driver/i2c_master.h"

typedef enum {
    MODE_COMMAND = 0x00,
    MODE_DATA    = 0x40 
} ssd1306_transfer_mode_t;

typedef struct ssd1306_bus_config_t {
    i2c_master_bus_handle_t bus_handle;
    i2c_master_dev_handle_t dev_handle;
    i2c_device_config_t *slave_device;
} ssd1306_bus_config_t;

typedef struct ssd1306_draw_t {
    unsigned char current_x;
	unsigned char current_y;
    unsigned char *buffer;
} ssd1306_draw_t;

#endif //SSD1306_TYPES_H