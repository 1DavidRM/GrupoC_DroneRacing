#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoOTA.h>

const char* ssid = "Gomez";
const char* password = "Gomez1395.";

WebServer server(80);

// --- PINES DRIVER DELANTERO (Placa Roja) ---
const int frontM1_IN1 = 5;  // Delantero Izquierdo (PWM)
const int frontM1_IN2 = 18; // Delantero Izquierdo (LOW)
const int frontM2_IN1 = 19; // Delantero Derecho (PWM)
const int frontM2_IN2 = 23; // Delantero Derecho (LOW)

// --- PINES DRIVER TRASERO (Placa Negra) ---
const int rearM1_IN1 = 33; // Trasero Izquierdo (PWM)
const int rearM1_IN2 = 14; // Trasero Izquierdo (LOW)
const int rearM2_IN1 = 2;  // Trasero Derecho (PWM y LED)
const int rearM2_IN2 = 4;  // Trasero Derecho (LOW)

// --- CONFIGURACIÓN PWM ---
const int freq = 5000;
const int resolution = 8; // Rango de 0 a 255

// --- VARIABLE GLOBAL DE VELOCIDAD ---
int velocidadGlobal = 0; // Controla los 4 motores al mismo tiempo

// --- FUNCIÓN PARA ACTUALIZAR TODOS LOS MOTORES ---
void actualizarMotores() {
  ledcWrite(frontM1_IN1, velocidadGlobal);
  ledcWrite(frontM2_IN1, velocidadGlobal);
  ledcWrite(rearM1_IN1, velocidadGlobal);
  ledcWrite(rearM2_IN1, velocidadGlobal);
}

// --- INTERFAZ WEB ---
void handleRoot() {
  String html = "<html lang='es'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<style>";
  html += "body{font-family:Arial;text-align:center;background:#111;color:#fff;margin:0;padding:20px;}";
  html += ".panel{background:#2c3e50;padding:30px;border-radius:15px;border:2px solid #34495e;max-width:400px;margin:20px auto;box-shadow: 0 4px 8px rgba(0,0,0,0.5);}";
  html += "h1{font-size:24px; margin-bottom: 5px;}";
  html += ".velocidad{font-size:80px;font-weight:bold;margin:20px 0;color:#2ecc71; text-shadow: 0 0 10px #27ae60;}";
  html += "button{font-size:24px;padding:15px 30px;margin:10px;cursor:pointer;border:none;border-radius:8px;font-weight:bold; width: 40%;}";
  html += ".btn-up{background:#3498db;color:white;}";
  html += ".btn-down{background:#f39c12;color:white;}";
  html += ".btn-max{background:#8e44ad;color:white; width: 85%; margin-top:15px;}";
  html += ".btn-off{background:#e74c3c;color:white; width: 85%; margin-top:15px; padding:20px;}";
  html += "</style></head><body>";
  
  html += "<h1>Control Unificado Kiwo K88</h1>";
  
  html += "<div class='panel'>";
  html += "<h2>Potencia Actual (0-255)</h2>";
  
  // Aquí es donde ves exactamente a cuánto están los motores
  html += "<div class='velocidad'>" + String(velocidadGlobal) + "</div>";
  
  // Botones de incremento/decremento
  html += "<a href='/up'><button class='btn-up'>+5</button></a>";
  html += "<a href='/down'><button class='btn-down'>-5</button></a><br>";
  
  // Botones de acceso rápido
  html += "<a href='/max'><button class='btn-max'>AL MÁXIMO (255)</button></a><br>";
  html += "<a href='/off'><button class='btn-off'>PARADA TOTAL (0)</button></a>";
  html += "</div>";
  
  html += "</body></html>";
  server.send(200, "text/html", html);
}

// --- CONTROLADORES DE RUTAS ---
void handleUp() {
  velocidadGlobal = min(255, velocidadGlobal + 5);
  actualizarMotores();
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleDown() {
  velocidadGlobal = max(0, velocidadGlobal - 5);
  actualizarMotores();
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleMax() {
  velocidadGlobal = 255;
  actualizarMotores();
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleOff() {
  velocidadGlobal = 0;
  actualizarMotores();
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);

  // 1. Configurar pines IN2 como salidas y ponerlos en LOW
  pinMode(frontM1_IN2, OUTPUT); pinMode(frontM2_IN2, OUTPUT);
  pinMode(rearM1_IN2, OUTPUT); pinMode(rearM2_IN2, OUTPUT);
  
  digitalWrite(frontM1_IN2, LOW); digitalWrite(frontM2_IN2, LOW);
  digitalWrite(rearM1_IN2, LOW); digitalWrite(rearM2_IN2, LOW);

  // 2. Configurar PWM en pines IN1
  ledcAttach(frontM1_IN1, freq, resolution);
  ledcAttach(frontM2_IN1, freq, resolution);
  ledcAttach(rearM1_IN1, freq, resolution);
  ledcAttach(rearM2_IN1, freq, resolution);

  // 3. Iniciar apagado
  handleOff();

  // 4. Iniciar Wi-Fi y OTA
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  
  ArduinoOTA.setHostname("ESP32-Kiwo-Unified");
  ArduinoOTA.begin();

  // 5. Configurar rutas
  server.on("/", handleRoot);
  server.on("/up", handleUp);
  server.on("/down", handleDown);
  server.on("/max", handleMax);
  server.on("/off", handleOff);

  server.begin();
}

void loop() {
  ArduinoOTA.handle();
  server.handleClient();
}