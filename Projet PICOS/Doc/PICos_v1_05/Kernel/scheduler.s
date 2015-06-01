/**********************************************************************/
/*                                                                    */
/* File name: scheduler.s                                             */
/*                                                                    */
/* Since:     2005-April-18                                           */
/*                                                                    */
/* Version:   PICos v1.05 - RTOS for PIC24/30/33 families.            */
/*            Copyright (C) 2005-2006 Pragmatec.                      */
/*                                                                    */
/* Author:    Designed by Pragmatec S.A.R.L.        www.pragmatec.net */
/*            MONTAGNE Xavier [XM]      xavier.montagne@pragmatec.net */
/*                                                                    */
/* Purpose:   Scheduler main routines & Interrupt Service Routine.    */
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


; disable interrupts
; bset SR, #7
; bset SR, #6
; bset SR, #5
; => prefer the DISI instruction !
; TODO : translate in C code in possible

; enable interrupts
; bclr SR, #5
; bclr SR, #6
; bclr SR, #7
; done by retfie !!!

;  .text
;  .global _T1_ISR
_T1_ISR:
;  bclr.b IFS0,#3
;  call ISR
;  retfie


;  .text
  .global _SAVE_TSK_CTX
_SAVE_TSK_CTX:
  mov.w w0,[w15++]
  mov.w w1,[w15++]
  mov.w w2,[w15++]
  mov.w w3,[w15++]
  mov.w w4,[w15++]
  mov.w w5,[w15++]
  mov.w w6,[w15++]
  mov.w w7,[w15++]
  mov.w w8,[w15++]
  mov.w w9,[w15++]
  mov.w w10,[w15++]
  mov.w w11,[w15++]
  mov.w w12,[w15++]
  mov.w w13,[w15++]
  mov.w SR,w0
  mov.w w0,[w15++]
  
  mov.w _currentTCB,w7
  mov.w w15,[w7++]
  mov.w w14,[w7]
  return

  .text
  .global _ordo
_ordo:
  mov.w #0x00,w0
  cp.w _W15_KERNEL
  bra nz, _Launcher
  mov.w w15, _W15_KERNEL
  mov.w w14, _W14_KERNEL
_Launcher:
  call _TaskLauncher
  return

  .text
  .global _KERNELJUMP
_KERNELJUMP:
  mov.w _W15_KERNEL,w15
  mov.w _W14_KERNEL,w14
  bra _ordo

  .text
  .global _SCHEDULE
_SCHEDULE:
  disi #3         ; disable interrupts
  bset  SR, #5
  bset  SR, #6
  bset  SR, #7
  mov.w w0,[w15++]
  mov.w w1,[w15++]
  mov.w w2,[w15++]
  mov.w w3,[w15++]
  mov.w w4,[w15++]
  mov.w w5,[w15++]
  mov.w w6,[w15++]
  mov.w w7,[w15++]
  mov.w w8,[w15++]
  mov.w w9,[w15++]
  mov.w w10,[w15++]
  mov.w w11,[w15++]
  mov.w w12,[w15++]
  mov.w w13,[w15++]
  mov.w SR,w0
  mov.w w0,[w15++]
  
  mov.w _currentTCB,w7
  mov.w w15,[w7++]
  mov.w w14,[w7]
  bset  _kernelState, #3; KERNEL mode ON
  bclr  _kernelState, #2; USER   mode OFF

  mov   #0x1FFE,w0
  mov   w0,_SPLIM
  mov.w _W15_KERNEL,w15
  mov.w _W14_KERNEL,w14
  disi  #3
  bclr  SR, #5      ; enable interrupts
  bclr  SR, #6
  bclr  SR, #7
  bra   _ordo


  .end 

;/* End of File :  scheduler.s */

