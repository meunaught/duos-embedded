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
 
#include <sys_init.h>
#include <cm4.h>
#include <kmain.h>
#include <kstdio.h>
#include <kstring.h>
#include <stdint.h>
#include <usart.h>
#include <gpio.h>

/**
 * BREAD BOARD TO GPIO PIN MAPPING
 * TOP
 * 1 -> DOT
 * 2 -> L1 -> PC1
 * 4 -> H1 -> PC2
 * 5 -> R1 -> PC3
 * BOTTOM
 * 1 -> L2 -> PC4
 * 2 -> H3 -> PC5
 * 4 -> R2 -> PC6
 * 5 -> H2 -> PC7
*/
static const int seven[][7] = {
	{3,6,0,0,0,0,0},
	{2,3,7,4,5,0,0},
	{2,3,7,6,5,0,0},
	{1,7,6,3,0,0,0},
	{2,1,7,6,5,0,0},
	{2,5,7,1,4,6,0},
	{3,6,2,0,0,0,0},
	{1,2,3,4,5,6,7},
	{2,7,3,6,1,0,0}
};

void InitSe7en() {
	RCC->AHB1ENR |= (1<<RCC_AHB1ENR_GPIOCEN_Pos); //Enable clock GPIOC

	uint16_t pin_mask = 0;
	for(int i = 0; i < 10; ++i) {
		pin_mask |= 1U << i;
	}	

	GPIO_InitTypeDef config;
	config.Mode = GPIO_MODE_OUTPUT_PP;
	config.Pull = GPIO_NOPULL;
	config.Speed = GPIO_SPEED_FREQ_HIGH;
	config.Pin= pin_mask;
	GPIO_Init(GPIOC, &config);
}

void khela(char c) {
	int n = c - '0' - 1;
	kprintf("%c should be lit\n", c);
	for(int i = 1; i < 8; ++i) {
		GPIO_WritePin(GPIOC, i, GPIO_PIN_RESET);
	}
	if(n < 0) return;
	for(int i = 0; i < 7; ++i) {
		GPIO_WritePin(GPIOC, seven[n][i], GPIO_PIN_SET);	
	}
}

void kmain(void) {
	__sys_init();

	kprintf("Time Elapse %d ms\n",__getTime());

	InitSe7en();
	while(1) {
		char ch = '*';
		kscanf("%c", &ch);
		kprintf("Jalao %c\n", ch);
		if(ch != '*') khela(ch);
	}
}

