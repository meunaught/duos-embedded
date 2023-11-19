#ifndef __SCHEDULE_H
#define __SCHEDULE_H
#include <types.h>
#include <stdint.h>
#include <cm4.h>
#include <kmain.h>

#define DUMMY_XPSR 0x01000000U

#define READY 1
#define RUNNING 2
#define TERMINATED 3
#define KILLED 4
#define BLOCKED 5

void contextSwitch(void);

// task related functions
uint16_t generate_task_id(void);
void task_create(TCB_TypeDef *tcb, void (*task_func)(void), uint32_t *stack);
void task_start(void);

// queue related functions
void initialize_queue(void);
void add_to_ready_queue(TCB_TypeDef *t);
TCB_TypeDef *ready_queue_front_(void);
int is_ready_queue_empty(void);
void set_sleeping_task(TCB_TypeDef *s);

void add_to_blocked_queue(TCB_TypeDef *t);
TCB_TypeDef *blocked_queue_front_(void);
int is_blocked_queue_empty(void);
void setPolicy(int policy);
#endif