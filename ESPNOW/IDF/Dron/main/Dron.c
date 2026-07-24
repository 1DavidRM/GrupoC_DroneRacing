#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_now.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "esp_log.h"

static const char *TAG = "DRON";

typedef struct {
    float matriz[4];
} struct_mensaje;

static struct_mensaje datosRecibidos;

// Callback de recepción
static void onDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
    (void) recv_info;
    (void) len;

    memcpy(&datosRecibidos, incomingData, sizeof(datosRecibidos));

    // Aquí tienes tu matriz limpia de 1x4 para inyectar a tu lazo de control
    float j1y = datosRecibidos.matriz[0];
    float j1x = datosRecibidos.matriz[1];
    float j2y = datosRecibidos.matriz[2];
    float j2x = datosRecibidos.matriz[3];

    // Telemetría por log (equivalente a Serial.printf)
    ESP_LOGI(TAG, "Matriz Recibida: [ %.3f, %.3f, %.3f, %.3f ]", j1y, j1x, j2y, j2x);

    // TODO: aquí entra tu interfaz con el flight controller (Simulink / control de estabilidad)
}

static void wifi_espnow_init(void) {
    ESP_ERROR_CHECK(nvs_flash_init());
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
    wifi_espnow_init();

    // El loop() de Arduino ya no existe: si necesitas un ciclo de control
    // continuo (mapear joysticks, estabilidad), va en una tarea FreeRTOS aquí,
    // o directo dentro del callback si la lógica es ligera.
}