// LILYGO LORA RECEPTOR (CON PANTALLA OLED)

#include <SPI.h>
#include <LoRa.h>
#include <Wire.h> 
#include <Adafruit_GFX.h> 
#include <Adafruit_SSD1306.h>

// --------CONFIGURACIÓN DE PINES-------------------------------------
// Pines de LoRa (Chip SX1276)
#define LORA_SCK 5
#define LORA_MISO 19
#define LORA_MOSI 27
#define LORA_SS 18
#define LORA_RST 23
#define LORA_DIO0 26

// Pines de la pantalla OLED (I2C)
#define OLED_SDA  21
#define OLED_SCL  22
#define OLED_RST  16  // Pin de reset

// Frecuencia de LoRa - DEBE COINCIDIR CON EL EMISOR - 433E6 (Asia), 868E6 (Europa), 915E6 (Norteamérica)
#define BAND    915.5E6 

// Tamaño de la pantalla
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET -1

// Pantalla
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); 

// -------------------------------------------------------------------
typedef struct telemetryData {
  double latitud;
  double longitud;
  double altura_gps;
  uint8_t satelites;
}telemetryData_t;


telemetryData_t telemetryData;

void setup() 
{
  Serial.begin(115200);
  Serial.println(BAND);
  // 1. Inicializar OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C))
  {
    Serial.println("Fallo OLED");
    while(1);
  }

  // 2. Config pantalla
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.println("Esperando datos...");
  display.display();

  // 3. Inicializar LoRa
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_SS);
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  
  // 4. Chequeo
  if(!LoRa.begin(BAND))
  {
    Serial.println("Fallo LoRa");
    while(1);
  }

  // Asignar palabra de sincronización (debe ser la misma en TX y RX)
  LoRa.setSyncWord(0xA0);

  // ESTAS LÍNEAS SON IMPORTANTES PARA LA LATENCIA 
  LoRa.setSpreadingFactor(7);     // Mínimo SF para máx velocidad
  LoRa.setSignalBandwidth(500E3); // Máximo BW para máx velocidad

  Serial.println("LoRa iniciado");
}

// ------------------------------------------------------------------
// LOOP
// ------------------------------------------------------------------
void loop() 
{

  int packetSize = LoRa.parsePacket(); 

  //Comprobrar que lo recibido sea igual a los datos de la telemetria
  if (packetSize == sizeof(telemetryData_t)) 
  {
    uint8_t buffer[sizeof(telemetryData_t)];

    for(int i = 0; i<sizeof(telemetryData_t); i++){ //Recibir todos los datos a la variable buffer
      buffer[i] = LoRa.read();
    }

    memcpy(&telemetryData,buffer,sizeof(buffer)); //Copiar los datos guardados en buffer a telemetryData
    int rssi = LoRa.packetRssi();
    float snr = LoRa.packetSnr();

    //Imprimir en el Monitor Serial
    Serial.print("\n-> MENSAJE: ");
    Serial.print("latitud: ");
    Serial.println(telemetryData.latitud,6);
    Serial.print("longitud: ");
    Serial.println(telemetryData.longitud,6);
    Serial.print("altura: ");
    Serial.println(telemetryData.altura_gps,2);
    Serial.print("   (RSSI: ");
    Serial.print(rssi);
    Serial.print(" dBm | SNR: ");
    Serial.print(snr);
    Serial.println(")");

    // 3. Mostrar en la pantalla OLED
    updateDisplay(rssi, snr);
  }
}

// ------------------------------------------------------------------

// Función para actualizar la pantalla con los datos recibidos
void updateDisplay(int rssi, float snr) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);

  // Encabezado
  display.println("<< DATOS RECIBIDOS >>");
  display.drawFastHLine(0, 9, 128, SSD1306_WHITE); // Línea divisoria

  // Datos
  display.setTextSize(1); // Texto más grande para el mensaje
  display.setCursor(0, 15);
  //Mostrar todos los datos obtenidos
  display.print("latitud: ");
  display.println(telemetryData.latitud,6);
  display.print("longitud: ");
  display.println(telemetryData.longitud,6);
  display.print("altura: ");
  display.println(telemetryData.altura_gps,2);
  
  // Métricas (Parte inferior)
  display.setTextSize(1);
  display.setCursor(0, 48);
  display.print("RSSI: ");
  display.print(rssi);
  display.println(" dBm");

  display.setCursor(0, 56);
  display.print("SNR: ");
  display.print(snr, 1); // 1 decimal
  display.println(" dB");

  display.display(); // Muestra el buffer en la pantalla física
}