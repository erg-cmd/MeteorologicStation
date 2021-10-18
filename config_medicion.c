#include "libreria.h"

/*::::::: status de datos_ingresados ::::::::::::
-Parametro-------------bit-----OK----NOK---------
temperatura------------0-------1-----0-------lsb-
humedadRel-------------1-------1-----0-----------
presionAtm-------------2-------1-----0-----------
xxxxxxx----------------3-------1-----0-----------
xxxxxxx----------------4-------1-----0-----------
nº muestras------------5-------1-----0-----------
tiempo e/ muestras-----6-------1-----0-----------
xxxxxxx----------------7-------1-----0-------msb-
:Nota::::::::::::::::::::::::::::::::::::::::::::
::  En datos_ingresados solo se guarda si se   ::
:: pudo configurar o no, pero no el valor      :: 
:: de esa configuracion.                       ::
:::::::::::::::::::::::::::::::::::::::::::::::*/


extern bit flag_actualizar;					// variable definida en tomar_muestra.c
extern U16 AUX[];							// variable definida en mdt.c
extern bit timer_2500us;					// variable definida en mdt.c
extern bit timer_1s;						// variable definida en mdt.c
extern bit timer_5s;						// variable definida en mdt.c
extern U8 estado;							// variable definida en main.c
extern U8 tecla;                  			// variable definida en main.c
extern U8 el_buffer[];            			// array definido en main.c
U8 n_de_muestras = N_MUESTRAS_DEFECTO; 				// numero de muestras a tomar antes del envio
U8 t_entre_muestras = T_ENTRE_MUESTRAS_DEFECTO;		// intervalo de tiempo entre toma de muestras
U8 datos_ingresados = 0x00;
U8 Habilitacion_Mediciones = 48;

/*--------------------------------------------------------*/

code U8 mensaje1[17] = {'1','-','°','C','[',' ',']',' ','2','-','R','H','%','[',' ',']','\0'};
code U8 mensaje2[17] = {'3','-','h','P','a','[',' ',']',' ','6','-',' ','>','>',' ',' ','\0'};
code U8 mensaje3[17] = {'1','-',' ',' ','<','<',' ',' ',' ','2','-','O','K','!',' ',' ','\0'};
code U8 mensaje4[17] = {'3','-','S','a','l','i','r',' ',' ',' ',' ',' ',' ',' ',' ',' ','\0'};

/**
 *	\fn void config_medicion ( void )
 *	\brief setea parametro de medicion si antes no encuentra una configuracion previa
 *	\param void
 *	\return void
 */

void config_medicion()
{
	if ( flag_actualizar == ON )			//!< flag activado en actualizar(), corremos la configuracion 
	{
		menu_display();
		return;
	}	  		
	
	actualizar_teclado();					//!< actualiza el valor de tecla cada 2.5ms
	
	if( Teclado() != NO_KEY ) 				//!< si se apretó el teclado
	{
		menu_display();						//!<corremos el menu en el display
	}										//!< sinó retornamos de la función
}

/*---Desarrollo de las funciones-----------------*/

/**
 *	\fn void menu_display ( void )
 *	\brief mostramos el menu de configuracion; tiempo para apretar una tecla es 5 segundos
 *	\param void
 *	\return void
 */

