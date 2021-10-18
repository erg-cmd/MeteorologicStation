#include "libreria.h"


void lcd_init(void)
{
	wrt_cmd(0X38);
	clrbitp(P4,7,ACTIVO_ALTO);//rs=0;
	clrbitp(P7,6,ACTIVO_ALTO);//rw=0;
	setbitp(P4,6,ACTIVO_ALTO);//en=1;
	clrbitp(P4,6,ACTIVO_ALTO);//en=0;
	busy();
	wrt_cmd(0X38);
	clrbitp(P4,7,ACTIVO_ALTO);//rs=0;
	clrbitp(P7,6,ACTIVO_ALTO);//rw=0;
	setbitp(P4,6,ACTIVO_ALTO);//en=1;
	clrbitp(P4,6,ACTIVO_ALTO);//en=0;
	busy();
	wrt_cmd(0X38);
	clrbitp(P4,7,ACTIVO_ALTO);//rs=0;
	clrbitp(P7,6,ACTIVO_ALTO);//rw=0;
	setbitp(P4,6,ACTIVO_ALTO);//en=1;
	clrbitp(P4,6,ACTIVO_ALTO);//en=0;
	busy();
	wrt_cmd(0X0C);
	clrbitp(P4,7,ACTIVO_ALTO);//rs=0;
	clrbitp(P7,6,ACTIVO_ALTO);//rw=0;
	setbitp(P4,6,ACTIVO_ALTO);//en=1;
	clrbitp(P4,6,ACTIVO_ALTO);//en=0;
	busy();
	wrt_cmd(0X01);
	clrbitp(P4,7,ACTIVO_ALTO);//rs=0;
	clrbitp(P7,6,ACTIVO_ALTO);//rw=0;
	setbitp(P4,6,ACTIVO_ALTO);//en=1;
	clrbitp(P4,6,ACTIVO_ALTO);//en=0;
	busy();
	wrt_cmd(0X06);
	clrbitp(P4,7,ACTIVO_ALTO);//rs=0;
	clrbitp(P7,6,ACTIVO_ALTO);//rw=0;
	setbitp(P4,6,ACTIVO_ALTO);//en=1;
	clrbitp(P4,6,ACTIVO_ALTO);//en=0;
	busy();
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**** Cheacking the busy flag of LCD
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void busy()
{
	unsigned char i,j;
    for(i=0;i<50;i++)        //A simple loop for delay
    	for(j=0;j<255;j++);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Writing command to LCD 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void wrt_cmd(bit_8 val)
{
	busy();
	LCD_PORT=val;
	clrbitp(P4,7,ACTIVO_ALTO); //rs=0;
	clrbitp(P7,6,ACTIVO_ALTO);//rw=0;
	setbitp(P4,6,ACTIVO_ALTO);//en=1;
	clrbitp(P4,6,ACTIVO_ALTO);//en=0;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Writing string to LCD
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void wrt_string(bit_8 *string)
{
	busy();
	while(*string)
		wrt_data(*string++);
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Writing data to LCD
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void wrt_data(bit_8 ch)
{
	busy();
	LCD_PORT = ch;
	setbitp(P4,7,ACTIVO_ALTO);//rs=1;
	clrbitp(P7,6,ACTIVO_ALTO);//rw=0;
	setbitp(P4,6,ACTIVO_ALTO);//en=1;
	clrbitp(P4,6,ACTIVO_ALTO);//en=0;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	Bringing Cursor to (X,Y) location of LCD
		X -> 1,2
		Y -> 1,16
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void cursorxy(bit_8 x, bit_8 y)
{
	if((x<1||x>2)&&(y<1||y>16))
	{
		x=1;
		y=1;
	}
	if(x == 1)
		wrt_cmd(0x7F+y);
	else
		wrt_cmd(0xBF+y);
}