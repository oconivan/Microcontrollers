; EE3376 - Lab 3
; Authors Mohammed Alkhatib and Eric MacDonald
; This Programm controls an 8 bit counter
; R4 stores the local variable that contains the value of the counter.
; Port 1 is used to display the 8-bit result using LEDs
; Port 2 is used as an input using the pushbutton
; Pushbutton 1 (PB1) is used to add 1 and store the result on R4 Register
	.cdecls C,LIST,  "msp430g2553.h"
	.text; Begin Code
BODY
  mov.w   #0280h,SP        ; Initialize stack pointe
  mov.w   #WDTPW + WDTHOLD, &WDTCTL ; Stop WD
  mov.b   #0xFF, &P1DIR            ; all outputs for 8 LED
  mov.b   #0x00, &P2DIR           ; all inputs for push button
  mov.b   #001h, R15 ; mask1 for push button 1
  mov.b   #000h, R4      ; Initial counter value - local variable
  mov.b   #07h,P2OUT
;******************** Main Loop to check the pushbuttons*****************************
infloop
sw1
  cmp.b   &P2IN, R15 ; Check if PB1 is ON - pin P2.0
  jnz     sw2 ; If not, return to main loop
  add.b   #0x01, R4 ; If pushed, increment R4
  mov.b   R4, &P1OUT ; Show the result on LED(s)
  call    #Delay ; Delay for 1 sec
  mov.b   #00h, &P1OUT ; clear LEDs
  jmp     infloop ; Go back to the main loop
sw2
  bit.b   #02h, &P2IN ; Check if PB1 is ON - pin P2.0
  jz     sw3 ; If not, return to main loop
  sub.b   #0x01, R4 ; If pushed, increment R4
  mov.b   R4, &P1OUT ; Show the result on LED(s)
  call    #Delay ; Delay for 1 sec
  mov.b   #00h, &P1OUT ; clear LEDs
  jmp     infloop ; Go back to the main loop
sw3
  bit.b   #04h, &P2IN ; Check if PB1 is ON - pin P2.0
  jz     infloop ; If not, return to main loop
  ;add.b   #0x01, R4 ; If pushed, increment R4
  ;mov.b   R4, &P1OUT ; Show the result on LED(s)
  mov.b		#00h,R4
  ;call    #Delay ; Delay for 1 sec
  mov.b   #00h, &P1OUT ; clear LEDs
  jmp     infloop ; Go back to the main loop

Delay:
	mov.w   #00006h, R11   ; 3 Cycle
loop_a
	mov.w   #0D901h, R10   ; 3 Cycles 6 times  (D901 in hex = 55553 in Decimal)
loop_b
    dec.w   R10            ; 1 Cycle (55553)*6 times
	jnz     loop_b ; 2 Cycles (55553)*6 times
	dec.w   R11 ; 1 Cycle6 times
	jnz     loop_a ; 2 Cycles 6 times
	ret
; **********************************************************************************
        .sect   ".reset"            ; MSP430 RESET Vector
          .short   BODY              ;
          .end
