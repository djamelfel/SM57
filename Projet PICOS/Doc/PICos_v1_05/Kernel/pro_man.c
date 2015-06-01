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
/* Purpose:   The process manager is in charge of changing or giving  */
/*            the state of the different tasks, and also of starting  */
/*            or stopping the kernel.                                 */
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
/* 2005/04/23 [RZR] Replace NULL by parserTCB in GetSpecificTCB.      */
/* 2006/02/17 [RZR] Bug fixing in RR algorithm (OrganizeTCB function).*/
/* 2006/04/24 [RZR] Bug fixing in RR algorithm (GetNextRunningTCB).   */
/* 2006/07/21 [XM]  Call Resume/SuspendOSInterrupt for critcal section*/
/*                                                                    */
/**********************************************************************/

#include "device.h"
#include "string.h"

/**********************************************************************
 * Shared variables defined in other section.
 **********************************************************************/
extern ptrTCB GetCurrentTCB(void);
extern void SCHEDULE(void);
extern void ordo(void);
extern ptrTCB GetSpecificTCB(TaskType TaskID);

extern unsigned int kernelState;
extern TCB  TCB_list[];
extern unsigned char TASKNUMBER;
extern Resource Resource_list[];
extern TCB TCB_const_list[];
extern unsigned char RESOURCENUMBER;

volatile AppModeType   appmode;
volatile unsigned char id_tsk_run;
volatile unsigned char kernelPanic;
volatile ptrTCB        firstTCB;
volatile ptrTCB        currentTCB = NULL;


/**********************************************************************
 * Functions declared in other sections.
 **********************************************************************/
#ifdef	SHUTDOWNHOOK
	extern void ShutdownHook(StatusType Error);
#endif

#ifdef	ERRORHOOK
	extern void ErrorHook(StatusType Error);
#endif

#ifdef	PRETASKHOOK
	extern void PreTaskHook(void);
#endif

#ifdef	POSTTASKHOOK
	extern void PostHook(void);
#endif


/**********************************************************************
 * Modify the priority of a task.
 * Not mandatory by OSEK standart.
 *
 * @param new_prio   IN  Futur prio of the task 
 * @param id_task    IN  ID of the task
 * @return void 
 **********************************************************************/
StatusType SetPriority (u_char priority, u_char taskID)
{
  return (E_OK);
}

/**********************************************************************
 * Set id_tsk_run into TaskID or INVALID_TASK (0x10) if the ID is out 
 * of range.
 *
 * @param TaskID     IN  Reference of the task
 * @return status    Always E_OK 
 **********************************************************************/
StatusType GetTaskID (TaskRefType TaskID)
{
  ptrTCB thisTCB;

  thisTCB = GetCurrentTCB();
  if (thisTCB == NULL)
  {
    *TaskID = (TaskType)INVALID_TASK;
    return (E_OK);
  }

  if ((thisTCB->TaskID > MIN_TASK_NB) && 
      (thisTCB->TaskID < MAX_TASK_NB))
    *TaskID = (TaskType)thisTCB->TaskID;
  else
    *TaskID = (TaskType)INVALID_TASK;

  return (E_OK);
}

/**********************************************************************
 * Copy a byte into a buffer.
 *
 * @param TaksID     IN  ID of the task 
 * @param State      OUT Reference of the task state
 * @return Status    E_OK if the task ID is correct
 *                   E_OS_ID otherwise
 **********************************************************************/
StatusType GetTaskState (TaskType TaskID, TaskStateRefType State)
{
  ptrTCB thisTCB;

  thisTCB = GetSpecificTCB(TaskID);
  if (thisTCB == NULL)
    return (E_OS_ID);

  *State = thisTCB->State;
  return (E_OK);
}

/**********************************************************************
 * Set the task to the READY state and call the scheduler to jump to 
 * another task if necessary.
 *
 * @param TaskID     IN  ID of the task
 * @return Status    E_OS_ID if ID is not correct
 *                   never return otherwise
 **********************************************************************/
StatusType ActivateTask (TaskType TaskID)
{
  ptrTCB thisTCB;

  thisTCB = GetSpecificTCB(TaskID);
  if (thisTCB == NULL)
    return (E_OS_ID);

  if (thisTCB->State == SUSPENDED)
  {
    thisTCB->State = READY;
    Schedule();
  }
  else
  {
    if ((thisTCB->State & 0xF0) < 0xF0)
      thisTCB->State += 0x10;
  }
  return (E_OK);
}

/**********************************************************************
 * Allow a task to terminate itself. Cannot terminate another task.
 * To prepare a new activation of the task, we need first to store in
 * stack the start adress of the task
 *
 * @param dest       OUT Destination buffer 
 * @param src        IN  The byte to copy
 * @return Status    E_OK if ID is not correct
 *                   In fact the function never return 
 **********************************************************************/
