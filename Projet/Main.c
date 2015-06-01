/*
; © 2005  Microchip Technology Inc.
;
; Microchip Technology Inc. (“Microchip”) licenses this software to you
; solely for use with Microchip dsPIC® digital signal controller
; products.  The software is owned by Microchip and is protected under
; applicable copyright laws.  All rights reserved.
;
; SOFTWARE IS PROVIDED “AS IS.”  MICROCHIP EXPRESSLY DISCLAIMS ANY
; WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT
; LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
; PARTICULAR PURPOSE, OR NON-INFRINGEMENT. IN NO EVENT SHALL MICROCHIP
; BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL
; DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT, COST OF
; PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
; BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
; ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS.
;
;
;FILE DESCRIPTION:
;  Please read the file named "README.txt", provided in this folder.
;  Please see detailed comments in this file, alongside source code.
;  Application code execution starts in this file.
;
;REVISION HISTORY:
;  $Log: Main.c,v $
;  Revision 1.1.1.1  2005/06/06 09:16:45  VasukiH
;  First release of software
;
;
*/

//Pre-Processor Directives:
//Provide pre-processor directives to include Device header files and
//any application specific header files. Path locations to the Device
//header files are set up within the MPLAB Project>>BuildOptions>>Project
//dialog box. The path should point to X:\\MPLAB C30\support\h\, where
//"X:" is the folder where the MPLAB C30 tools are installed.
#include <p30fxxxx.h>   //"p30fxxxx.h" is a generic header file for dsPIC30F
                        //devices. This file will in turn select the actual
                        //device header file, based on the device selected
                        //by the MPLAB workspace/project. Using the p30fxxxx.h
                        //file makes it easy to migrate a project from one
                        //dsPIC device to another.
#include "system.h"     //"system.h" is a header file defined for this demo
                        //application.
#include "can.h"
//Macros for Configuration Fuse Registers:
//Invoke macros to set up  device configuration fuse registers.
//The fuses will select the oscillator source, power-up timers, watch-dog
//timers, BOR characteristics etc. The macros are defined within the device
//header files. The configuration fuse registers reside in Flash memory.
_FOSC(CSW_FSCM_OFF & XT_PLL8);  //Run this project using an external crystal
                                //routed via the PLL in 8x multiplier mode
                                //For the 7.3728 MHz crystal we will derive a
                                //throughput of 7.3728e+6*8/4 = 14.74 MIPS(Fcy)
                                //,~67nanoseconds instruction cycle time(Tcy).
_FWDT(WDT_OFF);                 //Turn off the Watch-Dog Timer.
_FBORPOR(MCLR_EN & PWRT_OFF);   //Enable MCLR reset pin and turn off the
                                //power-up timers.
_FGS(CODE_PROT_OFF);            //Disable Code Protection

//Declaration to Link External Functions & Variables:
//Declare functions being used that are defined outside this file, or
//elsewhere in this MPLAB project.
/*
extern SPI_Init(void);
extern UpdateDisplayBuffer(void);
extern WriteUART_to_RS232(void);
extern WriteSPI_to_LCD(void);
extern UART_Init(void);
extern ADC_Init(void);
extern INTx_Init(void);
extern Timer1_Init(void);
extern Timer2_Init(void);
*/


//Functions and Variables with Global Scope:
//Declare functions in this file that have global scope.
int main (void);

//Code execution automatically reaches the main() function after
//two events have occurred;
//1. A Reset event triggered by hardware or software
//2. The execution of the C Start up library functions, present
//   in the crt0.o file in the libpic30-coff.a library file
int main (void)
{

		
        ADPCFG = 0xFFFF;        //After reset all port pins multiplexed
                                //with the A/D converter are configred analog.
                                //We will reconfigure them to be digital
                                //by writing all 1's to the ADPCFG register.
                                //Note: All dsPIC registers are memory mapped.
                                //The address of ADPCFG and other registers
                                //are defined in the device linker script
                                //in your project.

        //Function Delay5ms() available in file, Delay.s
        //Delay5ms(200);          //Provide 500ms delay for the LCD to start-up.

        //Function INTx_IO_Init() available in file, INTx_IO_pins.c
        INTx_IO_Init();         //Initialize the External interrupt pins and
                                //some I/O pins to accept input from the
                                //switches, S5 and S6 and drive the LEDs, D3
                                //and D4.

        //Function Timer1_Init() & Timer2_Init() available in file, Timers.c
        //Timer1_Init();          //Initialize Timer1 to provide "blinking" time
                                //for the LEDs.
        //Timer2_Init();          //Initialize Timer2 and Timer3 as a 32-bit
                                //Timer to be used for updating data sent to
                                //the LCD(SPI) and COM(UART) interfaces.
		
	
		init_CAN();

		LATBbits.LATB1 = ~LATBbits.LATB1; //toggling de la led
		LATBbits.LATB0 = ~LATBbits.LATB0; //toggling de la led

		CAN_frame* trame;

		while(1){
			//lecture_CAN(trame);
		}

        return 0;               //Code never reaches here!
}

/*
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                                                    ;
;                       Software License Agreement                   ;
;                                                                    ;
;   The software supplied herewith by Microchip Technology           ;
;   Incorporated (the "Company") for its dsPIC controller            ;
;   is intended and supplied to you, the Company's customer,         ;
;   for use solely and exclusively on Microchip dsPIC                ;
;   products. The software is owned by the Company and/or its        ;
;   supplier, and is protected under applicable copyright laws. All  ;
;   rights are reserved. Any use in violation of the foregoing       ;
;   restrictions may subject the user to criminal sanctions under    ;
;   applicable laws, as well as to civil liability for the breach of ;
;   the terms and conditions of this license.                        ;
;                                                                    ;
;   THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION.  NO           ;
;   WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING,    ;
;   BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND    ;
;   FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE     ;
;   COMPANY SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,  ;
;   INCIDENTAL OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.  ;
;                                                                    ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
*/

