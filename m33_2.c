#include "C8051F340.h"

#define START P2_6
#define RED P2_3
#define GREEN P2_4
#define BLUE P2_5
#define PORTA P1_6

unsigned char indexRecebe =0;
__data __at(0x40) unsigned char recebe[5]={0,0,0,0,0};
unsigned char seRecebeu=0;
unsigned char seEnviou=0;

void configDevice (void);
void configInt(void);



void main(){

	unsigned char estado =0;

	configDevice ();
	configInt();


	while (1){

		if(estado ==0){

			if(recebe[indexRecebe]=='S'){
				indexRecebe++;
				estado = 1;
				seRecebeu=0;
				}
				
			PORTA=0;	
		}//-------------------------------------------------------------------------

		if(estado ==1){

			if(seRecebeu){
			   	seRecebeu=0;
				if(recebe[indexRecebe]=='R'){
				
					if(RED){
						estado = 2;
					
					}else{
						estado = 0;
						seRecebeu=0;
				}
			}
			}
		

		}//---------------------------------------------------------------------------------

		if(estado ==2){

			if(seRecebeu){
				seRecebeu =0;
				if(recebe[indexRecebe]=='B'){
					estado = 3;
					indexRecebe++;
					
				}else{
					estado = 0;
					indexRecebe=0;
				}

			}
		

		}//-----------------------------------------------------------------------------------


		

		if(estado ==3){


				if(seRecebeu){
				seRecebeu =0;
				if(recebe[indexRecebe]=='G'){
					estado = 4;
					indexRecebe++;
					
				}else{
					estado = 0;
					indexRecebe=0;
				}

			}
		

		}//----------------------------------------------------------------------------------

		

		if(estado ==4){

				if(seRecebeu){
				seRecebeu =0;
				if(recebe[indexRecebe]=='R'){
					estado = 5;
					indexRecebe++;
					
				}else{
					estado = 0;
					indexRecebe=0;
				}

			}
		

		}

		

		if(estado ==5){

			if(seEnviou==0){
				
				SBUF=recebe[seEnviou];
				}
			if(seEnviou==5){
				estado=0;
				seEnviou=0;
				PORTA = 1;
			}

	
	}
}
}


void configDevice (void){

	P1MDOUT = 0x40;  		//define P1_6 (porta)

	XBR1 = 0x40;			//permite a crossbar
	OSCICN = 0x83;			// configura oscilador interno para 12 MHz

	PCA0MD &= 0xBF;			//estas duas fazem disable do watchdog
	PCA0MD = 0x00;
}
void configInt(void){

	IE = 0b10010001;		//int 0 configurada
	

}

void RSISerie (void) __interrupt(4){

	if(TI){
		TI=0;
		if(seEnviou<5){
		seEnviou++;
		SBUF=recebe[indexRecebe];
	}
	}
	if(RI==1){
		RI=0;
		seRecebeu=1;
		recebe[indexRecebe] = SBUF;
	}

}

