//Github de la libreria: https://github.com/adafruit/Adafruit_BNO08x/tree/master

#include <TinyGPS++.h>
#include <Wire.h>
#include "MS5611.h"
#include <Adafruit_BNO08x.h>
#include <SPI.h>
#include <LoRa.h>

// Pines del Teensy
  //RFM95W Lora
#define LORA_SS   10
#define LORA_RST   9
#define LORA_DIO0  2    // INT
#define BAND 915.5E6
  //Led-Buzzer
#define LED        0
  //Giroscopio
#define BNO08X_CS     37
#define BNO08X_INT    6
#define BNO08X_RESET  5
#define MOSI1         26
#define MISO1         12
#define SCK1          27

//Barometro
MS5611 MS5611(0x77);
float presion_ref = 0; 
long contador = 0;
double max_altura = 0;

//Giroscopio
Adafruit_BNO08x bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;
float accel_x, accel_y, accel_z;
float gyro_x, gyro_y, gyro_z;
float angulo_x, angulo_y;

//Objetos de sensores
TinyGPSPlus gps;

//Estructura que contenga los datos de telemetria
typedef struct telemetryData {
  double latitud;
  double longitud;
  double altura_gps;
  double altura_barometro;
  double angulo_x, angulo_y, angulo_z, accel_z;
}telemetryData_t;

//Crear una variable de la estructura
telemetryData_t telemetryData;

// Constantes de las etapas
int desacople = 300;
int alt_motor = 400;
int apogeo_etapa1 = 500;
int apogeo_etapa2 = 500;
const int pinBuzzer = 0;
const int mainn = 3;
const int drouge = 4;
const int segundaEtapa = 8;

void setup() {
  pinMode(pinBuzzer, OUTPUT);
  pinMode(mainn, OUTPUT);
  pinMode(drouge, OUTPUT);
  pinMode(segundaEtapa, OUTPUT);

  Serial.begin(115200);

  if (Serial.dtr()) {  // solo si está abierto el puerto en PC
  while (!Serial) delay(10);}

  // Iniciar SPI Para el Lora
  SPI.begin();

  // Iniciar SPI Para el Giroscopio
  SPI1.setMOSI(MOSI1);
  SPI1.setMISO(MISO1);
  SPI1.setSCK(SCK1);
  SPI1.begin();

  //Iniciar I2C
  Wire.begin();

  //Led-Buzzer
  pinMode(LED, OUTPUT);

  //Setup de Barometro
  if (MS5611.begin() == true) {
    Serial.println(MS5611.getAddress());
  } else {
    Serial.println("MS5611 no encontrado.");
  }

  for (int i = 0; i < 100; i++) { // calibra a 0 en la altura inicializada
      MS5611.read();
    presion_ref += MS5611.getPressure();
    delay(10);
  }
  presion_ref /= 100.0;

  //Setup Giroscopio
  if (!bno08x.begin_SPI(BNO08X_CS, BNO08X_INT, &SPI1, 1000000)) {
    Serial.println("Error al iniciar Giroscopio");
    delay(10);
    while (1);
  }
  setReports();

  //Setup de LoRa
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Fallo al iniciar LoRa");
    //while (1);
  }

  LoRa.setSyncWord(0xA0);
  LoRa.setSpreadingFactor(7); //SF modificar cuando se use la antena posiblmentente a 10
  LoRa.setSignalBandwidth(500E3); // SB modificar cuando la antena posiblemente a 125

  Serial.println("LoRa iniciado en Teensy!");
    
  //Setup GPS
  Serial8.begin(9600);
  Serial.println("Inicializando sistema de telemetría...");
}

void setReports(void) 
{
  Serial.println("Setting desired reports");
  if (!bno08x.enableReport(SH2_ACCELEROMETER)) 
  {
    Serial.println("Could not enable accelerometer");
  }
}

