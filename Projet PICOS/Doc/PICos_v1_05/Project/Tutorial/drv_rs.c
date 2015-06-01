/**********************************************************************/
/*                                                                    */
/* File name: drv_rs.c                                                */
/*                                                                    */
/* Since:     2005-April-18                                           */
/*                                                                    */
/* Version:   PICos v1.05 - RTOS for PIC24/30/33 families.            */
/*            Copyright (C) 2005-2006 Pragmatec.                      */
/*                                                                    */
/* Author:    Designed by Pragmatec S.A.R.L.        www.pragmatec.net */
/*            MONTAGNE Xavier [XM]      xavier.montagne@pragmatec.net */
/*                                                                    */
/* Purpose:   Manage RS232 generic buffers through the dsPIC USART.   */
/*            Can send ASCII (with printf) or binary string of data.  */
/*            No hardware FIFO management.                            */
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
/* 2006/06/02 [BE]  Defines for use with PIC24/33 UART1/2             */
/*                                                                    */
/**********************************************************************/

#include "define.h"
#include "drv_rs.h"

#include <stdio.h>
#include <string.h>

/* Callback function dedicated to printf output interface */
int _user_putc (char c);

/**********************************************************************
 * Definition dedicated to the local functions.
 **********************************************************************/
// PIC24 settings example
#define SYSCLK       16000000    // External oscillator frequency / 2
#define BRGx_19200         51    // BRGx values for Baudrate at SYSCLK
#define BRGx_38400         25    // BRGx values for Baudrate at SYSCLK
#define BRGx_115200         8    // BRGx values for Baudrate at SYSCLK

// PIC33 settings example
/*
#define SYSCLK       40000000    // External oscillator frequency / 2
#define BRGx_19200        129    // BRGx values for Baudrate at SYSCLK
#define BRGx_38400         64    // BRGx values for Baudrate at SYSCLK
#define BRGx_115200        21    // BRGx values for Baudrate at SYSCLK
*/

// Generic Defines for UART1/2 Registers and/or Bits used here
// UART1 is Default, wenn _UART2_ not defined
#ifdef _UART2_
#define UART_TX_TRIS		TRISFbits.TRISF5
#define UART_RX_TRIS		TRISFbits.TRISF4
#define UxBRG				U2BRG
#define UxMODE				U2MODE
#define UxMODEbits		U2MODEbits
#define UxSTA				U2STA
#define UxSTAbits			U2STAbits
#define IFSxbitsUxRXIF	IFS1bits.U2RXIF
#define IFSxbitsUxTXIF	IFS1bits.U2TXIF
#define IPCxbitsUxTXIP	IPC7bits.U2TXIP
#define IPCxbitsUxRXIP	IPC7bits.U2RXIP
#define IECxbitsUxTXIE	IEC1bits.U2TXIE
#define IECxbitsUxRXIE	IEC1bits.U2RXIE
#define UxTXREG			U2TXREG
#define UxRXREG			U2RXREG
#define _UxTXInterrupt	_U2TXInterrupt
#define _UxRXInterrupt	_U2RXInterrupt
#else
#define UART_TX_TRIS		TRISFbits.TRISF3
#define UART_RX_TRIS		TRISFbits.TRISF2
#define UxBRG				U1BRG
#define UxMODE				U1MODE
#define UxMODEbits		U1MODEbits
#define UxSTA				U1STA
#define UxSTAbits			U1STAbits
#define IFSxbitsUxRXIF	IFS0bits.U1RXIF
#define IFSxbitsUxTXIF	IFS0bits.U1TXIF
#define IPCxbitsUxTXIP	IPC3bits.U1TXIP
#define IPCxbitsUxRXIP	IPC2bits.U1RXIP
#define IECxbitsUxTXIE	IEC0bits.U1TXIE
#define IECxbitsUxRXIE	IEC0bits.U1RXIE
#define UxTXREG			U1TXREG
#define UxRXREG			U1RXREG
#define _UxTXInterrupt	_U1TXInterrupt
#define _UxRXInterrupt	_U1RXInterrupt
#endif

EventMaskType   RS_event;

RS_message_tRef	RS_list_head;			//	Start of message queue
RS_message_tRef	RS_current_message;		//	Current message
unsigned char	RS_list_count = 0;		//	Number of items currently in queue

RS_message_tRef	RS_list_head_rcv;		//	Start of message queue
unsigned char	RS_list_count_rcv = 0;	//	Number of items currently in queue

unsigned int    TX_ByteNumber = 0;
unsigned int    RX_ByteNumber = 0;

