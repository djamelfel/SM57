/**********************************************************************/
/*                                                                    */
/* File name: tsk_task4.c                                             */
/*                                                                    */
/* Since:     2005-April-18                                           */
/*                                                                    */
/* Version:   PICos v1.05 - RTOS for PIC24/30/33 families.            */
/*            Copyright (C) 2005-2006 Pragmatec.                      */
/*                                                                    */
/* Author:    Designed by Pragmatec S.A.R.L.        www.pragmatec.net */
/*            MONTAGNE Xavier [XM]      xavier.montagne@pragmatec.net */
/*                                                                    */
/* Purpose:   Fifth task of the tutorial.                             */
/*                                                                    */
/* Distribution: This file is part of PICos.                          */
/*            PICos is free software; you can redistribute it         */
/*            and/or modify it under the terms of the GNU General     */
/*            Public License as published by the Free Software        */
/*            Foundation; either version 2, or (at your option)       */
/*            any later version.                                      */
/*                                                                    */
/*            PICos is distributed in the hope that it will be        */
/*            useful, but WITHOUT ANY WARRANTY; without even the      */
/*            implied warranty of MERCHANTABILITY or FITNESS FOR A    */
/*            PARTICULAR PURPOSE.  See the GNU General Public         */
/*            License for more details.                               */
/*                                                                    */
/*            You should have received a copy of the GNU General      */
/*            Public License along with gpsim; see the file           */
/*            COPYING.txt. If not, write to the Free Software         */
/*            Foundation, 59 Temple Place - Suite 330,                */
/*            Boston, MA 02111-1307, USA.                             */
/*                                                                    */
/*          > A special exception to the GPL can be applied should    */
/*            you wish to distribute a combined work that includes    */
/*            PICos, without being obliged to provide the source      */
/*            code for any proprietary components.                    */
/*                                                                    */
/* History:                                                           */
/* 2005/04/18 [XM]  Create this file.                                 */
/*                                                                    */
/**********************************************************************/

#include "define.h"
#include <stdio.h>
#include <string.h>

extern unsigned char hour;
extern unsigned char min;
extern unsigned char sec;

/**********************************************************************
 * Definition dedicated to the local functions.
 **********************************************************************/
#define LCDLINE1CMD     0x80    //Command to set LCD display to Line 1
#define LCDLINE2CMD     0xC0;   //Command to set LCD display to Line 2

unsigned char  buffer[16];

/**********************************************************************
 * ------------------------------ TASK4 -------------------------------
 *
 * Fifth task of the tutorial.
 *
 **********************************************************************/
TASK(TASK4) 
{
  SPI_Init();
  while(1)
  {
    WaitEvent(UPDATE_EVENT);
    ClearEvent(UPDATE_EVENT); 
    LATBbits.LATB1 = ~LATBbits.LATB1;	// inversion de l'état de la LED

    sprintf(buffer,"%02d:%02d:%02d",hour,min,sec);
    WriteLineSPI_to_LCD(buffer,1);
  }
}

void SPI_Init(void)
{
        SPI1STAT = 0x0000;
        SPI1CON = 0x0020 ;      //Set the SPI1 module to 8-bit Master mode
        IFS0bits.SPI1IF = 0;    //Clear the SPI1 Interrupt Flag
        IEC0bits.SPI1IE = 0;    //SPI1 ISR processing is not enabled.
                                //SPI1 will be used in polling-mode
        SPI1STATbits.SPIEN = 1; //Turn on the SPI1 module
}

void WriteLineSPI_to_LCD(char *s, unsigned int line)
{
        int temp, i;
        i=0;
        temp = SPI1BUF;
        SPI1STATbits.SPIROV = 0;
        IFS0bits.SPI1IF = 0;
//        Delay5us(50);
        switch(line)
        {
	        case 1: SPI1BUF = LCDLINE1CMD;
	        		break;
	        case 2: SPI1BUF = LCDLINE2CMD;
	        		break;
	        default: return;
	     }
                                  
        while(*s)
        {
                while (IFS0bits.SPI1IF==0);     //Now write 16 characters
                temp = SPI1BUF;                 //to Line 2 of the LCD
                IFS0bits.SPI1IF = 0;
                SPI1STATbits.SPIROV = 0;
//                Delay5us(50);
                SPI1BUF = *s++;
                i++;
                if(i>=16) continue;
        }
} 
/* End of File : tsk_task4.c */