void loop() {
  

  leergps();

  barometro();

  giroscopio();
  
  //etapa1();

  //etapa2();

  //Paracaidas emergencia
  if(telemetryData.altura_barometro < (max_altura-200)){
      //activar paracaidas
    }
  
    
  
  //Telemetria 
    //Crear arreglo de bytes iguales(buffer)
  uint8_t buffer[sizeof(telemetryData_t)];
  memcpy(buffer, &telemetryData, sizeof(telemetryData_t));
    //Enviar los datos a LoRa
  LoRa.beginPacket();
  LoRa.write(buffer, sizeof(buffer));

  LoRa.endPacket();   // activar esto para pruebas con telemetria
  
  delay(100);

  Serial.println("");
  
}

void leergps(){

   //Leer GPS continuamente
  while (Serial8.available() > 0){
    gps.encode(Serial8.read());
    
  }
  if(gps.location.isUpdated()){
    telemetryData.latitud = gps.location.lat();
    telemetryData.longitud = gps.location.lng();
    telemetryData.altura_gps = gps.altitude.meters();
    //Es importante separarlo con comas sin espacios para formato kml

    Serial.print(gps.location.lat(), 6);
    Serial.print(",");
    Serial.print(gps.location.lng(), 6);
    Serial.print(",");
    Serial.print(gps.altitude.meters(),1);
    Serial.print(",");

    /*
    Serial.print(telemetryData.latitud, 6);
    Serial.print(",");
    Serial.print(telemetryData.longitud, 6);
    Serial.print(",");
    Serial.print(telemetryData.altura_gps,1);
    Serial.print(",");
    */

  } else {
    Serial.print("Esperando GPS...");
  }
  
}

void barometro(){
  if (MS5611.read() == MS5611_READ_OK){
    telemetryData.altura_barometro = 44330.0 * (1.0 - pow(MS5611.getPressure() / presion_ref, 0.1902949));
    Serial.print(telemetryData.altura_barometro);
    Serial.print(",");

    if(telemetryData.altura_barometro>max_altura){
      max_altura = telemetryData.altura_barometro;
      //Serial.print(max_altura);
    }
    
  }
}

void giroscopio(){
  if (bno08x.getSensorEvent(&sensorValue)) {
    switch (sensorValue.sensorId){
      case SH2_ACCELEROMETER:
      telemetryData.accel_z = -1*(sensorValue.un.accelerometer.x - 9.8); //Nuestra x es la z del cohete
      //telemetryData.accel_x = sensorValue.un.accelerometer.x;
      //telemetryData.accel_y = sensorValue.un.accelerometer.y;

      accel_x = sensorValue.un.accelerometer.x;
      accel_y = sensorValue.un.accelerometer.y;
      accel_z = sensorValue.un.accelerometer.z;

      telemetryData.angulo_x=atan2(accel_x, sqrt(accel_y*accel_y + accel_z*accel_z)) * RAD_TO_DEG;
      telemetryData.angulo_y=atan2(accel_y, sqrt(accel_x*accel_x + accel_z*accel_z)) * RAD_TO_DEG; //z

      Serial.print(telemetryData.angulo_x, 2);
      Serial.print(",");
      Serial.print(telemetryData.angulo_y, 2);
      Serial.print(",");
      Serial.print(telemetryData.accel_z, 2);
      Serial.print(",");
      break;
    }
  }
}

void etapa1(){
   if (telemetryData.altura_barometro > desacople){
      //Activar desacople
    }
    //activacion del paracaidas 
    if (telemetryData.altura_barometro > apogeo_etapa1){
      if(telemetryData.altura_barometro < (max_altura-200)){
      //Activar paracaidas
      }
    }
}

void etapa2(){
  //activar motor segunda etapa
  if ((telemetryData.angulo_y > -20 && telemetryData.angulo_y < 20) && 
    (telemetryData.angulo_x > 70) 
    //&& (telemetryData.altura_barometro > alt_motor)
    )
    {
      digitalWrite(mainn, HIGH);

    }else{

      digitalWrite(mainn, LOW);

    }

    //activacion del paracaidas 
    if (telemetryData.altura_barometro > apogeo_etapa2){
      if(telemetryData.altura_barometro < (max_altura-200)){
      //Activar paracaidas
      }
    }
}