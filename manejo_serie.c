
#include "libreria.h"

//---Buffers serie-------------

U8 buffer_Rx[TAM_RX];
U8 buffer_Tx[TAM_TX];
U8 in_tx,out_tx,in_rx,out_rx;

//---Variables Externas--------
extern U16 AUX[];
extern bit timer_5ms;
extern bit timer_1s;

extern U8 Habilitacion_Mediciones;	//!< variable definida en config_medicion.c
extern U8 datos_ingresados;			    //!< variable definida en config_medicion.c
extern U8 n_de_muestras;		  	    //!< variable definida en config_medicion.c
extern U8 t_entre_muestras;			    //!< variable definida en config_medicion.c
extern U8 estado;					          //!< variable definida en main.c
extern U8 el_buffer[];              //!< variable definida en main.c
extern U8 temperatura;
extern U8 temp_dht11;
extern U8 humedad_dht11;
extern U8 presion;

/*---Metodos para el manejo del buffer serie--------*/

/**	
	\fn  void envio_datos ( void )
	\brief  envia las muestras desde 0 a n_de_muestras
*/

void envio_datos()
{
	static U8 estado_envio = CARGO_BUFFER_TRAMA1;
	static U8 contador_envio = 0;
	int aux1 = 0;
	switch (estado_envio)
	
	{
	 		case CARGO_BUFFER_TRAMA1:
				 el_buffer[0] = INICIO_TRAMA;
				 el_buffer[1] = numero_de_estacion();
				 el_buffer[2] = TEMP;
                 aux1 = HextoInt (el_buffer[2]);
	             el_buffer[2] = aux1;
				 el_buffer[3] = temperatura;
				 aux1 = HextoInt (el_buffer [3]);
			
	             if(aux1 == '\0') aux1 = NOpARAM;		//!< seguridad para que no corte la cadena
				 el_buffer[3] = aux1;
				 el_buffer[4] = TEMP_DHT11;
				 aux1 = HextoInt (el_buffer [4]);
	             el_buffer[4] = aux1;
				 el_buffer[5] = temp_dht11;
				 aux1 = HextoInt (el_buffer [5]);
			
	             if(aux1 == '\0') aux1 = NOpARAM;		//!< seguridad para que no corte la cadena
				 el_buffer[5] = aux1;
				 el_buffer[6] = el_buffer[1] + el_buffer[2] + el_buffer[3] + el_buffer[4] + el_buffer[5];
				 aux1 = HextoInt (el_buffer [6]);
	             if(aux1 == '\0') aux1 = NOpARAM;		//!< seguridad para que no corte la cadena
				 el_buffer[6] = aux1;
				 el_buffer[7] = FIN_TRAMA;
				 el_buffer[8] = '\0';
				 
				 MandarDato(el_buffer);					//!< cargo la trama en el buffer de transmision
				 estado_envio = ESPERO_ENVIO_TRAMA;		//!< cambio al estado espero envio de trama ( fin del envio )
			break;
			
			case CARGO_BUFFER_TRAMA2:
				 el_buffer[0] = INICIO_TRAMA;
				 el_buffer[1] = numero_de_estacion();
				 el_buffer[2] = PRESaTM;
				 aux1 = HextoInt (el_buffer [2]);
	             el_buffer[2] = aux1;
				 el_buffer[3] = presion;
				 aux1 = HextoInt (el_buffer [3]);
	             if(aux1 == '\0') aux1 = NOpARAM;		//!< seguridad para que no corte la cadena
				 el_buffer[3] = aux1;
				 el_buffer[4] = HUMDDrEL;
				 aux1 = HextoInt (el_buffer [4]);
	             el_buffer[4] = aux1;
				 el_buffer[5] = humedad_dht11;
				 aux1 = HextoInt (el_buffer [5]);
	             if(aux1 == '\0') aux1 = NOpARAM;		//!< seguridad para que no corte la cadena
				 el_buffer[5] = aux1;
				 el_buffer[6] = el_buffer[1] + el_buffer[2] + el_buffer[3] + el_buffer[4] + el_buffer[5];
				 aux1 = HextoInt (el_buffer [6]);
	             if(aux1 == '\0') aux1 = NOpARAM;		//!< seguridad para que no corte la cadena
				 el_buffer[6] = aux1;
				 el_buffer[7] = FIN_TRAMA;
				 el_buffer[8] = '\0';
				 
				 MandarDato(el_buffer);				//!< cargo la trama en el buffer de transmision
				 estado_envio = ESPERO_ENVIO_TRAMA;		//!< cambio al estado espero envio de trama ( fin del envio )
			break;
   			

			case ESPERO_ENVIO_TRAMA:
					if(in_tx == out_tx)		//!< no hay mas datos que enviar
					{
					 contador_envio++;
					 if(contador_envio == 2)
					 {
					  contador_envio = 0;
					  estado = TOMA_DE_DATOS;
					 }
					
					if(contador_envio == 0 ) estado_envio = CARGO_BUFFER_TRAMA1;
					if(contador_envio == 1 ) estado_envio = CARGO_BUFFER_TRAMA2;
					
					timer_1s_on;
					while(timer_1s == ON);
					}
			break;
	
	}
}




