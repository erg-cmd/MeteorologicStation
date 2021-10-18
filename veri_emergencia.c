#include "libreria.h"



extern U8 estado;
extern U8 n_muestras;
//extern U8 flag_Config-_Veri;								  //__Esto mas que nada es ponerse a pensar que mierda podria salir mal.__
extern U8 flag_check_toma;							//guarda que esa pregunta es peligrosa
extern U8 veri_check;

void Verificacion_Emergencia (U8 estado_actual)
{
   U8 verificando;
   verificando = estado_actual;
   U8 cuenta_check = 0;
   U8 n_muestras_aux;

   Clear_LCD ();                                        //Armar Funcion!
	
	switch (verificando)
	{
              case CONFIG_MEDICION:		                // Lo pense en el caso de que se quisiera re configurar, le diria primero al operador
										                // los dispositivos que estan siendo usados. Tambien voy a ver de ponerlo que se inicie 
					   {   
					   	   printLCD ("Bienvenido a su Estacion Meteorologica");
					       if(flag_Config_Veri == ON)	// con un flag la primera ves que corre el programa y se configura.
					       {
						    printLCD ("Sensores:");

					       if (Habilitacion_Mediciones == 0x00)
					   		 { printLCD ("No se han registrado dispositivos");
					   		   return;					   
					   		 }
						   
						   if ((Habilitacion_Mediciones != 0x01) && (Habilitacion_Mediciones != 0x02) && (Habilitacion_Mediciones != 0x04))
								printLCD ("Error en la Conexion de Dispositivos, Comuniquese con el Servicio Tecnico      Error: 0003");

						   if (Habilitacion_Mediciones === 0x01)
						     {if(Sensor_Temp == _DHT11)
							   printLCD ("Temperatura: DHT11");		 
							  if(Sensor_Temp == _LM35)
					   		   printLCD ("Temperatura: LM35");
					   			 else
							   printLCD ("Temperatura: Unknown");
							  }
							    else
							  { printLCD ("Temperatura: No registrado");
							  }
					   	   
						   
						   if (Habilitacion_Mediciones === 0x02)
						     {if(Sensor_HumddRel == _DHT11)
							   printLCD ("Humedad: DHT11");
							  if(Sensor_HumddRel == _HIH4000)
					   		   printLCD ("Humedad: HIH4000");
					   			 else
							   printLCD ("Humedad: Unknown");
							  }
							    else
							  { printLCD ("Humedad: No registrado");
							  }


			               if (Habilitacion_Mediciones === 0x04)
						     {if(Sensor_PresAtm == _MPX2100AP)
							   printLCD ("Presion: MPX2100AP");						
					   			 else
							   printLCD ("Humedad: Unknown");
							  }
							    else
							  { printLCD ("Humedad: No registrado");
							  }

						   return;
							  }
							   
					   } break;

		    
			case TOMA_DATOS:							 
						{  
						   n_muestras_aux = n_muestras; 
						  
						  if(n_muestras_aux == n_muestras)
						    {													 
							   flag_check_toma = ON;     //encendemos flag del timer.
							   cuenta_check++;
							   return;							 							
						    }
							   else
							{
							   flag_check_toma = OFF;
							   cuenta_check = 0;
							}
						 if (cuenta_check > 50)	       // Valor al azar.
							{ status = reset;          // Define reset 0x00
							  printLCD ("Error en el procesamiento de Datos, Comuniquese con el servicio Tecnico      Error: 0004");
							  cuenta_check = 0;
							}
						 if(veri_check > 10)		   //El DHT11 tuvo 10 veces seguidas error de CheckSUM.
						    { status = reset;
							  printLCD ("Error en el prosesamiento de Datos, Comuniquese con el servicio Tecnico      Error: 0002");
							  cuenta_check = 0;
						 
						 if(veri_start > 10)		  //El DHT11 no pudo levantar la trama correctamente 10 veces.
							 printLCD  ("Error en la comunicacion con el Dispositivo: _DHT11, Comuniquese con el servicio Tecnico      Error: 0003");




						    }     
		              }break;
	  }
}											   