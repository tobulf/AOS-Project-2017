#include <miosix.h>
#include <cstdio>
#include "Util.h"
#include "ADC-driver.h"






Adc::Adc(){
	//Enable port b clock GPIOBEN
	set_bit(RCC->AHB1ENR, 1);

	//Enable analog input mode, set GPIOB_MODER of PB0
	GPIOB->MODER |= (0b11 << 0);
	//set_bit(GPIOB->MODER,4);
	//set_bit(GPIOB->MODER,1);


	//Set ADC1 clock:
	set_bit(RCC->APB2ENR, 8);

	//Set Resolution to 8 bit, 0-256:
	clear_bit(ADC1->CR1, 24);
	set_bit(ADC1->CR1, 25);
	

	//Set sampling time[1 0 1], 112 Cycles for channel 8(bit 26:24):
	set_bit(ADC1->SMPR2, 26);
	clear_bit(ADC1->SMPR2, 25);
	set_bit(ADC1->SMPR2, 24);

	//Set to 1st Conversion:
	set_bit(ADC1->SQR3, 3);

	//ADC prescaler, set to clk/8:
	set_bit(ADC->CCR,17);
	set_bit(ADC->CCR,16);


	//Toogle Continous mode:
	set_bit(ADC1->CR2, 1);
	//set Right side alignment.
	clear_bit(ADC1->CR2, 11);
	//Turn ADC on (ADON)
	set_bit(ADC1->CR2, 0);
	//Start continuous conversion (SWSTART)
	set_bit(ADC1->CR2, 30);
	
}

uint16_t Adc::read(){
	//Wait for the EOC-bit to switch + 
	while ((ADC_SR_EOC & ADC1->SR) == 0);

	return (uint16_t) ADC1->DR & ADC_DR_DATA;

}



