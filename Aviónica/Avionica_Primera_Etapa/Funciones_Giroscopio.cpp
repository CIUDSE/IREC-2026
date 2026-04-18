/********************************************************************************************************************************************
Club de Investigación Univesitario de Desarrollo en Sistemas Espaciales
AVIÓNICA PRIMERA ETAPA - Misión IREC 2026 - Archivo de funciones giroscopio
Código desarrollado por equipo de Aviónica y Estación Terrena

Archivo de funciones para el sensor giroscopio BNO085 (IMU 9 DoF).

Valores leídos por IMU son: velocidad angular en x, y, z; aceleracion lineal, x, y, z.
*********************************************************************************************************************************************/
#include "Avionica_Primera_Etapa.h"

Adafruit_BNO08x BNO08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;

void inicializarGiroscopio()
{
  // Iniciar SPI Para el Giroscopio
  SPI1.setMOSI(MOSI1);
  SPI1.setMISO(MISO1);
  SPI1.setSCK(SCK1);
  SPI1.begin();

  //Setup Giroscopio
  if (!BNO08x.begin_SPI(BNO08X_CS, BNO08X_INT, &SPI1, 1000000)) 
  {
    Serial.println("Error al iniciar Giroscopio");
    delay(10);
    while (1);
  }

  setReports();
}

void setReports(void) 
{
  Serial.println("Setting desired reports");
  if (!BNO08x.enableReport(SH2_ACCELEROMETER)) 
    Serial.println("Could not enable accelerometer");

  if (!BNO08x.enableReport(SH2_GYROSCOPE_CALIBRATED))
    Serial.println("Could not enable gyroscope");
}

void leerGiroscopio()
{
  if (BNO08x.getSensorEvent(&sensorValue)) 
  {
    switch (sensorValue.sensorId)
    {
    case SH2_GYROSCOPE_CALIBRATED:   //velocidad angular
      sensorData.vel_ang_x = sensorValue.un.gyroscope.x;
      sensorData.vel_ang_y = sensorValue.un.gyroscope.y;
      sensorData.vel_ang_z = sensorValue.un.gyroscope.z;
      break; 

    case SH2_ACCELEROMETER:          //aceleracion lineal
      sensorData.accel_z = (sensorValue.un.accelerometer.x - 9.81); //Nuestra z es la x del giroscopio. Restamos la gravedad.
      sensorData.accel_x = sensorValue.un.accelerometer.z;
      sensorData.accel_y = sensorValue.un.accelerometer.y;
      break;
    }
    
    //Valores convertidos a entero para enviar en la telemetría
    telemetryData.vel_ang_x_32 = (int32_t)(sensorData.vel_ang_x * 100);
    telemetryData.vel_ang_y_32 = (int32_t)(sensorData.vel_ang_y * 100);
    telemetryData.vel_ang_z_32 = (int32_t)(sensorData.vel_ang_z * 100);

    telemetryData.accel_x_16 = (int16_t)(sensorData.accel_x * 100);
    telemetryData.accel_y_16 = (int16_t)(sensorData.accel_y * 100);
    telemetryData.accel_z_16 = (int16_t)(sensorData.accel_z * 100); 
  } 
}