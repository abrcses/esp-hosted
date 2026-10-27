#include "stm32f4xx_hal.h"

UART_HandleTypeDef huart2;   // ESP32 transport
UART_HandleTypeDef huart3;   // debug console to PC

void SystemClock_Config(void)
{
    /* Minimal safe default config for the first bring-up.
       For a real project, generate this from CubeMX or set a proper PLL config.
    */
    __HAL_RCC_HSI_ENABLE();
    __HAL_RCC_SYSCLK_CONFIG(RCC_SYSCLKSOURCE_HSI);

    /* This is a very minimal placeholder; adjust to your final setup. */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
}

static void uart3_tx_string(const char *s)
{
    while (*s) {
        HAL_UART_Transmit(&huart3, (uint8_t *)s, 1, 100);
        s++;
    }
}

static void uart3_tx_hex_byte(uint8_t b)
{
    static const char hex[] = "0123456789ABCDEF";
    uint8_t out[2];
    out[0] = hex[(b >> 4) & 0x0F];
    out[1] = hex[b & 0x0F];

    HAL_UART_Transmit(&huart3, out, 2, 100);
    HAL_UART_Transmit(&huart3, (uint8_t *)" ", 1, 100);
}

static void uart3_tx_hex_buffer(const uint8_t *buf, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++) {
        uart3_tx_hex_byte(buf[i]);
    }
    HAL_UART_Transmit(&huart3, (uint8_t *)"\r\n", 2, 100);
}

/* ---- ESP32 transport UART (USART2) ---- */
static void init_esp32_uart(void)
{
    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef gpio = {0};

    /* USART2 TX = PA2, RX = PA3 */
    gpio.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &gpio);

    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart2);
}

/* ---- PC debug console UART (USART3) ---- */
static void init_debug_uart(void)
{
    __HAL_RCC_USART3_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef gpio = {0};

    /* USART3 TX = PB10, RX = PB11 */
    gpio.Pin = GPIO_PIN_10 | GPIO_PIN_11;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOB, &gpio);

    huart3.Instance = USART3;
    huart3.Init.BaudRate = 115200;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart3);
}

/* Example ESP32 reset pin on PD12 */
static void init_reset_gpio(void)
{
    __HAL_RCC_GPIOD_CLK_ENABLE();

    GPIO_InitTypeDef gpio = {0};
    gpio.Pin = GPIO_PIN_12;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &gpio);

    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
}

static void reset_esp32(void)
{
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
    HAL_Delay(50);
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    init_esp32_uart();
    init_debug_uart();
    init_reset_gpio();

    uart3_tx_string("STM32F407 debug console ready\r\n");
    uart3_tx_string("ESP32 transport on USART2 (PA2/PA3)\r\n");
    uart3_tx_string("Debug console on USART3 (PB10/PB11)\r\n");

    uart3_tx_string("Resetting ESP32...\r\n");
    reset_esp32();

    uart3_tx_string("Waiting for ESP32 UART bytes...\r\n");

    uint8_t rx_buf[32];
    uint16_t rx_len = 0;

    for (;;) {
        uint8_t b = 0;

        if (HAL_UART_Receive(&huart2, &b, 1, 10) == HAL_OK) {
            rx_buf[rx_len++] = b;

            if (rx_len >= sizeof(rx_buf)) {
                uart3_tx_string("RX: ");
                uart3_tx_hex_buffer(rx_buf, rx_len);
                rx_len = 0;
            }
        }
    }
}