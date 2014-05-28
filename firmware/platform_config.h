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

#define IR_RX_TIMER            TIM2

#define IR_TX_CARRIER_TIMER            TIM3
#define IR_TX_CARRIER_TIMER_RCC        RCC_APB1Periph_TIM3
#define IR_TX_CARRIER_TIMER_CH_Init          TIM_OC3Init
#define IR_TX_CARRIER_TIMER_CH_PreloadConfig TIM_OC3PreloadConfig
#define IR_TX_CARRIER_TIMER_CH_SetCompare    TIM_SetCompare3
#define IR_TX_CARRIER_RCC              RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO
#define IR_TX_CARRIER_PORT             GPIOB
#define IR_TX_CARRIER_PIN              GPIO_Pin_0

#define IR_TX_RCC              RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO
#define IR_TX_PORT             GPIOB
#define IR_TX_PIN              GPIO_Pin_1

#define IR_TX_DELAY_TIMER            TIM1
#define IR_TX_DELAY_TIMER_IRQ        TIM1_UP_IRQn
#define IR_TX_DELAY_TIMER_RCC        RCC_APB2Periph_TIM1

// SPI1 pins: SCK (GPIOA - pin 5) and MOSI (GPIOA - pin 7)
// SPI1 pins: MISO (GPIOA - pin 6)

#define CC3000_IRQ_RCC         RCC_APB2Periph_GPIOA
#define CC3000_IRQ_PORT        GPIOA
#define CC3000_IRQ_PIN         GPIO_Pin_1
#define CC3000_IRQ_EXTI_LINE   EXTI_Line1
#define CC3000_IRQ_EXTI_PORT   GPIO_PortSourceGPIOA
#define CC3000_IRQ_EXTI_PIN    GPIO_PinSource1
#define CC3000_IRQ_EXTI_CH     EXTI1_IRQn

#define CC3000_VBAT_EN_RCC     RCC_APB2Periph_GPIOA
#define CC3000_VBAT_EN_PORT    GPIOA
#define CC3000_VBAT_EN_PIN     GPIO_Pin_2

#define CC3000_CS_RCC          RCC_APB2Periph_GPIOA
#define CC3000_CS_PORT         GPIOA
#define CC3000_CS_PIN          GPIO_Pin_3

#endif // PLATFORM_CONFIG_H_INCLUDED
