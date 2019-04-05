/**
 * @file    main.c
 * @author  Pratik Kunkolienker
 * @date    27 March 2019
 * @brief   Program demonstrates using UART to control the input and output 
 *			ports of the Arduino.
 *
 * @details This program was made to run on the ATMEGA328p running at 16MHz. At
 *			 boot, it sends out simplistic menu 
 */


#include <avr/io.h>
#include <util/delay.h>
#include "L298.h"

#define BAUDRATE 9600
#define PRESCALER F_CPU/(BAUDRATE*16UL)-1

int main(){
	driver_init(5);

	circ();

	while(1){
	}

	return 0;
}
