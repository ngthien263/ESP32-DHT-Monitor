#ifndef DHT22_H
#define DHT22_H

#include "freertos/semphr.h"
    
#include "dht22_types.h"        

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DHT22_PORT_NUM
#define DHT22_PORT_NUM 4
#endif



/**
 * @brief Tạo và khởi tạo DHT22 subject
 * 
 * @return dht22_t* Đã khởi tạo và gán subject_t bên trong
 */
dht22_t* dht22_create(void);

/**
 * @brief Huỷ đối tượng DHT22
 * 
 * @param self 
 */
void dht22_destroy(dht22_t* self);

/**
 * @brief Lấy subject interface để dùng abstract
 * 
 * @param self dht22 instance
 * @return subject_t* interface để subscribe/get_data
 */
subject_t* dht22_subject(dht22_t* self);

/**
 * @brief Gọi đo dữ liệu từ sensor và lưu nội bộ
 * 
 * @param self 
 * @return dht22_error_t 
 */
dht22_error_t dht22_get_data(dht22_t* self);

/**
 * @brief Copy dữ liệu đã đo sang output (đã có mutex)
 * 
 * @param self 
 * @param out 
 */
void dht22_update_data(dht22_t* self, dht22_data_t* out);

#ifdef __cplusplus
}
#endif

#endif // DHT22_H
