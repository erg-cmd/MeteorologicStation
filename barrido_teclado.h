#ifndef __BARRIDO_TECLADO_H__
#define __BARRIDO_TECLADO_H__

#define		NO_KEY				0xff
#define		CANTIDADdErEBOTES	25

/*------------------------------------------*/
//---Prototipos de barrido_teclado

void ReboteTecla( void );
unsigned char  LecturaTecla4x3 ( void );
unsigned char Teclado( void );
void actualizar_teclado( void );

/*------------------------------------------*/

#endif