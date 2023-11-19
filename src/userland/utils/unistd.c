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

#include <unistd.h>
#include <types.h>
#include <syscall_def.h>

/* Write your highlevel I/O details */

void exit(void)
{
    __asm volatile("MOV R12, R11");
    __asm volatile("SVC %[value]"
                   :
                   : [value] "I"(SYS__exit));
    yield();
}

TCB_TypeDef getpid(void)
{
    TCB_TypeDef pid;
    __asm volatile("MOV R12, R11");
    __asm volatile("MOV R0, %0"
                   :
                   : "r"(&pid));
    __asm volatile("SVC %[value]"
                   :
                   : [value] "I"(SYS_getpid));
    return pid;
}

// attempts to read up to len bytes, returns the number of bytes read
int read(uint32_t fd, unsigned char *s, size_t len)
{
    int ret;
    __asm volatile("MOV R1, %0"
                   :
                   : "r"(fd));
    __asm volatile("MOV R2, %0"
                   :
                   : "r"(s));
    __asm volatile("MOV R3, %0"
                   :
                   : "r"(len));
    __asm volatile("MOV R12, %0"
                   :
                   : "r"(&ret));
    __asm volatile("SVC %[value]"
                   :
                   : [value] "I"(SYS_read));
    return ret;
}

int write(uint32_t fd, unsigned char *s, size_t len)
{
    int ret;
    __asm volatile("MOV R0, %0"
                   :
                   : "r"(fd));
    __asm volatile("MOV R1, %0"
                   :
                   : "r"(s));
    __asm volatile("MOV R2, %0"
                   :
                   : "r"(len));
    __asm volatile("MOV R12, %0"
                   :
                   : "r"(&ret));
    __asm volatile("SVC %[value]"
                   :
                   : [value] "I"(SYS_write));
    return ret;
}

int fopen(unsigned char *s, uint32_t fd)
{
    int ret;
    __asm volatile("MOV R1, %0"
                   :
                   : "r"(s));
    __asm volatile("MOV R2, %0"
                   :
                   : "r"(fd));
    __asm volatile("MOV R12, %0"
                   :
                   : "r"(&ret));
    __asm volatile("SVC %[value]"
                   :
                   : [value] "I"(SYS_open)); // SVC #45
    return ret;
}

int fclose(uint32_t fd)
{
    int ret;
    __asm volatile("MOV R1, %0"
                   :
                   : "r"(fd));
    __asm volatile("MOV R12, %0"
                   :
                   : "r"(&ret));
    __asm volatile("SVC %[value]"
                   :
                   : [value] "I"(SYS_close));
    return ret;
}

int get_time(void)
{
    unsigned int time = 0;
    __asm volatile("MOV R1, %0"
                   :
                   : "r"(&time));
    __asm volatile("SVC %[value]"
                   :
                   : [value] "I"(SYS___time));
    return (int)(time);
}

void reboot(void)
{
    __asm volatile("SVC %[value]"
                   :
                   : [value] "I"(SYS_reboot));
}

void yield(void)
{
    __asm volatile("SVC %[value]"
                   :
                   : [value] "I"(SYS_yield));
}

int printf(char *format, ...)
{
    char *tr;
    uint32_t i;
    uint8_t *str;
    va_list list;
    double dval;
    va_start(list, format);
    unsigned char result[256];
    int index = 0;
    for (tr = format; *tr != '\0'; tr++)
    {
        while (*tr != '%' && *tr != '\0')
        {
            result[index++] = (uint8_t)*tr;
            tr++;
        }
        if (*tr == '\0')
            break;
        tr++;
        switch (*tr)
        {
        case 'c':
            i = va_arg(list, int);
            result[index++] = (uint8_t)i;
            break;
        case 'd':
            i = va_arg(list, int);
            if (i < 0)
            {
                result[index++] = (uint8_t)'-';
                i = -i;
            }
            uint8_t *s1 = (uint8_t *)convert(i, 10);
            while (*s1)
            {
                result[index++] = *s1;
                s1++;
            }

            break;
        case 'o':
            i = va_arg(list, int);
            if (i < 0)
            {
                result[index++] = '-';
                i = -i;
            }
            s1 = (uint8_t *)convert(i, 8);
            while (*s1)
            {
                result[index++] = *s1;
                s1++;
            }
            break;
        case 'x':
            i = va_arg(list, int);
            if (i < 0)
            {
                result[index++] = '-';
                i = -i;
            }
            s1 = (uint8_t *)convert(i, 16);
            while (*s1)
            {
                result[index++] = *s1;
                s1++;
            }
            break;
        case 's':
            str = va_arg(list, uint8_t *);
            s1 = (uint8_t *)&str;
            while (*s1)
            {
                result[index++] = *s1;
                s1++;
            }
            break;
        case 'f':
            dval = va_arg(list, double);
            s1 = (uint8_t *)float2str(dval);
            while (*s1)
            {
                result[index++] = *s1;
                s1++;
            }
            break;
        default:
            break;
        }
    }
    va_end(list);
    result[index] = '\0';
    return write(STDOUT_FILENO, result, index);
}

