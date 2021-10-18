#include "libreria.h"

/*-
Trama de config:
_________________________________________________________________________________
 #  | n de estacion | config | t_entre_muestras| n_de_muestras| parametros | chksm| $
=================================================================================

# - inicio de trama
n_de_estacion : numero de estacion que envia\recibe la informacion
config: si este byte es 0xF0, entonces los datos siguientes son de configuracion
t_entre_muestras: tiempo entre muestras
n_de_muestras: numero de muestras
parametros:  si el bit 0 esta en 1 interesa la toma de temperatura
                        si el bit 1 esta en 1 interesa la toma de humedad
                        si el bit 2 esta en 1 interesa la toma de presión
chksm: checksum la suma de los bytes anteriores sin contar el inicio de trama
$: fin de trama
-*/


/**
 *	\fn void envio_de_config ( void )
 *	\brief envia la configuracion a la PC cada 1 segundos
 			espera respuesta de la PC tipo "#ACK(checksum)$"
 *	\param void
 *	\return void
 */

//Variable Externas
extern U16 AUX[];							// variable definida en mdt.c
extern bit timer_1s;
extern U8 n_de_muestras;					//!< variable declarada en config_medicion.c
extern U8 t_entre_muestras;					//!< variable declarada en config_medicion.c
extern U8 Habilitacion_Mediciones;			//!< variable declarada en config_medicion.c
extern U8 estado;							//!< variable declarada en main.c
extern U8 buffer_Rx[];						//!< variable declarada en manejo_serie.c
extern U8 buffer_Tx[];						//!< variable declarada en manejo_serie.c
extern U8 in_tx,out_tx,in_rx,out_rx; 		//!< variable declarada en manejo_serie.c
extern U8 el_buffer[];

void envio_de_config (void)
 {
  static U8 estado_envio = LLENO_BUFFER;	//!< seteo el estado inicial del switch(estado_envio)
  int aux = 0;
  switch (estado_envio)
  {
   					case LLENO_BUFFER:		//!< cargo el buffer con los valores de configuracion actuales
              							 
						el_buffer[0] = INICIO_TRAMA;
  						el_buffer[1] = numero_de_estacion();
  						el_buffer[2] = TRAMA_CONFIG;
  						el_buffer[3] = t_entre_muestras;
  						el_buffer[4] = n_de_muestras;
  				        el_buffer[5] = Habilitacion_Mediciones;
  				     	aux = HextoInt (el_buffer [5]);
	                    el_buffer [5] = aux;
				        el_buffer[6] = el_buffer[1] + el_buffer[2] + el_buffer[3] + el_buffer[4] + el_buffer[5];
  					    aux = HextoInt (el_buffer [6]);
	                    el_buffer [6] = aux;
						el_buffer[7] = FIN_TRAMA;
  						el_buffer[8] =	'\0';
						el_buffer[10] = 0;                 //!< inicio el contador
						
						MandarDato( el_buffer );
						estado_envio = MANDO_TRAMA;					//!< terminado el llenado del buffer, cambio al siguiente estado
					
					break;
					
					case MANDO_TRAMA:
						 if(in_tx == out_tx)						//!< preguntamos si se termino de enviar la trama
						 {
						 	timer_1s_on;						//!< activo el flag de 1 segundo para que la PC responda
							estado_envio = ESPERO_RTA;			//!< cargo el siguiente estado, espero rta de la PC
						 }
						 
					break;

					case ESPERO_RTA:								//!< espero contestacion de la PC
						 
						 if (timer_1s == OFF)						//!< si se cumplió 1 segundo sin respuesta
						 {
						 el_buffer[10] = 0; 						//!< reseteo contador
						 estado_envio = LLENO_BUFFER;				//!< volvemos al estado de LLENO_BUFFER
						 break;										//!< salgo del case
						 }
						 
				 		if( HayDato() )									//!< preguntamos si hay nuevos datos en buffer rx
				 		{
				 		 el_buffer[ el_buffer[10] ] = PopRecepcion(); 	//!< pongo el byte recibido en un array
						 if( el_buffer[0] != INICIO_TRAMA ) {break;}	//!< me fijo si es el inicio de trama
						 el_buffer[10]++;								//!< incremento el nro de bytes recibidos
						}						

						 if( el_buffer[10] == 5 )						//!< se han recibido los 5 bytes
						 {
							el_buffer[11] = 0;	  						//!< reseto variable contador
							 
							if(el_buffer[0] == INICIO_TRAMA)el_buffer[11]++;//!< si los bytes son los correspondientes con #ACK(chk)$
							if(el_buffer[1] == 'A')el_buffer[11]++;		 //!< contador es 6 de otra manera contador es menor a 6
							if(el_buffer[2] == 'C')el_buffer[11]++;		 //!< entonces los datos no fueron bien recibidos; se los envia de nuevo
							if(el_buffer[3] == 'K')el_buffer[11]++; 
							if(el_buffer[4] == FIN_TRAMA)el_buffer[11]++;

							if(el_buffer[11] == 5 )							//!< si contador es 5 la trama fue bien recibida por la PC
								{
								 estado = TOMA_DE_DATOS;					//!< cambio el estado de la maquina de estados
								 estado_envio = LLENO_BUFFER;				//!< dejo a estado_envio en su estado inicial
								}
							 else 											//!< la trama fue mal recibida entonces la volvemos a envia
								{
								 estado_envio = LLENO_BUFFER;				//!< vuelvo al estado inicial, lleno denuevo el buffer
								}
							 el_buffer[10] = 0;
						 }									
					break;
  }
 }