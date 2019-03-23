#include <avr/io.h>
#include <util/delay.h>
#include <string.h>


#define USART_BAUDRATE 9600
#define BAUD_PRESCALE F_CPU/(USART_BAUDRATE*16UL) -1

void init();
void outputstring(char *);
void receviestring(char *);
void get_read_write(char *);
void read(char *);
void error();

int main(void)
{

	char menu[][100] = 
	{
		"\nCOMMAND\t\tPIN_NUM\t\tSTATE\r\n",
		"READ\t\t9/11\t\t-\r\n",
		"WRITE\t\t8/10\t\tHIGH/LOW\r\n"
	};

	char prompt[] = ">>";

	char command[100] = "\0";

	init();
		for(int i=0;i<3;i++)
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

void init(){
	UCSR0B |= 0x18;
	UCSR0C |= 0x06;

	UBRR0L = BAUD_PRESCALE;
	UBRR0H = ((BAUD_PRESCALE)>>8);

	DDRD = 0x0A; // Set pins 8 and 10 to outputs, set 9 and 10 as inputs
}

void outputstring(char *str){
	int i=0;
	while(str[i] != '\0'){
		while((UCSR0A & (1<<UDRE0)) == 0){};
		UDR0 = str[i++];
	}
}

void receviestring(char *str){
	int i=0;
	while((str[i] != '\n') && (str[i-1] != '\r')){
		while((UCSR0A & (1<<RXC0)) == 0){};
		str[i++] = UDR0;
	}
	str[i] = '\0';
}

void get_read_write(char *str){

	if(strstr(str,"READ "))
	{
		read(str);
	}
	else if(strstr(str,"WRITE "))
	{

	}
	else
	{
		error();	
	}
}

void read(char *str){
	outputstring("In read\r\n");
	if(strstr(str," 9"))
		{
			if(PINB & 1<<PINB1)
				outputstring("Pin 9 reads HIGH");
			else
				outputstring("Pin 9 reads LOW");
		}
	else if(strstr(str," 11"))
			if(PINB & 1<<PINB3)
				outputstring("Pin 11 reads HIGH");
			else
				outputstring("Pin 11 reads LOW");
	else
		error();
}


void error()
{
	outputstring("INVALID STATE. Command structure: READ/(WRITE) PIN_NUM (HIGH/LOW)\r\n");
}
