/********************************************************************************************************************************************
Club de Investigación Univesitario de Desarrollo en Sistemas Espaciales
AVIÓNICA PRIMERA ETAPA - Misión IREC 2026 - Archivo de funciones de GPS
Código desarrollado por equipo de Aviónica y Estación Terrena

Archivo de funciones para el GPS GT-U7.

Valores leídos por GPS son: latitud, longitud, número de satelites.

//Los valores de latitud y longitud se multiplican por 1x10^6 para mantenerlos como int32_t al momento de enviarlos por LoRa.
*********************************************************************************************************************************************/
#include "Avionica_Primera_Etapa.h"

TinyGPSPlus gps;

void inicializarGPS()
{
  Serial8.begin(9600);
  Serial.println("GPS inciado");
}


void leerGPS()
{
  // Leer GPS continuamente
  while (Serial8.available()) 
  {
    gps.encode(Serial8.read());
  }

  //GPS datos
  if (gps.location.isUpdated()) 
  {
    //Valores de sensorData para aviónica interna
    sensorData.latitud = gps.location.lat();
    sensorData.longitud = gps.location.lng();
    sensorData.satelites = gps.satellites.value();
  }
  
  //Valores convertidos a entero para enviar en la telemetría
  telemetryData.latitud_32 = (int32_t)(sensorData.latitud * 1e6);
  telemetryData.longitud_32 = (int32_t)(sensorData.longitud * 1e6); 
  telemetryData.satellites = gps.satellites.value();
}


void asegurarGPS()
{
  while (!gps.location.isValid())
  {
    while (Serial8.available())
    {
      gps.encode(Serial8.read());
    }
  }
}
