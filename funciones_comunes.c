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

/* FUNCIONES */

/*
 * Espera a que el pulsador se suelte
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
 * Hace parpadear un LED
 * -th: tiempo high que debe permanecer encendido el LED
 * -tl: tiempo low que debe permanecer apagado el LED
 * -num: numero de veces que queremos que el LED parpadee
 * -pin: pin que se debe hacer parpadear (formato PIN_A0)
 */
void ParpadearLED(int th, int tl, int num, int pin){
	for(int x=0; x<num; x++){
		output_high(pin);
		delay_ms(th);
		output_low(pin);
		delay_ms(tl);
	}
}