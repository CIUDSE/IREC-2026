/********************************************************************************************************************************************
Club de Investigación Univesitario de Desarrollo en Sistemas Espaciales
AVIÓNICA PRIMERA ETAPA - Misión IREC 2026 - Archivo de funciones de LoRa
Código desarrollado por equipo de Aviónica y Estación Terrena

Archivo de funciones para el LoRa RFM95.
*********************************************************************************************************************************************/
#include "Avionica_Primera_Etapa.h"

unsigned long tiempoSegundo = 0;            // Variable usada en archivo .ino
unsigned long tiempoRespuesta = 0;          // Variable usada en archivo .ino 
int contadorPaquetes = 0;                   // Variable usada en archivo .ino


void inicializarLora()
{
   // Iniciar LoRa
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(BAND)) 
  {
    if(Serial)
      Serial.println("Fallo al iniciar LoRa");
    while (1);
  }

  LoRa.setSyncWord(0xA0);
  LoRa.setSpreadingFactor(11);      //SF modificar cuando se use la antena posiblmentente a 10
  LoRa.setSignalBandwidth(125E3);  // SB modificar cuando la antena posiblemente a 125
  LoRa.setCodingRate4(5);
}

void enviarDatos()
{
  //Crear arreglo de bytes iguales(buffer)
  uint8_t buffer[sizeof(telemetryData_t)];
  memcpy(buffer, &telemetryData, sizeof(telemetryData_t));  

  LoRa.beginPacket();                    // 1. Iniciar el paquete de LoRa
  LoRa.write(buffer, sizeof(buffer));    // 2. Escribir el contenido del mensaje
  LoRa.endPacket();                      // 3. Finalizar el paquete y enviarlo
}