void menu_display()
{
  bit atras_menu = OFF;								//!< flag para retornar al menu anterior
  wrt_cmd(LCD_CLEAR);								//!< Borramos la pantalla
  wrt_string(" Configuracion:");					//!< "Configuracion"
  cursorxy(2,1);									//!< corremos el cursor a la segunda fila
  wrt_string("1-Setear 2-Salir");					//!< "1-Setear 2-Salir"
  timer_5s_on;										//!< activamos el timer de 5 segundos, el usuario tiene 5 segundos
  while(timer_5s == ON)								//!< para apretar cualquier tecla, sino sale del menu
  {  
  	actualizar_teclado();             				//!< actualiza el valor de tecla cada 2.5ms
	switch( Teclado() )
	{
  					 case 1:								//!< Se eligió la configuracion
							timer_5s_on;					//!< se ha pulsado una tecla, +5 segundos
							wrt_cmd(LCD_CLEAR);				//!< borramos la pantalla
							wrt_string(mensaje1);			//!< "1-²C[ ] 2-RH%[ ]"
							cursorxy(2,1);					//!< corremos el cursor a la segunda fila
							wrt_string(mensaje2);			//!< "3-hPa[ ] 6- ->> "
							if(Habilitacion_Mediciones & TEMP)
							{
							 cursorxy(1,6);
							 wrt_data('X');
							}
							if(Habilitacion_Mediciones & HUMDDrEL)
							{
							 cursorxy(1,15);
							 wrt_data('X');
							}
							if(Habilitacion_Mediciones & PRESaTM)
							{
							 cursorxy(2,7);
							 wrt_data('X');
							}	
							while(timer_5s == ON)
							{
							 actualizar_teclado();      //!< actualiza el valor de tecla cada 2.5ms
														//!< actualiza selecciones anteriores de parametros en pantalla
							 switch( Teclado() )
							 {
									case 1:									//!< ON/OFF medir temperatura
											tildar_casilla(TEMP);			//!< escribo en fila 1, columna 5 'X' y 
																			//!< se presiono una tecla, + 5 segundos
									break;
									
									case 2:									//!<  ON/OFF medir humedad
											tildar_casilla(HUMDDrEL);		//!< escribo en fila 1, posicion 14 'X'
																			//!< se presiono una tecla, + 5 segundos
									break;
									
									case 3:									//!< ON/OFF medir presion
											tildar_casilla(PRESaTM);		//!< escribo en fila 2, posicion 6 'X'
																			//!< se presiono uno tecla, + 5	segundos
									break;
									
									case 6:									//!< nos desplazamos al segundo menu
											atras_menu = OFF;				//!< borramos flag
											wrt_cmd(LCD_CLEAR);				//!< borramos la pantalla
											wrt_string(mensaje3);			//!< "1-  <<   2-OK!  "
											cursorxy(2,1);					//!< corremos el cursor a la segunda fila
											wrt_string(mensaje4);			//!< "3-Salir		  "
											timer_5s_on;					//!< se presiono una tecla, + 5	segundos
											while( timer_5s == ON && atras_menu == OFF )					  
											{								//!< bucle hasta 5 seg o se active el timer
											actualizar_teclado();			//!< actualiza el valor de tecla cada 2.5ms
											switch( Teclado() )										
											{
															case 1:							//!< opcion volver al menu anterior
																	atras_menu = ON;		//!< activamos flag correpondiente
																	timer_5s_on;			//!< se ha pulsado una tecla, + 5 segundos
																	wrt_cmd(LCD_CLEAR);				//!< borramos la pantalla
																	wrt_string(mensaje1);			//!< "1-²C[ ] 2-RH%[ ]"
																	cursorxy(2,1);					//!< corremos el cursor a la segunda fila
																	wrt_string(mensaje2);			//!< "3-hPa[ ] 6- ->> "
																		//!< actualiza los parametros en pantalla
																	if(Habilitacion_Mediciones & TEMP)
																	{
																	 cursorxy(1,6);
																	 wrt_data('X');
																	}
																	if(Habilitacion_Mediciones & HUMDDrEL)
																	{
																	 cursorxy(1,15);
																	 wrt_data('X');
																	}
																	if(Habilitacion_Mediciones & PRESaTM)
																	{
																	 cursorxy(2,7);
																	 wrt_data('X');
																	}	
															break;

															case 2:								//!< Confirmamos seleccion 
																	flag_actualizar = OFF;
																	menu_display_2();		    //!< Pedimos tiempo entre muestras. Por default devuelve
																								//!< t_entre_muestras = 1(min) 
																	
																	datos_ingresados = Habilitacion_Mediciones;
																									//!< guardamos los parametro ingresados (temp / humm / pres)
																	datos_ingresados |= 0x60;			  //!< datos de n_de_muestras y t_entre datos ingresados

																	wrt_cmd(LCD_CLEAR);			    //!< borramos la pantalla
																	wrt_string("Configuracion OK");	//!< "Configuracion OK"
																	timer_1s_on;				    //!< activamos el timer de 1 segundo
																	while(timer_1s == ON);		//!< esperamos que pase 1 segundo; solamente para informar
																								//!< al usuario de la configuracion
																	timer_5s = OFF;	 	        //!< apagamos el timer de 5 segundo para salir
																	estado = ENVIO_DE_CONFIG;	//!< siguiente estado envio de configuracion a PC
															break;

															case 3:								//!< Salimos del menu y restablemos configuraciones
																								//!< anteriores si las hay o por default
																	flag_actualizar = OFF;		//!< reseteamos para que no escape al envio de configuracion
																	datos_ingresados = Habilitacion_Mediciones;
															//!< guardamos los parametro ingresados (temp / humm / pres)
																	datos_ingresados |= 0x60;			//!< datos de n_de_muestras y t_entre datos ingresados
																	wrt_cmd(LCD_CLEAR);	   				//!< borramos la pantalla
																	wrt_string("Configuracion");		//!< "Configuracion"
																	cursorxy(2,1);						//!< muevo cursor a fila 2
																	wrt_string("por defecto");			//!< "por defecto"
																	timer_1s_on;						//!< activo el timer de 1 segundo
																	while(timer_1s == ON);				//!< esperamos que pase 1 segundo; solamente para informar
															//!< al usuario de la configuracion
																	timer_5s = OFF;						//!< desactivamos el timer de 5 segundos para salir
																	estado = ENVIO_DE_CONFIG;			  //!< siguiente estado envio de configuracion a PC
															break;										
											}
											}
									break;
							 }
							}
					 break;

					 case 2:
							if (flag_actualizar == ON)				//!< solo valido cuando es llamado desde TOMA_DE_DATOS
																	//!< no se envia la nueva config y se sigue con la toma de datos
							{
							 	estado = TOMA_DE_DATOS;				//!< vuelvo al estado TOMA_DE_DATOS sin cambios que informar
								wrt_cmd(LCD_CLEAR);
								break;								//!< sale de menu_display
							}
							
							if(datos_ingresados != 0)				//!< nos fijamos si hay configuracion previa
							{
								wrt_cmd(LCD_CLEAR);	   				//!< borramos la pantalla
								wrt_string("Configuracion");		//!< "Configuracion"
								cursorxy(2,1);						//!< muevo cursor a fila 2
								wrt_string("anterior");				//!< "por defecto"
								timer_1s_on;						//!< activo el timer de 1 segundo
								while(timer_1s == ON);				//!< esperamos que pase 1 segundo; solamente para informar
																	//!< al usuario de la configuracion
								timer_5s = OFF;						//!< desactivamos el timer de 5 segundos para salir
							}
							else
							{
								wrt_cmd(LCD_CLEAR);	   				//!< borramos la pantalla
								wrt_string("Sin Configura-");		//!< "Sin Configura-"
								cursorxy(2,1);						//!< muevo cursor a fila 2
								wrt_string("-cion");				//!< "-cion"
								Habilitacion_Mediciones = 0;
								timer_1s_on;						//!< activo el timer de 1 segundo
								while(timer_1s == ON);				//!< esperamos que pase 1 segundo; solamente para informar
																	//!< al usuario de la configuracion
								timer_5s = OFF;						//!< desactivamos el timer de 5 segundos para salir	 	
							}
					 break;
							
	}
  }
  wrt_cmd(LCD_CLEAR);		//!< borro la pantalla 
}

