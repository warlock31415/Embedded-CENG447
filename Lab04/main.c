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

/** @mainpage
 * @section Introduction
 *          This will serve as documentation for lab 4 for the CENG447: Embedded
 *			systems classes for the Spring of 2019.
 *
 *          The <a href="https://github.com/warlock31415/Embedded-CENG447/blob/
 *			master/Lab04/main.c">source file</a> was compiled using avr-gcc and 
 *			flashed on the ATMEGA328p using the AVRDUDE utility and the arduino 
 *			UNO as a programmer.
 *
 * @section Video
 * Please follow <a href="https://youtu.be/2wUh4nnQhK4">this link</a>
 * 			
 * @section PDF document
 * Download the PDF by clicking<a href="./Lab4.pdf"> here</a>
 *
 * @section Issues
 *          -# Putty
 *            - Putty running on Linux spits out garbage before it is supposed 
 *				to but works on Windows. Works if a different shell emulator is 
 *				used
 *          -# Has to be exact
 *            - If an in correct command contains a valid command as a sub 
 *				string, the program will still work.
 */
#include "lab4.h"

/**
* @details  This is the main function. It initializes the menu
* 			and the prompt charater. Further, it contains a infinite
*			while loop so that the program runs over and over again.
* @returns  void
**/
int main(void)
{

	char menu[][100] = 
	{
		"\nCOMMAND\t\tPIN_NUM\t\tSTATE\r\n",
		"**************************************\r\n",
		"READ\t\t9/11\t\t-\r\n",
		"WRITE\t\t8/10\t\tHIGH/LOW\r\n",
		"**************************************\r\n"
	};

	char prompt[] = ">>";

	char command[100] = "\0";

	init();
		for(int i=0;i<5;i++)
		{
			outputstring(menu[i]);
		}	
		
	while(1)
	{
		outputstring(prompt);
		receviestring(command);

		get_read_write(command);


		//outputstring(command);
	}
	return 1;
}