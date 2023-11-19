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

#include <syscall.h>
#include <syscall_def.h>
#include <errno.h>
#include <errmsg.h>
#include <kstdio.h>
#include <usart.h>
#include <cm4.h>
#include <types.h>
#include <kmain.h>

dev_table directory[64];

int current_index = 0;

int open_directory(unsigned char *s, int fd)
{
    for (int i = 0; i < current_index; ++i)
    {
        if (kstrcmp(directory[i].name, s) == 0)
        {
            // kprintf("Device already open.\n");
            directory[i].t_ref++;
            return i;
        }
    }

    dev_table entry;
    // kprintf("Opening a new device...\n");
    kstrcpy(entry.name, s);
    entry.t_access = fd;
    entry.t_ref = 1;
    entry.op_addr = (uint32_t *)(DEVICE_STACK_START - (current_index * DEVICE_STACK_SIZE));

    directory[current_index] = entry;
    current_index++;

    return current_index - 1;
}

void close_directory(int fd)
{
    directory[fd].t_ref = 0;
    kprintf("\nClosing file #%d\n", fd);
}

void get_directory()
{
    for (int i = 0; i < current_index; ++i)
    {
        if (directory[i].t_ref != 0)
        {
            kprintf("----------------------------\n");
            kprintf("Device %d:\n", i);
            kprintf("Name: %s\n", directory[i].name);
            kprintf("t_ref: %d\n", directory[i].t_ref);
            kprintf("t_access: %d\n", directory[i].t_access);
            kprintf("op_addr: %x\n", directory[i].op_addr);
            kprintf("----------------------------\n");
        }
    }
}

void __sys_start_task(void)
{
    __asm volatile("POP {LR}");

    unsigned int *svc_args;
    __asm volatile("MOV %0, R1"
                   : "=r"(svc_args));
    uint32_t task_psp = svc_args[3];
    __asm volatile("MOV R0, %0"
                   :
                   : "r"(task_psp));

    __asm volatile("LDMIA R0!,{R4-R11}");
    __asm volatile("MSR PSP, R0"); // moves value of r0 onto psp
    __asm volatile("ISB");

    __asm volatile("MOV LR, 0xFFFFFFFD"); // exception return
    __asm volatile("BX LR");              // returns to thread mode
}

void __sys_getpid(void)
{
    unsigned int *svc_args;
    __asm volatile("MOV %0, R1"
                   : "=r"(svc_args));
    (*(TCB_TypeDef *)svc_args[0]) = (*((TCB_TypeDef *)svc_args[4]));
    return;
}

void __sys_exit(void)
{
    unsigned int *svc_args;
    __asm volatile("MOV %0, R1"
                   : "=r"(svc_args));

    (*((TCB_TypeDef *)svc_args[4])).status = 4;
    return;
}

void __sys_read(void)
{
    unsigned int *svc_args;
    __asm volatile("MOV %0, R1"
                   : "=r"(svc_args)
                   :);

    int *bytes_read = (int *)svc_args[4]; // R12
    int len = (int)svc_args[3];           // R3
    unsigned char *buff = (unsigned char *)svc_args[2];

    if (len == 1)
    {
        buff[0] = UART_GetChar(USART2);
        *bytes_read = 1;
    }
    else
    {
        *bytes_read = _USART_READ_STR(USART2, buff, 50);
    }
    return;
}

void __sys_write(void)
{
    unsigned char *s;
    unsigned int *svc_args;
    __asm volatile("MOV %0, R1"
                   : "=r"(svc_args)
                   :);
    s = (unsigned char *)svc_args[1]; // R1
    int len = _USART_WRITE(USART2, s);
    *((int *)svc_args[4]) = len;
    return;
}

void __sys_open(void)
{
    unsigned char *s;
    unsigned int *svc_args;
    __asm volatile("MOV %0, R1"
                   : "=r"(svc_args)
                   :);
    s = (unsigned char *)svc_args[1]; // R1
    int fd = (int)svc_args[2];        // R2

    *((int *)svc_args[4]) = open_directory(s, fd);
    get_directory();
    return;
}

void __sys_close(void)
{
    unsigned int *svc_args;
    __asm volatile("MOV %0, R1"
                   : "=r"(svc_args)
                   :);
    int fd = (int)svc_args[1]; // R1

    close_directory(fd);
    // get_directory();
    return;
}

void __sys_gettime(void)
{
    unsigned int *svc_args;
    __asm volatile("MOV %0, R1"
                   : "=r"(svc_args)
                   :);
    *((unsigned int *)svc_args[1]) = __getTime(); // R1
}

void __sys_reboot(void)
{
    kprintf("rebooting...");
    SCB->AIRCR = (0x05FA << 16) | (1 << 2);

    while (1)
        ;
}

void __sys_yield(void)
{
    SCB->ICSR |= (1 << 28);
}

void syscall(uint16_t callno)
{
    /* The SVC_Handler calls this function to evaluate and execute the actual function */
    /* Take care of return value or code */
    switch (callno)
    {
    case SYS_read:
        __sys_read();
        break;
    case SYS_write:
        __sys_write();
        break;
    case SYS_open:
        __sys_open();
        break;
    case SYS_reboot:
        __sys_reboot();
        break;
    case SYS__exit:
        __sys_exit();
        break;
    case SYS_getpid:
        __sys_getpid();
        break;
    case SYS___time:
        __sys_gettime();
        break;
    case SYS_yield:
        __sys_yield();
        break;
    case SYS_close:
        __sys_close();
        break;
    case SYS_start_task:
        __sys_start_task();
        break;
    default:
        break;
    }

    __asm volatile("POP {LR}");
    /* Handle SVC return here */
}
