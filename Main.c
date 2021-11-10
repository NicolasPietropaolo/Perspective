//Distance LED = D4 (PF0)
//Bus speed = 60MHz (PSYDIV = 7)

#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "PLL.h"
#include "SysTick.h"

int dir = 1;
int Angle=64;
int speed = 5;


void PortF_Init(void){ //For the onboard LED
        SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;
        while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R5) == 0){};
        GPIO_PORTF_DIR_R=0b10001;
        GPIO_PORTF_DEN_R=0b10001;
        return;}

void PortE_Init(void){	// For the Motor
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;		  		
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R4) == 0){};	
	GPIO_PORTE_DIR_R |= 0xFF;                   			
	GPIO_PORTE_AFSEL_R &= ~0xFF;     									
	GPIO_PORTE_DEN_R |= 0xFF;
	GPIO_PORTE_AMSEL_R &= ~0xFF;     										
	return;
	}

void PortD_Init(void){ // for the Push_botton
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3;          
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R3) == 0){}; 
	//GPIO_PORTM_DATA_R = 0b0;
	GPIO_PORTD_DIR_R |= 0b0000;       			 			
	GPIO_PORTD_DEN_R |= 0b1000; // D3 is used
	return;
}

void blink(int count) { 												//Flashes D4
		while(count--) {
			GPIO_PORTF_DATA_R ^= 0b1; 								
			SysTick_Wait10ms(20);										
			GPIO_PORTF_DATA_R ^= 0b1;			
		}
}

void spin(int dir, int number_of_steps,int delay) {	
	if(dir == 1){ 				//Rotating CW
		for(int i=0; i<number_of_steps; i++){
				GPIO_PORTE_DATA_R = 0b1001;
				SysTick_Wait10ms(delay);
				GPIO_PORTE_DATA_R = 0b0011;
				SysTick_Wait10ms(delay);
				GPIO_PORTE_DATA_R = 0b0110;
				SysTick_Wait10ms(delay);
				GPIO_PORTE_DATA_R = 0b1100;
				SysTick_Wait10ms(delay);
		}
	}
	else if(dir == 0){		//Rotating CCW
		for(int i=0; i<number_of_steps; i++){
				GPIO_PORTE_DATA_R = 0b00001100;
				SysTick_Wait10ms(delay);
				GPIO_PORTE_DATA_R = 0b0000110;
				SysTick_Wait10ms(delay);
				GPIO_PORTE_DATA_R = 0b00000011;
				SysTick_Wait10ms(delay);
				GPIO_PORTE_DATA_R = 0b00001001;
				SysTick_Wait10ms(delay);
	}
}
}

void start(){
restart:	
	while(GPIO_PORTD_DATA_R == 0b0000){};		// loops till buttoon is pressed
	SysTick_Wait10ms(10); 										// Buffer
	while(GPIO_PORTD_DATA_R == 0b1000){};		// loops till buttoon is released
		for(int i = 0; i < 8; i++){ 						//loops 8 time since 360/45 =8
			spin(dir,Angle,speed);
			blink(1);															// blinks LED
		}
		goto restart; 													// proccess repeats
}

int main(void){ 														//initailizes all the function
	PLL_Init();
	SysTick_Init();
	PortE_Init();
	PortD_Init();
	PortF_Init();
	start();
}

