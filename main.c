#include "C8051F340.h"

unsigned int contagemOverflows = 0;

void configTimer (void);
void reloadTimer (void);
void configDevice (void);
void configInt(void);


void main(){

    unsigned char estado=0;

    configTimer();
	configDevice ();

    while(1){
	switch(estado){

		case 0:
		    if(P1_0){		//sensor para detetar peças
		        estado=1;  
		    }
			TR0=0; //desliga o timer quando regressa de estados mais elevados
			P2_0=0;
			contagemOverflows = 0;
			reloadTimer();
		break;
		

		case 1:
		    TR0=1; //inicia o timer 
		    if(contagemOverflows == 10){ //10ms
			
			if(P1_0){		//continua a detetar, validamos como peça.
				estado = 2;
					
			}else{
				estado = 0;
				}
			}
		    	
		break;

		case 2:
			if(contagemOverflows == 2310){ //2.3 seg + o tempo que ja passou no case anterior
				estado =3;
				}
		break;

		case 3:
			P2_0=1;
			if(contagemOverflows == 2810){ // 500 ms mais o tempo que passou nos cases anteriores

				estado=0;
				
			}

		break;

		default:	
			estado=0;


}
}

}


void configTimer(void){

	TMOD = 0b00000001; //fazer o exercicio com apenas o timer 0
	reloadTimer();
}

void reloadTimer (void){

	unsigned int valorInicial = 65536 - 50000;
	
	TL0 = valorInicial;
	TH0 = (valorInicial >>8);
	TF0 = 0; 

}
void configDevice (void){

	P2MDOUT = 0x01;  		//define P2 como I/O digitalem push pull

	XBR1 = 0x40;			//permite a crossbar
	OSCICN = 0x83;			// configura oscilador interno para 12 MHz

	PCA0MD &= 0xBF;			//estas duas fazem disable do watchdog
	PCA0MD = 0x00;
}
void configInt(void){

	IE=0b10000010;		//timer 0 configurado como fonte de interrupção

}

void RSI_timer0(void) __interrupt(1){

	reloadTimer();    //o timer não tem auto reload.
	contagemOverflows++;  //contador
}