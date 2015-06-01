/**********************************************************************/
/*                                                                    */
/* File name: tsk_task2.c                                             */
/*                                                                    */
/* Since:     2005-April-18                                           */
/*                                                                    */
/* Version:   PICos v1.05 - RTOS for PIC24/30/33 families.            */
/*            Copyright (C) 2005-2006 Pragmatec.                      */
/*                                                                    */
/* Author:    Designed by Pragmatec S.A.R.L.        www.pragmatec.net */
/*            MONTAGNE Xavier [XM]      xavier.montagne@pragmatec.net */
/*                                                                    */
/* Purpose:   Third task of the tutorial.                             */
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

extern unsigned char hour;

/**********************************************************************
 * ------------------------------ TASK2 -------------------------------
 *
 * Third task of the tutorial.
 *
 **********************************************************************/
TASK(TASK2) 
{
  hour++;
  if (hour == 24)
    hour = 0;
  SetEvent(TASK4_ID, UPDATE_EVENT);
  IFS0bits.INT0IF = 0;
  IEC0bits.INT0IE = 1;
  TerminateTask();
}

void __attribute__((__interrupt__)) _INT0Interrupt(void)
{
  TaskStateType State;

  EnterISR();
  IEC0bits.INT0IE = 0;
  GetTaskState(TASK2_ID, &State);
  if (State == SUSPENDED) 
    ActivateTask(TASK2_ID);
  LeaveISR();
}

/* End of File : tsk_task2.c */