/**
 *	\fn void tildar_casilla ( U8 mensajeX, U8 posicion)
 *	\brief escribe o borra X de la posicion que le pasas
 *	\param [ U8 parametro ]						 
 *	\return void
 */
void tildar_casilla(U8 parametro)
{
 		switch (parametro)
	{
							case TEMP:
									cursorxy(1,6);
									if(Habilitacion_Mediciones & TEMP)
									{
                   wrt_data(' ');
                   Habilitacion_Mediciones &= ~TEMP;
                  }
									else
                  {
                    wrt_data('X');
                    Habilitacion_Mediciones |= TEMP;
                  }
							break;
							
							case HUMDDrEL:
								  cursorxy(1,15);
									if(Habilitacion_Mediciones & HUMDDrEL)
									{
                   wrt_data(' ');
                   Habilitacion_Mediciones &= ~HUMDDrEL;
                  }
									else
                  {
                    wrt_data('X');
                    Habilitacion_Mediciones |= HUMDDrEL;
                  }
							break;
							
							case PRESaTM:
								   cursorxy(2,7);
									if(Habilitacion_Mediciones & PRESaTM)
									{
                   wrt_data(' ');
                   Habilitacion_Mediciones &= ~PRESaTM;
                  }
									else
                  {
                    wrt_data('X');
                    Habilitacion_Mediciones |= PRESaTM;
                  }
							break;
	}
	timer_5s_on;                   //!< se presiono una tecla, entonces +5 seg	
}

