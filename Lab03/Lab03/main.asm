;
; Lab03.asm
;
; Created: 3/4/2019 8:49:32 PM
; Author : Pratik Sinai Kunkolienker
;


.EQU F_CPU = 16000000
.EQU BAUDRATE = 9600
.EQU BAUD_PRESCALE = F_CPU/(BAUDRATE*16)-1

.DEF out_buf = r16		;Output buffer
.DEF state = r24		;To check which function we're in

.DSEG 
pass: .BYTE 4



.CSEG
;;		The Menu
menu1: .DB '\r','\n',"******************************************",'\r','\n'
menu2: .DB 9,9,"MENU",'\r','\n'
menu3: .DB "******************************************",'\r','\n'
menu4: .DB "Press"		,9,		"'L'"		,9,		"To lock system",'\r','\n'
menu5: .DB "Press"		,9,		"'U'"		,9,		"To unlock system",'\r','\n'
menu6: .DB "Press"		,9,		"'R'"		,9,		"To set a new security code",'\r','\n'
menu7: .DB "******************************************",'\r','\n',0,0

;; Print if an error occurs
error_string: .DB '\r','\n',"Pressed the wrong button?",'\r','\n',0

;; Print to ask for user input as appropriate
enter_pass: .DB '\r','\n',"Enter Passcode:",0
new_pass: .DB '\r','\n',"Enter new passcode:",0

;; Print when an action is complete
lock_it: .DB '\r','\n',7,9,9,"LOCKED",0
unlock_it: .DB '\r','\n',7,9,9,"UNLOCKED",0
;; 9 is a horizontal tab
;; 7 is Bell, this makes a DING sound in Putty when an action is completed

;; The second 0 is added just to satisfy the assembler. It expects .DB to define an even number
;; of bytes. If a odd number is specifiedthe assembler throughs a warning that it had to pad 
;; the byte to make it even number of bytes
def_pass: .DB "1234"

LDI r18,0xFF	; Set all bits 
OUT DDRB,r18	; Set all pins as output

;;Initialize Stack pointer
Stack:
	PUSH r23
	LDI	r23, low(RAMEND)
	OUT	SPL, r23
	LDI	r23, high(RAMEND)
	OUT	SPH, r23
	POP r23

;;Initialize UART
Uart_init: 
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

/**
* @details				Retrieve default password from flash and store it in SRAM
* @return				void	
**/
Store_def_pass:
	LDI ZH,high(def_pass<<1)		;Load high byte of the def_pass address
	LDI ZL,low(def_pass<<1)			;Load low byte of the def_pass address
		
	LDI YH,high(pass<<1)			;Load high byte of the pass address in SRAM
	LDI YL,low(pass<<1)				;Load low byte of the pass address in SRAM

	LPM r19,Z+						;Load from flash
	ST Y+,r19						;Store to SRAM
	LPM r19,Z+						;Repeat for all 4 bytes
	ST Y+,r19
	LPM r19,Z+
	ST Y+,r19
	LPM r19,Z
	ST Y,r19



;;Lock device on startup
RJMP Lock

;;Main function
/**
* @details				Main loop
* @return				void	
**/
Main:
	RCALL Print_menu	;Print the menu
	RCALL Uart_Rx		;Wait for Input from the menu
	RJMP Get_state		;Parse the input
	done:
	RJMP Main			;loop back


;; Transmit function
/**
* @details				Transmit over UART
* @param[in]  out_buf	A byte of data to transmit over UART
* @return				void	
**/
Uart_Tx:
	LDS r17,UCSR0A		; Load control register
	SBRS r17,UDRE0		; Check for TX complete
		RJMP Uart_Tx	; Try again if it's not
	STS UDR0,out_buf	;Write to UDR0 from R16
	CLR r17
	RET

/**
* @details				Receive over UART
* @param[out]  out_buf	A byte of data received over UART
* @return				void	
**/
Uart_Rx:
	LDS r17,UCSR0A		;Load control register
	SBRS r17, RXC0		;Check if data is received
		RJMP Uart_Rx	;Try again if no data
	LDS out_buf,UDR0    ;Load data from UDR0
	CLR r17				;Clear r17
	RCALL Uart_Tx		;Echo keypress
	RET


/**
* @details					Handles printing the menu
* @param[in]	menu1		The address to the menu
* @param[out]	out_buf		The byte to transmit
* @return					void
**/
Print_menu:
	LDI ZL,low(menu1<<1)		;Load the menu address (Needs to be left shifted by 1 to get to the 
	LDI ZH,high(menu1<<1)		;correct address)
	Send_byte:				
		LPM out_buf,Z+			;Load data till it hits a null character
		CPI out_buf,0x00		;and send it over UART
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
	L:						;case 'L'
		CPI out_buf,0x4C	;if (out_buf == 'L')
		BREQ Lock			;go to the Lock subroutine

	U:						;case 'U'
		CPI out_buf,0x55	;if(out_buf == 'U')
		BREQ Ulock			;go to the Unlock subroutine

	R:						;case 'R'
		CPI out_buf,0x52    ;if(out_buf == 'R')
		BREQ RESET			;go to the Reset subroutine

	Error:							;default
		LDI ZL,low(error_string<<1)	;Load address of the error message
		LDI ZH,high(error_string<<1)
		RCALL Send_byte				;Print error message
		RJMP Main					;Try again
		
