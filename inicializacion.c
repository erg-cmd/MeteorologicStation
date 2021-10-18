/*-----Trabajo Prático Final-----------------------------
-----Estacion Meteorólogica------------------------------
Integrantes: Mauro Boffito, Diego Pirotta y Elias Gracia
-----modificado 05/08/2012-------------------------------
-------------------------------------------------------*/

/*---------Includes--------------------------------------*/

#include "libreria.h"


void inicializacion()
{
 inicio_wdt();
 inicio_oscx();
 inicio_puertos();
 inicio_uart0();
 inicio_adc0();
 inicio_timers();
 inicio_interrupciones();
 inicio_lcd();
 
}

/*---------Descripciones---------------------------------*/

void inicio_wdt()
{
   	WDTCN = 0xDE;   // Deshabilito el watchdog timer
	WDTCN = 0xAD;	// por ahora........
}

//-----------------------------------------------------------

void inicio_oscx()
{
	unsigned int i;						
	OSCXCN = 0x67;						//!< Configuro el registro de control del oscilador 
														//!< 01100111
														//!< Habilito el oscilador para funcionar con cristal (22.1184MHz)
														//!< Configuro el rango de trabajo del oscilador
	
	for (i=0; i < 256; i++);	//!< Demora "Cuidado esta demora (bloqueante) se puede hacer porque 
														//!< estoy en la inicializacion y no bloqueo ningun otro proceso!!!!
														//!< Hay que hacer esta demora para que se estabilice el oscilador

	while (!(OSCXCN & 0x80));	//!< Espero hasta que se estabilice (Cuidado es bloqueante!!)

	OSCICN = 0x88;						//!< Configuro el oscilador interno
														//!< 10001000
														//!< Configuro el micro para utilizar el oscilador externo

	while (!(OSCXCN & 0x80));	//!< Espero hasta que se estabilice (Cuidado es bloqueante!!)

	OSCICN = 0x08;						//!< Configuro el oscilador interno
														//!< 00001000
														//!< Configuro el micro para utilizar el oscilador externo
}

//------------------------------------------------------------

void inicio_puertos()
{
														//!< Seteo para pulsadores	(0 = Open-Drain \ 1 = Push-Pull)
	P0MDOUT = 0xFF;						//!< Puerto 0 como salida
	P1MDOUT = 0x63;						//Configuro todo el puerto1  0110 0011
	P2MDOUT = 0xF0;						//!< Puerto 2 como salida nibble bajo / entrada nibble alto.
	P2 = 0xFF;								//!< Seteo el puerto en 1
	P74OUT = 0xFB; 						//!<1111 1011 Todo salida menos P5.3-0
	XBR0 = 0x04;							//!< Ruteo pins P0.0 y P0.1 a la Uart0
	XBR2 = 0x44;							//!<Ruteo pins P0.2 y P0.3 a la uart1; 
                    				//!<Habilito la crossbar y habilito lo pull ups
}

//------------------------------------------------------------

void inicio_uart0()
{
 	SCON0 = 0x50; 						//!< Inicio en modo 1 ( baud rate determinado por el timer 1)
 	PCON |= 0x80;							//!< SMOD en 1, 
 	TH1 = t_timer1;	 					//!< cargo el timer 1
 	TL1 = t_timer1;						//!< recarga del timer 1
 	TR1 = ON;									//!< enciendo el timer 1
														//!< Lo inicio acá porque no genera interrupciones
}

//------------------------------------------------------------

void inicio_adc0()
{
  AMX0CF = 0; // Single-Ended. 
  ADC0CF = 0x27; // AD0SC = 4; CLKsar = 2.12Khz; Ganancia = 1;
  AMX0SL = 0x01; // Seleccionado por defecto AIN0.1
  REF0CN = 0x02; // Enciendo el generador externo VREF0 
  ADC0CN = 0xC0; // Enable - Reset de Busy, Int - Modo: 00 - Right Justified.
}

//------------------------------------------------------------

void inicio_timers()
{	
	
										//!< CKCON por defecto esta en 0x00
										//!< por lo que divide al clock por 12 
	TMOD = 0x21 ;  						//!< Timer 0 en modo 16 bits y Timer 1 
										//!< en modo 8 bits con recarga		
	TH0	= t_100us_l;				 	//!< cargo el timer 0 para que desborde cada 
	TL0 = t_100us_h;					//!< 100us
}

//------------------------------------------------------------

void inicio_interrupciones()
{
	EIE2 = 0x02; // habilito interrupcion por conversion ADC0
	ES0  = ON;  	//habilito la interrupcion por puerto serie
	TF0 = OFF ;	//borro el flag de desborde timer 0
	ET0 = ON ;	//habilito la interrupcion por desborde timer 0
	//TF2 = OFF;	//borro el flag de deborde 	timer 2
	//ET2 = ON;	//habilito la interrupcion del timer 2
	EA  = ON ;  //habilito la interrupcion global

}

//------------------------------------------------------------

void inicio_lcd()
{
 P4 = 0; P5 = 0;						//!< reseteo port 4 y 5
 lcd_init();							//!< rutina de inicio 
 wrt_cmd(LCD_CLEAR);					//!< parte de la rutina de inicio
 cursorxy(1,1);							//!< muevo el cursor a la posicion 1,1
 										//!< no se para que... porque el comando anterior
										//!< te manda el cursor a esa posicion, pero por las
										//!< dudas lo dejo así 
}