/**
 *	\fn void menu_display_2 ( void )
 *	\brief pide los datos de t_entre_muestras y n_de_muestras
 *	\param void
 *	\return void
 */
void menu_display_2()
{
 U8 t_muestras[2]={"01"};					    //!< cantidad de muestras a tomar
 U8 n_digitos = 0, n_aux = 0;				    //!< n_digitos se desplaza dentro de n-muestras; n_aux auxiliar
 U8 decena = 0, unidad = 0;
 bit flag = OFF;							    //!< datos ingresados, al estar activo los datos se ingresaron por defecto.
  
 while(flag == OFF)
 {
	 wrt_cmd(LCD_CLEAR);					    //!< borro la pantalla
	 wrt_string("min por muestra");			    //!< "cant de muestras"
	 cursorxy(2,1);							    //!< muevo el cursor a la segunda fila
	 wrt_string("(1-99): --");				    //!< "(1-99): --"
	 cursorxy(2,9);							    //!< muevor el cursor a la segunda fila, colum 9
	 timer_5s_on;							    //!< actvio timer de 5 segundos
	 while(timer_5s == ON)					    //!< para apretar cualquier tecla, sino sale del menu
	 {  
	  if(n_digitos > 1) {break;}  				//!< si digitos es mayor a 1 sale del while; 2 digitos ingresados
		actualizar_teclado();                 	//!< actualiza el valor de tecla cada 2.5ms
		n_aux = Teclado();						//!< actualizao auxiliar con el valor de tecla
		if(n_aux != NO_KEY)						//!< pregunto si se apreto alguna tecla
		{																      //!< en caso verdadero: 
		 t_muestras[n_digitos] = n_aux;			//!< guardo el valor de tecla
		 wrt_data(t_muestras[n_digitos]+48);	//!< escribo la tecla apretada en pantalla
		 n_digitos++;							//!< incremento digitos
		 timer_5s_on;							//!< se ha presionado una tecla, +5 segundos
		}
	 }
	decena = t_muestras[0];											//!< del while se sale si pasaron 5 seg o digitos > 1
	unidad = t_muestras[1]; 
	 if(n_digitos > 1)							//!< si digitos > 1 verifico que las teclas esten en el rango de  0 - 9
	 {																	
	  if((decena == 0) && (unidad == 0)) decena = 15; //!< tiempo mayor o igual a 5 minutos; 
																			//!< escribo muestras[0] para 
	  if(decena < 10)													//!< decena < 10
	  {
	  	if( (decena >= 0) && (unidad < 10) )
		{
		t_entre_muestras = (decena * 10) + unidad;	//!< guardamos el valor del usuario en t_entre_muestras_total
		 break;													    //!< salimos del while(n_verif == OFF)
		}												//!< 0 < unidad < 10. Unidad no puede ser 0; 
		else{n_digitos = 0;}							//!< la unidad esta fuera de rango, repetimos el proceso
	  }
	  else	{n_digitos = 0;} 							//!< la decena está fuera de rango. repetimos el proceso
	 }
	 else 												//!< pasaron 5 segundos sin apretarse una tecla
	 {													//!< cargamos los valores por defecto
	  n_de_muestras = N_MUESTRAS_DEFECTO;				//!< t_entre_muestras y n_muestras_total
	  t_entre_muestras = T_ENTRE_MUESTRAS_DEFECTO;
	  flag = ON;										//!< activamos el flag
	 } 
 }
}

/**
 *	\fn unsigned char dni_estacion ( void )
 *	\brief levanta del puerto 2 el numero de estacion
 *	\param void
 *	\return void
 */

int numero_de_estacion ( void )
{
	U8 dni = 0;
	dni = P2 >> 6; 			//!< así te quedas con el nibble alto en el bajo
    return (HextoInt(dni));
	//return (HextoInt(dni));
}