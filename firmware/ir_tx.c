
#include <stdint.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>
#include "ir_tx.h"
#include "platform_config.h"
#include "debug.h"
#include "delay.h"

#define IR_TX_CARRIER_FREQ       38000
#define IR_TX_PWM_PERIOD         (SystemCoreClock / IR_TX_CARRIER_FREQ)

GPIO_InitTypeDef irTxGpioInit;

void _ir_tx_on();
void _ir_tx_off();

void ir_tx_setup() {
  TIM_TimeBaseInitTypeDef timeBaseInit;
  TIM_OCInitTypeDef ocInit;

  debug_write_line("?BEGIN ir_tx_setup");

  RCC_APB1PeriphClockCmd(IR_TX_TIMER_RCC, ENABLE);
  RCC_APB2PeriphClockCmd(IR_TX_RCC, ENABLE);

  GPIO_StructInit(&irTxGpioInit);
  irTxGpioInit.GPIO_Pin = IR_TX_PIN;
  irTxGpioInit.GPIO_Mode = GPIO_Mode_AF_PP;
  irTxGpioInit.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(IR_TX_PORT, &irTxGpioInit);

  // Time base configuration
  TIM_TimeBaseStructInit(&timeBaseInit);
  timeBaseInit.TIM_Period = IR_TX_PWM_PERIOD;
  timeBaseInit.TIM_Prescaler = 0;
  timeBaseInit.TIM_ClockDivision = 0;
  timeBaseInit.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(IR_TX_TIMER, &timeBaseInit);

  TIM_OCStructInit(&ocInit);
  ocInit.TIM_OCMode = TIM_OCMode_PWM1;
  ocInit.TIM_OCPolarity = TIM_OCPolarity_High;
  ocInit.TIM_OutputState = TIM_OutputState_Enable;
  ocInit.TIM_Pulse = 0;
  IR_TX_TIMER_CH_Init(IR_TX_TIMER, &ocInit);
  IR_TX_TIMER_CH_PreloadConfig(IR_TX_TIMER, TIM_OCPreload_Enable);

  TIM_SelectOnePulseMode(IR_TX_TIMER, TIM_OPMode_Repetitive);
  TIM_ARRPreloadConfig(IR_TX_TIMER, ENABLE);
  IR_TX_TIMER_CH_SetCompare(IR_TX_TIMER, IR_TX_PWM_PERIOD / 2);

  debug_write_line("?END ir_tx_setup");
}

void ir_tx_send(uint16_t* buffer, uint16_t bufferLength) {
  for(uint16_t i = 0; i < bufferLength; i++) {
    if((i % 2) == 0) {
      _ir_tx_on();
    } else {
      _ir_tx_off();
    }
    delay_us(buffer[i]);
  }
  _ir_tx_off();
}

void _ir_tx_on() {
  irTxGpioInit.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(IR_TX_PORT, &irTxGpioInit);

  TIM_Cmd(IR_TX_TIMER, ENABLE);
}

void _ir_tx_off() {
  TIM_Cmd(IR_TX_TIMER, DISABLE);

  GPIO_ResetBits(IR_TX_PORT, IR_TX_PIN);
  irTxGpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(IR_TX_PORT, &irTxGpioInit);
}