// returns the number of fields that were successfully converted and assigned
int uscanf(char *format, ...)
{
    va_list list;
    char *ptr;
    int conversion_count = 0;
    uint8_t buff[50];
    ptr = format;
    va_start(list, format);

    while (*ptr)
    {
        /* code */
        if (*ptr == '%') // looking for format of an input
        {
            ptr++;
            switch (*ptr)
            {
            case 'c': // charater
                /* code */
                read(STDIN_FILENO, buff, 1);
                *(uint8_t *)va_arg(list, uint8_t *) = buff[0];
                conversion_count++;
                break;
            case 'd': // integer number
                // _USART_READ_STR(USART2,buff,50);
                read(STDIN_FILENO, buff, 50);
                *(uint32_t *)va_arg(list, uint32_t *) = __str_to_num(buff, 10);
                conversion_count++;
                break;
            case 's': // need to update -- string
                // _USART_READ_STR(USART2,buff,50);
                read(STDIN_FILENO, buff, 50);
                *(uint32_t *)va_arg(list, uint32_t *) = __str_to_num(buff, 10);
                conversion_count++;
                break;
            case 'x': // hexadecimal number
                // _USART_READ_STR(USART2,buff,50);
                read(STDIN_FILENO, buff, 50);
                *(uint32_t *)va_arg(list, uint32_t *) = __str_to_num(buff, 16);
                conversion_count++;
                break;
            case 'o': // octal number
                // _USART_READ_STR(USART2,buff,50);
                read(STDIN_FILENO, buff, 50);
                *(uint32_t *)va_arg(list, uint32_t *) = __str_to_num(buff, 8);
                conversion_count++;
                break;
            case 'f': // floating point number
                // _USART_READ_STR(USART2,buff,50);
                // *(uint32_t*)va_arg(list,double*)= str2float(buff);
                read(STDIN_FILENO, buff, 50);
                *(float *)va_arg(list, float *) = str2float(buff);
                conversion_count++;
                break;
            default: // rest not recognized
                break;
            }
        }
        ptr++;
    }
    va_end(list);
    return conversion_count;
}

int fprintf(int fd, char *format, ...)
{
    char *tr;
    uint32_t i;
    uint8_t *str;
    va_list list;
    double dval;
    va_start(list, format);
    unsigned char result[512];
    int index = 0;

    kprintf("\nPrinting in device #%d:\n", fd);

    for (tr = format; *tr != '\0'; tr++)
    {
        while (*tr != '%' && *tr != '\0')
        {
            result[index++] = (uint8_t)*tr;
            tr++;
        }
        if (*tr == '\0')
            break;
        tr++;
        switch (*tr)
        {
        case 'c':
            i = va_arg(list, int);
            result[index++] = (uint8_t)i;
            break;
        case 'd':
            i = va_arg(list, int);
            if (i < 0)
            {
                result[index++] = (uint8_t)'-';
                i = -i;
            }
            uint8_t *s1 = (uint8_t *)convert(i, 10);
            while (*s1)
            {
                result[index++] = *s1;
                s1++;
            }

            break;
        case 'o':
            i = va_arg(list, int);
            if (i < 0)
            {
                result[index++] = '-';
                i = -i;
            }
            s1 = (uint8_t *)convert(i, 8);
            while (*s1)
            {
                result[index++] = *s1;
                s1++;
            }
            break;
        case 'x':
            i = va_arg(list, int);
            if (i < 0)
            {
                result[index++] = '-';
                i = -i;
            }
            s1 = (uint8_t *)convert(i, 16);
            while (*s1)
            {
                result[index++] = *s1;
                s1++;
            }
            break;
        case 's':
            str = va_arg(list, uint8_t *);
            s1 = (uint8_t *)&str;
            while (*s1)
            {
                result[index++] = *s1;
                s1++;
            }
            break;
        case 'f':
            dval = va_arg(list, double);
            s1 = (uint8_t *)float2str(dval);
            while (*s1)
            {
                result[index++] = *s1;
                s1++;
            }
            break;
        default:
            break;
        }
    }
    va_end(list);
    result[index] = '\0';
    return write(STDOUT_FILENO, result, index);
}