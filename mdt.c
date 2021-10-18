#include "libreria.h"

//--- ON = el timer esta encendido ( esta contando )
//--- OFF = el timer esta apagado	( no cuenta )

extern U8 tick_muestra;	//!< definida en main.c

U16  AUX[10] = {0,0,0,0,0,0,0,0,0,0};

bit timer_2500us = OFF;
bit timer_5ms = OFF;
bit timer_1s = OFF;
bit timer_5s = OFF; 
bit timer_10s = OFF;
bit timer_1min = OFF;

void mdt ()
{
if (AUX[0]){AUX[0]--;}
	else {timer_2500us = OFF;}
if (AUX[1]){AUX[1]--;}
	else {timer_5ms = OFF;}
if (AUX[2]){AUX[2]--;}
	else {timer_1s = OFF;}
if (AUX[3]){AUX[3]--;}
	else {timer_5s = OFF;}
if (AUX[4])
	{
		if(AUX[5]){AUX[5]--;}
		else
		{
			AUX[5] = 50000;
			AUX[4]--;
		}
	}
	else {timer_10s = OFF;}
if (AUX[6])
	{
		if(AUX[7]){AUX[7]--;}
		else
		{
			AUX[7] = 50000;
			AUX[6]--;
		}
	}
	else {timer_1min = OFF;}
if (AUX[8])
	{
		if(AUX[9]){AUX[9]--;}
		else
		{
			AUX[9] = 50000;
			AUX[8]--;
		}
	}
	else {
		timer_tick_muestra_on;
		tick_muestra++;
		}
		
/*-if (AUX[6]){AUX[6]--;}
	else {timer_1s = OFF;}
if (AUX[7]){AUX[7]--;}
	else {timer_1s_muestra = OFF;}
if (AUX[8]){AUX[8]--;}
	else {timer_5s = OFF;}
-*/
}

