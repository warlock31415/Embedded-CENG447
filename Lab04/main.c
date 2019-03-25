/**
 * @file    main.c
 * @author  Pratik Kunkolienker
 * @date    21 Jan 2019
 * @brief   This program is proof that the correct environment was set up to work with the ATMEGA328p
 *
 * @details This program was made to run on the ATMEGA328p running at 16MHz. The program is set to
 *          blink the built-in LED connected to pin 13 at an interval of 1 second. This can be modified
 *          by changing the value of the t variable in the main function. In addition, if a LED is
 *          connected to pin 8 via a current limiting resistor (1k in my case) the LEDs will flash in
 *          an alternating manner.
 */

/** @mainpage
 * @section Introduction
 *          This will serve as documentation for lab 4 for the CENG447: Embedded systems classes for the Spring of 2019.
 *
 *          The <a href="main.c">source file</a> was compiled using avr-gcc and flashed on the ATMEGA328p using the AVRDUDE utility and the arduino
 *          UNO as a programmer.
 *
 * @section Video
 * 			
 * @section PDF document
 *
 * @section Issues
 *          -# Putty
 *            - Putty running on Linux spits out garbage before it is supposed to but works on Windows. Works if a different shell emulator is used
 *          -# Has to be exact
 *            - If an in correct command contains a valid command as a sub string, the program will still work.
 */

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

/** @brief	Baudrate for UART **/
#define USART_BAUDRATE 9600

/** @brief	Baudrate prescaler**/
#define BAUD_PRESCALE F_CPU/(USART_BAUDRATE*16UL) -1

// Function declarations
void init();
void outputstring(char *);
void receviestring(char *);
void get_read_write(char *);
void read(char *);
void error();
void write();

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


/**
* @details  Function initializes UART and sets pins 8 &10 to output
*			and pins 9 & 11 to input. Setting all bits to 0 initializes 
*			the in built pull down resistors.
* @returns  void
**/
void init(){
	UCSR0B |= 0x18;
	UCSR0C |= 0x06;

	UBRR0L = BAUD_PRESCALE;
	UBRR0H = ((BAUD_PRESCALE)>>8);

	DDRD = 0x0A; // Set pins 8 and 10 to outputs, set 9 and 10 as inputs
	PORTB = 0X00; // Initialize pull down
}

/**
* @details  		Writes the passed strings bit by bit untill a NULL
*					character is detected.
* @param [out] str	The input string. Must be null terminated
* @returns  		void
**/
void outputstring(char *str){
	int i=0;
	while(str[i] != '\0'){
		while((UCSR0A & (1<<UDRE0)) == 0){};
		UDR0 = str[i++];
	}
}

/**
* @details  		Reads from UDR0 bit by bit till a line feed
*				 	and a carriage return is detected.
* @param [in] str 	Input command string
* @returns  void
**/
void receviestring(char *str){
	int i=0;
	while((str[i] != '\n') && (str[i-1] != '\r')){
		while((UCSR0A & (1<<RXC0)) == 0){};
		str[i++] = UDR0;
	}
	str[i] = '\0';
}

/**
* @details  		Checks whether the user asked the program
*					to read or write to the pins.
* @param [in] str 	Input command string
* @returns  void
**/
void get_read_write(char *str){

	if(strstr(str,"READ "))
	{
		read(str);
	}
	else if(strstr(str,"WRITE "))
	{
		write(str);
	}
	else
	{
		error();	
	}
}

/**
* @details  		If READ, check the for which pin to read and output
*					approritate string
* @param [in]	str	Input command string
* @returns  		void
**/
void read(char *str){
	outputstring("In read\r\n");
	if(strstr(str," 9")){
			if(PINB & 1<<PINB1)
				outputstring("Pin 9 reads HIGH\r\n");
			else
				outputstring("Pin 9 reads LOW\r\n");
		}
	else if(strstr(str," 11")){
			if(PINB & 1<<PINB3)
				outputstring("Pin 11 reads HIGH\r\n");
			else
				outputstring("Pin 11 reads LOW\r\n");
		}
	else
		error();
}


/**
* @details          If WRITE, check which pin to write to.
*                   Writes to pin as specified (HIGH/LOW)
* @param [in]  str  Input command string
* @returns          void
**/
void write(char *str){
	outputstring("In write\r\n");

	if (strstr(str," 8 ")){
		if (strstr(str,"HIGH")){
			outputstring("Pin 8 is set HIGH\r\n");
			PORTB |= (1<<PINB0);
		}
		else if(strstr(str, "LOW")){
			outputstring("Pin 8 is set LOW\r\n");
			PORTB &= 0b11111110;
		}
		else
			error();
	}
	else if(strstr(str," 10 ")){
		if (strstr(str,"HIGH")){
			outputstring("Pin 10 is set HIGH\r\n");
			PORTB |= (1<<PINB2);
		}
		else if(strstr(str,"LOW")){
			outputstring("Pin 10 is set LOW\r\n");
			PORTB &= 0B11111011;
		}
		else
			error();
	}
	else
		error();
}

/**
* @details  		Is called when an error state is reached.
* @returns  void
**/
void error()
{
	outputstring("INVALID STATE. Command structure: READ/(WRITE) PIN_NUM (HIGH/LOW)\r\n");
}