/**********************************************************************
 * ----------------- RS DRIVER - dsPIC USART MANAGEMENT ---------------
 *
 * void
 *
 **********************************************************************/
TASK(RS_Drv)
{  
	// Set directions of UART IOs
	UART_TX_TRIS = 0;
	UART_RX_TRIS = 1;

	// Set Baudrate for SysClk
	UxBRG = BRGx_38400;
	// Set Low Baudrate, No Parity, 1 Stopbit, then enable UART
	UxMODE = 0;
	UxMODEbits.UARTEN = 1;

	// Set default Status, then enable Transmit
	UxSTA = 0;
	UxSTAbits.UTXEN = 1;

	// reset RX flag
	IFSxbitsUxRXIF = 0;

	// Interrupt enabled
	IECxbitsUxTXIE = 0;		// Activated by the driver itself
	IECxbitsUxRXIE = 1;

	// Interrupt priority
	IPCxbitsUxTXIP = 5;
	IPCxbitsUxRXIP = 5;


	RS_current_message = NULL;

	while(1)
	{
		WaitEvent(RS_NEW_MSG | RS_RCV_MSG);
		GetEvent(RS_DRV_ID, &RS_event);
    
		if (RS_event & RS_RCV_MSG)
			ClearEvent(RS_RCV_MSG);

		if (RS_event & RS_NEW_MSG)
		{
			ClearEvent(RS_NEW_MSG);
			if (IECxbitsUxTXIE == 0)
			{
				if (RS_current_message != NULL)
					{
						RS_current_message->length = 0;
						SetEvent(RS_current_message->CallerID, RS_QUEUE_EMPTY);
					}
				RS_current_message = RS_deqMsg();
				if (RS_current_message != NULL)
				{
					TX_ByteNumber = 0;	
					UxSTAbits.UTXEN = 1;
					while(UxSTAbits.UTXBF);
					UxTXREG = (unsigned int)(RS_current_message->data[0]);
					TX_ByteNumber++;
					IECxbitsUxTXIE = 1;
				}
			}
		}
	}
}

/**********************************************************************
 *	Enqueue a client packet object into the RS task queue.
 *
 *	Once placed in the queue, the client must not modify the data, 
 *	otherwise unpredictable results can occur.
 * To safely change the object, dequeue, modify, re-enqueue.
 *
 * The code in mainly executed in critical region [SuspendAllInterrupt]
 * because many tasks can call this function at the same time and break
 * the FIFO list.
 *
 * @param  toEnqueue  IN  New message structure reference
 * @param  data       IN  Buffer which contains the string of data
 * @param  length     IN  Length of the Buffer field
 * @return Status         E_OK if toEnqueue is well attached to the list
 *                        E_OS_STATE if message could not be enqueued
 **********************************************************************/
StatusType RS_enqMsg(RS_message_tRef toEnqueue, unsigned char *data, unsigned int length)
{
	RS_message_tRef RS_list_itor;
	ptrTCB thisTCB;
	thisTCB = GetCurrentTCB();

	if (toEnqueue != NULL)
	{
		SuspendOSInterrupts();
		if (RS_list_head == NULL)
			RS_list_head = toEnqueue;
		else
		{
			RS_list_itor = RS_list_head;
			while (RS_list_itor->next != NULL)
				RS_list_itor = RS_list_itor->next;
			RS_list_itor->next = toEnqueue;
		}
		toEnqueue->next     = NULL;
		toEnqueue->CallerID = (TaskType)thisTCB->TaskID;
		toEnqueue->length   = length;
		toEnqueue->data     = data;
		RS_list_count++;
		ResumeOSInterrupts();
		return E_OK;
	}
	else
		return E_OS_STATE;
}

/**********************************************************************
 *	Dequeue a client message from the RS task queue.
 *
 * @param  void
 * @return RS_list_itor   Reference to the top of the FIFO list
 *                        (next enqueued message)
 *********************************************************************/
RS_message_tRef RS_deqMsg(void)
{
	RS_message_tRef RS_list_itor;

	RS_list_itor = NULL;
	if (RS_list_head != NULL)
	{
		RS_list_itor = RS_list_head;
		RS_list_head = RS_list_head->next;
		RS_list_count--;
	}
	return RS_list_itor;
} 

