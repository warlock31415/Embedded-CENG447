#include <avr/io.h>
#include "serial.h"
#include "servo.h"
#include "ultrasonic.h"
#include "L298.h"
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>

#define set_point 15

#define kp 2

#define kd 1

#define ki 0

volatile int timer=0;


void init();
int get_distance();
void BTcontrol();
void pid();

enum boolean {left,right};

void start();
void first_bend();
void second_bend(enum boolean side);
void third_bend(enum boolean side);





int main(){
	init();
	char prev_control = NULL;
	int pwm;
	uint8_t error=0;

	forward(50);
	_delay_ms(200);
	forward(0);
	start();
	while(1){


	}
	return 1;
}


void init(){
	motor_init(5);
	ioinit();
	servo_init();
	distance_init();

	//Enable timer interrupt
	TIMSK0 |= (1<<TOIE0);

}

int get_distance(){
return distance_trigger();
}



void BTcontrol(){
	forward(0);
	char control = NULL;
	scanf("%c", &control);
	BTcontrol(control);
	while(1){
	switch(control){
		case 'w' :
			forward(50);
			break;
		case  's':
			back(50);
			break;
		case 'a':
			turn_left(80);
			break;
		case 'd':
			turn_right(80);
			break;
		default:
			forward(0);
			break;
		}
	}
}

void start(){
	servo_position(90);
	_delay_ms(1000);
	pid();

	first_bend();
	
}

void first_bend(){
	enum boolean side;

	// Get distance on the right
	servo_position(0);
	_delay_ms(300);
	int right_distance = get_distance();
	// Get distance on the left
	servo_position(180);
	_delay_ms(300);
	int left_distance = get_distance();

	servo_position(90);
	_delay_ms(300);

	if(left_distance-right_distance>0)
	{	
		turn_left(100);
		_delay_ms(350);
		side = left;
	}
	else 
	{
		turn_right(100);
		_delay_ms(500);
		side = right;
	}
	forward(0);

	pid();
	second_bend(side);
}

void second_bend(enum boolean side){
	if (side == left){
		turn_right(100);
		_delay_ms(350);
	}
	else {
		turn_left(100);
		_delay_ms(300);
	}
	pid();
	third_bend(side);
}

void third_bend(enum boolean side){
	if (side == left){
		turn_right(85);
		servo_position(180);
		_delay_ms(500);
	}
	else {
		turn_left(85);
		servo_position(0);
		_delay_ms(500);
	}
	forward(50);
	while(get_distance()<60);
	_delay_ms(500); 
	forward(0);


	PCICR |= (1<<PCIE2);
	PCMSK2 |= (1<<PCINT20); 

	//while(1);
	if(side == left)
	{
		turn_left(90);
		_delay_ms(350);
	}
	else
	{
		turn_right(90);
		_delay_ms(350);	
	}

	forward(80);
	_delay_ms(500);
	motor_init(0);
	//Add  code here

}

void pid()
{
	unsigned short integral =0;
	unsigned short derivative =0;
	unsigned short last_error =0;
	int pwm;
	int current_position = get_distance();
	int error = -(set_point-current_position);
	while(abs(error)>4 ){
		current_position = get_distance();
		error = current_position-set_point;

		integral = integral+error;

		derivative = error - last_error;

		pwm = (kp*error)+(ki*integral)+(kd*derivative);
		printf("pwm=%d -> distance = %d, error=%d\n",pwm,current_position,error);

		if(pwm>100){
		 pwm = 100;
		}
		else if (pwm<-100)
		{
			 pwm = -100;
		}
		///printf("pwm=%d -> distance = %d\n",pwm,current_position);

		if(pwm>0){
			forward(pwm);
		}
		else if (pwm<0){
			back(pwm);
			_delay_ms(100);
			back(0);

		}
		else motor_init(0);

		last_error = error;
		_delay_ms(250);
	}
}

ISR(TIMER0_OVF_vect)
{	
	cli();
	timer++;
	if (timer>=1838){
		BTcontrol();
	}
	sei();
}

ISR(PCINT2_vect){
	cli();
	BTcontrol();
}