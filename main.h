#ifndef __MAIN_H__
#define __MAIN_H__

/*-Referencia de Estados-------------------------------*/

#define TAM_TX		15
#define TAM_RX		15

#define CONFIG_MEDICION 	0
#define ENVIO_DE_CONFIG 	1
#define TOMA_DE_DATOS   	2
#define ENVIO_DE_DATOS		3
#define EN_PANTALLA			4
#define EMERGENCIA			5

/*-----Prototipos de las Funciones-----*/

void inicializacion();
void config_medicion();
void tomar_muestra();
void actualizar();
void envio_datos();
void envio_de_config();

#endif 