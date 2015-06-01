/**********************************************************************/
/*                                                                    */
/* File name: pro_man.c                                               */
/*                                                                    */
/* Since:     2005-April-18                                           */
/*                                                                    */
/* Version:   PICos v1.05 - RTOS for PIC24/30/33 families.            */
/*            Copyright (C) 2005-2006 Pragmatec.                      */
/*                                                                    */
/* Author:    Designed by Pragmatec S.A.R.L.        www.pragmatec.net */
/*            MONTAGNE Xavier [XM]      xavier.montagne@pragmatec.net */
/*                                                                    */
/* Purpose:   Interrupt manager (set and clear interrupts)            */
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
/* 2004/09/20 [XM]  Create this file.                                 */
/* 2006/07/21 [XM]  Buf fixed in LeaveISR (USER mode management).     */
/*                                                                    */
/**********************************************************************/

#include "device.h"

extern unsigned int kernelState;
extern ptrTCB  currentTCB;

/**********************************************************************
 * ROM area of the interrupt manager.
 *********************************************************************/
#pragma		code	KERNEL_ROM

/**********************************************************************
 * Set Global Interrupt Enable.
 *
 * @param void
 * @return void
 **********************************************************************/
void EnableAllInterrupts(void)
{
  asm volatile ("disi #0x3"); 
  asm ("bclr  SR, #5" : : );  // Enable interrupts
  asm ("bclr  SR, #6" : : );  // Enable interrupts
  asm ("bclr  SR, #7" : : );  // Enable interrupts
}

/**********************************************************************
 * Clear Global Interrupt Enable.
 *
 * @param void
 * @return void
 **********************************************************************/
void DisableAllInterrupts(void)
{
  asm volatile ("disi #0x3"); 
  asm ("bset   SR, #5" : : );  // Disable interrupts
  asm ("bset   SR, #6" : : );  // Disable interrupts
  asm ("bset   SR, #7" : : );  // Disable interrupts
}

/**********************************************************************
 * Set Global Interrupt Enable.
 *
 * @param void
 * @return void
 **********************************************************************/
void ResumeAllInterrupts(void)
{
  asm volatile ("disi #0x3"); 
  asm ("bclr  SR, #5" : : );  // Enable interrupts
  asm ("bclr  SR, #6" : : );  // Enable interrupts
  asm ("bclr  SR, #7" : : );  // Enable interrupts
}

/**********************************************************************
 * Clear Global Interrupt Enable.
 *
 * @param void
 * @return void
 **********************************************************************/
void SuspendAllInterrupts(void)
{
  asm volatile ("disi #0x3"); 
  asm ("bset   SR, #5" : : );  // Disable interrupts
  asm ("bset   SR, #6" : : );  // Disable interrupts
  asm ("bset   SR, #7" : : );  // Disable interrupts
}

/**********************************************************************
 * Set Global Interrupt Enable.
 *
 * @param void
 * @return void
 **********************************************************************/
void ResumeOSInterrupts(void)
{
  asm volatile ("disi #0x3"); 
  asm ("bclr  SR, #5" : : );  // Enable interrupts
  asm ("bclr  SR, #6" : : );  // Enable interrupts
  asm ("bclr  SR, #7" : : );  // Enable interrupts
}

/**********************************************************************
 * Clear Global Interrupt Enable.
 *
 * @param void
 * @return void
 **********************************************************************/
void SuspendOSInterrupts(void)
{
  asm volatile ("disi #0x3"); 
  asm ("bset   SR, #5" : : );  // Disable interrupts
  asm ("bset   SR, #6" : : );  // Disable interrupts
  asm ("bset   SR, #7" : : );  // Disable interrupts
}

/**********************************************************************
 * Set the ISR settings to disble the Schedule function if called
 * during the ISR.
 *
 * @param void
 * @return void
 **********************************************************************/
void EnterISR(void)
{
  DisableAllInterrupts();
  if (kernelState & USER) 
    currentTCB->kernelState_copy  = kernelState;
  kernelState |= ISR;
}

/**********************************************************************
 * Restore the kernel state and enable the Schedule function:
 * if a service like SetEvent has been called by an ISR then the service
 * didn't schedule when it's called but at the end of the ISR, thanks to
 * the LeaveISR function.
 *
 * @param void
 * @return void
 **********************************************************************/
void LeaveISR(void)
{    
  if (kernelState & USER) 
  {
    if (kernelState & SERVICES)
    {
      kernelState = currentTCB->kernelState_copy;
      Schedule();
    }
      kernelState = currentTCB->kernelState_copy;
  }
  else
     kernelState &= ~ISR;
  EnableAllInterrupts();
}


/* End of File : int_man.c */
