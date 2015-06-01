/**********************************************************************/
/*                                                                    */
/* File name: even_man.h                                              */
/*                                                                    */
/* Since:     2005-April-18                                           */
/*                                                                    */
/* Version:   PICos v1.05 - RTOS for PIC24/30/33 families.            */
/*            Copyright (C) 2005-2006 Pragmatec.                      */
/*                                                                    */
/* Author:    Designed by Pragmatec S.A.R.L.        www.pragmatec.net */
/*            MONTAGNE Xavier [XM]      xavier.montagne@pragmatec.net */
/*                                                                    */
/* Purpose:   The event manager is in charge of setting and clearing  */
/*            the events waited by the tasks.                         */
/*            2 areas are used to manage the arrival of events and    */
/*            the fact that some tasks are waiting for these events.  */
/*              - EVENT ram area : for events set by tasks or alarms  */
/*              - WAIT  ram area : for events waited by tasks         */
/*            The job of the event manager is to take care of task    */
/*            activation in case of any similarity inbetween events   */
/*            set and events waited.                                  */
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

#ifndef __EVEN_MAN_H__
#define __EVEN_MAN_H__

/***********************************************************************
 * ------------------------ Type definition ----------------------------
 **********************************************************************/
typedef unsigned int  EventMaskType;
typedef unsigned int *EventMaskRefType;


/***********************************************************************
 * ----------------------- From even_man.c file ------------------------
 **********************************************************************/
StatusType SetEvent(TaskType TaskID, EventMaskType Mask);
StatusType ClearEvent(EventMaskType Mask);
StatusType GetEvent(TaskType TaskID, EventMaskRefType Mask);
StatusType WaitEvent(EventMaskType Mask);


#endif /* __EVEN_MAN_H__ */


/* End of File : even_man.h */

