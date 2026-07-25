#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_now.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/ledc.h"
#include "driver/gpio.h"

static const char *TAG = "DRON_MOTORES";

// --- Pines driver delantero (Placa Roja) ---
#define FRONT_M1_IN1 GPIO_NUM_5
#define FRONT_M1_IN2 GPIO_NUM_18
#define FRONT_M2_IN1 GPIO_NUM_19
#define FRONT_M2_IN2 GPIO_NUM_23

// --- Pines driver trasero (Placa Negra) ---
#define REAR_M1_IN1 GPIO_NUM_33
#define REAR_M1_IN2 GPIO_NUM_14
#define REAR_M2_IN1 GPIO_NUM_2
#define REAR_M2_IN2 GPIO_NUM_4

#define PWM_FREQ_HZ 5000
#define PWM_RES     LEDC_TIMER_8_BIT // 0-255, igual que resolution=8 en Arduino

typedef struct {
    int velocidad; // 0-255
} struct_comando;

static struct_comando comandoRecibido;

static void aplicarVelocidad(int velocidad) {
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, velocidad);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, velocidad);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2, velocidad);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_2);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3, velocidad);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3);
}

// Callback de recepcion ESP-NOW
static void onDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
    (void) recv_info;
    if (len != sizeof(comandoRecibido)) return;

    memcpy(&comandoRecibido, incomingData, sizeof(comandoRecibido));
    ESP_LOGI(TAG, "Velocidad recibida: %d", comandoRecibido.velocidad);
    aplicarVelocidad(comandoRecibido.velocidad);
}

static void motores_init(void) {
    // Pines IN2 como salida digital, en LOW (igual que el setup() de Arduino)
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << FRONT_M1_IN2) | (1ULL << FRONT_M2_IN2) |
                        (1ULL << REAR_M1_IN2)  | (1ULL << REAR_M2_IN2),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&io_conf);

    gpio_set_level(FRONT_M1_IN2, 0);
    gpio_set_level(FRONT_M2_IN2, 0);
    gpio_set_level(REAR_M1_IN2, 0);
    gpio_set_level(REAR_M2_IN2, 0);

    // Un timer PWM compartido (5kHz, 8 bits) -- igual que ledcAttach() de Arduino
    ledc_timer_config_t timer_conf = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = PWM_RES,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = PWM_FREQ_HZ,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    ledc_timer_config(&timer_conf);

    // Un canal PWM por motor, los 4 sobre el mismo timer
    ledc_channel_config_t canales[4] = {
        { .gpio_num = FRONT_M1_IN1, .speed_mode = LEDC_LOW_SPEED_MODE, .channel = LEDC_CHANNEL_0, .timer_sel = LEDC_TIMER_0, .duty = 0, .hpoint = 0 },
        { .gpio_num = FRONT_M2_IN1, .speed_mode = LEDC_LOW_SPEED_MODE, .channel = LEDC_CHANNEL_1, .timer_sel = LEDC_TIMER_0, .duty = 0, .hpoint = 0 },
        { .gpio_num = REAR_M1_IN1,  .speed_mode = LEDC_LOW_SPEED_MODE, .channel = LEDC_CHANNEL_2, .timer_sel = LEDC_TIMER_0, .duty = 0, .hpoint = 0 },
        { .gpio_num = REAR_M2_IN1,  .speed_mode = LEDC_LOW_SPEED_MODE, .channel = LEDC_CHANNEL_3, .timer_sel = LEDC_TIMER_0, .duty = 0, .hpoint = 0 },
    };
    for (int i = 0; i < 4; i++) {
        ledc_channel_config(&canales[i]);
    }
}

// ---------- ESP-NOW (sin conectarse a ningun router) ----------
static void espnow_init(void) {
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_recv_cb(onDataRecv));
}

void app_main(void) {
    ESP_ERROR_CHECK(nvs_flash_init());
    motores_init();
    espnow_init();
    aplicarVelocidad(0); // arranca apagado, igual que handleOff() en el original
}