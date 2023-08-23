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
#include <seven_segment.h>
#include <sys.h>
#include <test_interrupt.h>

void kmain(void)
{
	__sys_init();
	__SysTick_init(100000000);

	int n = 0;
	EXTI_Init(GPIOC, 0);
	while (1)
	{

		if (n != 6)
		{
			kprintf(".........................Lab 2..................\n");
			kprintf("Press 1: Reboot.........\nPress 2: Enable HardFault.........\nPress 3: Enable SysTick Interrupt.............\nPress 4: Check NVIC Priority.........\nPress 5: Set Base Priority.........\nPress 6: Exit.........\n");
			kprintf("Enter number:\n");
			kscanf("%d", &n);
			kprintf("........................................................................\n");
		}
		if (n == 1)
		{
			n = 0;
			reboot();
		}
		else if (n == 2)
		{
			n = 0;
			enable_hardfault_event();
		}
		else if (n == 3)
		{
			n = 0;

			NVIC_SetPriority(SysTick_IRQn, 1);
			enableSysTickInterrupt();
			for (int i = 0; i < 1000000; i++);
		}
		else if (n == 4)
		{
			n = 0;
			kprintf("EXTI NVIC Priority: %d\n", NVIC_GetPriority(EXTI0_IRQn));
			kprintf("SysTick NVIC Priority: %d\n", NVIC_GetPriority(SysTick_IRQn));
			kprintf("BASE Priority: %d\n", Get_BASEPRI_Value());
		}
		else if (n == 5)
		{
			n = 0;
			Set_BASEPRI(5);
		}
		else if (n == 6)
		{
			n = 0;
			break;
		}
		
	}
}