StatusType TerminateTask (void)
{
  ptrTCB thisTCB;
  unsigned int * ptrRAM;

  thisTCB = GetCurrentTCB();
  if (thisTCB->State & 0xF0)
    thisTCB->State -= 0x10;
  else
    thisTCB->State = SUSPENDED;

  ptrRAM = thisTCB->StackAddress;
  if (ptrRAM != NULL)
  {
    *ptrRAM++ = (unsigned int)(thisTCB->StartAddress);
    *ptrRAM   = 0;
    thisTCB->Stack_register = SREG(thisTCB->StackAddress);
    thisTCB->Frame_register = FREG(thisTCB->StackAddress);
  }
  DisableAllInterrupts();
  kernelState |= KERNEL;
  kernelState &= ~USER;
  EnableAllInterrupts();
  TaskLauncher();
 
  return (E_OK);
}

/**********************************************************************
 * Chain a task by setting the calling task in a SUSPENDED state and 
 * the called taks in the READY state.
 * Call the scheduler to jump the chained task.
 *
 * @param TaskID     IN  ID of the next task to chain
 * @return Status    E_OK if ID is not correct
 *                   In fact the function never return 
 **********************************************************************/
StatusType ChainTask (TaskType TaskID)
{
  ptrTCB thisTCB;
  unsigned int * ptrRAM;

  thisTCB = GetCurrentTCB();
  thisTCB->State = SUSPENDED;

  thisTCB = GetSpecificTCB(TaskID);
  if (thisTCB == NULL)
    return (E_OS_ID);

  thisTCB->State = READY;

  ptrRAM = thisTCB->StackAddress;
  if (ptrRAM != NULL)
  {
    *ptrRAM++ = (unsigned int)(thisTCB->StartAddress);
    *ptrRAM   = 0;
    thisTCB->Stack_register = SREG(thisTCB->StackAddress);
    thisTCB->Frame_register = FREG(thisTCB->StackAddress);
  }
  DisableAllInterrupts();
  kernelState |= KERNEL;
  kernelState &= ~USER;
  EnableAllInterrupts();
  TaskLauncher();

  return (E_OK);
}

/**********************************************************************
 * Force a scheduler action
 *
 * @return Status    E_OK 
 *                   In fact the function never return 
 **********************************************************************/
StatusType Schedule(void)
{
  SuspendOSInterrupts();
  kernelState |= SERVICES;
  if (kernelState & ISR)       // Service called from ISR
    return (E_OK);
  kernelState &= ~SERVICES;
  /* Never return... */
  if (kernelState & USER)
  {
    #ifdef	PRETASKHOOK
	PostTaskHook();
    #endif
    SCHEDULE();
  }
  return (E_OK);
}

/**********************************************************************
 * Return the appmode global variable.
 *
 * @return AppMode   The mode set by application 
 **********************************************************************/
AppModeType GetActiveApplicationMode(void)
{
  return (appmode);
}

/**********************************************************************
 * Jump to the KERNEL code area by setting the appmode global variable. 
 * This service is for call from main routine only.
 * Store at first the return adress in main routine.
 *
 * @param Mode       IN  Mode to set into appmode
 * @return void
 **********************************************************************/
void StartOS(AppModeType Mode)
{
  appmode = Mode;
  kernelState = RUN;
  ordo();
  return;
}

/**********************************************************************
 * Stop the kernel and task activation after an error occurs. 
 * The function returns to the adress of the main function stored by 
 * the StartOS service.
 *
 * @param error      IN  Last error number detected
 * @return void
 **********************************************************************/
void ShutdownOS(StatusType Error)
{	
  kernelState = STOP;
#ifdef	SHUTDOWNHOOK
  ShutdownHook(Error);
#endif
  return;
}

/**********************************************************************
 * Force the kernel to bring into order the list of tasks function to  
 * priority.
 *
 * @param  void
 * @return void
 **********************************************************************/
