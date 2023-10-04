#include <schedule.h>
#include <types.h>

ReadyQ_TypeDef rq;
TCB_TypeDef *current, *__sleep;

void set_sleeping_task(TCB_TypeDef *s) {
    __sleep = s;
    return;
}

void task_start(void) {
    if(is_queue_empty() == 0) {
        kprintf("There is no task in the queue to start\n\r");
        return;
    }
    TCB_TypeDef *qf = queue_front_();
    current = qf;
    current->status = RUNNING;
    __asm volatile ("MOV R12, %0" : : "r"(qf->psp));
    //calls SYS_start_task from syscall
    __asm volatile ("SVC #121");
}

void initialize_queue(void) {
    rq.max = 5;
    rq.size = 0;
    rq.st = 0;
    rq.ed = -1;    
}

void add_to_ready_queue(TCB_TypeDef *t) {
    if(rq.max >= rq.size + 1) {
        rq.ed = (rq.ed + 1) % rq.max ;
        rq.q[rq.ed] = t;
        rq.size++;
    } 
    else {
        kprintf("The ready queue is full, thus the task could not be added\n\r");
    }
}

TCB_TypeDef *queue_front_(void) {
    if(is_queue_empty() == 0) {
        add_to_ready_queue(__sleep);
    }
    int front = rq.st;
    rq.st = (rq.st + 1) % rq.max;
    rq.size--;
    return *((rq.q) + front);
}

int is_queue_empty(void) {
    return rq.size;
}

const uint16_t initial_task_id = 1000;
uint16_t last_assigned = initial_task_id;

// Generate a unique ID for the task
uint16_t generate_task_id(void) {
    return last_assigned++;
} 

void task_create(TCB_TypeDef *tcb, void (*task_func)(void), uint32_t *stack) {
    // Initialize the TCB fields
    tcb->magic_number = 0xFECABAA0;

    // Generate a unique ID for the task
    tcb->task_id = generate_task_id();
    tcb->status = READY;
    // Initialize the stack for the task
    tcb->run = task_func;
    // Point to the top of the stack
    tcb->psp = stack;

    *(--tcb->psp) = DUMMY_XPSR;  // xPSR
    *(--tcb->psp) = (uint32_t) task_func;  // PC (task entry point)
    *(--tcb->psp) = 0xFFFFFFFD;  // LR Exception return table
    
    *(--tcb->psp) = 0x0000000;  // R12 - storing actual object address in here
    *(--tcb->psp) = 0x0000000;  // R3
    *(--tcb->psp) = 0x0000000;  // R2
    *(--tcb->psp) = 0x0000000;  // R1
    *(--tcb->psp) = 0x0000000;  // R0
    
    // initializing registers r11 to r4
    for(int i = 0; i < 8; i++) {
        if(i == 0) {
            *(--tcb->psp) = (uint32_t) tcb;
        } else {
            *(--tcb->psp) = 0x0000000;
        }
    }
}

void _schedule(void) {
    if(current->status == RUNNING) {
        current->status = READY;
        add_to_ready_queue(current);
    } 
    TCB_TypeDef *qf = queue_front_();
    current = qf;
    current->status = RUNNING;
    return;
}

void __attribute__((naked)) PendSV_Handler(void) {
    SCB->ICSR |= (1 << 27);
    /* Save current context */

    // Get current process stack pointer value
    __asm volatile("MRS R0,PSP");
    // Save R4 to R11 in task stack (8 regs)
    __asm volatile("STMDB R0!,{R4-R11}");
    __asm volatile("PUSH {LR}");
    // Save current psp value
    __asm volatile ("MOV %0, R0": "=r"( current->psp ): );

    _schedule();

    // Get new task psp value
    __asm volatile ("MOV R0, %0" : : "r"(current->psp));
    // Load R4 to R11 from taskstack (8 regs)
    __asm volatile ("LDMIA R0!,{R4-R11}");
    __asm volatile("MSR PSP,R0");
    __asm volatile("POP {LR}");
    __asm volatile("BX LR");
}
