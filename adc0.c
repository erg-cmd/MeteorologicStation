#include "libreria.h"


void isr_adc0(void) interrupt 15
{
 	AD0INT = OFF;		//!< borro el flag
}

