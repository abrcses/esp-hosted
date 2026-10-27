#include "eh_host_port_gpio.h"
#include "eh_host_port_err.h"
#include "stm32f4xx_hal.h"

static GPIO_TypeDef *gpio_port_from_index(uint32_t port)
{
    switch (port) {
        case EH_STM32_GPIOA: return GPIOA;
        case EH_STM32_GPIOB: return GPIOB;
        case EH_STM32_GPIOC: return GPIOC;
        case EH_STM32_GPIOD: return GPIOD;
        case EH_STM32_GPIOE: return GPIOE;
        case EH_STM32_GPIOF: return GPIOF;
        case EH_STM32_GPIOG: return GPIOG;
        default: return NULL;
    }
}

eh_host_port_err_t eh_host_port_gpio_config(const eh_host_port_gpio_config_cfg_t *cfg)
{
    if (!cfg) return EH_HOST_PORT_ERR_INVAL;

    GPIO_TypeDef *port = gpio_port_from_index(cfg->gpio.port);
    if (!port) return EH_HOST_PORT_ERR_INVAL;

    GPIO_InitTypeDef pin_cfg = {0};
    pin_cfg.Pin = (1u << cfg->gpio.pin);

    switch (cfg->dir) {
        case EH_HOST_PORT_GPIO_DIR_INPUT:
            pin_cfg.Mode = GPIO_MODE_INPUT;
            break;
        case EH_HOST_PORT_GPIO_DIR_OUTPUT:
            pin_cfg.Mode = GPIO_MODE_OUTPUT_PP;
            break;
        case EH_HOST_PORT_GPIO_DIR_INPUT_OUTPUT:
            pin_cfg.Mode = GPIO_MODE_OUTPUT_PP;
            break;
        default:
            return EH_HOST_PORT_ERR_INVAL;
    }

    switch (cfg->pull) {
        case EH_HOST_PORT_GPIO_PULL_UP:
            pin_cfg.Pull = GPIO_PULLUP;
            break;
        case EH_HOST_PORT_GPIO_PULL_DOWN:
            pin_cfg.Pull = GPIO_PULLDOWN;
            break;
        default:
            pin_cfg.Pull = GPIO_NOPULL;
            break;
    }

    pin_cfg.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(port, &pin_cfg);

    return EH_HOST_PORT_OK;
}

eh_host_port_err_t eh_host_port_gpio_set(const eh_host_port_gpio_desc_t *gpio,
                                        eh_host_port_gpio_level_t level)
{
    if (!gpio) return EH_HOST_PORT_ERR_INVAL;

    GPIO_TypeDef *port = gpio_port_from_index(gpio->port);
    if (!port) return EH_HOST_PORT_ERR_INVAL;

    HAL_GPIO_WritePin(port,
                      (1u << gpio->pin),
                      (level == EH_HOST_PORT_GPIO_LEVEL_HIGH) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    return EH_HOST_PORT_OK;
}

int eh_host_port_gpio_get(const eh_host_port_gpio_desc_t *gpio)
{
    if (!gpio) return EH_HOST_PORT_ERR_INVAL;

    GPIO_TypeDef *port = gpio_port_from_index(gpio->port);
    if (!port) return EH_HOST_PORT_ERR_INVAL;

    return (HAL_GPIO_ReadPin(port, (1u << gpio->pin)) == GPIO_PIN_SET) ? 1 : 0;
}

eh_host_port_err_t eh_host_port_gpio_set_drive(const eh_host_port_gpio_desc_t *gpio,
                                              eh_host_port_gpio_drive_t drive)
{
    (void)gpio;
    (void)drive;
    return EH_HOST_PORT_ERR_NOSYS;
}