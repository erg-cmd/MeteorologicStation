#include "libreria.h"

extern U8 tick_muestra;
U8	mis_segundos = 10;

void isr_timer0 () interrupt 1 
{
  
/*-	if(flag_muestra_1s == ON)					//!< pregunto si pasó 1 segundo
	{
		flag_muestra_1s_on();					//!< activo el timer de 1 segundo
		mis_segundos--;					//!< decremento mis segundos
		if(mis_segundos == 0)
		{
			ticks_muestras++;			//!< incremento ticks_muestras
			//mis_segundos = 50;		//!< reinicio mis segundos valor posta
			mis_segundos = 10;
		}
	}-*/
  mdt();
  TL0 = t_100us_l + TL0;
  TH0 = t_100us_h;	
}