/**
* @details				Turns on PORTB pin 1
* @return				void
**/
Lock:	
	ORI state,0x01
	CLR r18					;Clear R18
	OUT PORTB, r18			;Pull all pins LOW
	LDI r18, (1<<0)			;Pull pin 8 on the arduino HIGH
	OUT PORTB,r18
	LDI ZL,low(lock_it<<1)	;Load the lock message address
	LDI ZH,high(lock_it<<1)
	RCALL Send_byte			;Print lock message
	RJMP Main				;Print Menu

/**
* @details				Waits for the passcode. If the correct passcode is pressed
*						unlock by pulling pin 2 on PORTB HIGH
* @param[in]	state	Needs to be cleared to indicate that the safe is unlocked
* @return				void
**/
Ulock:
	CLR state				;Clear state register
	LDI ZL,low(enter_pass<<1)	;Load the enter pass message address
	LDI ZH,high(enter_pass<<1)
	RCALL Send_byte				;Print the enter pass message
	RJMP Get_bytes				;Get pass from user and check if the pass entered is correct
	Ulock_it:
		LDI ZL,low(unlock_it<<1)	;Load unlock message address
		LDI ZH,high(unlock_it<<1)
		RCALL Send_byte			;Print the unlock message
		OUT PORTB, state		;Pull all pins LOW
		LDI r18, (1<<1)			;Pull pin 9 HIGH
		OUT PORTB,r18			
	RJMP Main

/**
* @details				Waits for the passcode. If correct, waits for the new passcode.
*						Stores new passcode in SRAM
* @param[in]	state	Needs to be set to indicate that the safe is being reset
* @return				void
**/
RESET:
	ORI state,0x02				;Set state register to indicate reset mode
	LDI ZL,low(enter_pass<<1)	;Load enter pass message address
	LDI ZH,high(enter_pass<<1)
	RCALL Send_byte				;Print enter pass message

	RJMP Get_bytes				;Check if entered pass is correct
	Reset_it:
		LDI r18,(1<<2)
		OUT PORTB,r18
		RCALL Get_new_pass		;Get the new pass
		CLR r18
		OUT PORTB,r18
	SBRS state,0
		RJMP Ulock_it
	RJMP Lock
	RJMP Main

/**
* @details				Waits for each individual byte moves them into storage registers	
* @return				void
**/
Get_bytes:
	RCALL Uart_Rx				;Wait for first byte of pass
	MOV r19,out_buf				;Store first byte
	RCALL uart_Rx				;Wait for byte 2
	MOV r20,out_buf				;Store byte 2
	RCALL Uart_Rx				;Wait for byte 3
	MOV r21,out_buf				;Store byte 3
	RCALL Uart_Rx				;Wait for byte 4
	MOV r22,out_buf				;Store byte 4

	RJMP Check_pass				;Check if the pass is correct
	RJMP Main

/**
* @details				Checks if the entered pass matches the stored pass
* @params[in]	state	Branch depending on if state is cleared or set
* @return				void
**/
Check_pass:
	LDI ZH, high(pass<<1)		;Load the address of the pass in SRAM
	LDI ZL, low(pass<<1)
	COMP:
		LD r23,Z+				;Load a byte from SRAM as poited to by Z (post incremented)
		CP r23,r19				;Compare the first bytes of the password with the user input
			BRNE Error			;Throw an error if they are not equal
		LD r23,Z+
		CP r23,r20
			BRNE Error
		LD r23,Z+
		CP r23,r21
			BRNE Error
		LD r23,Z
		CP r23,r22
			BRNE Error

		SBRS state,1			;If bit 1 in register is clear
			RJMP Ulock_it		;the unlock subroutine called it
		RJMP Reset_it			;Else, the reset subroutine called it

/**
* @details				Gets the new pass and stores each byte into a register
* @return				void
**/
Get_new_pass:
	LDI ZH, high(new_pass<<1) ;Ask user to enter new pass
	LDI ZL, low(new_pass<<1)
	RCALL Send_byte

	RCALL Uart_Rx			;Process each key input
	MOV r19,out_buf			;Store key inputs is seperate registers
	RCALL uart_Rx
	MOV r20,out_buf
	RCALL Uart_Rx
	MOV r21,out_buf
	RCALL Uart_Rx
	MOV r22,out_buf
		
	RCALL Store_pass			;Store pass is SRAM
	RET

/**
* @details				Store new pass in SRAM
* @param[in]	R19:R22	Bits containing the new passcode
* @return				void
**/
Store_pass:
	LDI YH,high(pass<<1)		;Load address 
	LDI YL,low(pass<<1)

	ST Y+,r19					;Store each byte in SRAM at the address poited to by Y (post increment)
	ST Y+,r20
	ST Y+,r21
	ST Y,r22

	RET
