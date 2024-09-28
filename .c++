#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <HTTPClient.h>

// Credenciais 
const char* ssid = "Wokwi-GUEST"; //REDE WIFI 
const char* password = ""; // SENHA WIFI
const char* apiKey = "4H6AO9V2SOL37WFP"; // API KEY 
const char* server = "http://api.thingspeak.com"; // SERVER 

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);

  // Inicializar o MPU6050
  if (!mpu.begin()) {
    Serial.println("Falha ao encontrar o MPU6050!");
    while (1);
  }

  // Configurar o MPU6050 para o alcance desejado
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("MPU6050 inicializado com sucesso.");
  
  // Verificação WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wifi");
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
  Serial.println(" conectado!");

}

void loop() {
  if (WiFi.status() ==  WL_CONNECTED) {
  // Ler os dados do sensor
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Calcular a força G total nos três eixos
  float ax = a.acceleration.x / 9.81; // Aceleração em X convertida para G
  float ay = a.acceleration.y / 9.81; // Aceleração em Y convertida para G
  float az = a.acceleration.z / 9.81; // Aceleração em Z convertida para G

  // Magnitude da força G resultante
  float forceG = sqrt(ax * ax + ay * ay + az * az);

  // Exibir a força G no monitor serial
  Serial.print("Força G total: ");
  Serial.println(forceG);

  HTTPClient http;
  String url = String(server) + "/update?api_key=" + apiKey + "&field1=" + String(forceG);
  http.begin(url);

  // Verificação de Envio de Dados ao ThingSpeak
  int httpCode = http.GET();
  if (httpCode > 0) {
    String payload = http.getString(); // Resposta da requisição HTTP
    Serial.println("Dados enviados ao ThingSpeak.");
    Serial.print("Código HTTP: ");
    Serial.println(httpCode);
    Serial.println("Resposta: ");
    Serial.print(payload);
  } else {
    Serial.print("Erro ao enviar dados. Código HTTP: ");
    Serial.println(httpCode);
  }
    
    http.end();

  } else {
    Serial.println("Wifi não conectado, Tente novamente.");
  }
  delay(15000); // 15s entre as leituras
}