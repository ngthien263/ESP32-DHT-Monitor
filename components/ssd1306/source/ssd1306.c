#ifndef UNIT_TEST
#include "ssd1306.h"
#include <stdint.h>
#include <string.h>
unsigned char buffer[1024];

static const unsigned char ssd1306_font[][5] = {
		{0x00, 0x00, 0x00, 0x00, 0x00}, // space
		{0x00, 0x00, 0x2f, 0x00, 0x00}, // !
		{0x00, 0x07, 0x00, 0x07, 0x00}, // "
		{0x14, 0x7f, 0x14, 0x7f, 0x14}, // #
		{0x24, 0x2a, 0x7f, 0x2a, 0x12}, // $
		{0x23, 0x13, 0x08, 0x64, 0x62}, // %
		{0x36, 0x49, 0x55, 0x22, 0x50}, // &
		{0x00, 0x05, 0x03, 0x00, 0x00}, // '
		{0x00, 0x1c, 0x22, 0x41, 0x00}, // (
		{0x00, 0x41, 0x22, 0x1c, 0x00}, // )
		{0x14, 0x08, 0x3E, 0x08, 0x14}, // *
		{0x08, 0x08, 0x3E, 0x08, 0x08}, // +
		{0x00, 0x00, 0xA0, 0x60, 0x00}, // ,
		{0x08, 0x08, 0x08, 0x08, 0x08}, // -
		{0x00, 0x60, 0x60, 0x00, 0x00}, // .
		{0x20, 0x10, 0x08, 0x04, 0x02}, // /
		{0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
		{0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
		{0x42, 0x61, 0x51, 0x49, 0x46}, // 2
		{0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
		{0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
		{0x27, 0x45, 0x45, 0x45, 0x39}, // 5
		{0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
		{0x01, 0x71, 0x09, 0x05, 0x03}, // 7
		{0x36, 0x49, 0x49, 0x49, 0x36}, // 8
		{0x06, 0x49, 0x49, 0x29, 0x1E}, // 9
		{0x00, 0x36, 0x36, 0x00, 0x00}, // :
		{0x00, 0x56, 0x36, 0x00, 0x00}, // ;
		{0x08, 0x14, 0x22, 0x41, 0x00}, // <
		{0x14, 0x14, 0x14, 0x14, 0x14}, // =
		{0x00, 0x41, 0x22, 0x14, 0x08}, // >
		{0x02, 0x01, 0x51, 0x09, 0x06}, // ?
		{0x32, 0x49, 0x59, 0x51, 0x3E}, // @
		{0x7C, 0x12, 0x11, 0x12, 0x7C}, // A
		{0x7F, 0x49, 0x49, 0x49, 0x36}, // B
		{0x3E, 0x41, 0x41, 0x41, 0x22}, // C
		{0x7F, 0x41, 0x41, 0x22, 0x1C}, // D
		{0x7F, 0x49, 0x49, 0x49, 0x41}, // E
		{0x7F, 0x09, 0x09, 0x09, 0x01}, // F
		{0x3E, 0x41, 0x49, 0x49, 0x7A}, // G
		{0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
		{0x00, 0x41, 0x7F, 0x41, 0x00}, // I
		{0x20, 0x40, 0x41, 0x3F, 0x01}, // J
		{0x7F, 0x08, 0x14, 0x22, 0x41}, // K
		{0x7F, 0x40, 0x40, 0x40, 0x40}, // L
		{0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M
		{0x7F, 0x04, 0x08, 0x10, 0x7F}, // N
		{0x3E, 0x41, 0x41, 0x41, 0x3E}, // O
		{0x7F, 0x09, 0x09, 0x09, 0x06}, // P
		{0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q
		{0x7F, 0x09, 0x19, 0x29, 0x46}, // R
		{0x46, 0x49, 0x49, 0x49, 0x31}, // S
		{0x01, 0x01, 0x7F, 0x01, 0x01}, // T
		{0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
		{0x1F, 0x20, 0x40, 0x20, 0x1F}, // V
		{0x3F, 0x40, 0x38, 0x40, 0x3F}, // W
		{0x63, 0x14, 0x08, 0x14, 0x63}, // X
		{0x07, 0x08, 0x70, 0x08, 0x07}, // Y
		{0x61, 0x51, 0x49, 0x45, 0x43}, // Z
		{0x00, 0x7F, 0x41, 0x41, 0x00}, // [
		{0x55, 0xAA, 0x55, 0xAA, 0x55}, // Backslash (Checker pattern)
		{0x00, 0x41, 0x41, 0x7F, 0x00}, // ]
		{0x04, 0x02, 0x01, 0x02, 0x04}, // ^
		{0x40, 0x40, 0x40, 0x40, 0x40}, // _
		{0x00, 0x03, 0x05, 0x00, 0x00}, // `
		{0x20, 0x54, 0x54, 0x54, 0x78}, // a
		{0x7F, 0x48, 0x44, 0x44, 0x38}, // b
		{0x38, 0x44, 0x44, 0x44, 0x20}, // c
		{0x38, 0x44, 0x44, 0x48, 0x7F}, // d
		{0x38, 0x54, 0x54, 0x54, 0x18}, // e
		{0x08, 0x7E, 0x09, 0x01, 0x02}, // f
		{0x18, 0xA4, 0xA4, 0xA4, 0x7C}, // g
		{0x7F, 0x08, 0x04, 0x04, 0x78}, // h
		{0x00, 0x44, 0x7D, 0x40, 0x00}, // i
		{0x40, 0x80, 0x84, 0x7D, 0x00}, // j
		{0x7F, 0x10, 0x28, 0x44, 0x00}, // k
		{0x00, 0x41, 0x7F, 0x40, 0x00}, // l
		{0x7C, 0x04, 0x18, 0x04, 0x78}, // m
		{0x7C, 0x08, 0x04, 0x04, 0x78}, // n
		{0x38, 0x44, 0x44, 0x44, 0x38}, // o
		{0xFC, 0x24, 0x24, 0x24, 0x18}, // p
		{0x18, 0x24, 0x24, 0x18, 0xFC}, // q
		{0x7C, 0x08, 0x04, 0x04, 0x08}, // r
		{0x48, 0x54, 0x54, 0x54, 0x20}, // s
		{0x04, 0x3F, 0x44, 0x40, 0x20}, // t
		{0x3C, 0x40, 0x40, 0x20, 0x7C}, // u
		{0x1C, 0x20, 0x40, 0x20, 0x1C}, // v
		{0x3C, 0x40, 0x30, 0x40, 0x3C}, // w
		{0x44, 0x28, 0x10, 0x28, 0x44}, // x
		{0x1C, 0xA0, 0xA0, 0xA0, 0x7C}, // y
		{0x44, 0x64, 0x54, 0x4C, 0x44}, // z
		{0x00, 0x10, 0x7C, 0x82, 0x00}, // {
		{0x00, 0x00, 0xFF, 0x00, 0x00}, // |
		{0x00, 0x82, 0x7C, 0x10, 0x00}, // }
		{0x00, 0x06, 0x09, 0x09, 0x06}	// ~ (Degrees)
};

static void ssd1306_on_notify(void* instance){
    ssd1306_i2c_module* ctx = (ssd1306_i2c_module*)instance;
    if (ctx && ctx->base_observer.ctx.task_handle != NULL) {
        printf("ssd1306 notify success");
        xTaskNotifyGive(ctx->base_observer.ctx.task_handle); 
    } 
}


void ssd1306_init(struct ssd1306_i2c_module *module, uint8_t slave_addr){
    i2c_master_bus_config_t i2c_mst_config = {
        .i2c_port           = I2C_NUM_0,
        .sda_io_num         = SSD1306_SDA_PIN,
        .scl_io_num         = SSD1306_SCL_PIN,
        .clk_source         = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt  = 7,
		.flags.enable_internal_pullup = true,
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &module->ssd1306_bus_config->bus_handle));

    module->ssd1306_bus_config->slave_device = (i2c_device_config_t*)malloc(sizeof(i2c_device_config_t));
    module->ssd1306_bus_config->slave_device->dev_addr_length = I2C_ADDR_BIT_LEN_7;
    module->ssd1306_bus_config->slave_device->device_address  = slave_addr;
    module->ssd1306_bus_config->slave_device->scl_speed_hz    = SSD1306_I2C_SPEED_HZ;
    ESP_ERROR_CHECK(i2c_master_bus_add_device(module->ssd1306_bus_config->bus_handle, module->ssd1306_bus_config->slave_device, &module->ssd1306_bus_config->dev_handle));

    module->base_observer.self = module;
    module->base_observer.on_notify = ssd1306_on_notify;

    module->ssd1306_draw->current_x       = 0;
    module->ssd1306_draw->current_y       = 0;
	uint8_t init_cmds[] = {
        0x00,       // control byte: Co = 0, D/C# = 0 (command)
        0xAE,       // display OFF
        0xD5, 0x80, // set display clock divide
        0xA8, 0x3F, // set multiplex ratio (1 to 64)
        0xD3, 0x00, // set display offset
        0x40,       // set start line to 0
        0x8D, 0x14, // enable charge pump
        0x20, 0x00, // memory mode: horizontal
        0xA1,       // segment remap
        0xC8,       // COM scan dec
        0xDA, 0x12, // COM pins config
        0x81, 0xCF, // contrast
        0xD9, 0xF1, // pre-charge
        0xDB, 0x40, // VCOM detect
        0xA4,       // display all on resume
        0xA6,       // normal display
        0xAF        // display ON
    };
	
    esp_err_t ret = i2c_master_transmit(module->ssd1306_bus_config->dev_handle, init_cmds, sizeof(init_cmds), 100);
}

void ssd1306_clean(struct ssd1306_i2c_module *module){

}

static void ssd1306_i2c_write(struct ssd1306_i2c_module *module, unsigned char *buff, unsigned int len){
    ESP_ERROR_CHECK(i2c_master_transmit(module->ssd1306_bus_config->dev_handle, buff, len, -1));
}
static void ssd1306_i2c_read(struct ssd1306_i2c_module *module, unsigned char *read_buff, unsigned int len){
    ESP_ERROR_CHECK(i2c_master_receive(module->ssd1306_bus_config->dev_handle, read_buff, len, -1));
}

void ssd1306_command(struct ssd1306_i2c_module *module, unsigned char command){
    unsigned char buf[2] = {MODE_COMMAND, command};
    ssd1306_i2c_write(module, buf, 2);
}

void ssd1306_data(struct ssd1306_i2c_module *module, unsigned char data){
    unsigned char buf[2] = {MODE_DATA, data};
    ssd1306_i2c_write(module, buf, 2);
}

void ssd1306_commandList(struct ssd1306_i2c_module *module, unsigned char *commandList, unsigned int len){
    unsigned char buf[len + 1];
    buf[0] = MODE_COMMAND;
    memcpy(&buf[1], commandList, len);
    ssd1306_i2c_write(module, buf, len + 1);
}

void ssd1306_dataList(struct ssd1306_i2c_module *module, unsigned char *dataList, unsigned int len){
    unsigned char buf[len + 1];
    buf[0] = MODE_DATA;
    memcpy(&buf[1], dataList, len);
    ssd1306_i2c_write(module, buf, len + 1);
}

ssd1306_i2c_module* ssd1306_create(uint8_t slave_addr){
    ssd1306_i2c_module* i2c_dev = (ssd1306_i2c_module*)malloc(sizeof(ssd1306_i2c_module));
    if(i2c_dev != NULL){
        i2c_dev->ssd1306_bus_config = malloc(sizeof(ssd1306_bus_config_t));
        i2c_dev->ssd1306_draw = malloc(sizeof(ssd1306_draw_t));
        if (i2c_dev->ssd1306_bus_config == NULL || i2c_dev->ssd1306_draw == NULL) {
            // cleanup
            free(i2c_dev);
            return NULL;
        }
        ssd1306_init(i2c_dev, slave_addr);
    }
    return i2c_dev;
}


void ssd1306_destroy(ssd1306_i2c_module* module){
    if(module != NULL){
        ssd1306_clean(module);
    }
    free(module->ssd1306_bus_config->slave_device);
    free(module);
}

void ssd1306_display(ssd1306_i2c_module* module){
    static uint8_t  dlist1[] = {
        SSD1306_PAGEADDR,
        0,                   
        SSD1306_MAX_ROW,         
        SSD1306_COLUMNADDR,
        0,
        SSD1306_MAX_COL}; 
    ssd1306_commandList(module, dlist1, sizeof(dlist1));
    ssd1306_dataList(module,  buffer, SSD1306_WIDTH * SSD1306_HEIGHT / 8);
}

void ssd1306_clearDisplay(ssd1306_i2c_module* module) {
    memset(buffer, 0, SSD1306_WIDTH * (SSD1306_HEIGHT ) / 8);
    ssd1306_display(module); 
}

void ssd1306_drawPixel(ssd1306_i2c_module* module, int x, int y) {
    if (x < 0 || x >= SSD1306_WIDTH || y < 0 || y >= SSD1306_HEIGHT) return;
	buffer[x + (y / 8) * SSD1306_WIDTH] |= (1 << (y & 7));
}

void ssd1306_removePixel(ssd1306_i2c_module* module, int x, int y) {
    buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y & 7));
}

void ssd1306_fill(ssd1306_i2c_module* module) {
    for (int y = 0; y < SSD1306_HEIGHT ; y++) {
        for (int x = 0; x < SSD1306_WIDTH; x++) {
            ssd1306_drawPixel(module, x, y);
        } 
    }
    ssd1306_display(module);
}

void ssd1306_drawChar(ssd1306_i2c_module* module, int x, int y, char c) {
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) return;
    if (c < 0x20 || c > 0x7E) c = '?';

    const uint8_t* font_ptr = ssd1306_font[c - 0x20];

    for (int i = 0; i < SSD1306_FONT_WIDTH; i++) {
        uint8_t col = font_ptr[i];
        for (int j = 0; j < SSD1306_FONT_HEIGHT; j++) {
            if (col & (1 << j)) {
                ssd1306_drawPixel(module, x + i, y + j);
            }
        }
    }

    for (int j = 0; j < SSD1306_FONT_HEIGHT; j++) {
        ssd1306_removePixel(module, x + SSD1306_FONT_WIDTH, y + j);
    }
}

void ssd1306_drawString(ssd1306_i2c_module* module, int x, int y, const char* str) {
    while (*str && x + SSD1306_FONT_WIDTH < SSD1306_WIDTH) {
        ssd1306_drawChar(module, x, y, *str++);
        x += SSD1306_FONT_WIDTH + 1;
    }
}


void ssd1306_subscribe(ssd1306_i2c_module* module) {
    observer_subscribe(&module->base_observer);
}

void ssd1306_unsubscribe(ssd1306_i2c_module* module) {
    observer_unsubscribe(&module->base_observer);
}


#endif