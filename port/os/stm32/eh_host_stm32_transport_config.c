#include <stdbool.h>
#include "eh_host_transport_config.h"
#include "eh_host_stm32_board.h"

static struct eh_host_uart_config g_uart_cfg;

struct eh_host_uart_config eh_host_get_default_uart_config(void)
{
    struct eh_host_uart_config cfg = {0};

    cfg.port = 2;                   /* USART2 */
    cfg.pin_tx.port = EH_STM32_GPIOA;
    cfg.pin_tx.pin  = EH_STM32_UART_TX_PIN;
    cfg.pin_rx.port = EH_STM32_GPIOA;
    cfg.pin_rx.pin  = EH_STM32_UART_RX_PIN;
    cfg.pin_reset.port = EH_STM32_GPIOD;
    cfg.pin_reset.pin  = EH_STM32_RESET_PIN;

    cfg.num_data_bits = 8;
    cfg.parity = 0;
    cfg.stop_bits = 1;
    cfg.flow_ctrl = 0;
    cfg.clk_src = 0;
    cfg.baud_rate = 115200;
    cfg.tx_queue_size = 1024;
    cfg.rx_queue_size = 1024;

    return cfg;
}

eh_host_transport_err_t eh_host_transport_set_default_config(void)
{
    g_uart_cfg = eh_host_get_default_uart_config();
    return EH_HOST_TRANSPORT_RC_OK;
}

eh_host_transport_err_t eh_host_transport_get_config(struct eh_host_transport_config **config)
{
    static struct eh_host_transport_config cfg;

    if (!config) return EH_HOST_TRANSPORT_RC_ERR_INVALID_ARG;

    cfg.transport_in_use = EH_HOST_TRANSPORT_UART;
    cfg.u.uart = g_uart_cfg;

    *config = &cfg;
    return EH_HOST_TRANSPORT_RC_OK;
}

eh_host_transport_err_t eh_host_uart_get_config(struct eh_host_uart_config **config)
{
    if (!config) return EH_HOST_TRANSPORT_RC_ERR_INVALID_ARG;
    *config = &g_uart_cfg;
    return EH_HOST_TRANSPORT_RC_OK;
}

eh_host_transport_err_t eh_host_uart_set_config(struct eh_host_uart_config *config)
{
    if (!config) return EH_HOST_TRANSPORT_RC_ERR_INVALID_ARG;

    g_uart_cfg = *config;
    return EH_HOST_TRANSPORT_RC_OK;
}

bool eh_host_transport_is_config_valid(void)
{
    return (g_uart_cfg.baud_rate > 0) &&
           (g_uart_cfg.pin_tx.pin >= 0) &&
           (g_uart_cfg.pin_rx.pin >= 0);
}

void eh_host_transport_config_set_in_use(bool in_use)
{
    (void)in_use;
}

eh_host_transport_err_t eh_host_transport_get_reset_config(eh_gpio_pin_t *pin_config)
{
    if (!pin_config) return EH_HOST_TRANSPORT_RC_ERR_INVALID_ARG;
    pin_config->port = (void *)((uintptr_t)EH_STM32_GPIOD);
    pin_config->pin  = EH_STM32_RESET_PIN;
    return EH_HOST_TRANSPORT_RC_OK;
}