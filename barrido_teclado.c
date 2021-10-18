#include "libreria.h"

// Teclado de 4x3

// Columnas del Teclado
sbit FILA_1 = P1^0;
sbit FILA_2 = P1^1;
sbit FILA_3 = P1^6;
sbit FILA_4 = P1^5;

// Filas del Teclado
sbit COLUM_1 = P1^4;
sbit COLUM_2 = P1^3;
sbit COLUM_3 = P1^2;


extern bit timer_2500us;	// definida en mdt.c
extern U16 AUX[];			//definida en mdt.c
extern unsigned char tecla;	// definida en main.c

/**	
 *	\fn  void ReboteTecla( void )
 *	\brief Elimina el rebote de un código de tecla
 *	\param void	
 *	\return void
*/

void ReboteTecla( void ) {
	
	static unsigned char CodigoAnterior = NO_KEY ;
	static unsigned char EstadosEstables = 0 ;
	unsigned char CodigoActual ;
      
	CodigoActual = LecturaTecla4x3( );				
	

	if( CodigoActual == NO_KEY ) {  //!< No se pulso tecla o rebote
		CodigoAnterior = NO_KEY ;
		EstadosEstables = 0 ;
		return ;
	}

	if( EstadosEstables == 0 )	{  //!< Primera vez
		CodigoAnterior = CodigoActual ;
		EstadosEstables = 1 ;
		return ;
	}

	if( EstadosEstables == CANTIDADdErEBOTES )	{  	//!< Acepto la tecla
		EstadosEstables = CANTIDADdErEBOTES + 1;	//!< Hará que no vuelva a entrar hasta que suelten la tecla
		tecla = CodigoAnterior ;	
		return ;
	}
	
	if( EstadosEstables > CANTIDADdErEBOTES )		//!< La tecla sigue pulsada
		return ;

	if( CodigoActual == CodigoAnterior )
		EstadosEstables ++ ;
	else
		EstadosEstables = 0 ;	
	return ;
}



/**	
 *	\fn  unsigned char  LecturaTecla4x3 ( void )
 *	\brief Lectura del codigo de tecla con rebote de un teclado de 4 x 3
 *	\param void	
 *	\return unsigned char : Tecla ó NO_KEY si no se pulso tecla  
*/	

unsigned char  LecturaTecla4x3( void ) 
{
	
 //Barro Fila 1
	 
	 FILA_1 = 0;
	 FILA_2 = 1;
	 FILA_3 = 1;
	 FILA_4 = 1;

	  if(!COLUM_1) return  1;
	  if(!COLUM_2) return  2;
	  if(!COLUM_3) return  3;

//Barro Fila 2
	 FILA_1 = 1;
	 FILA_2 = 0;
	 FILA_3 = 1;
	 FILA_4 = 1;

	  if(!COLUM_1) return  4;
	  if(!COLUM_2) return  5;
	  if(!COLUM_3) return  6;	


//Barro Fila 3
	 FILA_1 = 1;
	 FILA_2 = 1;
	 FILA_3 = 0;
	 FILA_4 = 1;

	 if(!COLUM_1) return  7;
	 if(!COLUM_2) return  8;
	 if(!COLUM_3) return  9;

//Barro Fila 4
	 FILA_1 = 1;
	 FILA_2 = 1;
	 FILA_3 = 1;
	 FILA_4 = 0;

	 if(!COLUM_1) return  10;
	 if(!COLUM_2) return  0;
	 if(!COLUM_3) return  11;

	  //Si no se presiono ninguna tecla.
	  return NO_KEY;
}

unsigned char Teclado ()
{
   unsigned char Tecla;
   	 Tecla = tecla;
	 tecla = NO_KEY;
	 return Tecla;

}

void actualizar_teclado ()
{
 if(timer_2500us == OFF){ReboteTecla();timer_2500us_on;}//!< actualiza el valor de tecla cada 2.5ms
}
