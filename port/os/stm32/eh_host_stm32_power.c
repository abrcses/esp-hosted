#include "eh_host_port_power.h"
#include "eh_host_port_gpio.h"
#include "eh_host_port_task.h"
#include "eh_host_stm32_board.h"
#include "stm32f4xx_hal.h"

eh_host_port_err_t eh_host_port_reset_slave(void)
{
    eh_host_port_gpio_desc_t rst = {
        .port = EH_STM32_RESET_PORT,
        .pin  = EH_STM32_RESET_PIN
    };

    /* Assert reset low */
    eh_host_port_gpio_set(&rst, EH_HOST_PORT_GPIO_LEVEL_LOW);
    eh_host_port_task_delay_ms(100);

    /* Release reset */
    eh_host_port_gpio_set(&rst, EH_HOST_PORT_GPIO_LEVEL_HIGH);
    return EH_HOST_PORT_OK;
}

eh_host_port_err_t eh_host_port_restart_host(void)
{
    /* Simple host reboot */
    NVIC_SystemReset();
    return EH_HOST_PORT_OK;
}

eh_host_port_err_t eh_host_port_power_init(void)
{
    return EH_HOST_PORT_OK;
}

/* Power save is intentionally not implemented for the first pass */
eh_host_port_err_t eh_host_port_power_save_config(const eh_host_port_power_save_config_cfg_t *cfg)
{
    (void)cfg;
    return EH_HOST_PORT_ERR_NOSYS;
}

eh_host_port_err_t eh_host_port_power_save_enter(eh_host_port_ps_type_t ps_type)
{
    (void)ps_type;
    return EH_HOST_PORT_ERR_NOSYS;
}

eh_host_port_err_t eh_host_port_power_save_wakeup_gpio_config(eh_host_port_ps_type_t ps_type,
                                                             int gpio,
                                                             int wake_level)
{
    (void)ps_type;
    (void)gpio;
    (void)wake_level;
    return EH_HOST_PORT_ERR_NOSYS;
}