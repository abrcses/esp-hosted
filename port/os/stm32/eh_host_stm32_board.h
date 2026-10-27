#pragma once

#include <stdint.h>

#define EH_STM32_UART_PORT            2
#define EH_STM32_UART_TX_PIN          2   /* PA2 */
#define EH_STM32_UART_RX_PIN          3   /* PA3 */

#define EH_STM32_RESET_PORT           3   /* GPIOD */
#define EH_STM32_RESET_PIN            12  /* PD12 */

/* Generic GPIO bank mapping to STM32 HAL port indexes */
#define EH_STM32_GPIOA 0
#define EH_STM32_GPIOB 1
#define EH_STM32_GPIOC 2
#define EH_STM32_GPIOD 3
#define EH_STM32_GPIOE 4
#define EH_STM32_GPIOF 5
#define EH_STM32_GPIOG 6