/*	BASIC INTERRUPT VECTOR TABLE FOR STM8 devices
 *	Copyright (c) 2007 STMicroelectronics
 */
#include "main.h"
#include "led.h"
#include "stm8s.h"

typedef void @far (*interrupt_handler_t)(void);

struct interrupt_vector {
	unsigned char interrupt_instruction;
	interrupt_handler_t interrupt_handler;
};

@far @interrupt void NonHandledInterrupt (void)
{
	/* in order to detect unexpected events during development, 
	   it is recommended to set a breakpoint on the following instruction
	*/
	return;
}

@far @interrupt void TIM2Interrupt (void)
{
	extern char contact_state;
	extern char flipped_contact_state;
	
	extern unsigned int sys_time_cntr;
	extern unsigned int time_per_one_turn;
	
	unsigned int magnetic;
	unsigned int h=0;
  char l=0;
	TIM2->SR1&=~TIM2_SR1_UIF;//flag "0"
	


  
	
	if (sys_time_cntr) { // once in 3 sec
		sys_time_cntr--;
	} else {
		time_per_one_turn = 0;
	}
	//scip if < 0.1 sec last after previous contact catching
	if (sys_time_cntr/10 > 3650) return;
	
  ADC1->CSR = ADC1_CSR_CH & GAUSS_ADC_CH;
  ADC1->CR1 |= ADC1_CR1_ADON; 
  ADC1->CR1 |= ADC1_CR1_ADON;  //dubble 'ADON" for switch on ADC
  while((ADC1->CSR&ADC1_CSR_EOC) == 0);

  ADC1->CSR&=~ADC1_CSR_EOC;
  h=ADC1->DRH;
  l=ADC1->DRL;

  magnetic = ((h<<2)|l); //unsigned int
	
  if((magnetic > (ZERO_MAGNETIC + MAGNETIC_THRESHOLD)) 
	|| (magnetic < (ZERO_MAGNETIC - MAGNETIC_THRESHOLD))) {
		contact_state = 1;
		if (!flipped_contact_state) {
			time_per_one_turn = sys_time_cntr ? THREE_SEC_CNT - sys_time_cntr : 0;
			sys_time_cntr = THREE_SEC_CNT;
		}
		flipped_contact_state = 1;
	} else {
		contact_state = 0;
	}

	return;
}

extern void _stext();     /* startup routine */

struct interrupt_vector const _vectab[] = {
	{0x82, (interrupt_handler_t)_stext}, /* reset */
	{0x82, NonHandledInterrupt}, /* trap  */
	{0x82, NonHandledInterrupt}, /* irq0  */
	{0x82, NonHandledInterrupt}, /* irq1  */
	{0x82, NonHandledInterrupt}, /* irq2  */
	{0x82, NonHandledInterrupt}, /* irq3  */
	{0x82, NonHandledInterrupt}, /* irq4  */
	{0x82, NonHandledInterrupt}, /* irq5  */
	{0x82, NonHandledInterrupt}, /* irq6  */
	{0x82, NonHandledInterrupt}, /* irq7  */
	{0x82, NonHandledInterrupt}, /* irq8  */
	{0x82, NonHandledInterrupt}, /* irq9  */
	{0x82, NonHandledInterrupt}, /* irq10 */
	{0x82, NonHandledInterrupt}, /* irq11 */
	{0x82, NonHandledInterrupt}, /* irq12 */
	{0x82, TIM2Interrupt}, /* irq13 */
	{0x82, NonHandledInterrupt}, /* irq14 */
	{0x82, NonHandledInterrupt}, /* irq15 */
	{0x82, NonHandledInterrupt}, /* irq16 */
	{0x82, NonHandledInterrupt}, /* irq17 */
	{0x82, NonHandledInterrupt}, /* irq18 */
	{0x82, NonHandledInterrupt}, /* irq19 */
	{0x82, NonHandledInterrupt}, /* irq20 */
	{0x82, NonHandledInterrupt}, /* irq21 */
	{0x82, NonHandledInterrupt}, /* irq22 */
	{0x82, NonHandledInterrupt}, /* irq23 */
	{0x82, NonHandledInterrupt}, /* irq24 */
	{0x82, NonHandledInterrupt}, /* irq25 */
	{0x82, NonHandledInterrupt}, /* irq26 */
	{0x82, NonHandledInterrupt}, /* irq27 */
	{0x82, NonHandledInterrupt}, /* irq28 */
	{0x82, NonHandledInterrupt}, /* irq29 */
};
