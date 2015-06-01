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
#include "drv_rs.h"
#include <stdio.h>
#include <string.h>

#define ALARM_TSK4      1

int Printf (const char *fmt, ...);
extern unsigned char hour;
extern unsigned char min;
extern unsigned char sec;

/**********************************************************************
 * Definition dedicated to the local functions.
 **********************************************************************/
RS_message_t RS_msg; 
unsigned char  buffer[80];
EventMaskType  Time_event;

/**********************************************************************
 * ------------------------------ TASK4 -------------------------------
 *
 * Fifth task of the tutorial.
 *
 **********************************************************************/
TASK(TASK4) 
{
  SetRelAlarm(ALARM_TSK4, 1000, 1000);

  Printf("\r\n");

  Printf(" ______________________________________________________________________ \r\n");
  Printf(">                                                                      <\r\n");
  Printf(">                           PICos Tutorial                             <\r\n");
  Printf(">                                                                      <\r\n");
  Printf(">                                                                      <\r\n");
  Printf(">           PICos - Real-time kernel for PIC24/30/33 families          <\r\n");
  Printf(">                                                                      <\r\n");
  Printf(">                                                                      <\r\n");
  Printf("> www.picos18.com                                    www.pragmatec.net <\r\n");
  Printf(">______________________________________________________________________<\r\n");
  Printf("\r\n");
  Printf("\r\n");

  while(1)
  {
    WaitEvent(ALARM_EVENT | UPDATE_EVENT);
    GetEvent(TASK4_ID, &Time_event);

    if (Time_event & ALARM_EVENT)
      ClearEvent(ALARM_EVENT); 
    if (Time_event & UPDATE_EVENT)
      ClearEvent(UPDATE_EVENT); 

    Printf("%02d : %02d : %02d\r", (int)hour, (int)min, (int)sec);
  }
}

/**********************************************************************
 * Function in charge of structure registration and buffer transmission.
 *
 * @param  string   IN const string send to the USART port
 * @return void
 **********************************************************************/
int Printf (const char *fmt, ...)
{
  va_list ap;
  RS_enqMsg(&RS_msg, buffer, 0);
  va_start (ap, fmt); 
  RS_msg.length = vsprintf(RS_msg.data, fmt, ap);
  va_end (ap);
  SetEvent(RS_DRV_ID, RS_NEW_MSG); 
  WaitEvent(RS_QUEUE_EMPTY);
  ClearEvent(RS_QUEUE_EMPTY);
  return RS_msg.length;
}
 
/* End of File : tsk_task4.c */
