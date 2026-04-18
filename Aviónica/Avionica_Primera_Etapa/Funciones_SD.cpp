/********************************************************************************************************************************************
Club de Investigación Univesitario de Desarrollo en Sistemas Espaciales
AVIÓNICA PRIMERA ETAPA - Misión IREC 2026 - Archivo de funciones de memoria SD de Teensy 4.1
Código desarrollado por equipo de Aviónica y Estación Terrena

Archivo de funciones para guardar los datos en la memoria SD (Memoria de SD de Teensy 4.1).
*********************************************************************************************************************************************/
#include "Avionica_Primera_Etapa.h"

File datalog;

char fileName[40];
char buffer[200];

int i = 0;

void inicializarSD()
{
  //Hacer ifs de deteccion de gps
  if (!SD.begin(BUILTIN_SDCARD)) 
  {
    Serial.println("Error inicializando SD");
    return;
  }

  // Crear nombre con fecha (Ejemplo: avdata_20260403_153045.txt)
  if(Serial8.available())
    sprintf(fileName, "avdata_%04d%02d%02d_%02d%02d%02d.txt", gps.date.year(), gps.date.month(), gps.date.day(), gps.time.hour(), gps.time.minute(), gps.time.second());

  Serial.print("Archivo: "); 
  Serial.println(fileName);

}

void guardarSD()
{
  datalog = SD.open(fileName, FILE_WRITE);

  snprintf(buffer, sizeof(buffer), "%.6f,%.6f,%c,%u,%u,%d,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",
      sensorData.latitud,
      sensorData.longitud,
      telemetryData.id,
      i,
      telemetryData.indicadorEstadoVuelo,
      sensorData.satelites,
      sensorData.latitud,
      sensorData.longitud,
      sensorData.presion,
      sensorData.altura_barometro,
      sensorData.temperatura,
      sensorData.velocidad,
      sensorData.vel_ang_x,
      sensorData.vel_ang_y,
      sensorData.vel_ang_z,
      sensorData.accel_x,
      sensorData.accel_y,
      sensorData.accel_z
    );

    if(datalog)
    {    
      datalog.print(buffer);
      datalog.close();
    }
    else
    {
      Serial.println("Error abriendo archivo");
    }
    i++;
}