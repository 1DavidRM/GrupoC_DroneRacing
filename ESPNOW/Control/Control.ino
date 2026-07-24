#include <esp_now.h>
#include <WiFi.h>

uint8_t macReceptora[] = {0x78,0x42,0x1c,0x6c,0xc9,0xf8};

typedef struct struct_mensaje {
    float matriz[4]; // [0]=j1y, [1]=j1x, [2]=j2y, [3]=j2x
} struct_mensaje;

struct_mensaje misDatos;
esp_now_peer_info_t peerInfo;

unsigned long ultimoDatoSerial = 0;
bool failsafeYaEnviado = false;
const unsigned long FAILSAFE_MS = 500;

void onDataSent(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) {}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) return;

  esp_now_register_send_cb(onDataSent);
  memcpy(peerInfo.peer_addr, macReceptora, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);

  ultimoDatoSerial = millis(); // evita un falso failsafe justo al arrancar
}

void loop() {
  if (Serial.available() > 0) {
    String cadena = Serial.readStringUntil('\n');

    int c1 = cadena.indexOf(',');
    int c2 = cadena.indexOf(',', c1 + 1);
    int c3 = cadena.indexOf(',', c2 + 1);

    if (c1 != -1 && c2 != -1 && c3 != -1) {
      misDatos.matriz[0] = cadena.substring(0, c1).toFloat();
      misDatos.matriz[1] = cadena.substring(c1 + 1, c2).toFloat();
      misDatos.matriz[2] = cadena.substring(c2 + 1, c3).toFloat();
      misDatos.matriz[3] = cadena.substring(c3 + 1).toFloat();

      esp_now_send(macReceptora, (uint8_t *) &misDatos, sizeof(misDatos));

      ultimoDatoSerial = millis();
      failsafeYaEnviado = false; // se rearma para la próxima vez que se pierda la señal
    }
  } else {
    // No llegó nada nuevo por Serial — checa si ya se cumplió el timeout
    if (millis() - ultimoDatoSerial > FAILSAFE_MS && !failsafeYaEnviado) {
      misDatos.matriz[0] = -1.0f; // Throttle -> 0% (motores apagados)
      misDatos.matriz[1] = 0.0f;  // Yaw nivelado
      misDatos.matriz[2] = 0.0f;  // Pitch nivelado
      misDatos.matriz[3] = 0.0f;  // Roll nivelado

      esp_now_send(macReceptora, (uint8_t *) &misDatos, sizeof(misDatos));
      failsafeYaEnviado = true; // manda el paquete de failsafe UNA vez, no satura el aire repitiéndolo
    }
  }
}