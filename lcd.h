/******************************************************
*				LCD Library						      *
*	Supported hardware:		2x16 LCD				  *
*	Author:					Ajay Bhargav			  *
******************************************************/

#ifndef	__LCD_H__
#define __LDC_H__

//#include <C8051F020.H>


typedef unsigned char bit_8;


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			LCD Port Configuration
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define LCD_PORT P6
//sbit rs = 0xA0;
//sbit rw = 0xA1;
//sbit en = 0xA2;
//sbit flag = 0x97;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	LCD Command MACROS
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define LCD_CLEAR 0x01
#define RETURN_HOME 0x02
#define DEC_CURSOR 0x04
#define INC_CURSOR 0x06
#define DISP_OFF_CUR_OFF 0x08
#define DISP_OFF_CUR_ON 0x0A
#define DISP_ON_CUR_OFF 0x0C
#define DISP_ON_CUR_BLINK 0xE
#define SHIFT_CUR_LEFT 0x10
#define SHIFT_CUR_RIGHT 0x14
#define SHIFT_DISP_LEFT 0x18
#define SHIFT_DISP_RIGHT 0x1C

#define ACTIVO_ALTO 1
#define ACTIVO_BAJO 0
#define ON	1
#define OFF 0


#define setbitp(puerto,pin,activo) puerto=(activo==ACTIVO_ALTO) ? puerto|(1<<pin) : puerto&~(1<<pin)
#define clrbitp(puerto,pin,activo) puerto=(activo==ACTIVO_BAJO) ? puerto|(1<<pin) : puerto&~(1<<pin)
#define ifpinp(puerto,pin,estado) (((puerto<<pin)&0x01)==estado) ? ON : OFF
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~				   
		Function Prototypes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void SYSCLK_Crystal_Init(void);
void PORT_Init(void);
void Inicializar(void);
void lcd_init(void);		// Initialize LCD
void busy(void);			// Check Busy
void wrt_cmd(bit_8);		// Sending Command
void wrt_data(bit_8);		// Sending single character
void wrt_string(bit_8*);	// Sending String
void wrt_value(bit_8);		// Sending 8-bit Decimal value
void cursorxy(bit_8,bit_8);	// Bringing Cursor to (X,Y) location X -> 1,2 and Y -> 1-16

#endif