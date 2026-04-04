/********************************************************************************************************************************************
Club de Investigación Univesitario de Desarrollo en Sistemas Espaciales
AVIÓNICA PRIMERA ETAPA - Misión IREC 2026 - Archivo de funciones generales
Código desarrollado por equipo de Aviónica y Estación Terrena

Archivo de funciones generales.

telemetryData -> {ID etapa, Tiempo recibido, Número de paquete, Estado de vuelo, Satélites, Lat, Lon, Pres, Alt, Temp, Vel, GX, GY, GZ, AX, AY, AZ}
*********************************************************************************************************************************************/
#include "Avionica_Primera_Etapa.h"

void mostrarSerial()
{
  Serial.print(telemetryData.id);
  Serial.print(",");
  Serial.print(telemetryData.tiempoRecibido);
  Serial.print(",");
  Serial.print(telemetryData.numPaquete);
  Serial.print(",");
  Serial.print(telemetryData.indicadorEstadoVuelo);
  Serial.print(",");
  Serial.print(sensorData.satelites);
  Serial.print(",");
  Serial.print(sensorData.latitud, 6);
  Serial.print(",");
  Serial.print(sensorData.longitud, 6);
  Serial.print(",");
  Serial.print(sensorData.presion);
  Serial.print(",");
  Serial.print(sensorData.altura_barometro);
  Serial.print(",");
  Serial.print(sensorData.temperatura);
  Serial.print(",");
  Serial.print(sensorData.velocidad);
  Serial.print(",");
  Serial.print(sensorData.vel_ang_x);
  Serial.print(",");
  Serial.print(sensorData.vel_ang_y);
  Serial.print(",");
  Serial.print(sensorData.vel_ang_z);
  Serial.print(",");
  Serial.print(sensorData.accel_x);
  Serial.print(",");
  Serial.print(sensorData.accel_y);
  Serial.print(",");
  Serial.print(sensorData.accel_z);
  Serial.print(",");
  Serial.print("Enviado");
  Serial.println();
  
  /*
    //Imprimir en la Terminal Serial
    //Datos GPS
    Serial.println((telemetriaActual.latitud));
    Serial.print("   ");
    Serial.println(telemetriaActual.longitud, 6);
    Serial.print("   ");
    //Serial.println(telemetriaActual.altura_gps,1);
    //Datos Barometro
    Serial.print(alturaFinal);
    Serial.println("m");
    Serial.print(barometro.getTemperature(), 2);
    Serial.println("C");
    Serial.print(presionActualPromedio);
    Serial.println("hPa");




  Serial.print(presion);
  Serial.print(",");
  Serial.println(velocidad);
  Serial.print(",");
  Serial.print(temp);

  //velocidad angular
    Serial.print(vel_ang_x);
    Serial.print(",");
    Serial.print(vel_ang_y);
    Serial.print(",");
    Serial.print(vel_ang_z);
    //acceleracion
    Serial.print(accel_z, 2);
    Serial.print(",");
    Serial.print(accel_x, 2);
    Serial.print(",");
    Serial.print(accel_y, 2);
    Serial.print(",");

  //Es importante separarlo con comas sin espacios para formato kml
    Serial.print(telemetryData.satelites);
    Serial.print(",");
    Serial.print(telemetryData.latitud, 6);
    Serial.print(",");
    Serial.print(telemetryData.longitud, 6);
    Serial.print(",");  

  Serial.println("");
  */
}
