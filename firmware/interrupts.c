
extern void time_SysTick_Handler();
extern void on_usart1_irq();
extern void on_exti0_irq();
extern void on_tim2_irq();

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

void TIM2_IRQHandler() {
  on_tim2_irq();
}
