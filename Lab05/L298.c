#include "L298.h"
#include "pin_map.h"
#include <avr/io.h>
#include <util/delay.h>

//static char map(int duty_cyc);

/**
* @details 0 - No clock source (timer stopped)
*		   1 - No prescaler
*		   2 - clk/8
*		   3 - clk/64
*		   4 - clk/256
*		   5 - clk/1024
*		   6 - Ext clk on T0 pin. Clock on falling edge
*		   7 - Ext clk on T0 pin. Clock on rising edge
*/

void L298::init(char clk){
	TCCR0A |= (1<<COM0A1); //Clear OC0A on compare match
	TCCR0A |= (1<<COM0B1); //Clear oC0B on compare match

	TCCR0A |= ((1<<WGM01)|(1<<WGM00)); // Fast PWM mode 3

	TCCR0B |= clk;//(1<<CS02)|(0<<CS01)|(0<<CS00); 

	OCR0A = map(0);
	OCR0B = OCR0A;

	DDRD |= (1<<H_A_EN);
	DDRD |= (1<<H_B_EN);

	TCNT0 = 0;


}

 char L298::map(int duty_cyc)
{
	return duty_cyc*255/100;
}

void L298::turn_right(int speed){

	PORTD |= (1<<H_IN1);
	PORTB &= ~(1<<H_IN2); 
	PORTB |= (1<<H_IN3);
	PORTB &= ~(1<<H_IN4);

	OCR0A = map(speed);
	OCR0B = OCR0A;
}

void L298::forward(int speed){
	PORTD |= (1<<H_IN1);
	PORTB &= ~(1<<H_IN2); 
	PORTB |= (1<<H_IN4);
	PORTB &= ~(1<<H_IN3);

	OCR0A = map(speed);
	OCR0B = OCR0A;
}

void  L298::turn_left(int speed){
	PORTD &= ~(1<<H_IN1);
	PORTB |= (1<<H_IN2); 
	PORTB |= (1<<H_IN4);
	PORTB &= ~(1<<H_IN3);

	OCR0A = map(speed);
	OCR0B = OCR0A;
}

void L298::back(int speed){
	PORTD &= ~(1<<H_IN1);
	PORTB |= (1<<H_IN2); 
	PORTB &= ~(1<<H_IN4);
	PORTB |= (1<<H_IN3);

	OCR0A = map(speed);
	OCR0B = OCR0A;
}

void L298::square_turn(int speed){
	forward(speed);
	_delay_ms(3000);
	turn_right(speed);
	_delay_ms(1000);
	forward(speed);
	_delay_ms(3000);
	turn_right(speed);
	_delay_ms(1000);
	forward(speed);
	_delay_ms(3000);
	turn_right(speed);
	_delay_ms(1000);
	forward(speed);
	_delay_ms(3000);
	init(0);

}

void L298::circ()
{
	PORTD |= (1<<H_IN1);
	PORTB &= ~(1<<H_IN2); 
	PORTB |= (1<<H_IN4);
	PORTB &= ~(1<<H_IN3); 

	OCR0A = map(100); 
	OCR0B = map(30);
}