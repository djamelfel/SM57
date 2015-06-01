/**********************************************************************/
/*                                                                    */
/* File name: drv_rs.h                                                */
/*                                                                    */
/* Since:     2005-April-18                                           */
/*                                                                    */
/* Version:   PICos v1.05 - RTOS for PIC24/30/33 families.            */
/*            Copyright (C) 2005-2006 Pragmatec.                      */
/*                                                                    */
/* Author:    Designed by Pragmatec S.A.R.L.        www.pragmatec.net */
/*            MONTAGNE Xavier [XM]      xavier.montagne@pragmatec.net */
/*                                                                    */
/* Purpose:   Manage RS232 generic buffers through the PIC18 USART.   */
/*            Can send ASCII (with printf) or binary string of data.  */
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

#ifndef _DRV_RS_H_
#define _DRV_RS_H_

#include "pro_man.h"
#include "stdarg.h"

#ifndef NULL
#define NULL 0
#endif

typedef struct _RS_message 
{
	char						*data;
	unsigned int			length;
	unsigned char			pattern;
	TaskType					CallerID;
	struct _RS_message	*next;
} RS_message_t, *RS_message_tRef;


StatusType RS_enqMsg(RS_message_tRef toEnqueue, unsigned char *data, 
                     unsigned int length);
StatusType RS_RCV_Register(RS_message_tRef toEnqueue, unsigned char *data, 
                     unsigned int length, unsigned char pattern);
RS_message_tRef RS_deqMsg(void);


#endif /* _DRV_RS_H_ */

/* End of file : drv_rs.h */
