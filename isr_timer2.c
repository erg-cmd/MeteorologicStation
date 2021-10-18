#include "libreria.h"

//!< no entra a la interrucion; el timer 2 esta siempre encencido
//!< pero no genera interrupciones

sfr16 T2 = 0xCC;    			//!< cuentas del timer 2

void isr_timer2 () interrupt 5
{
 EA = OFF;  					//!< deshabilito las interrupciones
 TF2 = OFF; 					//!< borro el flag del timer 2
 T2 = 0;						//!< reinicio la cuenta del timer 2
 EA = ON;						//!< habilito las interrupciones
}
