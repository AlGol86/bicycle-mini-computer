/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */
#include "stm8s.h"
#include "main.h"
#include "led.h"

char contact_state;
char flipped_contact_state;
unsigned int sys_time_cntr = THREE_SEC_CNT;
unsigned int time_per_one_turn;

main() {
  char closing_contact_accepted = 0;
	unsigned int turn_cntr = 0;
	unsigned long mileage_meters = 0;
	init_LED(); 
	init_tim2();	
	
	CLK->CKDIVR = 0x00; //~ 16 MHz   << 0x11 - 2MHz  0x10 - 4MHz  0x01 - 8MHz  0x00 - 16MHz >>
	
  while (1){
			//listen contact flipping event	
		if (flipped_contact_state && !closing_contact_accepted) {
			closing_contact_accepted = 1;
			turn_cntr++;
		} else if (closing_contact_accepted && !contact_state) {
			closing_contact_accepted = 0;
			flipped_contact_state = 0;
		}
		//refresh speed
		mileage_meters = (turn_cntr * WHEEL_ROUND_LENGTH) / 200;
		if (mileage_meters > 9999) printHigherNumber(mileage_meters / 100, 2);
		else printValue(mileage_meters, 0, 2);
		printValue(time_per_one_turn != 0 ? WHEEL_EKVIVAL / time_per_one_turn : 0, 0, 1);
		

  }
}
/*
char get_magnetic_state(unsigned int gaussVal) {
	if((gaussVal > (zeroGauss + 20)) || (gaussVal < (zeroGauss - 20))) {
		return 1;
	} else {
		return 0;
	}
}
*/
//target: 3000 cycles/sec
/*
unsigned int get_ADC(char ch) {
unsigned int h=0;
char l=0;
  ADC1->CSR=ADC1_CSR_CH&ch;
  ADC1->CR1|=ADC1_CR1_ADON; 
  ADC1->CR1|=ADC1_CR1_ADON;  //dubble 'ADON" for switch on ADC
while((ADC1->CSR&ADC1_CSR_EOC)==0)
  {nop();}

ADC1->CSR&=~ADC1_CSR_EOC;
h=ADC1->DRH;
l=ADC1->DRL;
//return h;
return ((h<<2)|l); //unsigned int
}
*/

void init_tim2 (void) {//encoder_poller
  TIM2->IER|=TIM2_IER_UIE;
	//TIM2->IER|=TIM1_IER_CC1IE;
	/*
	TIM2->PSCR=0;
	TIM2->ARRH=100;
  TIM2->ARRL=0;
	  12750 counts per 20 sec
	  =637,5 / 1 sec
	*/
	//261 sec 50*65536=3 276 800 --> 12555 cycles / sec
  TIM2->PSCR=0;
	TIM2->ARRH=5; //2
  TIM2->ARRL=0;//20
	TIM2->CNTRH=0;
	TIM2->CNTRL=0;
	rim();
	TIM2->CR1|=TIM2_CR1_CEN;
  }
