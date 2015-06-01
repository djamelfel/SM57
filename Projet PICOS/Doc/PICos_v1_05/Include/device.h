/**********************************************************************/
/*                                                                    */
/* File name: device.h                                                */
/*                                                                    */
/* Since:     2005-April-18                                           */
/*                                                                    */
/* Version:   PICos v1.05 - RTOS for PIC24/30/33 families.            */
/*            Copyright (C) 2005-2006 Pragmatec.                      */
/*                                                                    */
/* Author:    Designed by Pragmatec S.A.R.L.        www.pragmatec.net */
/*            MONTAGNE Xavier [XM]      xavier.montagne@pragmatec.net */
/*                                                                    */
/* Purpose:   Specify the dsPIC device header file.                   */
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

#ifndef __DEVICE_H__
#define __DEVICE_H__

/* processor specific variable definitions */ 
#if defined(__dsPIC30F__) 
#include "p30fxxxx.h" 
#elif defined(__dsPIC33F__) 
#include "p33fxxxx.h" 
#elif defined(__PIC24H__) 
#include "p24hxxxx.h" 
#elif defined(__PIC24F__) 
#include "p24fxxxx.h" 
#endif 

#include "pro_man.h"
#include "even_man.h"
#include "alarm.h"
#include "int_man.h"


/**********************************************************************
 * --------------------------- TYPE MACRO -----------------------------
 **********************************************************************/

typedef unsigned char u_char;
typedef unsigned int  u_int;


/**********************************************************************
 * -------------------------- DEFINITIONS -----------------------------
 **********************************************************************/
#define NONE                 0
#define ON                   1
#define OFF                  0
#define SREG(x)              x + 0x11
#define FREG(x)              x + 0x10


#endif /* __DEVICE_H__ */


/* End of File : device.h */
