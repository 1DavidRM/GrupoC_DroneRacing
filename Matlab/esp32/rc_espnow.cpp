#include <esp_now.h>
#include <WiFi.h>
#include "rc_espnow.h"

typedef struct struct_mensaje {
    float matriz[4]; // [0]=j1y, [1]=j1x, [2]=j2y, [3]=j2x
} struct_mensaje;

static volatile struct_mensaje datosRecibidos = {0.0466f, 0, 0, 0}; // ~-0.9066 mapea a 47.4% throttle en reposo si tu stick no autocentra en 0; ajusta si el tuyo sí autocentra
static volatile unsigned long last_packet_ms = 0;
#define FAILSAFE_TIMEOUT_MS 500

void onDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
  if (len == sizeof(struct_mensaje)) {
    memcpy((void*)&datosRecibidos, incomingData, sizeof(struct_mensaje));
    last_packet_ms = millis();
  }
}

void rc_espnow_init(void) {
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) return;
  esp_now_register_recv_cb(onDataRecv);
  last_packet_ms = millis();
}

void get_rc_command(double *out) {
  unsigned long elapsed = millis() - last_packet_ms;

  if (elapsed > FAILSAFE_TIMEOUT_MS) {
    out[0] = 0.0; out[1] = 0.0; out[2] = 0.0; out[3] = 0.0; // failsafe: motores a 0
    return;
  }

  float j1y = datosRecibidos.matriz[0];
  float j1x = datosRecibidos.matriz[1];
  float j2y = datosRecibidos.matriz[2];
  float j2x = datosRecibidos.matriz[3];

  out[0] = (j1y + 1.0f) * 50.0f;  // Throttle: -1..1 -> 0..100
  out[1] = j2x * 100.0f;          // Roll:     -1..1 -> -100..100
  out[2] = j2y * 100.0f;          // Pitch:    -1..1 -> -100..100
  out[3] = j1x * 100.0f;          // Yaw:      -1..1 -> -100..100
}