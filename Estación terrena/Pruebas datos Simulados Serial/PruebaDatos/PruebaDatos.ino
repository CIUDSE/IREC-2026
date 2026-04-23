// LILYGO LORA - SIMULADOR COHETE PRINCIPAL (ID: 1)

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

typedef struct __attribute__((packed)) {
  char id;
  uint16_t tiempoRecibido;
  uint16_t numPaquete;
  uint8_t indicadorEstadoVuelo;
  uint8_t satellites;
  int32_t latitud_32;          
  int32_t longitud_32;         
  uint32_t presion_32;         
  int32_t altura_barometro_32; 
  int16_t temperatura_16;      
  int32_t velocidad_32;        
  int32_t vel_ang_x_32;        
  int32_t vel_ang_y_32;        
  int32_t vel_ang_z_32;        
  int16_t accel_x_16;          
  int16_t accel_y_16;          
  int16_t accel_z_16;          
} telemetryData_t;

telemetryData_t telemetryData;

unsigned long ultimoTiempo = 0;
int contadorSimulacion = 0;
float altitudSimulada = 0.0;

void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Fallo OLED");
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println(" SIMULADOR C1 (ID:1) ");
  display.println("     LISTO... ");
  display.display();
  delay(2000);
}

void loop() {
  if (millis() - ultimoTiempo > 1000) { // 1 Hz
    ultimoTiempo = millis();
    contadorSimulacion++;
    
    // Perfil de vuelo C1: 
    altitudSimulada += 25.5; 

    // Llenamos datos especificos para el Cohete 1
    telemetryData.id = '1';
    telemetryData.tiempoRecibido = millis() / 1000;
    telemetryData.numPaquete = contadorSimulacion;
    telemetryData.indicadorEstadoVuelo = 1; 
    telemetryData.satellites = 10;
    
    // Coordenadas Base
    telemetryData.latitud_32 = (32.5337045 + (contadorSimulacion * 0.00002)) * 1e6;
    telemetryData.longitud_32 = (-115.7261352 + (contadorSimulacion * 0.000015)) * 1e6;
    telemetryData.altura_barometro_32 = altitudSimulada * 100;
    telemetryData.velocidad_32 = (250.0 + (contadorSimulacion % 5)) * 100; 
    telemetryData.presion_32 = (1012.5 - (altitudSimulada * 0.1)) * 100; // Presión baja al subir
    telemetryData.temperatura_16 = 25.0 * 100;
    
    // IMU 
    telemetryData.vel_ang_x_32 = 15;   
    telemetryData.vel_ang_y_32 = -10;  
    telemetryData.vel_ang_z_32 = 45;   
    telemetryData.accel_x_16 = 20;     
    telemetryData.accel_y_16 = 1250; 
    telemetryData.accel_z_16 = -5;     

    float latDec = telemetryData.latitud_32 / 1e6;
    float lonDec = telemetryData.longitud_32 / 1e6;
    float altDec = telemetryData.altura_barometro_32 / 100.0;
    float tempDec = telemetryData.temperatura_16 / 100.0;
    float presDec = telemetryData.presion_32 / 100.0;
    float velDec = telemetryData.velocidad_32 / 100.0;

    float gx = telemetryData.vel_ang_x_32 / 100.0;
    float gy = telemetryData.vel_ang_y_32 / 100.0;
    float gz = telemetryData.vel_ang_z_32 / 100.0; 
    float ax = telemetryData.accel_x_16 / 100.0;
    float ay = telemetryData.accel_y_16 / 100.0;
    float az = telemetryData.accel_z_16 / 100.0; 

    int rssi = -40 - (contadorSimulacion % 3); // señal
    float bateria = 8.2; // Bateria 
    float snr = 9.8;

    // Imprimir CSV 
    char datosCSV[300];
    sprintf(datosCSV, "%c,%u,%u,%u,%d,%.1f,%u,%.6f,%.6f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",
            telemetryData.id, telemetryData.tiempoRecibido, telemetryData.numPaquete, telemetryData.indicadorEstadoVuelo,
            rssi, bateria, telemetryData.satellites, latDec, lonDec, altDec, velDec, presDec, tempDec,
            gx, gy, gz, ax, ay, az);
    Serial.print(datosCSV);

    updateDisplay('1', rssi, snr, latDec, lonDec, altDec);
  }
}

void updateDisplay(char id, int rssi, float snr, float lat, float lon, float alt) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("<< RECIBIENDO: "); display.print(id); display.println(" >>");
  display.drawFastHLine(0, 9, 128, SSD1306_WHITE); 
  display.setTextSize(0.5); 
  display.setCursor(0, 15);
  display.print("Lat: "); display.println(lat, 6);
  display.print("Lon: "); display.println(lon, 6);
  display.print("Alt(m): "); display.println(alt, 2);
  display.setCursor(0, 48);
  display.print("RSSI: "); display.print(rssi); display.println(" dBm");
  display.display(); 
}