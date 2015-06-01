/**********************************************************************/
/*                                                                    */
/* File name: pro_man.h                                               */
/*                                                                    */
/* Since:     2005-April-18                                           */
/*                                                                    */
/* Version:   PICos v1.05 - RTOS for PIC24/30/33 families.            */
/*            Copyright (C) 2005-2006 Pragmatec.                      */
/*                                                                    */
/* Author:    Designed by Pragmatec S.A.R.L.        www.pragmatec.net */
/*            MONTAGNE Xavier [XM]      xavier.montagne@pragmatec.net */
/*                                                                    */
/* Purpose:   Process manager.                                        */
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

#ifndef __PRO_MAN_H__
#define __PRO_MAN_H__

/**********************************************************************
 * ------------------------ Type definition ---------------------------
 **********************************************************************/
typedef unsigned int  TaskType;
typedef unsigned int *TaskRefType;
typedef unsigned int  TaskStateType;
typedef unsigned int *TaskStateRefType;

typedef struct _TCB {
  unsigned int *Stack_register;
  unsigned int *Frame_register;
  void(*StartAddress)(void);
  unsigned int *StackAddress;
  unsigned int StackSize;
  unsigned int TaskID;
  unsigned int Priority;
  unsigned int EventWaited;
  unsigned int EventReceived;
  unsigned int State;
  unsigned int Type;
  unsigned int Time;
  unsigned int kernelState_copy;
  struct _TCB *next;	
} TCB, *ptrTCB;

typedef unsigned int  StatusType;
typedef unsigned int  AppModeType;

typedef unsigned int  ResourceType;

typedef struct _Resource {
  unsigned int  priority;
  unsigned int  Taskprio;
  unsigned char lock;
} Resource;

/**********************************************************************
 * ----------------------------- Defines ------------------------------
 **********************************************************************/
#define TASK(TaskName)                  void TaskName(void)
#define DeclareTask(TaskIdentifier)	extern TASK(TaskIdentifier)
#define DeclareRAM(RamStartAddress)     extern ctx_tsk RamStartAddress
#define DeclareStack(TaskAddress)       extern char TaskAddress

#define RUN             0x01
#define STOP            0x02
#define USER            0x04
#define KERNEL          0x08
#define ISR             0x10
#define SERVICES        0x20

/* -------- Error returned by OS services --------- */ 
#define E_OK            0x00
#define E_OS_ACCESS     0x01
#define E_OS_CALLEVEL   0x02
#define E_OS_ID         0x03
#define E_OS_LIMIT      0x04
#define E_OS_NOFUNC     0x05
#define E_OS_RESOURCE   0x06
#define E_OS_STATE      0x07
#define E_OS_VALUE      0x08

#define INVALID_TASK    0x10

/* --------------- General defines ---------------- */
#define FALSE           0x00
#define TRUE            0x01
#define AUTO            0x00
#define DISABLE         0x00
#define ENABLE          0x01
#define MIN_TASK_NB     0x00
#define MAX_TASK_NB     0xFF

/* ----------------- Task states ------------------ */
#define SUSPENDED       0x00
#define READY           0x01
#define	RUNNING         0x02
#define WAITING         0x04
#define RR_RUNNING      0x08

/* ------------------ Task types ------------------ */
#define EXTENDED        0x01
#define BASIC           0x02
#define ROUND_ROBIN     0x04


/**********************************************************************
 * ---------------------- Function prototypes -------------------------
 **********************************************************************/
StatusType GetTaskID (TaskRefType TaskID);
StatusType GetTaskState (TaskType TaskID, TaskStateRefType State); 
StatusType ActivateTask (TaskType TaskID);
StatusType TerminateTask (void);
StatusType ChainTask (TaskType TaskID);
StatusType Schedule(void); 

AppModeType GetActiveApplicationMode(void);
void StartOS(AppModeType Mode);
void ShutdownOS(StatusType Error);

StatusType GetResource(ResourceType ID);
StatusType ReleaseResource(ResourceType ID);

void OrganizeTCB(void);
void InitTCB(void);
ptrTCB GetCurrentTCB(void);
ptrTCB GetNextRunningTCB(void);
ptrTCB GetSpecificTCB(TaskType TaskID);
void TaskLauncher(void);
void ComputeRR(void);


#endif /* __PRO_MAN_H__ */


/* End of File : pro_man.h */
