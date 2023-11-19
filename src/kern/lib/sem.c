#include <sem.h>
#include <schedule.h>
#include <unistd.h>

// sem_dec
// Declare for use from C as extern void sem_dec(void * semaphore);
extern void sem_inc(const void *semaphore)
{
    __asm volatile("1: LDREX R1, [%0]"
                   :
                   : "r"(semaphore));
    __asm volatile("CMP R1, #0");
    __asm volatile("BNE 1b");
    __asm volatile("ADD R1, #1");
    __asm volatile("STREX R2,R1,[%0]"
                   : "=r"(semaphore));
    __asm volatile("CMP R2, #0");
    __asm volatile("BNE 1b");
    __asm volatile("DMB");

    return;
}
// sem_inc
// Declare for use from C as extern void sem_inc(void * semaphore);
extern void sem_dec(const void *semaphore)
{
    __asm volatile("1: LDREX R1, [%0]"
                   :
                   : "r"(semaphore));
    __asm volatile("SUB R1, #1");
    __asm volatile("STREX R2,R1,[%0]"
                   : "=r"(semaphore));
    __asm volatile("CMP R2, #0");
    __asm volatile("BNE 1b");
    __asm volatile("DMB");

    return;
}
void wait(int *semaphore)
{
    sem_inc(semaphore);
}
void signal(int *semaphore)
{
    sem_dec(semaphore);
}