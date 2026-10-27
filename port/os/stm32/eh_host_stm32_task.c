#include "eh_host_port_task.h"
#include "eh_host_port_err.h"
#include "FreeRTOS.h"
#include "task.h"

eh_host_port_err_t eh_host_port_task_create(const eh_host_port_task_create_cfg_t *cfg,
                                           eh_host_port_task_t **out)
{
    if (!cfg || !cfg->fn) return EH_HOST_PORT_ERR_INVAL;

    TaskHandle_t task_handle = NULL;
    UBaseType_t prio = (cfg->priority > 0) ? (UBaseType_t)cfg->priority : configMAX_PRIORITIES - 2U;
    uint32_t stack_words = (cfg->stack_bytes > 0) ? (cfg->stack_bytes / sizeof(StackType_t)) : 512U;

    BaseType_t ok = xTaskCreate((TaskFunction_t)cfg->fn,
                                (cfg->name != NULL) ? cfg->name : "eh_task",
                                stack_words,
                                cfg->arg,
                                prio,
                                &task_handle);

    if (ok != pdPASS) return EH_HOST_PORT_ERR_NOMEM;

    if (out) {
        *out = (eh_host_port_task_t *)task_handle;
    }
    return EH_HOST_PORT_OK;
}

eh_host_port_err_t eh_host_port_task_join(eh_host_port_task_t *task)
{
    (void)task;
    return EH_HOST_PORT_OK;
}

eh_host_port_err_t eh_host_port_task_destroy(eh_host_port_task_t *task)
{
    (void)task;
    return EH_HOST_PORT_OK;
}

eh_host_port_err_t eh_host_port_task_cancel(eh_host_port_task_t *task)
{
    (void)task;
    return EH_HOST_PORT_ERR_NOSYS;
}

eh_host_port_err_t eh_host_port_task_wake(eh_host_port_task_t *task)
{
    (void)task;
    return EH_HOST_PORT_ERR_NOSYS;
}

void eh_host_port_task_delay_ms(uint32_t ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}

void eh_host_port_task_delay_us(uint32_t us)
{
    if (us == 0) {
        taskYIELD();
        return;
    }
    vTaskDelay(pdMS_TO_TICKS((us + 999U) / 1000U));
}