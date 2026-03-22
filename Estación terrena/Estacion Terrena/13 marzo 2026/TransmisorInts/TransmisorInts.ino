#include <TinyGPS++.h>
#include <Wire.h>
#include "MS5611.h"
#include <Adafruit_BNO08x.h>
#include <SPI.h>
#include <LoRa.h>
// Pines del Teensy en lora
#define LORA_SS 10
#define LORA_RST 9
#define LORA_DIO0 2
#define BAND 915E6


//Objetos de sensores
TinyGPSPlus gps;
MS5611 barometro(0x77);

typedef struct __attribute__((packed)) {
  char id;
  uint32_t tiempoRecibido;
  uint16_t numPaquete;
  uint8_t indicadorEstadoVuelo;
  uint8_t satellites;

  int32_t latitud;      // *1e6
  int32_t longitud;     // *1e6
  int32_t altura_bar;   // 100
  int16_t temperatura;  // 100
  uint32_t presion;     // 100

} telemetryData_t;



//Crear una variable de la estructura
telemetryData_t telemetriaActual;


//Variables globales barometro
float presionPromedioInicial = 0;
unsigned long tiempoLectura = 0;  // cada 10 ms
unsigned long tiempoSegundo = 0;  // cada 1 segundo
unsigned long tiempoActual = 0;
float presionActualPromedio = 0;
int contador = 0;

//Variables para enviar por telemetria
int contadorPaquetes = 0;
unsigned long tiempoRespuesta = 0;
void setup() {
  Serial.begin(115200);
  // Iniciar SPI no ocupa argumentos
  SPI.begin();
  Wire.begin();

  //Setup de LoRa
  // Configurar pines del modulo
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Fallo al iniciar LoRa");
    while (1)
      ;
  }

  LoRa.setSyncWord(0xA0);
  LoRa.setSpreadingFactor(11);      //SF modificar cuando se use la antena posiblmentente a 10
  LoRa.setSignalBandwidth(125E3);  // SB modificar cuando la antena posiblemente a 125
  LoRa.setCodingRate4(5);
  Serial.println("LoRa iniciado en Teensy!");


  //Setup GPS
  Serial8.begin(9600);
  Serial.println("GPS inciado");

  //Setup Barometro
  float ajustePromedio = 100;
  if (barometro.begin() == true) {
    Serial.println("barometro Iniciado correctamente.");
    barometro.setOversampling(OSR_STANDARD);  // activar oversampling máximo
  }
  // Calcular presión inicial promediada
  Serial.println("Esperando 5 segundos para calibrar barometro.");
  for (int i = 0; i < 50; i++) {
    tiempoActual = millis();
    while (millis() - tiempoActual <= 100) {
    }
  }
  for (int i = 1; i <= ajustePromedio; i++) {
    barometro.read();
    presionPromedioInicial += barometro.getPressure();
    tiempoActual = millis();
    while (millis() - tiempoActual <= 10) {
    }
  }
  presionPromedioInicial = presionPromedioInicial / ajustePromedio;
  Serial.println("Barometro calibrado.");
  Serial.print("Presion promedio inicial: ");
  Serial.println(presionPromedioInicial);

  //Setup Giroscopio
  //
  //
  Serial.println("Datos de telemetria:");


  //Definir datos (temporales)
  telemetriaActual.id = '1';
  telemetriaActual.indicadorEstadoVuelo = 0;
}

void loop() {
  // Leer GPS continuamente
  while (Serial8.available() > 0) {
    gps.encode(Serial8.read());
  }

  //Barometro
  if (millis() - tiempoLectura >= 19) {
    tiempoLectura = millis();
    barometro.read();
    float presionActual = barometro.getPressure();
    contador++;
    presionActualPromedio += presionActual;
  }

  //Giroscopio
  //
  //

  //Enviar los datos cada 1000 ms
  if (millis() - tiempoSegundo >= 500) {
    Serial.println("Enviando paquete: ");
    tiempoSegundo = millis();

    contadorPaquetes++;
    telemetriaActual.numPaquete = contadorPaquetes;

    //GPS datos
    if (gps.location.isUpdated() && gps.location.isValid()) {
      telemetriaActual.latitud = (int32_t)(gps.location.lat() * 1e6);  //espeicificamos tipo de variable
      telemetriaActual.longitud = (int32_t)(gps.location.lng() * 1e6); 
      telemetriaActual.satellites = gps.satellites.value();
    }


    //Barometro datos
    if (contador > 0) {  //Se actualiza si se contaron datos
      presionActualPromedio = presionActualPromedio / contador;
      //presionActualPromedio = 326.15; //presion aproximada para ver si daba los 9km
    }
    float alturaFinal = 44330 * (1 - pow((presionActualPromedio / presionPromedioInicial), (1.0 / 5.255)));


    telemetriaActual.altura_bar = (int32_t)(alturaFinal * 100);
    telemetriaActual.temperatura = (int16_t)(barometro.getTemperature() * 100);
    telemetriaActual.presion = (uint32_t)(presionActualPromedio * 100);

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

    // Reset para el siguiente intervalo
    contador = 0;
    presionActualPromedio = 0;


    telemetriaActual.tiempoRecibido =  millis() - tiempoRespuesta;
    tiempoRespuesta = millis();
    //Enviar los  datos por LoRa:
    uint8_t buffer[sizeof(telemetryData_t)];  //Crear arreglo de bytes iguales(buffer)
    memcpy(buffer, &telemetriaActual, sizeof(telemetryData_t));
    
    //Enviar los datos a LoRa
    LoRa.beginPacket();
    LoRa.write(buffer, sizeof(buffer));
    LoRa.endPacket();
    
  }
}
