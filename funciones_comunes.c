/*
 * Funciones comunes agrupadas
 * Las funciones que no se usan no se compilan ni ocupan espacio
 */

/* DEFINES */
#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

#ifndef MS_ANTIREBOTE
#define MS_ANTIREBOTE	20
#endif

#ifndef PULSADO
#error "Hay que declarar si pulsado es 0 o 1"
#else
	#ifndef NO_PULSADO
	#define NO_PULSADO	!PULSADO
	#endif
#endif

#define DATA_EEPROM_SIZE		getenv("DATA_EEPROM")

/* PROTOTIPOS */
void WaitBtn(int pin, short estado);
void WaitBtnPulsado(void);
void WaitBtnNoPulsado(void);

void ParpadearLED(int pin, int veces, long th, long tl);
void ParpadearLED(int veces, long th, long tl);

short ParpadearLEDreturnBtn(int pin_led, int veces, long th, long tl, int pin_btn, short estado);
short ParpadearLEDreturnBtn(int veces, long th, long tl);

#if DATA_EEPROM_SIZE > 0
void erase_eeprom(void);
void fill_eeprom(int val);
#if definedinc(STDOUT)
void print_eeprom(void);
#endif
#endif

int CausaReinicio(void);

#if definedinc(STDOUT)
void CausaReinicio_Serial(int rst);
#endif

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
#ifdef P_BTN
void WaitBtnPulsado(void){
	while(input(P_BTN) == PULSADO){delay_ms(MS_ANTIREBOTE);}
}
#endif

/*
 * Espera a que el pulsador se presione
 * Esta funcion es mas eficiente, pero requiere que el pulsador este declarado como P_BTN
 */
#ifdef P_BTN
void WaitBtnNoPulsado(void){
	while(input(P_BTN) == NO_PULSADO){delay_ms(MS_ANTIREBOTE);}
}
#endif

/*
 * Hace parpadear un LED
 * -pin: pin que se debe hacer parpadear (formato PIN_A0)
 * -num: numero de veces que queremos que el LED parpadee
 * -th: tiempo high que debe permanecer encendido el LED
 * -tl: tiempo low que debe permanecer apagado el LED
 */
void ParpadearLED(int pin, int veces, long th, long tl){
	for(int x=0; x<veces; x++){
		output_high(pin);
		delay_ms(th);
		output_low(pin);
		delay_ms(tl);
	}
}

/*
 * Hace parpadear un LED
 * -veces: numero de veces que queremos que el LED parpadee
 * -th: tiempo high que debe permanecer encendido el LED
 * -tl: tiempo low que debe permanecer apagado el LED
 * Esta funcion es mas eficiente que la anterior, pero requiere que el LED este
 * declarado como P_LED
 */
#ifdef P_LED
void ParpadearLED(int veces, long th, long tl){
	for(int x=0; x<veces; x++){
		output_high(P_LED);
		delay_ms(th);
		output_low(P_LED);
		delay_ms(tl);
	}
}
#endif

/*
 * Parpadea el LED
 * Devuelve TRUE si se presiono el pulsador mientras estaba en esta funcion
 * -pin_led: es el pin del led que queremos hacer parpadear (en formato PIN_A0)
 * -veces: numero de veces que queremos que parpadee el led
 * -th: tiempo que mantiene el LED encendido
 * -tl: tiempo que mantiene el LED apagado
 * -pin_btn: es el pin del pulsador que estamos comprobando (en formato PIN_A0)
 * -estado: es el estado que estamos comprobando
 */
