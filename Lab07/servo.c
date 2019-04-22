#include "servo.h"
#include "pin_map.h"
#include<avr/interrupt.h>

static uint8_t map(uint8_t angle);

void servo_init(){
	DDRB = 0xFF;
	PORTB = 0xFF;
	DDRD = 0xFF;
	sei();

	TCCR2A = 0xA1;
	TCCR2B = 0x0F;

	TCNT2=0;

	OCR2A = 156;
	OCR2B =16;
}

void servo_position(uint8_t angle){
	if (angle<0) angle =0;
	if (angle>180) angle =180;
	OCR2B = map(angle);

}

static uint8_t map(uint8_t angle){
	double servo_angle = (angle)*(SERVO_MAX-SERVO_MIN)/180+SERVO_MIN;
	if (servo_angle-(int)servo_angle>=0.5){
		servo_angle = servo_angle+0.5;
	}
	return (int) servo_angle;
}