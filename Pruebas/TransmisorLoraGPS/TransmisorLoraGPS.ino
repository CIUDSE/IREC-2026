#include <TinyGPS++.h>
#include <Wire.h>
#include "MS5611.h"
#include <Adafruit_BNO08x.h>
#include <SPI.h>
#include <LoRa.h>
// Pines del Teensy en lora
#define LORA_SS    10
#define LORA_RST    9
#define LORA_DIO0   2
#define BAND 915.5E6

long contador = 0;

//Objetos de sensores
TinyGPSPlus gps;

//Estructura que contenga los datos de telemetria
typedef struct telemetryData {
  double latitud;
  double longitud;
  double altura_gps;
  uint8_t satelites;
}telemetryData_t;

//Crear una variable de la estructura
telemetryData_t telemetryData;

void setup() {
  Serial.begin(115200);
  while(!Serial);

  // Iniciar SPI no ocupa argumentos
  SPI.begin();

  //Setup de LoRa
  // Configurar pines del modulo
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Fallo al iniciar LoRa");
    while (1);
  }

  LoRa.setSyncWord(0xA0);
  LoRa.setSpreadingFactor(7); //SF modificar cuando se use la antena posiblmentente a 10
  LoRa.setSignalBandwidth(500E3); // SB modificar cuando la antena posiblemente a 125

  Serial.println("LoRa iniciado en Teensy!");

  //Setup GPS
  Serial8.begin(9600);
  Serial.println("Inicializando sistema de telemetría...");
  
}

void loop() {
  Serial.println("Enviando paquete: ");
  //Serial.println(contador);


  // Leer GPS continuamente (prioridad alta)
  while (Serial8.available() > 0) {
    gps.encode(Serial8.read());
  }
    if(gps.location.isUpdated()){
      telemetryData.latitud = gps.location.lat();
      telemetryData.longitud = gps.location.lng();
      telemetryData.altura_gps = gps.altitude.meters();
    
      Serial.println(telemetryData.latitud, 6);
      Serial.print("   ");
      Serial.println(telemetryData.longitud, 6);
      Serial.print("   ");
      Serial.println(telemetryData.altura_gps,1);
    } 

  //Crear arreglo de bytes iguales(buffer)
  uint8_t buffer[sizeof(telemetryData_t)];
  
  memcpy(buffer, &telemetryData, sizeof(telemetryData_t));

  //Enviar los datos a LoRa
  LoRa.beginPacket();
  LoRa.write(buffer, sizeof(buffer));
  LoRa.endPacket();;
  delay(100);
}
