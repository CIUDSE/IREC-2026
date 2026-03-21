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
float gyro_x, gyro_y, gyro_z;


//Objetos de sensores
TinyGPSPlus gps;

//Estructura que contenga los datos de telemetria
typedef struct telemetryData {
  int IDetapa;
  float timestap;
  int numPaquete;
  int evento;
  //RSSI fuerza señal
  //bateria
  int satelites;
  float latitud, longitud;
  float altura_barometro;
  float velocidad;
  float presion;
  float temp;
  float vel_ang_x, vel_ang_y, vel_ang_z;
  float accel_z, accel_x, accel_y;
  float angulo_x, angulo_y, angulo_z;
}telemetryData_t;

//Crear una variable de la estructura
telemetryData_t telemetryData;

// Constantes de las etapas
int desacople = 300;
int alt_motor = 400;
int apogeo_etapa1 = 500;
int apogeo_etapa2 = 500;
int mainE1= 350;
int drogueE1= 400;
int mainE2= 350;
int drogueE2= 400;
const int pinBuzzer = 0;
const int mainn = 3;
const int drouge = 4;
const int segundaEtapa = 8;
bool inicio = true;
float alpha = 0.1; // Factor de suavizado (0.0 a 1.0)
unsigned long tAnterior = 0;
float altitudAnterior = 0;

//variables
int IDetapa;
float timestap;
int numPaquete;
int evento;
//RSSI fuerza señal
//bateria
int satelites;
float latitud, longitud;
float altura_barometro;
float velocidad;
float presion;
float temp;
float vel_ang_x, vel_ang_y, vel_ang_z;
float accel_z, accel_x, accel_y;
float angulo_x, angulo_y, angulo_z;

void setup() {
  pinMode(pinBuzzer, OUTPUT);
  pinMode(mainn, OUTPUT);
  pinMode(drouge, OUTPUT);
  pinMode(segundaEtapa, OUTPUT);

  Serial.begin(115200);
  Serial.begin(9600);

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

  Serial.println("Iniciando avionica");
    
  //Setup GPS
  Serial8.begin(9600);
  Serial8.write("\xB5\x62\x06\x08\x06\x00\xC8\x00\x01\x00\x01\x00\xDE\x6A"); //comando para 5Hz
  
}

void setReports(void) 
{
  Serial.println("Setting desired reports");
  if (!bno08x.enableReport(SH2_ACCELEROMETER)) 
  {
    Serial.println("Could not enable accelerometer");
  }
  if (!bno08x.enableReport(SH2_GYROSCOPE_CALIBRATED)) {
    Serial.println("Could not enable gyroscope");
  }
}

void loop() {
  
  //etapa1();

  //etapa2();

  leergps();

  barometro();

  giroscopio();
  
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  //Estado Cohete
  if((-1 < telemetryData.accel_z < 1)&&(telemetryData.altura_barometro < 50)){//Cambiar valores segun el acelerometro en reposo
      //estado: Pre-Launch
  }
  if((telemetryData.accel_z > 200)&&(telemetryData.altura_barometro > 50)){//
      //estado: Launch
  }

  //Este cambia dependiendo si es primera o segunda etapa
  //Pimera etapa:
  if((telemetryData.accel_z < 100)&&(telemetryData.altura_barometro > 500)){//
      //estado: Motor Burnout
  }
  //Segunda etapa:
  if((telemetryData.accel_z < 100)&&(telemetryData.altura_barometro > 500)){//
  //estado: Interstage Separation
    if((telemetryData.accel_z > 200)&&(telemetryData.altura_barometro > 600)){//
      //estado: Ignition
    }
  }


  if((telemetryData.accel_z < 1)&&(telemetryData.altura_barometro > 50)){//
      //estado: Descent
  }

  //Detecta paracaidas bla bla
  //dentro del if del paracaidas
  if((-1 < telemetryData.accel_z < 1)&&(telemetryData.altura_barometro < 50)){//Cambiar valores segun el acelerometro en reposo
      //estado: Landing
  }
  */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  
  
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


  //ID etapa
  //Tiempo interno del mcu
  //Número de paquete
  //numero de evento
  //fuerza de la senal (esto lo hace estacion terrena)
  //voltaje (este creo que no se puede)

  //$ID,Tiempo,Paquete,Evento,RSSI,Bat,Sat,Lat,Lon,Alt,Vel,Pres,Temp,GX,GY,GZ,AX,AY,AZ\n


  
}

