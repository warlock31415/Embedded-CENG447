#include "lab4.h"



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
	//outputstring("In read\r\n");
	if(atoi(str) == 9){//if(strstr(str," 9")){
		if(PINB & 1<<PINB1)
				outputstring("Pin 9 reads HIGH\r\n");
			else
				outputstring("Pin 9 reads LOW\r\n");
		}
	else if(atoi(str) == 11){//strstr(str," 11")){
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
	if (atoi(str)==8){//strstr(str," 8 ")){
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
	else if(atoi(str)==10){//strstr(str," 10 ")){
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
	outputstring("INVALID STATE. Command structure: READ/(WRITE) PIN_NUM \
		(HIGH/LOW)\r\n");
}


/**
* @details		Converts ASCII to integer. Will not work if two numbers are 
*				seperrated by a space
* @param [in] str The input command string
* @returns int 
*
**/
int atoi(char *str)
{
	int i = 0;
	int num = 0;
	int flag =0;
	while(str[i] != 0){
		if(str[i]>='0' && str[i]<='9'){
		    flag =1;
			num = num*10+(str[i]-'0');
		}
	else{
	    if(flag){
	        return num;
	    }
	}
		i++;
	}
	return num;
}