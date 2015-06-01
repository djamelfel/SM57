// Note : we use the values contained in the TD2

#ifndef __CAN_HEADER__
#define __CAN_HEADER__

#include "system.h"
#include <libpic30.h>

#define taille_CAN 10
#define CAN_noerror 0x0000
#define CAN_buf_error 0x0001
#define CAN_RxTx_error 0x0002
#define BAUDRATE 250000 // 250 kbits/s ?

// FCY = 10 000 000*8/4 = 20 000 000 = FCAN
// Dans le TD, Fcy = 29.4912 MHz ?
// Nominal bit time = 20 TQ
// Sauts de resynchronisation = 2TQ
// Segment de phase 1 = 6TQ
// Segment de phase 2 = 6TQ
// Echantillonnage en 3 points
// Segment de propagation = 7 TQ

typedef struct CAN_frame{
	unsigned long STDID;
	unsigned int length;
	unsigned char data;
} CAN_frame;

void init_CAN();
unsigned char requete_CAN(unsigned long);
unsigned char envoi_CAN(CAN_frame);

void __attribute__((__interrupt__)) _C1Interrupt(void);
unsigned char CAN_recu(void);
void lecture_CAN(CAN_frame*);

#endif
