#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_now.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_timer.h"
#include "nvs_flash.h"
#include "esp_log.h"

static const char *TAG = "CONTROL";

// MAC del receptor (el ESP32 en el dron)
static uint8_t macReceptora[] = {0x78, 0x42, 0x1c, 0x6c, 0xc9, 0xf8};

typedef struct {
    float matriz[4]; // [0]=j1y, [1]=j1x, [2]=j2y, [3]=j2x
} struct_mensaje;

static struct_mensaje misDatos;

static int64_t ultimoDatoSerial = 0;
static bool failsafeYaEnviado = false;
#define FAILSAFE_MS 500

// Callback de envío — igual que en Arduino, no hacemos nada con el status
static void onDataSent(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) {
    (void) tx_info;
    (void) status;
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
    ESP_ERROR_CHECK(esp_now_register_send_cb(onDataSent));

    esp_now_peer_info_t peerInfo = {0};
    memcpy(peerInfo.peer_addr, macReceptora, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    ESP_ERROR_CHECK(esp_now_add_peer(&peerInfo));
}

static int64_t millis(void) {
    return esp_timer_get_time() / 1000;
}

static void control_task(void *arg) {
    // Clave: ponemos stdin en modo no-bloqueante.
    // Sin esto, fgets() se queda esperando un '\n' y el failsafe nunca corre a tiempo.
    fcntl(fileno(stdin), F_SETFL, O_NONBLOCK);

    char linea[64];
    ultimoDatoSerial = millis(); // evita un falso failsafe justo al arrancar

    while (1) {
        char *res = fgets(linea, sizeof(linea), stdin);

        if (res != NULL) {
            float valores[4];
            int i = 0;
            char *rest = linea;
            char *token;

            while (i < 4 && (token = strtok_r(rest, ",\n", &rest)) != NULL) {
                valores[i++] = atof(token);
            }

            if (i == 4) {
                misDatos.matriz[0] = valores[0];
                misDatos.matriz[1] = valores[1];
                misDatos.matriz[2] = valores[2];
                misDatos.matriz[3] = valores[3];

                esp_now_send(macReceptora, (uint8_t *) &misDatos, sizeof(misDatos));

                ultimoDatoSerial = millis();
                failsafeYaEnviado = false; // se rearma para la próxima pérdida de señal
            }
        } else {
            // No llegó nada nuevo por serial — checa si ya se cumplió el timeout
            if ((millis() - ultimoDatoSerial > FAILSAFE_MS) && !failsafeYaEnviado) {
                misDatos.matriz[0] = -1.0f; // Throttle -> 0% (motores apagados)
                misDatos.matriz[1] = 0.0f;  // Yaw nivelado
                misDatos.matriz[2] = 0.0f;  // Pitch nivelado
                misDatos.matriz[3] = 0.0f;  // Roll nivelado

                esp_now_send(macReceptora, (uint8_t *) &misDatos, sizeof(misDatos));
                failsafeYaEnviado = true;
                ESP_LOGW(TAG, "Failsafe enviado");
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10)); // evita que la tarea acapare el CPU
    }
}

void app_main(void) {
    wifi_espnow_init();
    xTaskCreate(control_task, "control_task", 4096, NULL, 5, NULL);
}