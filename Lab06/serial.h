#ifndef SERIAL_H
#define SERIAL_H
	

	#include <stdio.h>
	#include <avr/io.h>
	#include <util/delay.h>

	#define BAUD 9600
	#define MYUBRR F_CPU/16/BAUD-1 


	void ioinit();
	static void uart_putchar(char c, FILE *stream);
	char uart_getchar(void);


	static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
	static FILE mystdin = FDEV_SETUP_STREAM(NULL,uart_getchar, _FDEV_SETUP_READ);
#endif