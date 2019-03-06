;
; Lab03.asm
;
; Created: 3/4/2019 8:49:32 PM
; Author : 7410983
;


; 

.EQU F_CPU = 16000000
.EQU BAUDRATE = 9600
.EQU BAUD_PRESCALE = F_CPU/(BAUDRATE*16)-1

.DEF out_buf = r16
.DSEG .
pass: .BYTE 4

.CSEG
menu1: .DB '\r','\n',"******************************************",'\r','\n'
menu2: .DB 9,9,"MENU",'\r','\n'
menu3: .DB "******************************************",'\r','\n'
menu4: .DB "Press"		,9,		"'L'"		,9,		"To lock system",'\r','\n'
menu5: .DB "Press"		,9,		"'U'"		,9,		"To unlock system",'\r','\n'
menu6: .DB "Press"		,9,		"'R'"		,9,		"To set a new security code",'\r','\n'
menu7: .DB "******************************************",'\r','\n',0,0

error_string: .DB '\r','\n',"Pressed the wrong button?",'\r','\n',0

enter_pass: .DB '\r','\n',"Enter Passcode:",0
new_pass: .DB '\r','\n',"Enter new passcode:",0

lock_it: .DB '\r','\n',7,9,9,"LOCKED",0
unlock_it: .DB '\r','\n',7,9,9,"UNLOCKED",0

//The second 0 is added just to satisfy the assembler. It expects .DB to define an even number of bytes. If a odd number is specified
//the assembler throughs a warning that it had to pad the byte to make it even number of bytes
def_pass: .DB "1234"

LDI r18,0xFF
STS DDRB,r18

;Initialize Stack pointer
Stack:
	PUSH r23
	LDI	r23, low(RAMEND)
	OUT	SPL, r23
	LDI	r23, high(RAMEND)
	OUT	SPH, r23
	POP r23

Uart_init:;Initialize UART 
	PUSH r22
	PUSH r23
	LDS r22,UCSR0B
	LDI r22,(1<<RXEN0)|(1<<TXEN0) ;Enable Rx and Tx
	STS UCSR0B,r22 

	LDS r22,UCSR0C
	ORI r22,0b00000110
	STS UCSR0C,r22

	LDI r22,high(BAUD_PRESCALE)
	LDI r23,low(BAUD_PRESCALE)
	STS UBRR0H,r22
	STS UBRR0L,r23
	POP r23
	POP r22

RJMP Lock

Main:
	RCALL Print_menu
	RCALL Uart_Rx\
	;LDI out_buf,0x4C
	RJMP Get_state
	done:
	RJMP Main


/**
* @details				Transmit over UART
* @param[in]  out_buf	A byte of data to transmit over UART
* @return				void	
**/
Uart_Tx:
	LDS r17,UCSR0A
	SBRS r17,UDRE0
		RJMP Uart_Tx
	STS UDR0,out_buf ;Write to UDR0 from R16
	CLR r17
	RET

/**
* @details				Receive over UART
* @param[out]  out_buf	A byte of data received over UART
* @return				void	
**/
Uart_Rx:
	LDS r17,UCSR0A
	SBRS r17, RXC0
		RJMP Uart_Rx
	LDS out_buf,UDR0
	CLR r17
	RCALL Uart_Tx
	RET


/**
* @details					Handles printing the menu
* @param[in]	menu1		The address to the menu
* @param[out]	out_buf		The byte to transmit
* @return					void
**/
Print_menu:
	LDI ZL,low(menu1*2)
	LDI ZH,high(menu1*2)
	Send_byte:
		LPM out_buf,Z+
		CPI out_buf,0x00
			BREQ exit
		RCALL Uart_Tx
		RJMP Send_byte
	exit:
		RET

/**
* @details					Redirects program based on input
* @param[in]	out_buf		The address to the menu
* @return					void
**/
Get_state:
	L:
		CPI out_buf,0x4C
		BREQ Lock
	U:
		CPI out_buf,0x55
		BREQ Ulock
	R:
		CPI out_buf,0x52
		BREQ RESET

	Error:
		LDI ZL,low(error_string*2)
		LDI ZH,high(error_string*2)
		RCALL Send_byte
		RJMP Main
		
/**
* @details				Turns on PORTB pin 1
* @return				void
**/
Lock:	
	CLR r18
	OUT PORTB, r18
	LDI r18,0x01
	OUT PORTB,r18
	LDI ZL,low(lock_it*2)
	LDI ZH,high(lock_it*2)
	RCALL Send_byte
	RJMP Main


Ulock:
	CLR r24
	LDI ZL,low(enter_pass*2)
	LDI ZH,high(enter_pass*2)
	RCALL Send_byte
	RJMP Check_bytes
	Ulock_it:
		LDI ZL,low(unlock_it*2)
		LDI ZH,high(unlock_it*2)
		RCALL Send_byte
		OUT PORTB, r24
		LDI r18, 0x02
		OUT PORTB,r18
	RJMP Main


RESET:
	LDI r24,0xFF
	LDI ZL,low(enter_pass*2)
	LDI ZH,high(enter_pass*2)
	RCALL Send_byte
	RJMP Check_bytes
	Reset_it:
		RCALL Get_new_pass
		LDI out_buf,0x07
		RCALL Uart_Tx
	RJMP Main

Check_bytes:
	RCALL Uart_Rx
	MOV r19,out_buf
	RCALL uart_Rx
	MOV r20,out_buf
	RCALL Uart_Rx
	MOV r21,out_buf
	RCALL Uart_Rx
	MOV r22,out_buf
	RJMP Check_pass
	RJMP Main

Check_pass:
	LDI ZH, high(def_pass*2)
	LDI ZL, low(def_pass*2)
	COMP:
		LPM r23,Z+
		CP r23,r19
			BRNE Error
		LPM r23,Z+
		CP r23,r20
			BRNE Error
		LPM r23,Z+
		CP r23,r21
			BRNE Error
		LPM r23,Z
		CP r23,r22
			BRNE Error
		SBRS r24,1
			RJMP Ulock_it
		RJMP Reset_it

Get_new_pass:
	LDI ZH, high(new_pass*2)
	LDI ZL, low(new_pass*2)
	RCALL Send_byte
	RCALL Uart_Rx
	MOV r19,out_buf
	RCALL uart_Rx
	MOV r20,out_buf
	RCALL Uart_Rx
	MOV r21,out_buf
	RCALL Uart_Rx
	MOV r22,out_buf
	RCALL Store_pass
	RET

Store_pass:
	LDI ZH, high(pass)
	LDI ZL, low(pass)
	ST Z+,r19
	ST Z+,r20
	ST Z+,r21
	ST Z, r22
	LDS r19,pass
	RCALL Uart_Tx
	RET

		


