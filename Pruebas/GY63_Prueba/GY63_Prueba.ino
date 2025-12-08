/***********************************************************************
Club de Investigación Univesitario de Desarrollo en Sistemas Espaciales
PRUEBA GY63 - Misión IREC 2026

Librerias necesarias:
MS5611 por Rob Tillaart
Código obtenido de: Codigos de ejemplo de libreria.
***********************************************************************/

#include "MS5611.h"

MS5611 MS5611(0x77);

void setup() 
{
  //Inicializamos monitor Serial
  Serial.begin(115200);
  while (!Serial);

  //Inicializamos I2C 
  Wire.begin();

  if (MS5611.begin() == true)
  {
    Serial.print("MS5611 encontrado: ");
    Serial.println(MS5611.getAddress());
  }
  else
  {
    Serial.println("MS5611 no encontrado.");
  }

  Serial.println();
  Serial.println("Celsius\tmBar\tMetros\tMetros");
}

void loop() 
{  
  if (MS5611.read() == MS5611_READ_OK)   // Lectura exitosa
  {
    Serial.print("Temperatura: ");
    Serial.print(MS5611.getTemperature(), 2);
    Serial.print(" °C\t");
    
    Serial.print("Presión: ");
    Serial.print(MS5611.getPressure(), 2);
    Serial.print(" Pa\t");

    Serial.print("AltitudSensor: ");    //Altitud calculada por el sensor. Realmente es la misma formula de abajo pero con mas decimales en el calculo.
    Serial.print(MS5611.getAltitude(), 2);
    Serial.print(" m\t");
    
    // Calcular altitud aproximada (opcional)
    float altitude = 44330.0 * (1.0 - pow((MS5611.getPressure() / 101325.0), 0.1903));
    Serial.print("AltitudCalculada: ");
    Serial.print(altitude, 2);
    Serial.println(" m");
  } 
  else 
  {
    Serial.print("Error en la lectura: ");
    Serial.println(result);
  }
  
  delay(1000); // Esperar 1 segundo entre lecturas

}
