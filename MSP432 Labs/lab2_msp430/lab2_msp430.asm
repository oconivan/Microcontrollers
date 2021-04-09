;******************************************************************************
;   MSP430G2xx3 - Software Toggle P1.0
;
;   Description: Toggle P1.0 by xor'ing P1.0
;   ACLK = n/a, MCLK = SMCLK = default DCO ~800kHz
;
;                MSP430G2xx3
;             -----------------
;         /|\|              XIN|-
;          | |                 |
;          --|RST          XOUT|-
;            |                 |
;            |             P1.0|-->LED
;
;   D. Dang
;   Texas Instruments Inc.
;   December 2010
;   Built with Code Composer Essentials Version: 4.2.0
;******************************************************************************
	.cdecls C,LIST,  "msp430g2553.h"
;-------------------------------------------------------------
            .text                           ; Progam Start
;--------------------------------------------------------------
RESET       mov.w   #0280h,SP               ; stack
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop WDT
			mov.b	#0x00,P1OUT
			mov.b	#0x40,P1DIR
			mov.b	#0x08,P1REN

			mov.b	#0x08,P1OUT
main		bit.b	#0x08,P1IN
			jz		LED
			bic.b	#0xF7,P1OUT
			jmp		main
LED			bis.b	#0x40,P1OUT
			jmp		main
;--------------------------------------------------------------
;           Interrupt Vectors
;--------------------------------------------------------------
            .sect   ".reset"     ; MSP430 RESET Vector
            .short  RESET        ;
            .end
