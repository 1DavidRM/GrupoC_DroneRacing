#include <esp_now.h>
#include <WiFi.h>
#include "rc_espnow.h"

typedef struct rc_packet_t {
    float throttle;
    float roll;
    float pitch;
    float yaw;
} rc_packet_t;

static volatile rc_packet_t rc_data = {47.4f, 0, 0, 0};
static volatile unsigned long last_packet_ms = 0;
#define FAILSAFE_TIMEOUT_MS 500

void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
    if (len == sizeof(rc_packet_t)) {
        memcpy((void*)&rc_data, incomingData, sizeof(rc_packet_t));
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
        out[0] = 0.0; out[1] = 0.0; out[2] = 0.0; out[3] = 0.0;
        return;
    }
    out[0] = rc_data.throttle;
    out[1] = rc_data.roll;
    out[2] = rc_data.pitch;
    out[3] = rc_data.yaw;
}