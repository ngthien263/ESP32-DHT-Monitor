#include "http_server.h"
#include "wifi_cred.h"
#include "esp_heap_caps.h"

static esp_err_t root_get_handler(httpd_req_t *req)
{
    extern const char index_html_start[] asm("_binary_index_html_start");
    extern const char index_html_end[]   asm("_binary_index_html_end");
    const size_t html_len = index_html_end - index_html_start;

    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, index_html_start, html_len);
    return ESP_OK;
}

static esp_err_t save_get_handler(httpd_req_t *req)
{
    char query[100];           
    char ssid[32], pass[64];   
    if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK) {
        if (httpd_query_key_value(query, "ssid", ssid, sizeof(ssid)) == ESP_OK &&
            httpd_query_key_value(query, "pass", pass, sizeof(pass)) == ESP_OK) {
            wifi_cred_save(ssid, pass);
            httpd_resp_send(req, "Saved! Rebooting...", HTTPD_RESP_USE_STRLEN);
            vTaskDelay(pdMS_TO_TICKS(1000));
            esp_restart();
            return ESP_OK;
        }
    }

    httpd_resp_send_500(req);
    return ESP_FAIL;
}
httpd_uri_t uri_root = {
    .uri      = "/",
    .method   = HTTP_GET,
    .handler  = root_get_handler,
    .user_ctx = NULL
};

httpd_uri_t uri_save = {
    .uri      = "/save",
    .method   = HTTP_GET,
    .handler  = save_get_handler,
    .user_ctx = NULL
};

httpd_handle_t http_server_init(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &uri_root);  
        httpd_register_uri_handler(server, &uri_save);  
    }
    return server;
}
