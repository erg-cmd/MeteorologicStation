#ifndef __MDT_H__
#define __MDT_H__

/*-defines----------------------*/

#define timer_2500us_on;    	{timer_2500us = ON; AUX[0] = 25;}  
#define timer_5ms_on;    		{timer_5ms = ON; AUX[1] = 50;}
#define timer_1s_on;	 		{timer_1s = ON; AUX[2] = 10000;}
#define timer_5s_on;	 		{timer_5s = ON; AUX[3] = 50000;}
#define timer_10s_on;			{timer_10s = ON; AUX[4] = 2;AUX[5] = 50000;}
#define timer_1min_on;			{timer_1min = ON; AUX[6] = 12;AUX[7] = 50000;}
#define timer_tick_muestra_on;	{AUX[8] = 1; AUX[9]  = 50000;}

/*-Prototipos de funciones-------*/

void mdt();

#endif