short ParpadearLEDreturnBtn(int pin_led, int veces, long th, long tl, int pin_btn, short estado){
	short P = FALSE;
	int x;
	
	for(x=0;x<veces;x++){
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
 * -veces: numero de veces que queremos que parpadee el led
 * -th: tiempo que mantiene el LED encendido
 * -tl: tiempo que mantiene el LED apagado
 * Esta funcion es mas eficiente que la anterior, pero requiere que el LED este
 * declarado como P_LED y el pulsador como P_BTN
 */
#ifdef P_LED
#ifdef P_BTN
short ParpadearLEDreturnBtn(int veces, long th, long tl){
	short P = FALSE;
	int x;
	
	for(x=0;x<veces;x++){
		if(input(P_BTN) == PULSADO){P = TRUE;}
		output_high(P_LED);
		delay_ms(th);
		if(input(P_BTN) == PULSADO){P = TRUE;}
		output_low(P_LED);
		delay_ms(tl);
	}
	
	return(P);
}
#endif
#endif

//Funciones disponibles para PICs que tengan EEPROM interna
#if DATA_EEPROM_SIZE > 0
/*
 * Borra la EEPROM interna
 */
void erase_eeprom(void){	
	for(long x = 0; x < DATA_EEPROM_SIZE; x++){
		write_eeprom(x, 0xFF);
	}
}

/*
 * Llena la EEPROM interna con un valor
 */
void fill_eeprom(int val){
	for(long x = 0; x < DATA_EEPROM_SIZE; x++){
		write_eeprom(x, val);
	}
}

#if definedinc(STDOUT)
/*
 * Sirve para leer lo que hay en la EEPROM interna y mostrarlo por puerto serie
 */
void print_eeprom(void){
	printf("\r\nDATA EEPROM: %Lu bytes\r\n\r\n", DATA_EEPROM_SIZE);

	//imprimimos cabeceras de columna
	printf("    ");
	for(int x = 0; x < 8; x++){
		printf(" %02u", x);
	}
	printf("\r\n");
	
	//imprimimos valores
	for(long x = 0; x < DATA_EEPROM_SIZE; x++){
		//imprimo cambio de linea en los multiplos de 8
		if(x%8 == 0){
			//imprimo cambio de linea en los multiplos de 64
			if((x%64 == 0) && (x != 0)){
				printf("\r\n");
			}
			
			//imprimimos numero de linea
			printf("\r\n%02LX:  ", x);
		}
		
		printf("%02X ", read_eeprom(x));	//imprimo valor
	}
	
	printf("\r\n\r\n");
}
#endif
#endif

/*
 * Devuelve un valor con la causa del reinicio
 * Sirve para localizar errores y reinicios no contemplados
 */
int CausaReinicio(void){
	int a = 0;
	
	switch(restart_cause()){
		case NORMAL_POWER_UP:
			a = 1;
			break;
		case MCLR_FROM_POWER_UP:
			a = 2;
			break;
		case BROWNOUT_RESTART:
			a = 3;
			break;
		case MCLR_FROM_SLEEP:
			a = 4;
			break;
		case WDT_TIMEOUT:
			a = 5;
			break;
		case WDT_FROM_SLEEP:
			a = 6;
			break;
		case INTERRUPT_FROM_SLEEP:
			a = 7;
			break;
		case MCLR_FROM_RUN:
			a = 8;
			break;
		case RESET_INSTRUCTION:
			a = 9;
			break;
		case STACK_OVERFLOW:
			a = 10;
			break;
		case STACK_UNDERFLOW:
			a = 11;
			break;
	}
	
	return(a);
}

#if definedinc(STDOUT)
/*
 * Imprime por el puerto serie la causa del reinicio
 * Sirve para localizar errores y reinicios no contemplados
 */
void CausaReinicio_Serial(int rst){
	printf("Start: ");

	switch(rst){
		case NORMAL_POWER_UP:
			printf("Normal power up");
			break;
		case MCLR_FROM_POWER_UP:
			printf("MCLR");
			break;
		case BROWNOUT_RESTART:
			printf("Brownout restart");
			break;
		case MCLR_FROM_SLEEP:
			printf("MCLR from sleep");
			break;
		case WDT_TIMEOUT:
			printf("WDT timeout");
			break;
		case WDT_FROM_SLEEP:
			printf("WDT from sleep");
			break;
		case INTERRUPT_FROM_SLEEP:
			printf("Interrupt from sleep");
			break;
		case MCLR_FROM_RUN:
			printf("MCLR from run");
			break;
		case RESET_INSTRUCTION:
			printf("Reset instruction");
			break;
		case STACK_OVERFLOW:
			printf("Stack overflow");
			break;
		case STACK_UNDERFLOW:
			printf("Stack underflow");
			break;
		default:
			printf("Undefined");
	}
	
	printf(" (0x%02X)", rst);
}