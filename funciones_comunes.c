/*
 * Funciones comunes agrupadas
 */

/* DEFINES */
#ifndef MS_ANTIREBOTE
#define MS_ANTIREBOTE	20
#endif

#ifndef PULSADO
#ERROR "Hay que declara si pulsado es 0 o 1"
#else
	#ifndef NO_PULSADO
	#define NO_PULSADO	!PULSADO
	#endif
#endif

/* PROTOTIPOS */
void WaitBtn(int pin, short estado);
void WaitBtnPulsado(void);
void WaitBtnNoPulsado(void);

void ParpadearLED(int pin, int num, long th, long tl);
void ParpadearLED(int num, long th, long tl);

short ParpadearLEDreturnBtn(int pin_led, int num, long th, long tl, int pin_btn, short estado);
short ParpadearLEDreturnBtn(int num, long th, long tl);

/* FUNCIONES */

/*
 * Espera a que el pulsador se presione o se suelte
 * -pin: es el pin del pulsador que estamos comprobando (en formato PIN_A0)
 * -estado: es el estado que estamos comprobando
 * 
 * ej: si se ha pulsado un pulsador con pull up y queremos esperar a que se suelte:
 * WaitBtn(PIN_B3, PULSADO)
 */
void WaitBtn(int pin, short estado){
	while(input(pin) == estado){delay_ms(MS_ANTIREBOTE);}
}

/*
 * Espera a que el pulsador se suelte
 * Esta funcion es mas eficiente, pero requiere que el pulsador este declarado como P_BTN
 */
void WaitBtnPulsado(void){
	while(input(P_BTN) == PULSADO){delay_ms(MS_ANTIREBOTE);}
}

/*
 * Espera a que el pulsador se presione
 * Esta funcion es mas eficiente, pero requiere que el pulsador este declarado como P_BTN
 */
void WaitBtnNoPulsado(void){
	while(input(P_BTN) == NO_PULSADO){delay_ms(MS_ANTIREBOTE);}
}

/*
 * Hace parpadear un LED
 * -pin: pin que se debe hacer parpadear (formato PIN_A0)
 * -num: numero de veces que queremos que el LED parpadee
 * -th: tiempo high que debe permanecer encendido el LED
 * -tl: tiempo low que debe permanecer apagado el LED
 */
void ParpadearLED(int pin, int num, long th, long tl){
#warning "Se ha cambiado el orden de los parametros!"
	for(int x=0; x<num; x++){
		output_high(pin);
		delay_ms(th);
		output_low(pin);
		delay_ms(tl);
	}
}

/*
 * Hace parpadear un LED
 * -num: numero de veces que queremos que el LED parpadee
 * -th: tiempo high que debe permanecer encendido el LED
 * -tl: tiempo low que debe permanecer apagado el LED
 * Esta funcion es mas eficiente que la anterior, pero requiere que el LED este
 * declarado como P_LED
 */
void ParpadearLED(int num, long th, long tl){
	for(int x=0; x<num; x++){
		output_high(P_LED);
		delay_ms(th);
		output_low(P_LED);
		delay_ms(tl);
	}
}

/*
 * Parpadea el LED
 * Devuelve TRUE si se presiono el pulsador mientras estaba en esta funcion
 * -pin_led: es el pin del led que queremos hacer parpadear (en formato PIN_A0)
 * -num: numero de veces que queremos que parpadee el led
 * -th: tiempo que mantiene el LED encendido
 * -tl: tiempo que mantiene el LED apagado
 * -pin_btn: es el pin del pulsador que estamos comprobando (en formato PIN_A0)
 * -estado: es el estado que estamos comprobando
 */
short ParpadearLEDreturnBtn(int pin_led, int num, long th, long tl, int pin_btn, short estado){
	short P = FALSE;
	int x;
	
	for(x=0;x<num;x++){
		if(input(pin_btn) == estado){P = TRUE;}
		output_high(pin_led);
		delay_ms(th);
		if(input(pin_btn) == estado){P = TRUE;}
		output_low(pin_led);
		delay_ms(tl);
	}
	
	return(P);
}

/*
 * Parpadea el LED
 * Devuelve TRUE si se presiono el pulsador mientras estaba en esta funcion
 * -num: numero de veces que queremos que parpadee el led
 * -th: tiempo que mantiene el LED encendido
 * -tl: tiempo que mantiene el LED apagado
 * Esta funcion es mas eficiente que la anterior, pero requiere que el LED este
 * declarado como P_LED y el pulsador como P_BTN
 */
short ParpadearLEDreturnBtn(int num, long th, long tl){
	short P = FALSE;
	int x;
	
	for(x=0;x<num;x++){
		if(input(P_BTN) == PULSADO){P = TRUE;}
		output_high(P_LED);
		delay_ms(th);
		if(input(P_BTN) == PULSADO){P = TRUE;}
		output_low(P_LED);
		delay_ms(tl);
	}
	
	return(P);
}