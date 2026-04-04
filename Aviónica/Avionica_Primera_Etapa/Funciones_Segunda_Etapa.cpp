/********************************************************************************************************************************************
Club de Investigación Univesitario de Desarrollo en Sistemas Espaciales
AVIÓNICA PRIMERA ETAPA - Misión IREC 2026 - Archivo de funciones de segunda etapa
Código desarrollado por equipo de Aviónica y Estación Terrena

Archivo de funciones para el funcionamiento de la SEGUNDA ETAPA.
*********************************************************************************************************************************************/
#include "Avionica_Primera_Etapa.h"

int tiempoApogee = 0;

void etapa2(){

  if((sensorData.presion < (max_altura-150))&&(sensorData.accel_z < 1)){
    digitalWrite(main, HIGH);  
  }else{
    digitalWrite(main, LOW);
  }


/*
      if(sensorData.altura_barometro < (max_altura-200)){
      //activar paracaidas 
      digitalWrite(mainE1, HIGH);
    }

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
    }*/
}
