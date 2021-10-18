#include "libreria.h"

extern U16 AUX[];
extern bit timer_5ms; 	//definido en mdt.c
extern U8  numerOdByte; 	//definida en main.c
extern U8  humedad_dht11;//definida en main.c
extern U8  temp_dht11;	//definida en main.c
extern U8  el_buffer[];	//definida en main.c

//------Variables globales-----------------------

U16 AUXILIAR = 0;
U8 indice = 7, cant_de_bits = 0;
bit estado_anterior = OFF, estado_actual = OFF;
sfr16 T2    =   0xCC; 	

/*----------------------------------------*/

void Medicion_DHT11( void )
{	
	if( COMM_DHT11() )
	{
		//wrt_string("ERROR COMM");
		humedad_dht11 = el_buffer[0];
		temp_dht11 = el_buffer[2];
	}
	else
	{ 
		//wrt_string("TRAMA OK!");
		humedad_dht11 = el_buffer[0];
		temp_dht11 = el_buffer[2];
	}

}

bit COMM_DHT11 (void)
{
	U8 estado_DHT11 = OCIOSO;
 	bit flag = ON, comunicacion = ON;
 	indice = 7, cant_de_bits=0; 
 	estado_anterior = OFF, estado_actual = OFF;
 	el_buffer[0] = 0;
 	el_buffer[1] = 0;
 	el_buffer[2] = 0;
 	el_buffer[3] = 0;
 	el_buffer[4] = 0;
 //globales
 	AUXILIAR = 0;numerOdByte = 0;
 
 /*-Descripcion de varibles-------
 contador: lleva la cuenta de los bits recibidos
 datos: un array de 5 donde se guarda humedad_entera, hum_decimal, temp_entera,temp_decimal, chksm
 		en ese orden.
 ---------------------------------*/ 
  
  while(comunicacion)
  {
   switch(estado_DHT11)
   {
   			case OCIOSO:
				 if(medir)
				 {
				 	estado_DHT11 = PIDIENDO_DATOS;
				 	flag = ON; 
				 }
			break;
			
			case PIDIENDO_DATOS:				  
				 
						  //----------seteo del puerto
				 if (flag)//debe pasar una sola vez por acá, por eso el flag
				 {
				  p_data = OFF;
				  T2 = 0;		  //reseteo la cuenta del timer 2
				  flag = OFF;      //una vez iniciado, apago el flag
				 }
				 if (T2 > 18000)	//Bajo p_data hasta 18ms
				 {
				 	p_data = ON; //Levanto P_data por 25us
					P2MDOUT &=0xFE; //seteo como salida P2.0 
					T2 = 0;
					estado_DHT11 = SINCRONIZACION;
				 }
				 break;
			
			case SINCRONIZACION:
				
				timer_5ms_on;
			 	while(p_data && timer_5ms == ON); //!<espero que el DHT11 me conteste
				 				 //!<son unos 80us en bajo	
				 while(!p_data && timer_5ms == ON); //!<espero que el DHT11 me conteste
				 				 //!<son unos 80us en bajo	
				 while(p_data && timer_5ms == ON); //!<espero que el DHT11 me conteste
				 				 //!<son unos 80us en alto	
				  T2 = 0;			  //Reinicio el timer 2
				 estado_DHT11 = RECIBIENDO_DATOS; //cambio al sig estado
				 timer_5ms_on; //Inicio la cuenta de 5 ms; tiempo limite de comunicacion
			break;
			
			case RECIBIENDO_DATOS:
				 recibiendo_bit();
				 if(timer_5ms == OFF)	//chequeo si pasaron los 5ms
				 {
					//wrt_data(48+(cant_de_bits/10));
 				 //wrt_data(48+(cant_de_bits%10));
				  estado_DHT11 = CHEQUEO;
				 }	 
			break;
			
			case CHEQUEO:
				 estado_DHT11 = OCIOSO;
				 if (el_buffer[4] == (el_buffer[3] + el_buffer[2] + el_buffer[1] + el_buffer[0]))
				 {
				  return DHT11_COMM_OK; // se levanto la trama correctamente
				 }
				 else //la trama no verifica el checksum
				 {
				  return DHT11_COMM_ERROR;	//problemas de comunicacion con  el DHT11 
				 }
			break;
   }
  }
}

/*---Funciones-------------------*/

/*--------------------------------------------------
 
 	si es un "0" el periodo es de 78us, divido 1.085us (tiempo de la cuenta) = 71.88
	si es un "1" el periodo es de 120us, divido 1.085us (tiempo de la cuenta) = 110.6
  
 El bit recibido es el mas significativo;
 --------------------------------------------------*/ 


void recibiendo_bit()
{
 		estado_actual = p_data;

 		if(estado_actual == estado_anterior) return; //no hubo cambios
 
 		if((estado_actual == ON) && (estado_anterior == OFF)) 
 		{
			estado_anterior = estado_actual;
			return;
		}
 
 		if((estado_actual == OFF) && (estado_anterior == ON))
 		{
 			AUXILIAR = T2; 	//!<copio el valor 
			T2 = 0;		//!<reseteo la cuenta
			estado_anterior = estado_actual;
 			if (AUXILIAR < 135)
 			{
 						if(AUXILIAR < 90)
 						{
						el_buffer[numerOdByte] |= 0<<indice; // es un 0 
						}
						else
						{		  
		 				el_buffer[numerOdByte] |=  1<<indice; // es un 1
						}
		 	cant_de_bits++; //cuenta los bits recibidos
		 
 						if (indice == 0) 
 						{
 							numerOdByte++; 
							indice = 7;
 						}
 						else
 						{
 						indice--;
 						}
			}
		}
}