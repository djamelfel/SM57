/**********************************************************************/
/*                                                                    */
/* File name: define.h                                                */
/*                                                                    */
/* Since:     2005-April-18                                           */
/*                                                                    */
/* Version:   PICos v1.05 - RTOS for PIC24/30/33 families.            */
/*            Copyright (C) 2005-2006 Pragmatec.                      */
/*                                                                    */
/* Author:    Designed by Pragmatec S.A.R.L.        www.pragmatec.net */
/*            MONTAGNE Xavier [XM]      xavier.montagne@pragmatec.net */
/*                                                                    */
/* Purpose:   Specify all the specific definitions of the project.    */
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
/* 2005/04/18 [XM]  Create the file.                                  */
/*                                                                    */
/**********************************************************************/

#ifndef _DEFINE_H
#define _DEFINE_H

#include "device.h"

/***********************************************************************
 * ------------------------ Timer settings -----------------------------
 **********************************************************************/
#define _08MHZ_PIC30      250
#define _10MHZ_PIC30      375
#define _16MHZ_PIC30      500
#define _20MHZ_PIC30      750
#define _32MHZ_PIC30     1000
#define _40MHZ_PIC30     1250
#define _59MHZ_PIC30     1840    // 58,9824 (for MCP demo board with 8PLL)
#define _64MHZ_PIC30     2000
#define _80MHZ_PIC30     2500
#define _96MHZ_PIC30     3000
#define _120MHZ_PIC30    3750

#define _40MHZ_PIC33     5250    // Explorer 16 Board with PLL, Prescale 1:8
#define _16MHZ_PIC24     2000    // Explorer 16 Board with PLL, Prescale 1:8

/***********************************************************************
 * ----------------------------- Events --------------------------------
 **********************************************************************/
#define ALARM_EVENT       0x80
#define TASK1_EVENT       0x10
#define UPDATE_EVENT      0x02


/***********************************************************************
 * ----------------------------- Task ID -------------------------------
 **********************************************************************/
#define TASK0_ID             1
#define TASK1_ID             2
#define TASK2_ID             3
#define TASK3_ID             4
#define TASK4_ID             5

#define TASK0_PRIO           7
#define TASK1_PRIO           10
#define TASK2_PRIO           1
#define TASK3_PRIO           2
#define TASK4_PRIO           3

#endif /* _DEFINE_H */


/* End of File : define.h */
