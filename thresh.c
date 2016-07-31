/*
 * thresh.c
 *
 * Created: 17/05/2016 18:33:58
 * Author : Simone Dassi
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/ or send a letter 
 * to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define F_CPU 1000000

int main(void)
{
    /* Replace with your application code */
	
	volatile uint8_t lightlevel = 0;
	volatile uint8_t r;
	volatile uint8_t oldlightlevel = 0;
	volatile uint8_t _adsc = 1;
	
	TCCR0A = (1<<WGM00)|(1<<WGM01)|(1<<COM0A1)|(0<<COM0A0); // fast pwm, OC0A pin
	TCCR0B = (1<<CS00); // no prescaling
	OCR0A = 0; //duty cycle
	DDRB = (1<<DDB0); //pin 0 output
	
	ADMUX = (1<<MUX1)|(1<<ADLAR); //pin 2 adc input, left align
	ADCSRA = (1<<ADEN)|(1<<ADSC); // turn on and start conversion
	
	while(_adsc){
		_adsc = (ADCSRA & 0b01000000); //wait for conversion to end
	}
	
	srand(ADCH);
	ADCSRA &= 0x00; //turn adc off
	
    while (1) 
    {
		r = (uint8_t) rand();
		lightlevel = 100 + r*155/255;
		//lightlevel = (uint8_t)(r/RAND_MAX*255);
		for (uint8_t i=oldlightlevel; oldlightlevel<lightlevel ? i<lightlevel  : i>lightlevel; oldlightlevel<lightlevel ? i++ : i--) //good luck understanding this
		{
			OCR0A = i;
			_delay_ms(2.8);
		}
		oldlightlevel = lightlevel;
	}
}
