/**
 * @file    lab4.h
 * @author  Pratik Kunkolienker
 * @date    21 Jan 2019
 * @brief   The Program demonstrates using UART to control the input and output 
 *			ports of the Arduino.
 *
 * @details This program was made to run on the ATMEGA328p running at 16MHz. 
 */



#ifndef LAB_4_H
#define LAB_4_H

	#include <string.h>
	#include <avr/io.h>
	
	/** @brief	Baudrate for UART **/
	#define USART_BAUDRATE 9600
	
	/** @brief	Baudrate prescaler**/
	#define BAUD_PRESCALE F_CPU/(USART_BAUDRATE*16UL) -1
	void init();
	void outputstring(char *);
	void receviestring(char *);
	void get_read_write(char *);
	void read(char *);
	void error();
	void write(char *);
	int atoi(char *);
#endif