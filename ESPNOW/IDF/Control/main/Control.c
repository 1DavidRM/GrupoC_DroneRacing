#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <fcntl.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_now.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "esp_log.h"

static const char *TAG = "CONTROL";

// MAC del ESP32 esclavo (el que trae los motores) -- AJUSTAR con la MAC real
static uint8_t macEsclavo[] = {78, 0x42, 0x1c, 0x6c, 0xc9, 0xf8};

#define LED_AZUL_PIN GPIO_NUM_2

typedef struct {
    int velocidad; // 0-255
} struct_comando;

static struct_comando comando;
static int velocidadGlobal = 0;

static void onDataSent(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) {
    (void) tx_info;
    (void) status;
}

static void enviarComando(void) {
    comando.velocidad = velocidadGlobal;
    esp_now_send(macEsclavo, (uint8_t *) &comando, sizeof(comando));
    ESP_LOGI(TAG, "Enviado -> velocidad: %d", velocidadGlobal);
}

// ---------- ESP-NOW (sin conectarse a ningun router) ----------
static void espnow_init(void) {
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start()); // WiFi arriba pero sin unirse a ninguna red

    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_send_cb(onDataSent));

    esp_now_peer_info_t peerInfo = {0};
    memcpy(peerInfo.peer_addr, macEsclavo, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    ESP_ERROR_CHECK(esp_now_add_peer(&peerInfo));
}

// ---------- Interfaz por Serial ----------
// Acepta: un numero directo (0-255), o los comandos "up", "down", "max", "off"
static void procesar_linea(char *linea) {
    linea[strcspn(linea, "\r\n")] = 0; // quita el salto de linea
    if (strlen(linea) == 0) return;

    if (strcasecmp(linea, "off") == 0) {
        velocidadGlobal = 0;
    } else if (strcasecmp(linea, "max") == 0) {
        velocidadGlobal = 255;
    } else if (strcasecmp(linea, "up") == 0) {
        velocidadGlobal = (velocidadGlobal + 5 > 255) ? 255 : velocidadGlobal + 5;
    } else if (strcasecmp(linea, "down") == 0) {
        velocidadGlobal = (velocidadGlobal - 5 < 0) ? 0 : velocidadGlobal - 5;
    } else {
        int val = atoi(linea);
        if (val < 0) val = 0;
        if (val > 255) val = 255;
        velocidadGlobal = val;
    }

    enviarComando();
}

static void control_task(void *arg) {
    // Stdin no-bloqueante: si no hay linea nueva, el while sigue sin trabarse
    fcntl(fileno(stdin), F_SETFL, O_NONBLOCK);
    char linea[32];

    ESP_LOGI(TAG, "Listo. Escribe un numero 0-255, o 'up' / 'down' / 'max' / 'off' y Enter.");

    while (1) {
        if (fgets(linea, sizeof(linea), stdin) != NULL) {
            procesar_linea(linea);
        }
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void app_main(void) {
    ESP_ERROR_CHECK(nvs_flash_init());
    espnow_init();
    xTaskCreate(control_task, "control_task", 4096, NULL, 5, NULL);
}