void leergps(){
  // numero de satelites
  //latitud
  //longitud
  //altitud
  
   //Leer GPS continuamente
  while (Serial8.available()){
    gps.encode(Serial8.read());
  }

  if(gps.location.isUpdated()){

    latitud = gps.location.lat();
    longitud = gps.location.lng();
    satelites = gps.satellites.value();

    telemetryData.latitud = latitud;
    telemetryData.longitud = longitud;
    telemetryData.satelites = satelites;
  } 
  //Es importante separarlo con comas sin espacios para formato kml
    Serial.print(satelites);
    Serial.print(",");
    Serial.print(latitud, 6);
    Serial.print(",");
    Serial.print(longitud, 6);
    Serial.print(",");  
}

void barometro(){
  //presion
  //temperatura
  if (MS5611.read() == MS5611_READ_OK){
    altura_barometro = 44330.0 * (1.0 - pow(MS5611.getPressure() / presion_ref, 0.1902949));
    temp = MS5611.getTemperature();
    presion = MS5611.getPressure();

    telemetryData.presion = presion;
    telemetryData.temp = temp;
    telemetryData.altura_barometro = altura_barometro;

    unsigned long t = micros();
    float dt = (t - tAnterior) / 1000000.0;
    tAnterior = t;
    // 1. Velocidad cruda
    float velocidadInstantanea = (altura_barometro - altitudAnterior) / dt;
    // 2. Filtro paso bajo (suavizado)
    velocidad = (alpha * velocidadInstantanea) + ((1.0 - alpha) * velocidad);
    altitudAnterior = altura_barometro;

    telemetryData.velocidad = velocidad;

  }
    Serial.print(presion);
    Serial.print(",");
    Serial.println(velocidad);
    Serial.print(",");
    Serial.print(temp);
}

void giroscopio(){
  //velocidad en z
  //velocidad angular en x, y, z
  //aceleracion lineal, x, y, z

  

  if (bno08x.getSensorEvent(&sensorValue)) {
    switch (sensorValue.sensorId){
    //velocidad angular
    case SH2_GYROSCOPE_CALIBRATED:
    vel_ang_x = sensorValue.un.gyroscope.x;
    vel_ang_y = sensorValue.un.gyroscope.y;
    vel_ang_z = sensorValue.un.gyroscope.z;
    telemetryData.vel_ang_x = vel_ang_x;
    telemetryData.vel_ang_y = vel_ang_y;
    telemetryData.vel_ang_z = vel_ang_z;
    
    break; 

    //aceleracion lineal
    case SH2_ACCELEROMETER:
    accel_z = -1*(sensorValue.un.accelerometer.x - 9.81); //Nuestra x es la z del cohete
    accel_x = sensorValue.un.accelerometer.x;
    accel_y = sensorValue.un.accelerometer.y;
    telemetryData.accel_z = accel_z;
    telemetryData.accel_x = accel_x;
    telemetryData.accel_y = accel_y;
    
    break;
  }
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
  }
      
}

void etapa1(){
  //telemetryData.etapa = 'S1';
  //Serial.print(telemetryData.etapa);
/*
   if (telemetryData.altura_barometro > desacople){
      //Activar desacople
    }
    //activacion del paracaidas 
    if (telemetryData.altura_barometro > apogeo_etapa1){
      if(telemetryData.altura_barometro < (max_altura-200)){
      //Activar paracaidas
      }
  }


  
  if (inicio = true){
    if((-1 < telemetryData.accel_z < 1)&&(telemetryData.altura_barometro < 50)){//Cambiar valores segun el acelerometro en reposo
      //estado: Pre-Launch
    }
  }
  
  //si detecta aceleración positiva y su altura es mayor a 50
  if((telemetryData.accel_z > 200)&&(telemetryData.altura_barometro > 50)){//
    //estado: Launch
  }

  //si pasa la altura del desacople activar desacople
  if (telemetryData.altura_barometro > desacople){
    //descople

    //si detecta disminucion de velocidad detecta el apogeo
    if(-10 < telemetryData.accel_z < 10){
      //estado: apogeo
    }

    if(telemetryData.altura_barometro < drogueE1)){
      //Activar Drogue
      //estado: DrogueActivado

      if((telemetryData.altura_barometro < mainE1)&&()){
        //Activar Main
        //estado: MainActivado
      }
      //Desactivar Main
    }
      
  }
  


  

  if((telemetryData.accel_z < 1)&&(telemetryData.altura_barometro > 50)){//
      //estado: Descent
  }
*/

}

void etapa2(){
  //telemetryData.etapa = 'S2';
  //Serial.print(telemetryData.etapa);
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