void OrganizeTCB(void)
{
  unsigned char i, done;
  ptrTCB BestTCB, weakestTCB, parserTCB;

  firstTCB = NULL;
  done     = 0;

  /* Clear the next reference of each TCB */
  for (i = 0; i < TASKNUMBER; i++)
    TCB_list[i].next = NULL;

  /* Find out the TCB with the smallest priority */
  weakestTCB = (ptrTCB)(&TCB_list[0]);
  for (i = 0; i < TASKNUMBER; i++)
  {
    if (TCB_list[i].Priority < weakestTCB->Priority)
      weakestTCB = (ptrTCB)(&TCB_list[i]);
  } 

  /* Start to order from the smallest TCB */
  while (!done)
  {
    BestTCB = weakestTCB;

    /* Find out the highest priority */
    done = 1;
    for (i = 0; i < TASKNUMBER; i++)
    {
      parserTCB = (ptrTCB)(&TCB_list[i]);
      if ((parserTCB->Priority == BestTCB->Priority) && \
          (parserTCB->TaskID != BestTCB->TaskID)) 
          // [RZR]:to avoid to RR with itself !
      {
        parserTCB->Type  |= ROUND_ROBIN;
        BestTCB->Type    |= ROUND_ROBIN;
        parserTCB->State |= RR_RUNNING;
        BestTCB->State   &= ~RR_RUNNING;
      }
      if ((parserTCB->Priority >=  BestTCB->Priority) && \
          (parserTCB->next == NULL)) 
      {
        BestTCB = (ptrTCB)(&TCB_list[i]);
        done = 0;
      }
    }

    /* The first one is the highest one */
    if (firstTCB == NULL)
    {
      firstTCB = BestTCB;
      firstTCB->next = firstTCB;
    }
    /* if not we chain the others... */
    else
    {
      parserTCB = firstTCB;
      while (parserTCB->next != parserTCB)
        parserTCB = parserTCB->next;
      parserTCB->next = BestTCB;
      BestTCB->next   = BestTCB;
    }
  }
}

/**********************************************************************
 * Set the task priority to the resource priority. 
 * See the ceiling protocol of the OSEK/VDX standart.
 *
 * @param ID         IN  ID of the resource to be accessed
 * @return Status    E_OS_ID if the resource does not exist
 *                   E_OS_ACCESS if the resource is still in use
 *                   Elsewise the function never returns 
 **********************************************************************/
StatusType GetResource(ResourceType ID)
{
  if (ID > RESOURCENUMBER)
    return E_OS_ID;
  if (Resource_list[ID].lock == 1)
    return E_OS_ACCESS;
  Resource_list[ID].Taskprio = GetCurrentTCB()->Priority;
  GetCurrentTCB()->Priority  = Resource_list[ID].priority;
  Resource_list[ID].lock     = 1;
  OrganizeTCB();
  Schedule();
  return (E_OK);
}

/**********************************************************************
 * Restore the task priority to original value. 
 * See the ceiling protocol of the OSEK/VDX standart.
 *
 * @param ID         IN  ID of the resource to be accessed
 * @return Status    E_OS_ID if the resource does not exist
 *                   E_OS_ACCESS if the resource is still in use
 *                   Elsewise the function never returns 
 **********************************************************************/
StatusType ReleaseResource(ResourceType ID)
{
  if (ID > RESOURCENUMBER)
    return E_OS_ID;
  if (Resource_list[ID].lock == 0)
    return E_OS_ACCESS;
  GetCurrentTCB()->Priority = Resource_list[ID].Taskprio;
  Resource_list[ID].lock = 0;
  Schedule();
  return (E_OK);
}

/**********************************************************************
 * Copy the TCB const list (from tasdesc.c file) to the TCB list in RAM
 *
 * @param  void
 * @return void
 **********************************************************************/
void InitTCB(void)
{
  unsigned char  i;
  unsigned int * ptrRAM;
  ptrTCB parserTCB;

  parserTCB = (ptrTCB)(&TCB_list[0]);
  for (i = 0; i < TASKNUMBER; i++)
  {
    memcpy(parserTCB, &TCB_const_list[i], sizeof(TCB));
    ptrRAM = parserTCB->StackAddress;
    if (ptrRAM != NULL)
    {
      *ptrRAM++ = (unsigned int)(parserTCB->StartAddress);
      *ptrRAM   = 0;
    }
    parserTCB++;
  }
  OrganizeTCB();
}

/**********************************************************************
 * Return the TCB of the current running task.
 *
 * @param  void
 * @return ptrTCB    Addresse of the TCB
 **********************************************************************/
ptrTCB GetCurrentTCB(void)
{
  return (currentTCB);
}

/**********************************************************************
 * Return the TCB of the next task ready to run. According to OSEK
 * specification, the next running task is the task in READY state
 * with the highest priority.
 *
 * @param  void
 * @return ptrTCB    Addresse of the TCB
 **********************************************************************/
ptrTCB GetNextRunningTCB(void)
{
  ptrTCB parserTCB;
  parserTCB = firstTCB;
  SuspendOSInterrupts();
  while (1)
  {
    if ((parserTCB->State & READY) && (parserTCB->State != WAITING))
    {
      if ((parserTCB->Type & ROUND_ROBIN) && !(parserTCB->State & RR_RUNNING))
      {
        if (parserTCB == parserTCB->next)
          break;
        else
          parserTCB = parserTCB->next;
        continue;
      }
      currentTCB = parserTCB;
      ResumeOSInterrupts();
      return(parserTCB);
    }
    if (parserTCB == parserTCB->next)
      break;
    else
      parserTCB = parserTCB->next;
  }
  ResumeOSInterrupts();
  return (NULL);
}

