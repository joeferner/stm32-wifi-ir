
#include <stm32f10x_exti.h>
#include "platform_config.h"

extern void time_SysTick_Handler();
extern void on_usart1_irq();
extern void on_exti0_irq();
extern void on_tim2_irq();
extern void cc3000_irq();

void NMI_Handler() {
}

void HardFault_Handler() {
}

void MemManage_Handler() {
}

void BusFault_Handler() {
}

void UsageFault_Handler() {
}

void SVC_Handler() {
}

void DebugMon_Handler() {
}

void PendSV_Handler() {
}

void SysTick_Handler() {
  time_SysTick_Handler();
}

void USART1_IRQHandler(void) {
  on_usart1_irq();
}

void EXTI0_IRQHandler(void) {
  on_exti0_irq();
}

void EXTI1_IRQHandler(void) {
  if (EXTI_GetITStatus(CC3000_IRQ_EXTI_LINE) != RESET) {
    cc3000_irq();
    EXTI_ClearITPendingBit(CC3000_IRQ_EXTI_LINE);
  }
}

void TIM2_IRQHandler() {
  on_tim2_irq();
}
