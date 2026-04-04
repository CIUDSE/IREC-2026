/********************************************************************************************************************************************
Club de Investigación Univesitario de Desarrollo en Sistemas Espaciales
AVIÓNICA PRIMERA ETAPA - Misión IREC 2026 - Archivo principal
Código desarrollado por equipo de Aviónica y Estación Terrena

Código de aviónica para el envío de telemetría y funcionamiento de la PRIMERA ETAPA del cohete.

Microcontrolador: Teensy 4.1

Librerias necesarias para descargar:
Adafruit BNO08x by Adafruit
LoRa by Sandeep Mistry
TinyGPSPlus by Mikal Hart
MS5611 by Rob Tillart

Estructura de envio de datos:
telemetryData -> {ID etapa, Tiempo recibido, Número de paquete, Estado de vuelo, Satélites, Lat, Lon, Pres, Alt, Temp, Vel, GX, GY, GZ, AX, AY, AZ}
*********************************************************************************************************************************************/
#include "Avionica_Primera_Etapa.h"

sensorData_t sensorData;            //Struct utilizado para datos de sensores en sus tipo de datos originales.
telemetryData_t telemetryData;      //Struct utilizado para envío de datos de telemetria. Aqui todos los datos son de tipo int.

void setup() 
{
  //Definir datos
  telemetryData.id = ID;
  telemetryData.indicadorEstadoVuelo = 0;

  Serial.begin(115200);
  if (Serial.dtr()) {   // solo si está abierto el puerto en PC
    while (!Serial) delay(10);}

  SPI.begin();  // Iniciar SPI Para el Lora
  Wire.begin(); // Iniciar I2C

  inicializarLora();
  inicializarGiroscopio();
  inicializarBarometro();
  
  inicializarGPS();
  asegurarGPS();

  pinMode(pinBuzzer, OUTPUT); 
  pinMode(LED, OUTPUT);
  pinMode(main, OUTPUT);
  pinMode(drogue, OUTPUT);
  pinMode(segundaEtapa, OUTPUT);

  inicializarSD();
    
  Serial.println("Iniciando avionica");
}

void loop() 
{
  leerGPS();
  leerBarometro();
  leerGiroscopio();
  //eventoDeVuelo();
  //etapa1();

  //Secuencia de envio de datos por LoRa
  timeoutLora();  // Timeout de seguridad — libera loraEnviando si el callback no se disparó

  if (!loraEnviando)           //Enviar los datos por LoRa
  {
    tiempoSegundo = millis();
    contadorPaquetes++;
    telemetryData.numPaquete = contadorPaquetes;
    telemetryData.tiempoRecibido = millis() - tiempoRespuesta;
    tiempoRespuesta = millis();

    enviarDatos();
  }

  guardarSD();
  mostrarSerial();  //Muestra todos los datos en el monitor serial
}
