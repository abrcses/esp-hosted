#include "eh_host_port_uart.h"
#include "eh_host_transport_config.h"
#include "eh_host_stm32_board.h"
#include "stm32f4xx_hal.h"

static UART_HandleTypeDef huart2;

void *hosted_uart_init(void)
{
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef gpio_init = {0};

    gpio_init.Pin = (1u << EH_STM32_UART_TX_PIN) | (1u << EH_STM32_UART_RX_PIN);
    gpio_init.Mode = GPIO_MODE_AF_PP;
    gpio_init.Pull = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_init.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &gpio_init);

    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart2) != HAL_OK) {
        return NULL;
    }

    return &huart2;
}

esp_err_t hosted_uart_deinit(void *ctx)
{
    (void)ctx;
    return ESP_OK;
}

int hosted_uart_read(void *ctx, uint8_t *data, uint16_t size)
{
    UART_HandleTypeDef *uart = (UART_HandleTypeDef *)ctx;
    if (!uart || !data) return -1;
    return HAL_UART_Receive(uart, data, size, 1000);
}

int hosted_uart_write(void *ctx, uint8_t *data, uint16_t size)
{
    UART_HandleTypeDef *uart = (UART_HandleTypeDef *)ctx;
    if (!uart || !data) return -1;
    return HAL_UART_Transmit(uart, data, size, 1000);
}

int hosted_uart_flush_input(void *ctx)
{
    (void)ctx;
    return 0;
}

int hosted_wait_rx_data(uint32_t ticks_to_wait)
{
    (void)ticks_to_wait;
    return 0;
}