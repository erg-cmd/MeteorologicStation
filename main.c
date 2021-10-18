/*--------Trabajo Prático Final-------------------------*/
//-------Estacion Meteorólogica---------------------------
//Integrantes: Mauro Boffito, Diego Pirotta y Elias Gracia
//-----modificado 05/08/2012------------------------------
/*------------------------------------------------------*/

#include "libreria.h"

//Variables Externas
extern bit timer_100us;
extern bit timer_500us;
extern bit timer_1ms;
extern bit timer_2ms;
extern bit timer_5ms;
extern bit timer_10ms;
extern bit timer_1s;
extern U16	AUX[];

extern U8 n_de_muestras; 								//!< definida en config_medicion.c
extern U8 datos_ingresados;
extern bit timer_1s;										//!< definida en mdt.c
extern bit flag_actualizar;								//!< definida en tomar_muestra.c

/*------Variables---------------------------------------*/

bit flag_tomarMuestra = OFF;			//!< cuando esta en ON paso el tiempo entre muestras, indicando la toma muestra 
U8 estado = CONFIG_MEDICION;
U8 tick_muestra = 0;
U8 tecla = NO_KEY;
U8 temperatura = 0;
U8 temp_dht11 = 0;
U8 humedad_dht11 = 0;
U8 presion = 0;
U8 numerOdByte = 0;
U8 el_buffer[15] = {'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'};

/*------------------------------------------------------*/

void main ()
{
   inicializacion();
   TR0 = ON; 							//!<inicio el timer 0
   TR2 = ON; 							//!<inicio el timer 2
   while(1)
   {
   switch (estado)
   {
   				case CONFIG_MEDICION:
					 config_medicion();							//!< si se pulso alguna tecla accede a configurar
					 											//!< de lo contrario retorna
					 if(flag_actualizar == ON)
					 {
						flag_actualizar = OFF;
						estado = TOMA_DE_DATOS;
						break;
					 }
					 if(datos_ingresados != VACIO) 				//!< si se configuro anteriormente corre la ultima config
					 {
						estado = ENVIO_DE_CONFIG; 				//!< se encontro config anterior y la enviamos a la PC
					 }
				break;

				case ENVIO_DE_CONFIG:  							//!< hasta que no se envie la configuracion no
																//!< no empiezo a tomar datos
					envio_de_config();
					
				break;
								
				
				case TOMA_DE_DATOS:
					 
				//!< en TOMA_DE_DATOS: 	-espero al tiempo entre muestras para tomar las mediciones.
				//!< 			 		-Si se llegó al número de muestras total, cambio al siguente estado	el envio de datos. 
				//!<					-Me fijo si no llego configuracion nueva desde la PC
				//!<					-Me fijo si el teclado es presionado

				actualizar();								//!< refresca el valor de ticks y si llego a t_entre_muestras setea flag_tomarMuestras
															//!< actualiza el valor de tecla cada 1 segundo; si tecla apretada estado = CONFIG_MEDICION
				RecibirTrama();								//!< nos fijamos si llego nueva configuracion al uc; responde con Ack o Nack
				 if( flag_tomarMuestra ) 				//!< pregunto si es tiempo de tomar la muestra
				 {
					tomar_muestra();
					flag_tomarMuestra = OFF;  		//!< se reinicia flag para nueva cuenta del timer0
					estado = ENVIO_DE_DATOS;
					break;
				 }
				
				break;
					
				case ENVIO_DE_DATOS:				

				 envio_datos();						//!< hasta no enviar todos los datos no continuar
				 
				break;	 
   	}
   }
}