/**********************************************************************
 *	Enqueue a client packet object into the RS task queue.
 *
 *	Once placed in the queue, the client must not modify the data,
 *	otherwise unpredictable results. 
 *	To safely change the object, dequeue, modify, re-enqueue.
 *
 * The code in mainly executed in critical region [SuspendAllInterrupt]
 * because many tasks can call this function at the same time and break
 * the FIFO list.
 *
 * @param  toEnqueue  IN  New message structure reference
 * @return Status         E_OK if toEnqueue is well attached to the list
 *                        E_OS_STATE if message could not be enqueued
 **********************************************************************/
StatusType RS_RCV_Register(RS_message_tRef toEnqueue, unsigned char *data, 
                           unsigned int length, unsigned char pattern)
{
	RS_message_tRef RS_list_itor;
	ptrTCB thisTCB;
	thisTCB = GetCurrentTCB();

	if (toEnqueue != NULL)
	{
		SuspendOSInterrupts();
		if (RS_list_head_rcv == NULL)
			RS_list_head_rcv = toEnqueue;
		else
		{
			RS_list_itor = RS_list_head_rcv;
			while (RS_list_itor->next != NULL)
				RS_list_itor = RS_list_itor->next;
			RS_list_itor->next = toEnqueue;
		}
		toEnqueue->next     = NULL;
		toEnqueue->CallerID = (TaskType)thisTCB->TaskID;
		toEnqueue->length   = length;
		toEnqueue->data     = data;
		toEnqueue->pattern  = pattern;
		RS_list_count_rcv++;
		RX_ByteNumber = 0;
		ResumeOSInterrupts();
		return E_OK;
	}
	else
		return E_OS_STATE;
}

/**********************************************************************
 *	Fill the structure with the printf content.
 *
 * The code in mainly executed in critical region [SuspendAllInterrupt]
 * because many tasks can call this function at the same time and break
 * the FIFO list.
 *
 * @param  c          IN  Next character coming from the printf function
 * @return ByteNumber     Current char index in the buffer
 *********************************************************************/
int _user_putc (char c)
{
	RS_message_tRef RS_list_itor;
	ptrTCB thisTCB;
	thisTCB = GetCurrentTCB();

	SuspendOSInterrupts();
	RS_list_itor = RS_list_head;
	while (RS_list_itor->CallerID != (TaskType)thisTCB->TaskID)
	{
		if (RS_list_itor->next == NULL)
			return 0;
		RS_list_itor = RS_list_itor->next;
	}
	RS_list_itor->data[RS_list_itor->length] = c;
	RS_list_itor->length++;
	ResumeOSInterrupts();
	return RS_list_itor->length;
}

/**********************************************************************
 * Part of the transmiter interrupt routine.
 *
 * @param  void
 * @return void 
 **********************************************************************/
void __attribute__((__interrupt__)) _UxTXInterrupt(void)
{   
	EnterISR();
	IFSxbitsUxTXIF = 0;
	if ((TX_ByteNumber > 0) && (TX_ByteNumber < RS_current_message->length))
	{
		while(UxSTAbits.UTXBF);
		UxTXREG = RS_current_message->data[TX_ByteNumber];
	}
	if (TX_ByteNumber  == (RS_current_message->length))
	{
		IECxbitsUxTXIE = 0;
		SetEvent(RS_DRV_ID, RS_NEW_MSG);
		TX_ByteNumber = -1;
	}
	TX_ByteNumber++;
	LeaveISR();
	return;
}

/**********************************************************************
 * Part of the receiver interrupt routine.
 *
 * @param  void
 * @return void 
 **********************************************************************/
void __attribute__((__interrupt__)) _UxRXInterrupt(void)
{
	EnterISR();
	IFSxbitsUxRXIF = 0;
	// A length is expected
	if (RS_list_head_rcv->length != 0)
	{
		if ((RX_ByteNumber >= 0) && 
			(RX_ByteNumber < RS_list_head_rcv->length))
			RS_list_head_rcv->data[RX_ByteNumber] = UxRXREG;
		if (RX_ByteNumber == (RS_list_head_rcv->length) - 1)
		{
			SetEvent(RS_list_head_rcv->CallerID, RS_QUEUE_FULL);
			RX_ByteNumber = -1;
		}
		RX_ByteNumber++;
	}
	// A special character is expected
	else
	{
		RS_list_head_rcv->data[RX_ByteNumber] = UxRXREG;
		if (RS_list_head_rcv->pattern == RS_list_head_rcv->data[RX_ByteNumber])
		{
			SetEvent(RS_list_head_rcv->CallerID, RS_QUEUE_FULL);
			RX_ByteNumber = -1;
		}
		RX_ByteNumber++;
	}
	LeaveISR();
	return;
}

/* End of file : drv_rs.c */
