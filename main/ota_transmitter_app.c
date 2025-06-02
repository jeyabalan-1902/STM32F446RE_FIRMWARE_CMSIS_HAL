#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_system.h"
#include "esp_http_client.h"
#include "cJSON.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "mqtt_client.h"


#define TAG "OTA_UART"

#define OTA_BUF_SIZE 1024
#define UART_NUM UART_NUM_1
#define UART_TXD  (GPIO_NUM_17)
#define UART_RXD  (GPIO_NUM_16)
#define UART_BAUD 115200

#define ACK_BYTE 0xCC
#define ACK_TIMEOUT_MS 1000

// OTA Protocol framing
#define ETX_OTA_SOF  0xAA
#define ETX_OTA_EOF  0xBB
#define ETX_OTA_CMD_START  0x00
#define ETX_OTA_CMD_END    0x01
#define ETX_OTA_PACKET_TYPE_CMD 0
#define ETX_OTA_PACKET_TYPE_HEADER 2
#define ETX_OTA_PACKET_TYPE_DATA 1

#define WIFI_SSID "Onwords"
#define WIFI_PASS "nikhil8182"

char *product_id = "stmota01";

esp_mqtt_client_handle_t client_id;

typedef struct {
    uint32_t package_size;
    uint32_t package_crc;
    uint32_t reserved1;
    uint32_t reserved2;
} __attribute__((packed)) meta_info;

typedef struct {
    uint8_t sof;
    uint8_t packet_type;
    uint16_t data_len;
    uint8_t cmd;
    uint32_t crc;
    uint8_t eof;
} __attribute__((packed)) etx_ota_cmd_t;

typedef struct {
    uint8_t sof;
    uint8_t packet_type;
    uint16_t data_len;
    meta_info info;
    uint32_t crc;
    uint8_t eof;
} __attribute__((packed)) etx_ota_header_t;

typedef struct {
    uint8_t sof;
    uint8_t packet_type;
    uint16_t data_len;
    uint8_t data[OTA_BUF_SIZE];
    uint32_t crc;
    uint8_t eof;
} __attribute__((packed)) etx_ota_data_t;

static void wifi_init(void) {
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS
        },
    };
    
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "Connecting to WiFi...");

    ESP_ERROR_CHECK(esp_wifi_connect());
}



static esp_err_t http_event_handler(esp_http_client_event_t *evt) {
    return ESP_OK;
}

static bool wait_for_ack(int timeout_ms) {
    uint8_t resp_buf[9];
    int len = uart_read_bytes(UART_NUM, resp_buf, sizeof(resp_buf), pdMS_TO_TICKS(timeout_ms));
    if (len == 9 && resp_buf[0] == 0xAA && resp_buf[1] == 0x03 && resp_buf[3] == 0x00) {
        ESP_LOGI(TAG, "Received ACK from STM32");
        return true;
    } else {
        ESP_LOGE(TAG, "Invalid ACK response or timeout");
        return false;
    }
}


static bool send_uart_with_ack(const uint8_t *data, size_t len) {
    uart_write_bytes(UART_NUM, (const char *)data, len);
    return wait_for_ack(ACK_TIMEOUT_MS);
}

static bool send_ota_command(uint8_t cmd) {
    etx_ota_cmd_t cmd_pkt = {
        .sof = ETX_OTA_SOF,
        .packet_type = ETX_OTA_PACKET_TYPE_CMD,
        .data_len = 1,
        .cmd = cmd,
        .crc = 0,  // TODO: Add CRC if needed
        .eof = ETX_OTA_EOF
    };

    // Send byte-by-byte with delay
    const uint8_t *p = (uint8_t *)&cmd_pkt;
    for (int i = 0; i < sizeof(etx_ota_cmd_t); i++) {
        uart_write_bytes(UART_NUM, &p[i], 1);
        sys_delay_ms(10);  // 10 ms delay per byte
    }

    return wait_for_ack(ACK_TIMEOUT_MS);
}


static bool send_ota_header(uint32_t size) {
    etx_ota_header_t header = {
        .sof = ETX_OTA_SOF,
        .packet_type = ETX_OTA_PACKET_TYPE_HEADER,
        .data_len = sizeof(meta_info),
        .info = {
            .package_size = size,
            .package_crc = 0,  // TODO: Add CRC later
            .reserved1 = 0,
            .reserved2 = 0
        },
        .crc = 0,
        .eof = ETX_OTA_EOF
    };

    const uint8_t *p = (uint8_t *)&header;
    for (int i = 0; i < sizeof(etx_ota_header_t); i++) {
        uart_write_bytes(UART_NUM, &p[i], 1);
        sys_delay_ms(10);  // 10 ms
    }

    return wait_for_ack(ACK_TIMEOUT_MS);
}


