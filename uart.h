#ifndef UART_H
#define UART_H

	#define BAUDRATE 9600
	#define PRESCALER F_CPU/(BAUDRATE*16UL)-1

	#include <avr/io.h>

	void UART_init();
	void sendstring(char *str);
	void receviestring(char *str);
	void sendbyte(char data);
	char receivebyte();


#endif