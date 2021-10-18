#ifndef __MANEJO_SERIE_H__
#define __MANEJO_SERIE_H__

//---defines-------------


#define CARGO_BUFFER_TRAMA1			14
#define CARGO_BUFFER_TRAMA2  		15
#define ESPERO_ENVIO_TRAMA 			16
#define ESPERO_HEADER				17
#define LEVANTO_TRAMA				18
#define CARGO_CONFIG				19

//cabezeras de funcion

int 	PopRecepcion ();
bit 	HayDato();
void 	RecibirTrama();
void 	envio_datos();
void 	MandarDato (char *);
void 	SendNack();
void 	SendAck();

#endif