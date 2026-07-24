#include <esp_now.h>
#include <WiFi.h>

typedef struct struct_mensaje {
    float matriz[4]; 
} struct_mensaje;

struct_mensaje datosRecibidos;

// Callback actualizado para compatibilidad con versión 3.x
void onDataRecv(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len) {
  memcpy(&datosRecibidos, incomingData, sizeof(datosRecibidos));
  
  // Aquí tienes tu matriz limpia de 1x4 para inyectar a tu lazo de control
  float j1y = datosRecibidos.matriz[0];
  float j1x = datosRecibidos.matriz[1];
  float j2y = datosRecibidos.matriz[2];
  float j2x = datosRecibidos.matriz[3];

  // Imprimimos el vector recibido para telemetría
  Serial.printf("Matriz Recibida: [ %.3f, %.3f, %.3f, %.3f ]\n", j1y, j1x, j2y, j2x);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) return;
  esp_now_register_recv_cb(onDataRecv);
}

void loop() {
  // Aquí irá tu código de control de estabilidad del dron mapeando los joysticks
}