/**********************************************************************
 * Return the TCB of a specific task ID.
 *
 * @param  void
 * @return ptrTCB    Addresse of the TCB
 *                   NULL if the ID is not found
 **********************************************************************/
ptrTCB GetSpecificTCB(TaskType TaskID)
{
  ptrTCB parserTCB;
  parserTCB = firstTCB;
  while (parserTCB->next != parserTCB)
  {
    if (parserTCB->TaskID == TaskID)
      return(parserTCB);
    parserTCB = parserTCB->next;
  }
  
  if (parserTCB->TaskID == TaskID)
    return(parserTCB);
  else
    return(NULL);
}

/**********************************************************************
 * Restore CTX and call another task.
 *
 * @return void
 **********************************************************************/
void TaskLauncher(void)
{
  ptrTCB nextTask;
  /* we are in KERNEL mode : done by SCHEDULE macro */

  nextTask = NULL;
  while (nextTask == NULL)
    nextTask = GetNextRunningTCB();

  if (kernelState & RUN)
  {
    SuspendOSInterrupts();
    id_tsk_run = nextTask->TaskID;
    #ifdef	PRETASKHOOK
	PreTaskHook();
    #endif
    WREG15 = (unsigned int)(nextTask->Stack_register);
    Nop();
    SPLIM  = (unsigned int)(nextTask->StackAddress);
    SPLIM += (unsigned int)(nextTask->StackSize);
    Nop();
    WREG14 = (unsigned int)(nextTask->Frame_register);
    asm ("mov.w [--w15],w0" : : );
    asm ("mov.w w0,SR" : : );
    asm ("mov.w [--w15],w13" : : );
    asm ("mov.w [--w15],w12" : : );
    asm ("mov.w [--w15],w11" : : );
    asm ("mov.w [--w15],w10" : : );
    asm ("mov.w [--w15],w9" : : );
    asm ("mov.w [--w15],w8" : : );
    asm ("mov.w [--w15],w7" : : );
    asm ("mov.w [--w15],w6" : : );
    asm ("mov.w [--w15],w5" : : );
    asm ("mov.w [--w15],w4" : : );
    asm ("mov.w [--w15],w3" : : );
    asm ("mov.w [--w15],w2" : : );
    asm ("mov.w [--w15],w1" : : );
    asm ("mov.w [--w15],w0" : : );
    /* Enable interrupts */
    asm ("bclr  _kernelState, #3" : : ); // Exit KERNEL mode
    asm ("bset  _kernelState, #2" : : ); // Enter USER  mode
    asm volatile ("disi #0x3"); 
    asm ("bclr  SR, #5" : : ); // Enable interrupts
    asm ("bclr  SR, #6" : : ); // Enable interrupts
    asm ("bclr  SR, #7" : : ); // Enable interrupts
    asm ("return" : : );
  }
  /* ...else kernelState = STOP  */
  /* => back to the StartOs call */  
}

/**********************************************************************
 * SUSPEND the task if a stack overflow occured.
 *
 * @return void    // Actually never returns...
 **********************************************************************/
void __attribute__((__interrupt__)) _StackError(void)
{
  ptrTCB thisTCB;

  kernelPanic = id_tsk_run;
  thisTCB = GetSpecificTCB(kernelPanic);
  thisTCB->State = SUSPENDED;
  asm ("mov SPLIM,w14" : : );
  asm ("mov SPLIM,w15" : : );
  asm ("sub w14,#30,w14" : : );
  asm ("sub w15,#30,w15" : : );
  SPLIM = 0x1FFE;
  INTCON1bits.STKERR = 0;
  SRbits.IPL = 0;
  CORCONbits.IPL3 = 0;
  TaskLauncher();
}

/**********************************************************************
 * Find out the next task to be READY and NOT WAITING in the 
 * Round Robin task list.
 * @param  void
 * @return void     
 **********************************************************************/
void ComputeRR(void)
{
  ptrTCB parserTCB, firstRR;
  parserTCB = firstTCB;
  firstRR = NULL;
  while (1)
  {
    if ((parserTCB->Type & ROUND_ROBIN) && (firstRR == NULL))
      firstRR = parserTCB;
    if (parserTCB->State & RR_RUNNING)
    {
      parserTCB->State &= ~RR_RUNNING;
      if (parserTCB->next == parserTCB)
      {
        firstRR->State |= RR_RUNNING;
        return;
      }
      else
      {
        parserTCB = parserTCB->next;
        if (parserTCB->Type & ROUND_ROBIN)
        {
          parserTCB->State |= RR_RUNNING;
          return;
        }
        else
        {
          firstRR->State |= RR_RUNNING;
          return;
        }
      }
    }
    if (parserTCB->next == parserTCB)
      return;
    else
      parserTCB = parserTCB->next; 
  }
}


/* End of File : pro_man.c */
