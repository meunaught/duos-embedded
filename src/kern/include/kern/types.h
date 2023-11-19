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

#ifndef __TYPES_H
#define __TYPES_H

// #define

#include <stdint.h>
#include <kstdio.h>
/*
 * Task, thread or process header
 */
typedef struct task_tcb
{
	uint32_t magic_number;	   // here it is 0xFECABAA0
	uint16_t task_id;		   // a unsigned 16 bit integer starting from 1000
	uint32_t *psp;		   // task stack pointer or stackframe address
	uint16_t status;		   // task status: running, waiting, ready, killed, or terminated
	uint16_t priority;	   // total waiting time (in ms)
	uint32_t start_time_t;	   // process creation time
	uint32_t reponse_time_t;   // first time CPU allocation (execution) time
	uint32_t execution_time_t; // total execution time (in ms)
	uint32_t waiting_time_t;   // total waiting time (in ms)
	uint16_t turn_time;
	uint16_t prev_ready_time;

	uint16_t completion_time;
	uint32_t digital_sinature; // current value is 0x00000001
} TCB_TypeDef;

typedef struct ready_queue
{
	int size;
	int max;
	int st;
	int ed;
	TCB_TypeDef *q[25];
} ReadyQ_TypeDef;

typedef struct dev_t
{
	char name[32]; // Device name or symbol
	int t_ref;	   // Number of open count
	int t_access;  // open type O_RDONLY, O_WRDONLY, O_APPEND
	int op_addr;   // Address of the datastructure operations
} dev_table;

#define task_count 10 // Define the size of the array

extern int end_task[task_count];

#endif