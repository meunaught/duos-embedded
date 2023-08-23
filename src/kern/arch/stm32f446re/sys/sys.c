#include <sys.h>
#include <seven_segment.h>
#include <kstdio.h>
/* Functions of NVIC */
void __NVIC_SetPriority(IRQn_TypeDef IRQn, uint32_t priority)
{
      if (IRQn >= 0)
      {
            NVIC->IP[IRQn] = (uint8_t)((priority << 4));
      }
      else
      {
            SCB->SHPR[(IRQn & 15) - 4] = (uint8_t)(priority << 4);
      }
}

uint32_t __NVIC_GetPriority(IRQn_TypeDef IRQn)
{
      if (IRQn >= 0)
      {
            return (NVIC->IP[IRQn] >> 4);
      }
      else
      {
            return (SCB->SHPR[(IRQn & 15) - 4] >> 4);
      }
}

void __NVIC_EnableIRQn(IRQn_TypeDef IRQn)
{
      if (IRQn >= 0)
      {
            NVIC->ISER[IRQn / 32] |= (1 << (IRQn % 32));
      }
}

void __NVIC_DisableIRQn(IRQn_TypeDef IRQn)
{
      if (IRQn >= 0)
      {
            NVIC->ICER[IRQn / 32] |= (1 << (IRQn % 32));
      }
}

void __disable_irq()
{
      asm("mov r0,#1");
      asm("msr primask,r0;");
}
void __set_BASEPRI(uint32_t value)
{
      value = (value << 4);
      asm volatile("MSR BASEPRI, %0"
                   :
                   : "r"(value)
                   : "memory");
      kprintf("BASEPRI disabled interrupt with priority lower than %d\n", (value >> 4));
}

uint32_t __get_basepri_value(void)
{

      uint32_t value;
      asm("mrs %0, basepri"
          : "=r"(value));
      return (value >> 4);
}
void __unset_BASEPRI(uint32_t value)
{
      asm("mov r0,#0");
      asm("msr basepri,r0;");
      kprintf("BASEPRI reset complete.");
}
void __enable_irq()
{
      asm("mov r0,#0");
      asm("msr primask,r0;");
}

void __set_PRIMASK(uint32_t priMask)
{
      asm("mov r0, #1");
      asm("msr primask, r0");
}

uint32_t get_PRIMASK(void)
{
      uint32_t value;
      asm("mrs r0,primask");
      asm("mov %0,r0"
          : "=r"(value));
      return value;
}

void __enable_fault_irq(void)
{
      asm("mov r0, #0");
      asm("msr primask, r0");
}

void __set_FAULTMASK(uint32_t faultMask)
{
      asm("mov r0, #1");
      asm("msr primask, r0");
}

void __disable_fault_irq(void)
{
      asm("mov r0, #1");
      asm("msr primask, r0");
}

uint32_t __get_FAULTMASK(void)
{
      uint32_t value;
      asm("mrs r0,faultmask");
      asm("mov %0,r0"
          : "=r"(value));
      return value;
}

void __clear_pending_IRQn(IRQn_TypeDef IRQn)
{
      if (IRQn >= 0)
      {
            NVIC->ICPR[IRQn / 32] |= (1 << (IRQn % 32));
      }
}

uint32_t __get_pending_IRQn(IRQn_TypeDef IRQn)
{
      uint32_t pendingState;
      if (IRQn >= 0)
      {
            int regNumber = IRQn / 32;
            int offset = IRQn % 32;
            pendingState = NVIC->ICPR[regNumber] & (1 << regNumber);
            pendingState = pendingState >> 5;
            return pendingState;
      }
}

uint32_t __NVIC_GetActive(IRQn_TypeDef IRQn)
{
      uint32_t pendingState;
      if (IRQn >= 0)
      {
            int regNumber = IRQn / 32;
            int offset = IRQn % 32;
            pendingState = NVIC->IABR[regNumber] & (1 << regNumber);
            pendingState = pendingState >> 5;
            return pendingState;
      }
}

uint32_t PortNumber(GPIO_TypeDef *port)
{
      if (port == GPIOA)
      {
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
            return 0;
      }
      else if (port == GPIOB)
      {
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
            return 1;
      }
      else if (port == GPIOC)
      {
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
            return 2;
      }
      else if (port == GPIOD)
      {
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
            return 3;
      }
      else if (port == GPIOE)
      {
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
            return 4;
      }
      else if (port == GPIOF)
      {
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
            return 5;
      }

      else if (port == GPIOG)
      {
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
            return 6;
      }
      else if (port == GPIOH)
      {
            RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
            return 7;
      }
}
volatile int line;
void EXTI_Init(GPIO_TypeDef *port, uint16_t pin)
{
      line = pin;
      int arr = pin / 4;
      uint32_t portNumber = PortNumber(port);
      GPIO_InitTypeDef gpio_init;
      gpio_init.Mode = GPIO_MODE_INPUT;
      gpio_init.Pull = GPIO_PULLUP;
      gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
      gpio_init.Pin = 1 << pin;

      GPIO_Init(port, &gpio_init);

      RCC->APB2ENR |= (1 << 14);                            // Enable SYSCNFG
      SYSCFG->EXTICR[arr] &= ~(0xFFFF);                     // Bits[7:6:5:4] = (0:0:0:0)  -> reset EXTI1 line for PC0
      SYSCFG->EXTICR[arr] |= portNumber << ((pin % 4) * 4); // Bits[7:6:5:4] = (0:0:0:0)  -> configure EXTI1 line for PC0
      EXTI->IMR |= (1 << pin);                              // Bit[1] = 1  --> Disable the Mask on EXTI 0
      EXTI->RTSR &= ~(1 << pin);                            // Disable Rising Edge Trigger for PC0
      EXTI->FTSR |= (1 << pin);                             // Enable Falling Edge Trigger for PC0
      NVIC_SetPriority(EXTI0_IRQn, 2);                      // Set Priority
      NVIC_EnableIRQ(EXTI0_IRQn);                           // Enable Interrupt
}
int CNT;
void EXTI0_Handler(void)
{
      if (EXTI->PR & (1 << line))
      {
            EXTI->PR |= 1 << line;
            kprintf("Button Pressed %dth Time\n", CNT);
            lit_digit(CNT);
            CNT += 1;
            if (CNT == 10)
                  CNT = 0;
      }
}
