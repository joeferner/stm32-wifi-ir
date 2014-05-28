#ifndef PLATFORM_CONFIG_H_INCLUDED
#define PLATFORM_CONFIG_H_INCLUDED

#define DEBUG_LED_RCC          RCC_APB2Periph_GPIOB
#define DEBUG_LED_PORT         GPIOB
#define DEBUG_LED_PIN          GPIO_Pin_12

#define DEBUG_USART            USART1
#define DEBUG_USART_BAUD       115200
#define DEBUG_USART_IRQ        USART1_IRQn
#define DEBUG_USART_RCC        RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1
#define DEBUG_USART_TX         GPIOA
#define DEBUG_USART_TX_PIN     GPIO_Pin_9
#define DEBUG_USART_RX         GPIOA
#define DEBUG_USART_RX_PIN     GPIO_Pin_10

#define IR_RX_RCC              RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO
#define IR_RX_PORT             GPIOA
#define IR_RX_PIN              GPIO_Pin_0

#define IR_TX_CARRIER_TIMER            TIM3
#define IR_TX_CARRIER_TIMER_RCC        RCC_APB1Periph_TIM3
#define IR_TX_CARRIER_TIMER_CH_Init          TIM_OC1Init
#define IR_TX_CARRIER_TIMER_CH_PreloadConfig TIM_OC1PreloadConfig
#define IR_TX_CARRIER_TIMER_CH_SetCompare    TIM_SetCompare1
#define IR_TX_CARRIER_RCC              RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO
#define IR_TX_CARRIER_PORT             GPIOB
#define IR_TX_CARRIER_PIN              GPIO_Pin_0

#define IR_TX_RCC              RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO
#define IR_TX_PORT             GPIOB
#define IR_TX_PIN              GPIO_Pin_1

#endif // PLATFORM_CONFIG_H_INCLUDED
