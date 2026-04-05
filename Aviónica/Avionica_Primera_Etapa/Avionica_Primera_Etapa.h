/********************************************************************************************************************************************
Club de Investigación Univesitario de Desarrollo en Sistemas Espaciales
AVIÓNICA PRIMERA ETAPA - Misión IREC 2026 - Archivo header
Código desarrollado por equipo de Aviónica y Estación Terrena

Archivo header de la primera etapa con la declaración de pines y configuración inicial.
*********************************************************************************************************************************************/
#pragma once

#include <TinyGPS++.h>
#include <Wire.h>
#include "MS5611.h"
#include <Adafruit_BNO08x.h>
#include <SPI.h>
#include <LoRa.h>
#include <SD.h>

//--------------------------- Pines del Teensy ---------------------------//
//Paracaídas
#define main 3
#define drogue 4
#define segundaEtapa 8

//RFM95W Lora
#define LORA_SS   10
#define LORA_RST   9
#define LORA_DIO0  2
#define BAND   917E6

//Led-Buzzer
#define pinBuzzer  0
#define LED        36

//Giroscopio
#define BNO08X_CS     37
#define BNO08X_INT    6
#define BNO08X_RESET  5
#define MOSI1         26
#define MISO1         12
#define SCK1          27

//--------------------------- Objetos ---------------------------//
//Barometro
extern MS5611 barometro;

//Giroscopio
extern Adafruit_BNO08x BNO08x;

//GPS
extern TinyGPSPlus gps;

//SD
extern File datalog;

//--------------------------- Estructura de datos de telemetría ---------------------------//
//Tiempo de envío con este struct y SF de 11 y BW de 125E3 es de 1070 ms
typedef struct __attribute__((packed)) 
{
  char id;
  uint16_t tiempoRecibido;
  uint16_t numPaquete;
  uint8_t indicadorEstadoVuelo;
  
  uint8_t satellites;
  int32_t latitud_32;          // *1e6
  int32_t longitud_32;         // *1e6

  uint32_t presion_32;         // 100
  int32_t altura_barometro_32; // 100
  int16_t temperatura_16;      // 100
  int32_t velocidad_32;        // 100

  int32_t vel_ang_x_32;        // 100
  int32_t vel_ang_y_32;        // 100
  int32_t vel_ang_z_32;        // 100
  int16_t accel_x_16;          // 100
  int16_t accel_y_16;          // 100
  int16_t accel_z_16;          // 100

} telemetryData_t;

extern telemetryData_t telemetryData;

//--------------------------- Estructura de datos de sensores ---------------------------//
typedef struct sensorData 
{
  int satelites;
  double latitud;
  double longitud;
  float altura_barometro;
  float velocidad;
  float presion;
  float temperatura;
  float vel_ang_x, vel_ang_y, vel_ang_z;
  float accel_z, accel_x, accel_y;
} sensorData_t;

extern sensorData_t sensorData;

//--------------------------- Variables y constantes en el código ---------------------------//
// NOTA: Estas son las declaraciones de que se usarán estas variables y constantes en el código, pero cada una esta definida en su respectivo archivo
// de función. Aquí solo se declaran, pero en cada archivo se le da su respectivo valor. Las variables aqui son tipo "extern" por ese motivo.

#define ID '2'    //ID correspondiente a la primera etapa

//LoRa
#define SYNC_WORD 0xA0
#define SPREADING_FACTOR 11
#define BANDWIDTH 125E3
#define CODING_RATE 5
extern unsigned long tiempoSegundo;
extern unsigned long tiempoRespuesta;
extern int contadorPaquetes;
extern volatile bool loraEnviando;
extern unsigned long tiempoInicioEnvio;

//Giroscopio
extern sh2_SensorValue_t sensorValue;

//Barómetro 
extern float presionPromedioInicial;
extern int ajustePromedio;
extern unsigned long tiempoActual;
extern float alpha;                     // Factor de suavizado (0.0 a 1.0)
extern unsigned long tAnterior;
extern float altitudAnterior;
extern int max_altura;

//Paracaídas
extern int desacople;
extern int alt_motor;
extern int apogeo_etapa1;
extern int apogeo_etapa2;


//SD
extern char fileName[40];

//--------------------------- Funciones ---------------------------//
//Funciones LoRa
void inicializarLora();
void enviarDatos();
void actualizarLora();
void timeoutLora();

//Funciones Giroscopio
void inicializarGiroscopio();
void setReports(void);
void leerGiroscopio();

//Funciones GPS
void inicializarGPS();
void leerGPS();
void asegurarGPS();

//Funciones Barómetro
void inicializarBarometro();
void leerBarometro();

//Funciones Primera Etapa
void etapa1();
void eventoDeVuelo();

//Funciones SD
void inicializarSD();
void guardarSD();

//Funciones globales
void mostrarSerial();
