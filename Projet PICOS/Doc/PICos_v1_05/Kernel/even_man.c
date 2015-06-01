/**********************************************************************/
/*                                                                    */
/* File name: even_man.c                                              */
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
/*            The job of the event manager is to take care of task    */
/*            activation in case of any similarity inbetween events   */
/*            set and events waited.
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
/* 2006/02/17 [RZR] We need to keep the RR_RUNNING flag.              */
/*                  Modified functions : SetEvent & WaitEvent.        */
/*                                                                    */
/**********************************************************************/

#include "device.h"


extern ptrTCB GetSpecificTCB(TaskType TaskID);
extern ptrTCB GetCurrentTCB(void);

/**********************************************************************
 * Set an event in the EVENT ram area of a task.
 * The event is reset if the task is already waiting for this event 
 * (task set into READY state).
 *
 * @param TaskID     IN  Reference of the task
 * @param Mask       IN  Bitmask of the event to set 
 * @return status    E_OK if no waiting task
 *                   E_OS_STATE if task suspended
 *                   E_OS_ID if ID is not correct
 *                   no return value otherwise
 **********************************************************************/
StatusType SetEvent(TaskType TaskID, EventMaskType Mask)
{
  ptrTCB thisTCB;

  thisTCB = GetSpecificTCB(TaskID);
  if (thisTCB == NULL)
    return (E_OS_ID);

  if (thisTCB->State == SUSPENDED)
    return (E_OS_STATE);

  thisTCB->EventReceived |= Mask;
  if (thisTCB->EventWaited & Mask)
  {
	// [RZR] We need to keep the RR_RUNNING flag
	thisTCB->State &= ~WAITING;
    thisTCB->State |= READY;
    Schedule();
  }

  return (E_OK);
}

/**********************************************************************
 * Clear an event in the EVENT ram area and the WAIT ram area of the
 * calling task.
 * Should be called by a task just after the waitevent service call.
 *
 * @param Mask       IN  Bitmask of the event to set 
 * @return status    Always E_OK
 **********************************************************************/
StatusType ClearEvent(EventMaskType Mask)
{
  ptrTCB thisTCB;

  thisTCB = GetCurrentTCB();
  thisTCB->EventWaited   &= ~Mask;
  thisTCB->EventReceived &= ~Mask;

  return (E_OK);
}

/**********************************************************************
 * Get all the event (byte value)from the EVENT ram area of a task.
 * Allow the task to guess from which event it is waked up.
 *
 * @param TaskID     IN  Reference of the task
 * @param Mask       IN  Reference of the event to get 
 * @return status    E_OS_STATE if task suspended
 *                   E_OS_ID if ID is not correct
 *                   E_OK otherwise
 **********************************************************************/
StatusType GetEvent(TaskType TaskID, EventMaskRefType Mask)
{
  ptrTCB thisTCB;

  thisTCB = GetSpecificTCB(TaskID);
  if (thisTCB == NULL)
    return (E_OS_ID);

  *Mask = thisTCB->EventReceived; /* instead of EventWaited  !!! */

  return (E_OK);
}


/**********************************************************************
 * Set an event in the WAIT ram area of a task.
 * If the same event is set in the EVENT ram area, the function 
 * immediatly returns. If not the task is set in WAITING state.
 *
 * @param Mask       IN  Bitmask of the event to wait 
 * @return status    E_OK if the event exists
 *                   E_OS_ID if ID is not correct 
 *                   no return value otherwise
 **********************************************************************/
StatusType WaitEvent(EventMaskType Mask)
{
  ptrTCB thisTCB;

  thisTCB = GetCurrentTCB();
  DisableAllInterrupts();
  thisTCB->EventWaited |= Mask;

  if ((thisTCB->EventReceived & Mask) == 0)
  {
	// [RZR] We need to keep the RR_RUNNING flag.
    thisTCB->State &= ~READY;
	thisTCB->State |= WAITING;
    Schedule();
  }
  EnableAllInterrupts();
  return (E_OK);
}


/* End of File : even_man.c */
