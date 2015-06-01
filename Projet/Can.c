#include "can.h"

unsigned int nbval_CAN = 0;
unsigned int RD_CAN = 0;
unsigned int WR_CAN = 0; 
unsigned int CAN_error = CAN_noerror;
CAN_frame CANbuf[taille_CAN];

void init_CAN(){

	C1CTRLbits.REQOP = 0b100; // passage en mode config
	while(C1CTRLbits.OPMODE != 0b100); // Attendre que le changement de mode se fasse
	C1CTRLbits.CANCKS = 1;
	C1CFG1bits.BRP = 1; // BRP = ((TQ*Fcy)/2)-1
	C1CFG1bits.SJW = 1; // SJW time is 2xTQ
	C1CFG2bits.PRSEG = 6; // 7 TQ
	C1CFG2bits.SEG1PH = 5; // 6TQ
	C1CFG2bits.SEG2PHTS = 1; // Freely programable
	C1CFG2bits.SEG2PH = 5; // 6TQ
	C1CFG2bits.SAM = 1; // 3pts d'echantillonnage

	unsigned long EIDFilter = 0x10030200;
	unsigned long EIDMask = 0xFFFFFFFFF;

	C1RXF0SID = EIDFilter >> 16 ;
	C1RXF0SIDbits.EXIDE = 1;
	C1RXF0EIDH = EIDFilter >> 6;
	C1RXF0EIDLbits.EID5_0 = EIDFilter;

	C1RXM0SID = EIDMask >> 16 ;
	C1RXM0SIDbits.MIDE = 1;
	C1RXM0EIDH = EIDMask >> 6;
	C1RXM0EIDLbits.EID5_0 = EIDMask ;
	

	C1INTE = 0x0001; // enable RX0IE
	IEC1bits.C1IE = 1;
	
	C1CTRLbits.REQOP = 0b000; //passage en mode nomal
	while(C1CTRLbits.OPMODE != 0b000); // Attendre que le changement de mode se fasse

}

unsigned char envoi_CAN(CAN_frame CAN_mes){
	
	if(C1TX0CONbits.TXREQ == 1) return 0;
	
	// initialisation de la trame
	C1TX0DLCbits.EID5_0 = CAN_mes.STDID;
	C1TX0EIDbits.EID13_6 = CAN_mes.STDID >> 6;
	C1TX0EIDbits.EID17_14 = CAN_mes.STDID >> 14;
	C1TX0SIDbits.SID5_0 = CAN_mes.STDID >> 18;
	C1TX0SIDbits.SID10_6 = CAN_mes.STDID >> 24;

	C1TX0SIDbits.TXIDE = 1; // Extended ID
	C1TX0SIDbits.SRR = 1; // Substitute RTR, set to 1 when TXIDE = 1
	C1TX0DLCbits.TXRTR = 0; // RTR : trame de donnees (car 0)
	C1TX0DLCbits.TXRB0 = 0; // substitute bit
	C1TX0DLCbits.TXRB1 = 0; // Substitute bit
	C1TX0DLCbits.DLC = CAN_mes.length; // Nombre d'octets

	// Concatenation de la data car data sur 16 bits (2 blocs de 8)
	C1TX0B1 = CAN_mes.data;

	C1TX0CONbits.TXREQ = 1; // Program resets here ??

	return 1;

}

unsigned char requete_CAN(unsigned long STDID){
	
	if(C1TX0CONbits.TXREQ == 1) return 0; // précédent message pas encore transmis
	
	// initialisation de la trame
	C1TX0DLCbits.EID5_0 = STDID;
	C1TX0EIDbits.EID13_6 = STDID >> 6;
	C1TX0EIDbits.EID17_14 = STDID >> 14;
	C1TX0SIDbits.SID5_0 = STDID >> 18;
	C1TX0SIDbits.SID10_6 = STDID >> 24;

	C1TX0SIDbits.TXIDE = 1; // Extended ID
	C1TX0SIDbits.SRR = 1; // Substitute RTR, set to 1 when TXIDE = 1
	C1TX0DLCbits.TXRTR = 1; // RTR : trame de donnees (car 0)
	C1TX0DLCbits.TXRB0 = 0; // substitute bit
	C1TX0DLCbits.TXRB1 = 0; // Substitute bit
	C1TX0DLCbits.DLC = 0;

	C1TX0CONbits.TXREQ = 1;

	return 1;
}


unsigned char CAN_recu(void) {
	return(nbval_CAN > 0);
}

void lecture_CAN(CAN_frame *t) {
	while(!CAN_recu());
	
	CAN_frame CAN_request;
	CAN_request.length = 1;	
	*t = CANbuf[RD_CAN++];

	switch(t->data){
	case 0:
		LATBbits.LATB1 = ~LATBbits.LATB1; //toggling de la led

		CAN_request.STDID = 0x10520110; // port A
		CAN_request.data = 0x13;

		break;
	case 1:
		
		CAN_request.STDID = 0x10520111; // port B
		CAN_request.data = 0x62;

		break;
	case 2:

		CAN_request.STDID = 0x10520112; // port C
		CAN_request.data = 0x13;

		break;
	}

		envoi_CAN(CAN_request);
		while (C1TX0CONbits.TXREQ == 1);
		requete_CAN(CAN_request.STDID);

	if(RD_CAN >= taille_CAN)
		RD_CAN=0;
	
	nbval_CAN--;
}

void __attribute__((__interrupt__)) _C1Interrupt(void) {
	IFS1bits.C1IF = 0; //RAZ flag interruption CAN1
	LATBbits.LATB0 = ~LATBbits.LATB0; //toggling de la led
	if(C1INTFbits.RX0IF) {		
		if(nbval_CAN < taille_CAN) {
			
			// On enregistre l'ID recu
			CANbuf[WR_CAN].STDID  = C1RX0SIDbits.SID << 18;
			CANbuf[WR_CAN].STDID  += C1RX0EID << 6;
			CANbuf[WR_CAN].STDID  += C1RX0DLCbits.EID5_0 ;
		
			CANbuf[WR_CAN].length = C1RX0DLCbits.DLC ;
			
			CANbuf[WR_CAN].data = C1RX0B1 ;
			
			WR_CAN++ ;

			if(WR_CAN >= taille_CAN )
			{
				WR_CAN = 0 ;
			}

			nbval_CAN++ ;
		} else{
			CAN_error |= CAN_buf_error;
		}
		
		C1INTFbits.RX0IF = 0 ; // RAZ des flags (0 <=> request has not occured)
		C1RX0CONbits.RXFUL = 0 ; // RXFUL = 0 indicates that RXB1 is empty, RXFUL = 1 indicates that RXB0 is full
		
	}
	
	if(C1INTFbits.ERRIF){
		CAN_error |= CAN_RxTx_error; // signalisation de l'erreur
		C1INTFbits.ERRIF = 0;
	}
}
