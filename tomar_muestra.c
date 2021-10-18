#include "libreria.h"

sfr16 ADC0 = 0xBE;

bit flag_actualizar = OFF;
extern bit flag_tomarMuestra;		//!< variable definida en main.c
extern bit timer_1s;					//!< variable definida en mdt.c
extern bit timer_2500us;
extern U8 temperatura;
//extern U8 temp_dht11;
//extern U8 humedad_dht11;
extern U8 presion;

extern U8 estado;					//!< variable definida en main.c
extern U8 tecla;          //!< variable definida en main.c
extern U8 t_entre_muestras;			//!< variable definida en config_medicion.c		
extern U8 tick_muestra;			//!< variable definida en main.c	  
extern U8 Habilitacion_Mediciones;

/**
 *	\fn bit tomar_muestra ( void ) 
 *	\brief pide las muestras a los sensores y las guarda
 *	\param void
 *	\return bit: 1 si todos las muestras fueron pedidos
								 0 si todavia se estan pidiendo las muestras
 */
void tomar_muestra()
{
		bit retorno = OFF;
static 	bit flag_T = OFF;
static 	U8 estado_sensores = _MPX2200;

while(retorno == OFF)
{
	switch(estado_sensores)
	{
		case _MPX2200: 
					if(AD0BUSY == OFF)
					{
					AMX0SL = 0x01;						//!< selecciono la entrada del mux AIN0.1
					ADC0CF |= 0x07;						//!< configuro el PGA a 0.5
					AD0BUSY = ON;						//!< incio la conversión
					flag_T = OFF;
					estado_sensores = _DHT11;			//!< cambio al siguiente estado = DHT11							
					}
		break;

		case _DHT11:				
					if(flag_T == OFF)					//!< pregunto si ya se había pasado por acá a un flag testigo
					{									//!< llamo a la función para levantar los datos
						Medicion_DHT11();				//!< guardo los datos en el buffer vector_Humedad[n_muestras]
						
						flag_T = ON;
					}									//!< y seteo el flag testigo de conversión
					if(AD0BUSY == OFF)					//!< pregunto si termino la conversión del ADC
					{							
						P5 |= 0x80;						//!< testigo
						estado_sensores = _LM35;		//!< cambio al siguiente estado = LM35
					}
					break;									

		case _LM35:
					AMX0SL = 0x02;												//!< selecciono la entrada del mux AIN0.2							
					ADC0CF &= 0xF8;												//!< seteo el PGA = 1
					P5|=0x40;													//!< testigo
					estado_sensores = _ESPERO_CONVERSION;						//!< cambio al siguiente estado = ESPERO_CONVERSION
					presion = ADC0/10; 								//!< guardo la presión en vector_presion[n_muestras]
					AD0BUSY = ON;												//!< inicio la conversión
		break;

		case _ESPERO_CONVERSION:
					if(AD0BUSY == OFF)											//!< sí terminó de convertir guardo la conversion
					{															//!< sobre su factor de conversion temperatura[n_muestras];
					 AMX0SL = 0x01;												//!< selecciono la entrada del mux AIN0.2							
					 ADC0CF |= 0x07;											//!< PGA = 0.5
					 temperatura = ADC0 / 13;						
					 estado_sensores = _MPX2200;								//!< cambio al estado de inicio = MPX2200
					 P5 |= 0x20;												//!< testigo
					 retorno = ON;												//!< retorno un 1
					}
		break;
	}
 }
}

/**
 *	\fn void actualizar ( void ) 
 *	\brief  Si ticks llego al tiempo entre muestras seteo el flag_tomar para tomar las muestras. 
			Cada un segundo actualiza el valor de Tecla. Si hay tecla presionada estado = CONFIG_MEDICION
 *	\param void
 *	\return void
 */

void actualizar()
 { 
	  	actualizar_teclado();					//!< actualiza el valor de Tecla cada 2.5ms
	  	if(Teclado() != NO_KEY)					//!< pregunto si se apretó alguna tecla
		{
		flag_actualizar = ON;							//!< si se apreto una tecla seteo el flag actualizar
		estado = CONFIG_MEDICION;					//!< cambio el estado a CONFIG_MEDICION
		}											 
	  
	  if(tick_muestra >= t_entre_muestras) //!< tiempo de tomar la muestra
	  {
	  	
		tick_muestra = 0;								 
		 flag_tomarMuestra = ON;
	  }
 } 