static bool send_ota_chunk(uint8_t *data, uint16_t len) {
    uint8_t pkt[OTA_BUF_SIZE + 9];  // SOF, TYPE, LEN, DATA, CRC, EOF

    pkt[0] = ETX_OTA_SOF;
    pkt[1] = ETX_OTA_PACKET_TYPE_DATA;
    pkt[2] = (uint8_t)(len & 0xFF);
    pkt[3] = (uint8_t)((len >> 8) & 0xFF);
    memcpy(&pkt[4], data, len);

    uint32_t crc = 0;  // TODO: Add real CRC
    memcpy(&pkt[4 + len], &crc, 4);
    pkt[8 + len] = ETX_OTA_EOF;

    for (int i = 0; i < len + 9; i++) {
        uart_write_bytes(UART_NUM, &pkt[i], 1);
        sys_delay_ms(10);  // 10 ms
    }

    return wait_for_ack(ACK_TIMEOUT_MS);
}


static void ota_task_from_url(void *arg) {
    const char *firmware_url = (const char *)arg;
    ESP_LOGI(TAG, "Starting OTA from URL: %s", firmware_url);

    esp_http_client_config_t config_fw = {
        .url = firmware_url,
        .event_handler = http_event_handler
    };
    esp_http_client_handle_t client = esp_http_client_init(&config_fw);
    if (esp_http_client_open(client, 0) != ESP_OK) {
        ESP_LOGE(TAG, "Failed to open firmware URL");
        esp_http_client_cleanup(client);
        free(arg);
        vTaskDelete(NULL);
    }

    uint8_t *ota_buf = malloc(OTA_BUF_SIZE);
    uint32_t total = 0;
    int fw_size = esp_http_client_fetch_headers(client);

    if (!send_ota_command(ETX_OTA_CMD_START)) goto cleanup;
    vTaskDelay(pdMS_TO_TICKS(100));

    if (!send_ota_header(fw_size)) goto cleanup;
    vTaskDelay(pdMS_TO_TICKS(100));

    int read_len = 0;
    while ((read_len = esp_http_client_read(client, (char *)ota_buf, OTA_BUF_SIZE)) > 0) {
        if (!send_ota_chunk(ota_buf, read_len)) goto cleanup;
        total += read_len;
        vTaskDelay(pdMS_TO_TICKS(50));
    }

    ESP_LOGI(TAG, "Total Sent: %lu bytes", total);
    send_ota_command(ETX_OTA_CMD_END);

cleanup:
    esp_http_client_cleanup(client);
    free(ota_buf);
    free(arg);
    vTaskDelete(NULL);
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    client_id = event->client;
    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT Connected");
            esp_mqtt_client_subscribe(client, "onwords/stmota01/firmware", 0);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;
        case MQTT_EVENT_SUBSCRIBED:
            //ESP_LOGI(TAG_mqtt, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            //ESP_LOGI(TAG_mqtt, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT Data Received");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);

            // Parse the firmwareUrl and status
            char *data_copy = strndup(event->data, event->data_len);
            cJSON *json = cJSON_Parse(data_copy);
            if (json) {
                int fw_sts = cJSON_GetObjectItem(json, "firmware_sts")->valueint;
                const char *firmware_url = cJSON_GetObjectItem(json, "firmwareUrl")->valuestring;

                ESP_LOGI(TAG, "Firmware Status: %d, URL: %s", fw_sts, firmware_url);

                if (fw_sts == 1 && firmware_url) {
                    // Store URL and launch OTA task
                    char *url_copy = strdup(firmware_url);
                    xTaskCreate(ota_task_from_url, "ota_task", 8192, url_copy, 6, NULL);
                }
                cJSON_Delete(json);
            }
            free(data_copy);
            break;
        default:
            break;
    }
}

static void mqtt_init(void) {
    esp_mqtt_client_config_t mqtt_cfg = {
         .broker.address.uri = "mqtt://Nikhil:Nikhil8182@mqtt.onwords.in",
         //.broker.address.uri = "mqtt://onwords:nikhil8182@192.168.88.118:1883",
         .credentials.client_id = product_id,
    };
    mqtt_cfg.session.keepalive = 10;
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}


void app_main(void) {
    // Init UART
    uart_config_t uart_config = {
        .baud_rate = UART_BAUD,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_driver_install(UART_NUM, 2048, 2048, 0, NULL, 0);
    uart_param_config(UART_NUM, &uart_config);
    uart_set_pin(UART_NUM, UART_TXD, UART_RXD, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // Init WiFi + MQTT
    ESP_ERROR_CHECK(nvs_flash_init());
    wifi_init();
    mqtt_init();
}