/**
 *	\fn void Pop_Recibir ( unsigned char)
 *	\brief Toma un byte de la cola circular BUfferRecepcion
 *	\param void
 *	\return dato si la cola tiene elementos y -1 si está vacio
 */

int PopRecepcion (void)
{
 int aux;
 aux = buffer_Rx[out_rx];
 out_rx++;
 out_rx %= TAM_RX;
 return aux;

}


/**	
	\fn  bit HayDato ( void )
	\brief Indica si hay datos disponibles para leer en el buffer de recepcion
*/

bit HayDato ( void )
{
	return ( in_rx != out_rx );
}

/**	
	\fn  void RecibirTrama ( void )
	\brief  si hay datos en el buffer de recepcion 
			levanto la trama y elijo entre configurar y enviar datos
*/

void RecibirTrama ()
{
 static U8 estado_recepcion = ESPERO_HEADER;

 if(HayDato())						//!< pregunto si hay datos en el buffer de recepcion
 {									
  	el_buffer[0]	= PopRecepcion();			//!< guardo el byte recibido en el_buffer[0]
	switch (estado_recepcion)
	{
	 			case ESPERO_HEADER:
						if(el_buffer[0] == INICIO_TRAMA)
						{
						estado_recepcion = LEVANTO_TRAMA;
						el_buffer[14] = 8;						   	//!< reseteo el contador de bytes
						}
				break;
				
				case LEVANTO_TRAMA:
					el_buffer[ el_buffer[14] ] = el_buffer[0];
					el_buffer[14]++;
					if (el_buffer[14]  > 13)		            //!< en la posicion 13 guardo el checksum recibido
					{
					 el_buffer[0] = el_buffer[8] + el_buffer[9] + el_buffer[10] + el_buffer[11] + el_buffer[12]; //!< calculo el checksum
					 if(el_buffer[13] == el_buffer[0])			//!< la trama se levanto correctamente
					 {
					  estado_recepcion = CARGO_CONFIG;	//!< cargamos la configuracion
					  SendAck();						//!< trama reconocida
					 }
					 else 
					 {
					  estado_recepcion = ESPERO_HEADER;	//!< volvemos a esperar el inicio de trama
					  SendNack();						//!< trama no reconicida
					 }
					}
				break;
				
				case CARGO_CONFIG:									//!< entramos acá con el byte de fin de trama
						if( el_buffer[8] == numero_de_estacion() )
						{
						t_entre_muestras = el_buffer[10];			//!< cargo el tiempo entre muestras
						n_de_muestras = el_buffer[11];			//!< cargo el numero de muestras
						Habilitacion_Mediciones =  el_buffer[12];	//!< cargo los parametros a medir
						datos_ingresados = Habilitacion_Mediciones;	//!< dejamos grabada la nueva configuracion
						estado_recepcion = ESPERO_HEADER;			//!< seteo el estado_recepcion para su proximo ingreso
						}
				break;	 
	}
 }
}


/**
	\fn  void MandarDato ( char *)
	\brief Escribe en el buffer de transmision una cadena (terminada en '\0')
 	
*/

void MandarDato ( char * msg )
{

	bit mandar;
	int i;
    
	
	
	//!<Si in_tx es igual a out_tx significa que ya se mandaron todos los datos de una trama anterior:
	mandar = (in_tx == out_tx);
    
	
	for(i = 0 ; msg[i] ; i++ )
	{
		buffer_Tx[in_tx] = msg[i];
		in_tx ++;
		in_tx %= TAM_TX;
	}
	//!<Si no se estan mandando datos FUERZO una interrupcion para mandar el primero:
	if ( mandar )
		TI0 = 1;

}

/**	
	\fn  void SendAck ( void )
	\brief Arma la trama de respuesta correspondiente a TRAMA VALIDA

*/

void SendAck ( void )
{
	el_buffer[0] = INICIO_TRAMA;
	el_buffer[1] = 'A';
	el_buffer[2] = 'C';
	el_buffer[3] = 'K';
	el_buffer[4] = el_buffer[0] + el_buffer[1] + el_buffer[2] + el_buffer[3];
	el_buffer[5] = FIN_TRAMA;
	el_buffer[6] = '\0';
	
	MandarDato( el_buffer );
}

/**	
	\fn  void SendNack ( void )
	\brief Arma la trama de respuesta correspondiente a TRAMA NO VALIDA

*/

void SendNack ( void )
{
	el_buffer[0] = INICIO_TRAMA;
	el_buffer[1] = 'N';
	el_buffer[2] = 'A';
	el_buffer[3] = 'C';
	el_buffer[4] = 'K';
	el_buffer[5] = el_buffer[0] + el_buffer[1] + el_buffer[2] + el_buffer[3] + el_buffer[4];
	el_buffer[6] = FIN_TRAMA;
	el_buffer[7] = '\0';
    
	MandarDato( el_buffer );
}