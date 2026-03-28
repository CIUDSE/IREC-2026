/********************************************************************************************************************************************
Club de Investigación Univesitario de Desarrollo en Sistemas Espaciales
AVIÓNICA PRIMERA ETAPA - Misión IREC 2026 - Archivo de funciones de barometro
Código desarrollado por equipo de Aviónica y Estación Terrena

Archivo de funciones para el barómetro MS5611 (Módulo GY63).
*********************************************************************************************************************************************/
#include "Avionica_Primera_Etapa.h"

MS5611 barometro(0x77);

float presionPromedioInicial = 0.0;   //Variable de presion de referencia en el standby del cohete.
int ajustePromedio = 100;             //Variable de número de iteraciones para calcular el promedio inicial.
float alpha = 0.1;                    // Factor de suavizado (0.0 a 1.0)
unsigned long tAnterior = 0;
float altitudAnterior = 0;
unsigned long tiempoActual = 0;

void inicializarBarometro()
{
  //Setup de Barometro
  if (barometro.begin() == true) 
  {
    Serial.println("Barometro iniciado correctamente.");
    //barometro.setOversampling(OSR_STANDARD);  // activar oversampling máximo
  } 
  else 
  {
    Serial.println("Barometro no encontrado.");
  }

   //Espera de 5 segundos para que no suelte datos random el barometro.
  delay(5000);
  /*Serial.println("Esperando 5 segundos para calibrar barometro.");
  for (int i = 0; i < 50; i++) 
  {
    tiempoActual = millis();
    while (millis() - tiempoActual <= 100) {}
  }*/

  //Calibra a 0 en la altura inicializada. 
  for (int i = 0; i < ajustePromedio; i++) 
  { 
    barometro.read();
    presionPromedioInicial += barometro.getPressure();
    delay(10);
  }
  presionPromedioInicial /= ajustePromedio;
}

void leerBarometro()
{
  if (barometro.read() == MS5611_READ_OK)
  {
    sensorData.presion = barometro.getPressurePascal();                //Leer presión
    sensorData.temperatura = barometro.getTemperature();         //Leer temperatura
    sensorData.altura_barometro = barometro.getAltitude();//44330.0 * (1.0 - pow(barometro.getPressure() / presionPromedioInicial, 0.1902949));   //Leer altura

    //Tiempo
    unsigned long t = micros();
    float dt = (t - tAnterior) / 1000000.0;
    tAnterior = t;

    //Velocidad
    float velocidadInstantanea = (sensorData.altura_barometro - altitudAnterior) / dt;
    sensorData.velocidad = (alpha * velocidadInstantanea) + ((1.0 - alpha) * sensorData.velocidad);
    altitudAnterior = sensorData.altura_barometro;     //Actualizamos la altitud anterior para el cálculo de la velocidad

    //Valores convertidos a entero para enviar en la telemetría
    telemetryData.presion_32 = (int32_t)(sensorData.presion*100);
    telemetryData.temperatura_16 = (int16_t)(sensorData.temperatura*100);
    telemetryData.altura_barometro_32 = (int32_t)(sensorData.altura_barometro*100);
    telemetryData.velocidad_32 = (int32_t)(sensorData.velocidad*100);
  }

  /*
  //Barometro NATALIA
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
  
  */

  
  /*
  //Barometro PABLO
  if (millis() - tiempoLectura >= 19) {
    tiempoLectura = millis();
    barometro.read();
    float presionActual = barometro.getPressure();
    contador++;
    presionActualPromedio += presionActual;
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
  */
  
}