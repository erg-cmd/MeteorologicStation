#ifndef	__LIBRERIA_H__
#define __LIBRERIA_H__

#include "C8051F020.H"
#include "main.h"
#include "barrido_teclado.h"
#include "fw_dht11.h"
#include "inicializacion.h"
#include "mdt.h"
#include "lcd.h"
#include "adc0.h"
#include "config_medicion.h"
#include "envio_de_config.h"
#include "manejo_serie.h"
#include "stdlib.h"

//------Definicion de tipos
typedef unsigned char U8;
typedef unsigned int U16;

//Defines

#define ON  					1
#define OFF 					0
#define reset 				0x00
#define VACIO 				0x00


// Respecto a la trama
#define INICIO_TRAMA 	'#'
#define FIN_TRAMA			'$'


#define TRAMA_CONFIG  	'C'
#define TRAMA_TEMP    	0x01
#define TRAMA_HUMM  		0x02
#define TRAMA_PRES  		0x04
#define TRAMA_NO				0x08

// Propias de la estacion
#define N_DE_ESTACION 				4
#define MAX_MUESTRAS 				1 // esto es tentativo, puede haber una mejor idea.....!!!
#define N_MUESTRAS_DEFECTO 			1	//compro esta idea....
#define T_ENTRE_MUESTRAS_DEFECTO 	1  //valor en minutos

// Sensores
#define _DHT11      				10
#define _MPX2200    				11
#define _LM35						12
#define _ESPERO_CONVERSION			13	


// Parametros Climatologicos
#define TEMP							0x01
#define HUMDDrEL						0x02
#define PRESaTM 						0x04
#define TEMP_DHT11						0x08
#define NOpARAM							254


/*---------------------------------------------------
---Errores-------------------------------------------
la lista de errores debe ser la misma que la central 
 y todas las estaciones meteorologicas
----------------------------------------------------*/
#define ERROR_5 0xF0  // error por mal configuracion del sensor de temperatura
#define ERROR_6 0xF1  // error por mal configuracion del sensor de humedad relativa
#define ERROR_7 0xF2  // error por mal configuracion del sensor de presion atmosferica
#define ERROR_8 0xFFFF  // error por ingreso incorrecto del tiempo entre muestras
 

/*---------------------------------------------------
el timer 0 va a desbordar cada 100us
----------------------------------------------------*/
#define t_100us_l	  ((0xFFFF-100+1) & 0x00FF)
#define t_100us_h	  ((0xFFFF-100+1) >> 8)

/*---------------------------------------------------
configuramos la uart0 para que trabaje con 57600 baudios, 8 bits de datos,
 bit de paridad NO, 1 bit de stop. N = 
 entonces N = 6
----------------------------------------------------*/
#define BAUDRATE 	57600
#define SYSCLCK		11059200

#define t_timer1 (0xFF - 6 + 1)

#endif