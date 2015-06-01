/**********************************************************************/
/*                                                                    */
/* File name: tascdesc.c                                              */
/*                                                                    */
/* Since:     2005-April-18                                           */
/*                                                                    */
/* Version:   PICos v1.05 - RTOS for PIC24/30/33 families.            */
/*            Copyright (C) 2005-2006 Pragmatec.                      */
/*                                                                    */
/* Author:    Designed by Pragmatec S.A.R.L.        www.pragmatec.net */
/*            MONTAGNE Xavier [XM]      xavier.montagne@pragmatec.net */
/*                                                                    */
/* Purpose:   Kind of OIL file where all the features of the tasks    */
/*            are described.                                          */
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
 * --------------------- COUNTER & ALARM DEFINITION -------------------
 **********************************************************************/
volatile Counter Counter_list[] = 
  {
   /*******************************************************************
    * -------------------------- First counter ------------------------
    *******************************************************************/
   {
     {
       65535,                               /* maxAllowedValue        */
           1,                               /* ticksPerBase           */
           0                                /* minCycle               */
     },
     0,                                     /* CounterValue           */
     0                                      /* Nbr of Tick for 1 CPT  */
   }
  };

volatile Counter Counter_kernel = 
  {
    {
      65535,                               /* maxAllowedValue        */
          1,                               /* ticksPerBase           */
          0                                /* minCycle               */
    },
    0,                                     /* CounterValue           */
    0                                      /* Nbr of Tick for 1 CPT  */
  };

volatile AlarmObject Alarm_list[] = 
  {
   /*******************************************************************
    * -------------------------- First alarm ---------------------------
    *******************************************************************/
   {
     OFF,                                  /* State                   */
     0,                                    /* AlarmValue              */
     0,                                    /* Cycle                   */
     (RefCounter)&Counter_kernel,          /* ptrCounter              */
     TASK0_ID,                             /* TaskID2Activate         */
     ALARM_EVENT,                          /* EventToPost             */
     NULL                                  /* CallBack                */
   },
};

#define _ALARMNUMBER_          sizeof(Alarm_list)/sizeof(AlarmObject)
#define _COUNTERNUMBER_        sizeof(Counter_list)/sizeof(Counter)
unsigned char ALARMNUMBER    = _ALARMNUMBER_;
unsigned char COUNTERNUMBER  = _COUNTERNUMBER_;
unsigned long global_counter = 0;

/**********************************************************************
 * ----------------------- RESOURCE DEFINITION ------------------------
 **********************************************************************/
volatile Resource Resource_list[] = 
  {
   /* ADC for example */
   {
      10,                                /* priority           */
       0,                                /* Task prio          */
       0,                                /* lock               */
   },
  };

#define _RESOURCENUMBER_       sizeof(Resource_list)/sizeof(Resource)
unsigned char RESOURCENUMBER = _RESOURCENUMBER_;


/**********************************************************************
 * ----------------------- TASK & STACK DEFINITION --------------------
 **********************************************************************/
#define DEFAULT_STACK_SIZE     64  // Warning : in 16 bits not 8 bits !
DeclareTask(TASK0);
DeclareTask(TASK1);
DeclareTask(TASK2);
DeclareTask(TASK3);
DeclareTask(TASK4);

u_int __attribute__((__section__(".stack_tsk0"))) stack0[DEFAULT_STACK_SIZE];
u_int __attribute__((__section__(".stack_tsk1"))) stack1[DEFAULT_STACK_SIZE];
u_int __attribute__((__section__(".stack_tsk2"))) stack2[DEFAULT_STACK_SIZE];
u_int __attribute__((__section__(".stack_tsk3"))) stack3[DEFAULT_STACK_SIZE];
u_int __attribute__((__section__(".stack_tsk4"))) stack4[128];

/**********************************************************************
 * ---------------------- TASK DESCRIPTOR SECTION ---------------------
 **********************************************************************/
