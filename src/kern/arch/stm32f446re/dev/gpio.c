/*
 * Copyright (c) 2022 
 * Computer Science and Engineering, University of Dhaka
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include <gpio.h>

void GPIO_WritePin(GPIO_TypeDef *GPIOx,uint16_t GPIO_pin,GPIO_PinState PinState)
{
	
	if (PinState == GPIO_PIN_SET) {
		GPIOx->BSRR |= GPIO_pin;
	}		
	else if(PinState == GPIO_PIN_RESET) {
		GPIOx->BSRR |= (uint32_t) GPIO_pin << 16U; 
	}
}

void GPIO_Init(GPIO_TypeDef* GPIOx,GPIO_InitTypeDef *GPIO_Init)
{
	uint32_t temp;
	uint32_t pin;
	uint32_t cur_pin;
	uint32_t check_pin;
	
	for (pin = 0U; pin < GPIO_NUMBER; ++pin){
		
		cur_pin = (uint32_t)(1U << pin);
		check_pin = (uint32_t)(GPIO_Init->Pin) & cur_pin;
		
		if(cur_pin == check_pin){
			
			/* Set the PIN Mode */
			temp = GPIOx->MODER;
			temp &= ~(0x00000003 << (pin * 2U));
			temp |= ((GPIO_Init->Mode & GPIO_MODE) << (pin * 2U));
			GPIOx->MODER = temp;

			/* Set the PIN Output Type */
			temp = GPIOx->OTYPER;
			temp &= ~(0x00000001 << pin);
			temp |= (((GPIO_Init->Mode & GPIO_OUTPUT_TYPE) >> 4U) << pin);
			GPIOx->OTYPER = temp;
			
			/* Set the PIN Speed */
			temp = GPIOx->OSPEEDR;
			temp &= ~(0x00000003 << (pin * 2U));
			temp |= ((GPIO_Init->Speed) << (pin * 2U));
			GPIOx->OSPEEDR = temp;
			
			/* Activate the Pull-up or Pull down resistor for the current IO */
			temp = GPIOx->PUPDR;
			temp &= ~(0x00000003 << (pin * 2U)); 
			temp |= ((GPIO_Init->Pull) << (pin * 2U)); 
			GPIOx->PUPDR = temp; 
		}
	}
}



