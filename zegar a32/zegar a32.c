/*
 * zegar_a32.c
 *
 * Created: 2015-04-01 01:13:49
 *  Author: Michal
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "hd44780.h"
#include <util/delay.h>




int main(void)
{
	DDRB |= 0x08; //wyjscie PB3
	DDRC = 0x0f;


		
	ADMUX |=(1<<REFS0); //NAP ODN VCC
	ADCSRA |= (1<<ADEN) | (1<<ADPS1) | (1<<ADPS0); //WLACZENIE ADC; PRESKALER 8	
	
	TCCR0 |= 0x48; //fast PWM
	TCCR0 |= 0x20; // star wys,

	
	
	TCCR0 |= 0x03; // preskaler 64
	OCR0 = 0;
	double procpt, proclp, proclpt, predk;
	double pt, lp, lpt;
	double naskr;//nastawa skretu
	double pzlp, pzlpt, pzpt; //punkt zero lewo prawo

	lcd_init();
	lcd_cls();
	lcd_locate(0,0);
	
	ADMUX |= 0x07; // ADC7
	ADCSRA |= (1<<ADSC); //uruchomienie pomiaru adc
	while(ADCSRA & (1<<ADSC));
	pzpt=(ADCW*25)/255;
	
	ADMUX ^= 0x01; // ADC6
	ADCSRA |= (1<<ADSC); //uruchomienie pomiaru adc
	while(ADCSRA & (1<<ADSC));
	pzlp = (ADCW*25)/255;
	
	ADMUX ^= 0x03; // ADC5
	ADCSRA |= (1<<ADSC); //uruchomienie pomiaru adc
	while(ADCSRA & (1<<ADSC));
	pzlpt = (ADCW*25)/255;

	
	while(1)
	{
				lcd_locate(3,0);
				lcd_int(pzlp);
				lcd_str("lp ");
				lcd_int(pzlpt);
				lcd_str("lpt ");
				lcd_int(pzpt);
				lcd_str("pt ");								
		
		ADMUX |= 0x07; // ADC7
		ADCSRA |= (1<<ADSC); //uruchomienie pomiaru adc
		while(ADCSRA & (1<<ADSC));
		pt=ADCW;
		
		ADMUX ^= 0x01; // ADC6 
		ADCSRA |= (1<<ADSC); //uruchomienie pomiaru adc
		while(ADCSRA & (1<<ADSC));
		lp = ADCW;
		
		ADMUX ^= 0x03; // ADC5
		ADCSRA |= (1<<ADSC); //uruchomienie pomiaru adc
		while(ADCSRA & (1<<ADSC));
		lpt = ADCW;
		
		proclpt = (lpt*25)/255;
		lcd_locate(0,0);
		lcd_int(proclpt);
		lcd_str(" lewo/prawo t");
				
		procpt = (pt*25)/255;
		lcd_locate(1,0);
		lcd_int(procpt);
		lcd_str(" przod/tyl   ");

		proclp = (lp*25)/255;		
		lcd_locate(2,0);
		lcd_int(proclp);		
		lcd_str(" lewo / prawo ");
		
				naskr = (12*(proclp-pzlp))/11;
				
				if (naskr>(proclpt-pzlpt))
				{
					PORTC |= 0x01;
					PORTC = ~0x02;
					OCR0 = 255;
				}
				
				if (naskr<(proclpt-pzlpt)+1)
				{
					PORTC |= 0x02;
					PORTC = ~0x01;
					OCR0 = 255;					
				}
				if ((proclpt-pzlpt) >naskr-1 & (proclpt-pzlpt)<naskr +1)
				{OCR0=0;	}

		
		/*if (procpt >= 30) //przod
		{	
			predk = ((pzpt - procpt)*255)/10;
			OCR0 = predk;
			PORTC |= 0x04;
			PORTC = ~0x05;  
		/	
		if (procpt<=26) //tyl
		{
			predk = ((28-procpt)*255)/18;
			if (predk>239){OCR2=255;}else {OCR2=predk;}
			PORTC |= 0x05;
			PORTC = ~0x04;
			
		}
		if (procpt <30 & procpt > 26) //stop
		{
			OCR2 = 0;
		}*/
		
	
		
	}
			
		

}