#ifndef __FW_DHT11_H__
#define __FW_DHT11_H__


/*-Define------------------------*/
#define medir 			 1
#define OCIOSO 			 90
#define PIDIENDO_DATOS	 91
#define SINCRONIZACION	 92
#define RECIBIENDO_DATOS 93
#define CHEQUEO			 94

#define DHT11_COMM_OK	 0
#define DHT11_COMM_ERROR 1

			
/*-Declaraciones-----------------*/

sbit p_data = P2^0; // pin del dht11

/*-Prototipos de funciones-------*/
void Medicion_DHT11( void );
bit COMM_DHT11 ( void );
void recibiendo_bit( void );

 #endif