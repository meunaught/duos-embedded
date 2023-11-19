#ifndef __SEM_H
#define __SEM_H
#include <types.h>

extern void sem_dec(const void *semaphore);
extern void sem_inc(const void *semaphore);

void add_task(TCB_TypeDef task);
TCB_TypeDef get_task(void);
void wait(int *semaphore);
void signal(int *semaphore);

#endif