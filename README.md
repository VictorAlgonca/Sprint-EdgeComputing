# Sprint-EdgeComputing

Victor de Almeida Gonçalves RM: 558799

Pedro Henrique Martins Alves dos Santos RM: 558107

Felipe Cerboncini Cordeiro RM: 554909

Matheus Rezende da Silva RM: 559190

# Projeto: Sistema de Cálculo de Força G para Carros de Fórmula E

Descrição:

Este projeto utiliza um ESP32 e um sensor MPU6050 para calcular a força G de um carro de Fórmula E durante sua movimentação. O sistema lê as informações de aceleração em três eixos (X, Y e Z) fornecidas pelo MPU6050 e calcula a magnitude da força G total utilizando derivadas e limites para uma leitura mais precisa. Os resultados são enviados para a plataforma ThingSpeak e HTML para monitoramento e análise em tempo real.

Funcionalidades:

- Cálculo da força G total nos três eixos (X, Y e Z) a partir do MPU6050;
- Envio dos dados para a plataforma ThingSpeak e HTML;
- Exibição da força G calculada no monitor serial.

Itens Utilizados:

- ESP32: Microcontrolador para gerenciar a leitura dos sensores e a comunicação Wi-Fi;
- Sensor MPU6050: Acelerômetro e giroscópio para medir a aceleração e a velocidade angular;
- Resistores e Jumpers: Para conexão dos componentes.

Biblioteca: 

- Wire.h: Biblioteca para comunicação I2C;
- Adafruit_MPU6050.h: Biblioteca para uso do sensor MPU6050;
- Adafruit_Sensor.h: Biblioteca base para sensores;
- WiFi.h: Biblioteca para conexão Wi-Fi no ESP32;
- HTTPClient.h: Biblioteca para realizar requisições HTTP.

Montagem:

1.ESP32:

- Conecte o ESP32 à protoboard para facilitar a organização dos fios.

2.Sensor MPU6050:

- Conecte os pinos VCC e GND do sensor aos pinos 3.3V e GND do ESP32, respectivamente;
- Conecte os pinos SCL e SDA do sensor aos pinos correspondentes do ESP32 (normalmente GPIO 22 para SCL e GPIO 21 para SDA).

Instruções de Uso:

- Monte o circuito conforme descrito na seção Montagem.
- Faça o upload do código fornecido para o ESP32 usando a IDE do Arduino.
- Abra o monitor serial para visualizar a força G calculada em tempo real.
- Conecte-se à rede Wi-Fi utilizando as credenciais fornecidas no código.
- A cada 15 segundos, o sistema enviará a força G total calculada para o ThingSpeak e HTML, onde você poderá monitorar os dados.

![Draft](https://github.com/user-attachments/assets/166a5070-f818-4d4f-8588-fbef1ffc6ed3)

Link Wokwi: 

[https://wokwi.com/projects/410218519444978689]

Link ThingSpeak: 

[https://thingspeak.com/channels/2667482]

Link Vídeo Explicativo:

[https://youtu.be/_XjEq_VP71M]

Código Fonte: 

```c++
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
```














