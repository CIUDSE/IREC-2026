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
volatile bool loraEnviando = false;
unsigned long tiempoInicioEnvio = 0;

// ─── Callback: se dispara por DIO0/G0 cuando termina la transmisión ──────────
void onLoraTxDone()
{
  loraEnviando = false;
}

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

  LoRa.setSyncWord(SYNC_WORD);
  LoRa.setSpreadingFactor(SPREADING_FACTOR);      //SF modificar cuando se use la antena posiblmentente a 10
  LoRa.setSignalBandwidth(BANDWIDTH);  // SB modificar cuando la antena posiblemente a 125
  LoRa.setCodingRate4(CODING_RATE);

  //Registrar callback AL FINAL
  LoRa.onTxDone(onLoraTxDone);

  if (Serial)
    Serial.println("LoRa iniciado correctamente");
}

void enviarDatos()
{
  // Si aún está transmitiendo, no interrumpir
  if (loraEnviando) return;

  loraEnviando = true;

  LoRa.beginPacket();
  LoRa.write((uint8_t*)&telemetryData, sizeof(telemetryData_t));
  LoRa.endPacket(true);   // true = asíncrono, NO bloqueante

  tiempoInicioEnvio   = millis();
}

void timeoutLora()
{
  // Timeout de seguridad — libera loraEnviando si el callback no se disparó
  if (loraEnviando && millis() - tiempoInicioEnvio > 1500)
  {
    loraEnviando = false;  // Forzar liberación
  }
}
