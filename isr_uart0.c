#include "libreria.h"

extern U8 buffer_Rx[];
extern U8 buffer_Tx[];
extern U8 in_tx,out_tx,in_rx,out_rx;

void isr_uart0() interrupt 4
{
 if (RI0) // interrupcion por dato recibido
 {
  buffer_Rx[in_rx] = SBUF0;
  in_rx++;
  in_rx %= TAM_RX;
  RI0 = OFF;		//!< borramos el flag por byte recibido
 }
 
 //!<Si la interrupcion es por transmision (se termino de mandar un dato)
 	if ( TI0 )
	{
		TI0 = 0;
		//!<Solo mando un dato si el indice out_tx todavia no alcanzo a in_tx:
		if ( in_tx != out_tx )
		{
			SBUF0 = buffer_Tx[out_tx];
			out_tx++;
			out_tx %= TAM_TX;
		}
	}
 }


