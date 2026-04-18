/********************************************************************************************************************************************
Club de Investigación Univesitario de Desarrollo en Sistemas Espaciales
AVIÓNICA PRIMERA ETAPA - Misión IREC 2026 - Archivo de funciones de primera etapa
Código desarrollado por equipo de Aviónica y Estación Terrena

Archivo de funciones para el funcionamiento de la PRIMERA ETAPA.

-                                          | Evento de vuelo
- Detectar el standby                      |        0
- Detectar el liftoff                      |        1
- Detectar apogeo                          |        2
- Detectar activacion de main              |        3
- Detectar aterrizaje                      |        4

*********************************************************************************************************************************************/
#include "Avionica_Primera_Etapa.h"

int desacople = 1400;
int alt_motor = 17;
int apogeo_etapa1 = 500;
int apogeo_etapa2 = 500;
int tiempoEvento = 0;
bool inicioVuelo = false;

bool bajando = false;

bool candidatoLiftoff = false;
unsigned long tiempoLiftoffDetectado = 0;

bool candidatoApogeo = false;
unsigned long tiempoApogeoDetectado = 0;
bool paracaidasActivado = false;
unsigned long tiempoApogeoConfirmado = 0;

bool candidatoAterrizaje = false;
unsigned long tiempoAterrizajeDetectado = 0;

void eventoDeVuelo()
{
  switch(telemetryData.indicadorEstadoVuelo)
  {
    //------------------------ Evento de vuelo 0 - Standby ------------------------//
    case 0:
      if(abs(sensorData.accel_z) < 1 && (inicioVuelo == false))
      {
        // sigue en standby
      }

      //------------------------ Evento de vuelo 1 - Liftoff ------------------------//
      else if(sensorData.accel_z > 6)
      {
        if(candidatoLiftoff == false)
        {
          tiempoLiftoffDetectado = millis();
          candidatoLiftoff = true;
        }
        if(millis() - tiempoLiftoffDetectado > 50) // 50 ms de aceleración sostenida
        {
          telemetryData.indicadorEstadoVuelo = 1;
          inicioVuelo = true;
        }
      }
      else
      {
        candidatoLiftoff = false;
      }    
    break;

    //------------------------ Evento de vuelo 2 - Apogeo ------------------------//
    case 1:
    {
      bajando = sensorData.altura_barometro < (max_altura - 2); // margen de ruido

      if(bajando)
      {
        if(candidatoApogeo == false)
        {
          tiempoApogeoDetectado = millis();
          candidatoApogeo = true;
        }

        if(millis() - tiempoApogeoDetectado > 100) // 100 ms
        {
          // Apogeo confirmado
          telemetryData.indicadorEstadoVuelo = 2;
        }
      }
      else
      {
        candidatoApogeo = false;
      }
    }
    break;

    //------------------------ Evento de vuelo 3 - Paracaídas main  ------------------------//
    case 2: 
      if(paracaidasActivado == false)
      {
        /*
        CODIGO PARA ABRIR PARACAIDAS DESPUES DE 1 SEGUNDO DE APOGEO
          if(tiempoApogeoConfirmado == 0)
          {
            tiempoApogeoConfirmado = millis();
          }

          if(millis() - tiempoApogeoConfirmado > 1000) // 1 segundo después del apogeo
          {
            activarParacaidas();
            paracaidasActivado = true;
            telemetryData.indicadorEstadoVuelo = 3;
          }
        
        */

        activarParacaidas();
        paracaidasActivado = true;
        telemetryData.indicadorEstadoVuelo = 3;
      }
    break;

    //------------------------ Evento de vuelo 4 - Aterrizaje ------------------------//
    case 3: 
      if(abs(sensorData.accel_z) < 1 && (inicioVuelo == true))
      {
        if(candidatoAterrizaje == false)
        {
          tiempoAterrizajeDetectado = millis();
          candidatoAterrizaje = true;
        }

        if(millis() - tiempoAterrizajeDetectado > 1000) // 1 s de reposo sostenido
        {
          telemetryData.indicadorEstadoVuelo = 4;
        }
      }
      else
      {
        candidatoAterrizaje = false;
      }
    break;
  }
}

void activarParacaidas()
{
  digitalWrite(main, HIGH);
  //digitalWrite(pinBuzzer, HIGH); //Buzzer de prueba
}


  /*

void etapa1()
{
  if((sensorData.altura_barometro < (max_altura-150)) && (sensorData.accel_z < 0))
  {
    digitalWrite(main, HIGH);
  }
  else
  {
    digitalWrite(main, LOW);
  }

-----------------------------------------------------------------------
  /*
  //------------------------ Evento de vuelo 0 - Standby ------------------------//
  if((abs(sensorData.accel_z < 1)) && (inicioVuelo == false) )
  {
    telemetryData.indicadorEstadoVuelo = 0;
  }

  //------------------------ Evento de vuelo 1 - Liftoff ------------------------//
  if((sensorData.accel_z > 10) && (telemetryData.indicadorEstadoVuelo == 0))
  {
    telemetryData.indicadorEstadoVuelo = 1;
    inicioVuelo = true;
  }

  //------------------------ Evento de vuelo 2 - Desacople ------------------------//
  if((sensorData.accel_z < 1.2) && (telemetryData.indicadorEstadoVuelo == 1))
  {
    telemetryData.indicadorEstadoVuelo = 2;
  }

  //------------------------ Evento de vuelo 3 - Apogeo ------------------------//
  if((sensorData.velocidad < -1 ) && (telemetryData.indicadorEstadoVuelo == 2))
  {
    telemetryData.indicadorEstadoVuelo = 3;
  }

  //------------------------ Evento de vuelo 4 - Paracaídas main  ------------------------//
  if(telemetryData.indicadorEstadoVuelo == 3)
  {
    activarParacaidas();
    telemetryData.indicadorEstadoVuelo = 4;
  }

  //------------------------ Evento de vuelo 5 - Aterrizaje ------------------------//
  if(abs(sensorData.accel_z < 1) && (telemetryData.indicadorEstadoVuelo == 4))
  {
    telemetryData.indicadorEstadoVuelo = 5;
  }*/









  -------------------------------------------------------------------
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
*/
}
