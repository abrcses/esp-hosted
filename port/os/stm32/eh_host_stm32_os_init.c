#include "stm32f4xx_hal.h"
#include "eh_host_stm32_board.h"

void eh_host_stm32_os_init(void)
{
    /* This is intentionally minimal. In a real CubeMX project, most of this
     * is already generated into MX_GPIO_Init() / MX_USART2_UART_Init(). */

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();

    /* The pin config for TX/RX is done in the UART driver.
       The reset GPIO is configured in the GPIO wrapper. */
}