#ifndef SSD1306_H
#define SSD1306_H

#include "driver/i2c_master.h"
#include "ssd1306_types.h"
#include "ssd1306_commands.h"
#include "common_observer.h"
#include "dht22.h"
#define SSD1306_WIDTH 128 
#define SSD1306_HEIGHT 64 
#define SSD1306_I2C_SPEED_HZ 400000 

#define SSD1306_SDA_PIN 4
#define SSD1306_SCL_PIN 5
#define SSD1306_MAX_COL 127
#define SSD1306_MAX_ROW 7

#define SSD1306_FONT_WIDTH 5
#define SSD1306_FONT_HEIGHT 8
typedef struct ssd1306_i2c_module {
    uint8_t               *slave_addr;
    ssd1306_bus_config_t  *ssd1306_bus_config;
    ssd1306_draw_t        *ssd1306_draw;
    observer_t base_observer;                   /** Observer context. */
    dht22_data_t data;                          /** Sensor data. */
}ssd1306_i2c_module;

//Constructor & Desconstructor
void ssd1306_init(struct ssd1306_i2c_module *module, uint8_t slave_addr);
void ssd1306_clean(struct ssd1306_i2c_module *module);
ssd1306_i2c_module* ssd1306_create(uint8_t slave_addr);
void ssd1306_del(ssd1306_i2c_module *module);
void ssd1306_command(struct ssd1306_i2c_module *module, unsigned char command);
//SSD1306 operation function
void ssd1306_write(struct ssd1306_i2c_module *module, ssd1306_transfer_mode_t mode, unsigned char data);
void ssd1306_display(ssd1306_i2c_module* module);
void ssd1306_clearDisplay(ssd1306_i2c_module* module);
void ssd1306_setCursor(ssd1306_i2c_module *module, int x, int y);
void ssd1306_drawPixel(ssd1306_i2c_module *module, int x, int y);
void ssd1306_removePixel(ssd1306_i2c_module* module, int x, int y);
void ssd1306_fill(ssd1306_i2c_module* module);
void ssd1306_drawChar(ssd1306_i2c_module* module, int x, int y, char c);
void ssd1306_drawString(ssd1306_i2c_module* module, int x, int y, const char* str);
void ssd1306_destroy(ssd1306_i2c_module* module);
void ssd1306_data(struct ssd1306_i2c_module *module, unsigned char data);
void ssd1306_dataList(struct ssd1306_i2c_module *module, unsigned char *dataList, unsigned int len);
void ssd1306_subscribe(ssd1306_i2c_module* module);
void ssd1306_unsubscribe(ssd1306_i2c_module* module);
#endif

