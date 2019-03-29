#include "uart.h"
static void wait_for_transmit();
static void wait_for_receive();

/**
* @details  Function initializes UART and sets pins 8 &10 to output
*			and pins 9 & 11 to input. Setting all bits to 0 initializes 
*			the in built pull down resistors.
* @returns  void
**/

inline void UART_init(){
	UCSR0B |= 0x18;
	UCSR0C |= 0x06;

	UBRR0L  = PRESCALER;
	UBRR0H = ((PRESCALER)>>8);
}

/**
* @details  		Writes the passed strings bit by bit untill a NULL
*					character is detected.
* @param [out] str	The input string. Must be null terminated
* @returns  		void
**/

void sendstring(char *str){
	int i=0;
	while(str[i] != '\0'){
		wait_for_transmit();
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
		wait_for_receive();
		str[i++] = UDR0;
	}
	str[i] = '\0';
}

void sendbyte(char data)
{
	wait_for_transmit();
	UDR0 = data;
}

char receivebyte(){
	wait_for_receive();
	return UDR0;
}

static void wait_for_transmit(){
	while((UCSR0A & (1<<UDRE0))==0){}

}

static void wait_for_receive(){
	while((UCSR0A & (1<<RXC0)) == 0){};

}