const unsigned int descromarea;
const TCB TCB_const_list[] = 
  {
   /*******************************************************************
    * -------------------------- Task 0 -------------------------------
    *******************************************************************/
   {
     SREG(stack0),                         /* Stack_register          */
     FREG(stack0),                         /* Frame_register          */
     TASK0,                                /* StartAddress            */
     stack0,                               /* StackAddress            */
     sizeof(stack0),                       /* StackSize               */
     TASK0_ID,                             /* TaskID                  */
     TASK0_PRIO,                           /* Priority                */
     NONE,                                 /* EventWaited             */
     NONE,                                 /* EventReceived           */
     READY,                                /* State                   */
     EXTENDED,                             /* Type                    */
     0,                                    /* Time                    */ 
     0,                                    /* kernelState_copy        */ 
     NULL                                  /* next                    */	
   },
   /*******************************************************************
    * -------------------------- Task 1 -------------------------------
    *******************************************************************/
   {
     SREG(stack1),                         /* Stack_register          */
     FREG(stack1),                         /* Frame_register          */
     TASK1,                                /* StartAddress            */
     stack1,                               /* StackAddress            */
     sizeof(stack1),                       /* StackSize               */
     TASK1_ID,                             /* TaskID                  */
     TASK1_PRIO,                           /* Priority                */
     NONE,                                 /* EventWaited             */
     NONE,                                 /* EventReceived           */
     READY,                                /* State                   */
     EXTENDED,                             /* Type                    */
     0,                                    /* Time                    */ 
     0,                                    /* kernelState_copy        */ 
     NULL                                  /* next                    */	
   },
   /*******************************************************************
    * -------------------------- Task 2 -------------------------------
    *******************************************************************/
   {
     SREG(stack2),                         /* Stack_register          */
     FREG(stack2),                         /* Frame_register          */
     TASK2,                                /* StartAddress            */
     stack2,                               /* StackAddress            */
     sizeof(stack2),                       /* StackSize               */
     TASK2_ID,                             /* TaskID                  */
     TASK2_PRIO,                           /* Priority                */
     NONE,                                 /* EventWaited             */
     NONE,                                 /* EventReceived           */
     SUSPENDED,                            /* State                   */
     BASIC,                                /* Type                    */
     0,                                    /* Time                    */ 
     0,                                    /* kernelState_copy        */ 
     NULL                                  /* next                    */	
   },
   /*******************************************************************
    * -------------------------- Task 3 -------------------------------
    *******************************************************************/
   {
     SREG(stack3),                         /* Stack_register          */
     FREG(stack3),                         /* Frame_register          */
     TASK3,                                /* StartAddress            */
     stack3,                               /* StackAddress            */
     sizeof(stack3),                       /* StackSize               */
     TASK3_ID,                             /* TaskID                  */
     TASK3_PRIO,                           /* Priority                */
     NONE,                                 /* EventWaited             */
     NONE,                                 /* EventReceived           */
     SUSPENDED,                            /* State                   */
     BASIC,                                /* Type                    */
     0,                                    /* Time                    */ 
     0,                                    /* kernelState_copy        */ 
     NULL                                  /* next                    */	
   },
   /*******************************************************************
    * -------------------------- Task 4 -------------------------------
    *******************************************************************/
   {
     SREG(stack4),                         /* Stack_register          */
     FREG(stack4),                         /* Frame_register          */
     TASK4,                                /* StartAddress            */
     stack4,                               /* StackAddress            */
     sizeof(stack4),                       /* StackSize               */
     TASK4_ID,                             /* TaskID                  */
     TASK4_PRIO,                           /* Priority                */
     NONE,                                 /* EventWaited             */
     NONE,                                 /* EventReceived           */
     READY,                                /* State                   */
     EXTENDED,                             /* Type                    */
     0,                                    /* Time                    */ 
     0,                                    /* kernelState_copy        */ 
     NULL                                  /* next                    */	
   },
  };

volatile unsigned int  taskdesc_addr = (unsigned int)(TCB_const_list);

#define  _TASKNUMBER_          sizeof(TCB_const_list)/sizeof(TCB)
unsigned char TASKNUMBER    = _TASKNUMBER_;
volatile TCB                   TCB_list[_TASKNUMBER_];


	
/* End of File : taskdesc.c */
