
#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_exti.h>
#include <misc.h>
#include <stdint.h>
#include <stddef.h>
#include "ir_rx.h"
#include "debug.h"
#include "platform_config.h"
#include "ir_code.h"

void on_exti1_irq();
void on_tim2_irq();
void _ir_rx_process_buffer();

#define IR_RX_CAPTURE_BUFFER_LEN 200
uint16_t irRxCaptureBuffer[IR_RX_CAPTURE_BUFFER_LEN];
volatile int16_t irRxCaptureBufferIndex;

#define RX_BUFFER_SIZE 20
ring_buffer_voidptr irRxRecvRingBuffer;
void* irRxRecvBuffer[RX_BUFFER_SIZE];

#define TIM_PRESCALER  72
#define TIM_PERIOD     0xfffe
#define US_PER_TIM     ((1.0 / (SystemCoreClock / TIM_PRESCALER)) * 1000000.0)

void ir_rx_setup() {
  GPIO_InitTypeDef gpioConfig;
  NVIC_InitTypeDef nvicInit;
  EXTI_InitTypeDef extiInit;
  TIM_TimeBaseInitTypeDef timeBaseInit;

  debug_write_line("?BEGIN ir_rx_setup");

  ring_buffer_voidptr_init(&irRxRecvRingBuffer, irRxRecvBuffer, RX_BUFFER_SIZE);

  irRxCaptureBufferIndex = -1;

  RCC_APB2PeriphClockCmd(IR_RX_RCC, ENABLE);
  gpioConfig.GPIO_Pin = IR_RX_PIN;
  gpioConfig.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  gpioConfig.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(IR_RX_PORT, &gpioConfig);

  // enable EXTI
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

  EXTI_StructInit(&extiInit);
  extiInit.EXTI_Line = EXTI_Line0;
  extiInit.EXTI_Mode = EXTI_Mode_Interrupt;
  extiInit.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  extiInit.EXTI_LineCmd = ENABLE;
  EXTI_Init(&extiInit);

  nvicInit.NVIC_IRQChannel = EXTI0_IRQn;
  nvicInit.NVIC_IRQChannelPreemptionPriority = 0;
  nvicInit.NVIC_IRQChannelSubPriority = 0;
  nvicInit.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvicInit);

  // setup timer
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  debug_write("?ir_rx timer period: ");
  debug_write_u32(TIM_PERIOD, 10);
  debug_write_line("");
  debug_write("?ir_rx timer us: ");
  debug_write_u32(US_PER_TIM, 10);
  debug_write_line("");

  TIM_TimeBaseStructInit(&timeBaseInit);
  timeBaseInit.TIM_Period = TIM_PERIOD;
  timeBaseInit.TIM_Prescaler = TIM_PRESCALER;
  timeBaseInit.TIM_ClockDivision = 0;
  timeBaseInit.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &timeBaseInit);

  nvicInit.NVIC_IRQChannel = TIM2_IRQn;
  nvicInit.NVIC_IRQChannelPreemptionPriority = 1;
  nvicInit.NVIC_IRQChannelSubPriority = 0;
  nvicInit.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvicInit);

  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  TIM_SetCounter(TIM2, 0);
  TIM_Cmd(TIM2, ENABLE);

  debug_write_line("?END ir_rx_setup");
}

IrCode* ir_rx_recv() {
  return (IrCode*)ring_buffer_voidptr_read(&irRxRecvRingBuffer);
}

void _ir_rx_process_buffer() {
  IrCode* code = ir_code_decode(irRxCaptureBuffer, irRxCaptureBufferIndex);
  if(code != NULL) {
    ring_buffer_voidptr_write(&irRxRecvRingBuffer, code);
  }
}

void on_exti0_irq() {
  if(EXTI_GetITStatus(EXTI_Line0) != RESET) {
    // skip the first signal change, this is the start of the signal and should not be recorded
    if(irRxCaptureBufferIndex < 0) {
      irRxCaptureBufferIndex++;
    } else {
      irRxCaptureBuffer[irRxCaptureBufferIndex] = TIM_GetCounter(TIM2);
      if(irRxCaptureBuffer[irRxCaptureBufferIndex] > 10000) {
        irRxCaptureBufferIndex++;
        _ir_rx_process_buffer();
        irRxCaptureBufferIndex = 0;
      } else {
        irRxCaptureBufferIndex++;
      }
      debug_led_set(1);
    }

    TIM_SetCounter(TIM2, 0);
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
}

void on_tim2_irq() {
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
    debug_led_set(0);
    if(irRxCaptureBufferIndex > 1) {
      _ir_rx_process_buffer();
    }
    irRxCaptureBufferIndex = -1;
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  }
}

