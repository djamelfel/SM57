/**********************************************************************/
/*                                                                    */
/* File name: main.c                                                  */
/*                                                                    */
/* Since:     2005-April-18                                           */
/*                                                                    */
/* Version:   PICos v1.05 - RTOS for PIC24/30/33 families.            */
/*            Copyright (C) 2005-2006 Pragmatec.                      */
/*                                                                    */
/* Author:    Designed by Pragmatec S.A.R.L.        www.pragmatec.net */
/*            MONTAGNE Xavier [XM]      xavier.montagne@pragmatec.net */
/*                                                                    */
/* Purpose:   Custom initialization and kernel start.                 */
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

/**********************************************************************
 * Definition dedicated to the local functions.
 **********************************************************************/
#define DEFAULT_MODE       0

/**********************************************************************
 * Function prototypes.
 **********************************************************************/
int  main (void);
void Init(void);
void StartupHook(void);
void ShutdownHook(StatusType error);
void ErrorHook(StatusType error);
void PreTaskHook(void);
void PostTaskHook(void);

volatile AppModeType SelectedMode;
volatile unsigned int __attribute__((__section__(".stack_kernel"))) stack_k[128];
volatile unsigned int W15_KERNEL, W14_KERNEL;
volatile unsigned int kernelState;


/**********************************************************************
 * -------------------------- main function ---------------------------
 *
 * Setup the different alarms and start the kernel.
 *
 **********************************************************************/
int main(void)
{
  asm ("mov.w #_stack_k,w14" : : ); //WREG14 = (unsigned int)(stack_k);
  asm ("mov.w #_stack_k,w15" : : ); //WREG15 = (unsigned int)(stack_k);
  SelectedMode = DEFAULT_MODE;
  kernelState  = STOP;
  Init();

  IEC1bits.INT1IE = 1;
  IEC1bits.INT2IE = 1;

  StartOS(SelectedMode);

  return(0);
}

/**********************************************************************
 * Clear all RAM memory and set PORTB to output mode.
 *
 * @return void
 **********************************************************************/
void Init(void)
{
  IEC0bits.T1IE = 1;
  W15_KERNEL = 0;
  W14_KERNEL = 0;

  /* interrupt automaticaly disabled during ISR */
  //INTCON1bits.NSTDIS = 1; 

  T1CON = 0xA010;
  T1CONbits.TON = 1; /* for debug only !!*/
  INTCON2bits.ALTIVT = 1;
  
  // Set clock frequency (for 1 ms tick)
  PR1 = _16MHZ_PIC24; 
  IEC0bits.T1IE = 1;
  IFS0bits.T1IF = 0;
  TMR1 = 0;

  InitTCB();
  return;
}

/**********************************************************************
 * Hook routine called just before entering in kernel.
 *
 * @param error      IN The new error stored in buffer
 * @return error     Error level
 **********************************************************************/
void StartupHook(void)
{
}

/**********************************************************************
 * Hook routine called just after leaving the kernel.
 *
 * @param error      IN The last error detected by OS
 * @return void
 **********************************************************************/
void ShutdownHook(StatusType error)
{
}

/**********************************************************************
 * Store a new error in a global buffer keeping a track of the 
 * application history.
 *
 * @param error      IN The new error stored in buffer
 * @return void
 **********************************************************************/
void ErrorHook(StatusType error)
{
}

/**********************************************************************
 * Hook routine called just before entering in a task.
 *
 * @return void
 **********************************************************************/
void PreTaskHook(void)
{
}

/**********************************************************************
 * Hook routine called just after leaving a task.
 *
 * @return void
 **********************************************************************/
void PostTaskHook(void)
{
}

/* End of File : main.c */
