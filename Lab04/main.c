#include <avr/io.h>
#include <util/delay.h>

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE F_CPU/(USART_BAUDRATE*16UL) -1

void init();
void outputstring(char *);
void receviestring(char *);

int main(void)
{
	char menu[][100] = 
	{
		"COMMAND\t\tPIN_NUM\t\tSTATE\r\n",
		"READ\t\t9/11\t\t-\r\n",
		"WRITE\t\t8/10\t\tHIGH/LOW\r\n"
	};

	char command[100] = {'\0'};

	init();
		while(1)
	{
		for(int i=0;i<3;i++)
		{
			outputstring(menu[i]);
		}
		//delay_ms(1000);		
		receviestring(command);
		outputstring("\r\n")
		outputstring(command);
	}
	return 1;
}

void init(){
	UCSR0B |= 0x18;
	UCSR0C |= 0x06;

	UBRR0L = BAUD_PRESCALE;
	UBRR0H = ((BAUD_PRESCALE)>>8);
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
}