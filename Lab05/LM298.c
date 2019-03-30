#include "LM298.h"
#include "pin_map.h"

void driver_init(){
	DDRB |= (1<<H_IN2)|(1<<H_IN3)|(1<<H_IN4);
	DDRD |= (1<<H_IN1);
}