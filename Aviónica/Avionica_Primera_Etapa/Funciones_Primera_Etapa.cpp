/********************************************************************************************************************************************
Club de Investigación Univesitario de Desarrollo en Sistemas Espaciales
AVIÓNICA PRIMERA ETAPA - Misión IREC 2026 - Archivo de funciones de primera etapa
Código desarrollado por equipo de Aviónica y Estación Terrena

Archivo de funciones para el funcionamiento de la PRIMERA ETAPA.

-                                          | Evento de vuelo
- Detectar el standby                      |        0
- Detectar el liftoff                      |        1
- Detectar altura desacople                |        2         Separacion de etapas*
- Detectar apogeo                          |        3
- Detectar activación de paracaídas drogue |        4
- Detectar activacion de main              |        5
- Detectar aterrizaje                      |        6



*********************************************************************************************************************************************/
#include "Avionica_Primera_Etapa.h"


int desacople = 1400;
int alt_motor = 17;
int apogeo_etapa1 = 500;
int apogeo_etapa2 = 500;



void etapa1()
{
  if((sensorData.altura_barometro < (max_altura-150))&&(sensorData.accel_z < 0)){
    digitalWrite(main, HIGH);
  }else{
    digitalWrite(main, LOW);
  }

  /*
  //telemetryData.etapa = 'S1';
  //Serial.print(telemetryData.etapa);

  //------------------------ Evento de vuelo 0 - Standby ------------------------//
  if (inicio = true)
  {
    if((-1 < telemetryData.accel_z < 1)&&(telemetryData.altura_barometro < 50))
    {//Cambiar valores segun el acelerometro en reposo
      //estado: Pre-Launch
    }
  }

  //------------------------ Evento de vuelo 1 - Liftoff ------------------------//
  //si detecta aceleración positiva y su altura es mayor a 50
  if((telemetryData.accel_z > 200)&&(telemetryData.altura_barometro > 50))
  {//
    //estado: Launch
  }

  //------------------------ Evento de vuelo 2 - Separación ------------------------//

  if (telemetryData.altura_barometro > desacople)
  {
      //Activar desacople
  }
  //Activación del paracaidas 
  if (telemetryData.altura_barometro > apogeo_etapa1)
  {
    if(telemetryData.altura_barometro < (max_altura-200))
    {
      //Activar paracaidas            REVISAR ESTOS IFS DE ACTIVACION DE PARACAIDAS
    }
  }

  //si pasa la altura del desacople activar desacople
  if (telemetryData.altura_barometro > desacople)
  {
    //descople

    //si detecta disminucion de velocidad detecta el apogeo
    if(-10 < telemetryData.accel_z < 10){
      //estado: apogeo
    }

    if(telemetryData.altura_barometro < drogueE1))
    {
      //Activar Drogue
      //estado: DrogueActivado

      if((telemetryData.altura_barometro < mainE1)&&())
      {
        //Activar Main
        //estado: MainActivado
      }
      //Desactivar Main
    }
      
  }

  if((telemetryData.accel_z < 1)&&(telemetryData.altura_barometro > 50))
  {//
      //estado: Descent
  }

}


void eventoDeVuelo()
{
  //Estado Cohete
  if((-1 < telemetryData.accel_z < 1)&&(telemetryData.altura_barometro < 50))       //Cambiar valores segun el acelerometro en reposo
  {
      //estado: Pre-Launch
  }

  if((telemetryData.accel_z > 200)&&(telemetryData.altura_barometro > 50))
  {//
      //estado: Launch
  }

  //Este cambia dependiendo si es primera o segunda etapa
  //Pimera etapa:
  if((telemetryData.accel_z < 100)&&(telemetryData.altura_barometro > 500))
  {//
      //estado: Motor Burnout
  }
  //Segunda etapa:
  if((telemetryData.accel_z < 100)&&(telemetryData.altura_barometro > 500))
  {//
    
    //estado: Interstage Separation
    if((telemetryData.accel_z > 200)&&(telemetryData.altura_barometro > 600))
    {//
      //estado: Ignition
    }
  }


  if((telemetryData.accel_z < 1)&&(telemetryData.altura_barometro > 50))
  {//
      //estado: Descent
  }

  //Detecta paracaidas bla bla
  //dentro del if del paracaidas
  if((-1 < telemetryData.accel_z < 1)&&(telemetryData.altura_barometro < 50))
  {//Cambiar valores segun el acelerometro en reposo
      //estado: Landing
  }
  */
}
