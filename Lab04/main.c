/*
 * Lab04.c
 *
 * Created: 3/22/2019 6:19:36 PM
 * Author : 7410983
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>


#define BAUD 9600
#define BAUD_PRESCALE F_CPU/(BAUD*16UL)-1


void init();

void transmit(char str[]);
void receive();


int main(void)
{
	char command_prompt[] = ">> ";

	char menu[][2] = {"Hello world","Hello world"};
	
	char command[200] = "";
	init();
    //while (1) 
    //{
		transmit(command_prompt);
		receive(command);
		transmit("receive complete");
		
		
    //}
	while(1);
	return 1;
}

void init(){
	DDRB |= 0xFF;
	PORTB = 0x00;
	
	UCSR0B |= (1<<TXEN0 | 1<<RXEN0); // Enable RX and TX
	
	UCSR0C |= 0x06;					// 8-bit, async, no parity, 1 stop bit
	
	//Set baud rate to 9600
	UBRR0L = BAUD_PRESCALE;
	UBRR0H = ((BAUD_PRESCALE)>>8);
	}
	
void transmit(char str[]){
	
	for(int i = 0; str[i] != '\0';i++){
		while((UCSR0A) &(1<<UDRE0)){
			UDR0 = str[i];
		}
	}
	
}

void receive(char command[]){
	while((UCSR0A) & (1<<RXC0)){
		command[0] = UDR0;
	}
	
	for(int i= 1;command[i] != '\n';i++){
		while(UCSR0A & (1<<RXC0)){
			command[i] = UDR0;
		}
	}
}
