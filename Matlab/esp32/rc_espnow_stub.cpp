// rc_espnow_stub.cpp — SOLO para simulación en PC, nunca se usa en el ESP32 real
#include "rc_espnow.h"

void rc_espnow_init(void) {
// No hace nada en simulación
}

void get_rc_command(double *out) {
out[0] = 47.4;  // Throttle hover, valor de prueba fijo
out[1] = 0.0;   // Roll
out[2] = 0.0;   // Pitch
out[3] = 0.0;   // Yaw
}