#include <test_interrupt.h>
#include <cm4.h>
#include <kstdio.h>

volatile int hardfault_event_enabled = 0;    // Volatile to prevent optimization

void enable_hardfault_event(void)
{
      //SCB->SHCSR |= (1 << 16);               // Enable UsageFault handler
      volatile int *ptr = (int *)0xFFFFFFFF; // Invalid memory address
      int value = *ptr;                      // This will trigger a HardFault
}

void reboot()
{
      kprintf("Rebooting..................\n");
      SCB->AIRCR = (0x5FA << 16) | (0x4 << 0); // SCB_AIRCR = VECTKEY | SYSRESETREQ
}
void enableSysTickInterrupt(void)
{
      SYSTICK->CTRL |= (1 << 1);
}

void disableSysTickInterrupt(void)
{
      SYSTICK->CTRL &= ~(1 << 1);
}