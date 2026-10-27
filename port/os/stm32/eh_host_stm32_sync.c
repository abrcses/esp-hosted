#include "eh_host_port_sync.h"
#include "eh_host_port_err.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

struct eh_host_port_mutex {
    SemaphoreHandle_t h;
};

struct eh_host_port_cond {
    SemaphoreHandle_t sem;
};

struct eh_host_port_sem {
    SemaphoreHandle_t h;
};

struct eh_host_port_event_group {
    EventGroupHandle_t h;
};

struct eh_host_port_queue {
    QueueHandle_t h;
    size_t item_size;
    uint32_t depth;
};

eh_host_port_mutex_t *eh_host_port_mutex_create(void)
{
    eh_host_port_mutex_t *m = pvPortMalloc(sizeof(*m));
    if (!m) return NULL;

    m->h = xSemaphoreCreateMutex();
    if (!m->h) {
        vPortFree(m);
        return NULL;
    }

    return m;
}

void eh_host_port_mutex_destroy(eh_host_port_mutex_t *m)
{
    if (!m) return;
    if (m->h) vSemaphoreDelete(m->h);
    vPortFree(m);
}

void eh_host_port_mutex_lock(eh_host_port_mutex_t *m)
{
    if (!m || !m->h) return;
    xSemaphoreTake(m->h, portMAX_DELAY);
}

void eh_host_port_mutex_unlock(eh_host_port_mutex_t *m)
{
    if (!m || !m->h) return;
    xSemaphoreGive(m->h);
}

eh_host_port_err_t eh_host_port_mutex_lock_wait_ms(eh_host_port_mutex_t *m, uint32_t timeout_ms)
{
    if (!m || !m->h) return EH_HOST_PORT_ERR_INVAL;
    return (xSemaphoreTake(m->h, pdMS_TO_TICKS(timeout_ms)) == pdTRUE) ? EH_HOST_PORT_OK : EH_HOST_PORT_ERR_TIMEOUT;
}

eh_host_port_cond_t *eh_host_port_cond_create(void)
{
    eh_host_port_cond_t *c = pvPortMalloc(sizeof(*c));
    if (!c) return NULL;

    c->sem = xSemaphoreCreateBinary();
    if (!c->sem) {
        vPortFree(c);
        return NULL;
    }

    return c;
}

void eh_host_port_cond_destroy(eh_host_port_cond_t *c)
{
    if (!c) return;
    if (c->sem) vSemaphoreDelete(c->sem);
    vPortFree(c);
}

void eh_host_port_cond_wait(eh_host_port_cond_t *c, eh_host_port_mutex_t *m)
{
    (void)c;
    (void)m;
}

eh_host_port_err_t eh_host_port_cond_wait_ms(eh_host_port_cond_t *c, eh_host_port_mutex_t *m,
                                            uint32_t timeout_ms)
{
    (void)c;
    (void)m;
    (void)timeout_ms;
    return EH_HOST_PORT_ERR_NOSYS;
}

void eh_host_port_cond_signal(eh_host_port_cond_t *c)
{
    if (!c || !c->sem) return;
    xSemaphoreGive(c->sem);
}

void eh_host_port_cond_broadcast(eh_host_port_cond_t *c)
{
    eh_host_port_cond_signal(c);
}

eh_host_port_sem_t *eh_host_port_sem_create(void)
{
    eh_host_port_sem_t *s = pvPortMalloc(sizeof(*s));
    if (!s) return NULL;

    s->h = xSemaphoreCreateBinary();
    if (!s->h) {
        vPortFree(s);
        return NULL;
    }

    return s;
}

eh_host_port_sem_t *eh_host_port_sem_create_counting(uint32_t max_count)
{
    eh_host_port_sem_t *s = pvPortMalloc(sizeof(*s));
    if (!s) return NULL;

    s->h = xSemaphoreCreateCounting((UBaseType_t)max_count, 0U);
    if (!s->h) {
        vPortFree(s);
        return NULL;
    }

    return s;
}

void eh_host_port_sem_destroy(eh_host_port_sem_t *s)
{
    if (!s) return;
    if (s->h) vSemaphoreDelete(s->h);
    vPortFree(s);
}

eh_host_port_err_t eh_host_port_sem_post(eh_host_port_sem_t *s)
{
    if (!s || !s->h) return EH_HOST_PORT_ERR_INVAL;
    return (xSemaphoreGive(s->h) == pdTRUE) ? EH_HOST_PORT_OK : EH_HOST_PORT_ERR;
}

eh_host_port_err_t eh_host_port_sem_wait_ms(eh_host_port_sem_t *s, uint32_t timeout_ms)
{
    if (!s || !s->h) return EH_HOST_PORT_ERR_INVAL;
    return (xSemaphoreTake(s->h, pdMS_TO_TICKS(timeout_ms)) == pdTRUE) ? EH_HOST_PORT_OK : EH_HOST_PORT_ERR_TIMEOUT;
}

eh_host_port_err_t eh_host_port_sem_try_wait(eh_host_port_sem_t *s)
{
    if (!s || !s->h) return EH_HOST_PORT_ERR_INVAL;
    return (xSemaphoreTake(s->h, 0) == pdTRUE) ? EH_HOST_PORT_OK : EH_HOST_PORT_ERR_TIMEOUT;
}

#if EH_HOST_PORT_HAS_SEM_POST_FROM_ISR
eh_host_port_err_t eh_host_port_sem_post_from_isr(eh_host_port_sem_t *s)
{
    if (!s || !s->h) return EH_HOST_PORT_ERR_INVAL;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t ok = xSemaphoreGiveFromISR(s->h, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    return (ok == pdTRUE) ? EH_HOST_PORT_OK : EH_HOST_PORT_ERR;
}
#endif

eh_host_port_event_group_t *eh_host_port_event_group_create(void)
{
    eh_host_port_event_group_t *g = pvPortMalloc(sizeof(*g));
    if (!g) return NULL;
    g->h = xEventGroupCreate();
    if (!g->h) {
        vPortFree(g);
        return NULL;
    }
    return g;
}

void eh_host_port_event_group_delete(eh_host_port_event_group_t *g)
{
    if (!g) return;
    if (g->h) vEventGroupDelete(g->h);
    vPortFree(g);
}

eh_host_port_event_bits_t eh_host_port_event_group_set_bits(eh_host_port_event_group_t *g,
                                                            eh_host_port_event_bits_t bits_to_set)
{
    if (!g || !g->h) return 0;
    return (eh_host_port_event_bits_t)xEventGroupSetBits(g->h, (EventBits_t)bits_to_set);
}

eh_host_port_event_bits_t eh_host_port_event_group_clear_bits(eh_host_port_event_group_t *g,
                                                              eh_host_port_event_bits_t bits_to_clear)
{
    if (!g || !g->h) return 0;
    return (eh_host_port_event_bits_t)xEventGroupClearBits(g->h, (EventBits_t)bits_to_clear);
}

eh_host_port_event_bits_t eh_host_port_event_group_wait_bits(eh_host_port_event_group_t *g,
                                                            eh_host_port_event_bits_t bits_to_wait_for,
                                                            int clear_on_exit,
                                                            int wait_for_all_bits,
                                                            uint32_t timeout_ms)
{
    if (!g || !g->h) return 0;
    EventBits_t bits = xEventGroupWaitBits(g->h,
                                          (EventBits_t)bits_to_wait_for,
                                          (clear_on_exit != 0) ? pdTRUE : pdFALSE,
                                          (wait_for_all_bits != 0) ? pdTRUE : pdFALSE,
                                          pdMS_TO_TICKS(timeout_ms));
    return (eh_host_port_event_bits_t)bits;
}

eh_host_port_queue_t *eh_host_port_queue_create(uint32_t depth, size_t item_size)
{
    eh_host_port_queue_t *q = pvPortMalloc(sizeof(*q));
    if (!q) return NULL;

    q->depth = depth;
    q->item_size = item_size;

    q->h = xQueueCreate((UBaseType_t)depth, (UBaseType_t)item_size);
    if (!q->h) {
        vPortFree(q);
        return NULL;
    }
    return q;
}

void eh_host_port_queue_destroy(eh_host_port_queue_t *q)
{
    if (!q) return;
    if (q->h) vQueueDelete(q->h);
    vPortFree(q);
}

eh_host_port_err_t eh_host_port_queue_send(eh_host_port_queue_t *q,
                                          const void *item,
                                          uint32_t timeout_ms)
{
    if (!q || !q->h || !item) return EH_HOST_PORT_ERR_INVAL;
    return (xQueueSend(q->h, item, pdMS_TO_TICKS(timeout_ms)) == pdTRUE) ? EH_HOST_PORT_OK : EH_HOST_PORT_ERR_TIMEOUT;
}

eh_host_port_err_t eh_host_port_queue_receive(eh_host_port_queue_t *q,
                                             void *out_item,
                                             uint32_t timeout_ms)
{
    if (!q || !q->h || !out_item) return EH_HOST_PORT_ERR_INVAL;
    return (xQueueReceive(q->h, out_item, pdMS_TO_TICKS(timeout_ms)) == pdTRUE) ? EH_HOST_PORT_OK : EH_HOST_PORT_ERR_TIMEOUT;
}

eh_host_port_err_t eh_host_port_queue_peek(eh_host_port_queue_t *q,
                                           void *out_item,
                                           uint32_t timeout_ms)
{
    if (!q || !q->h || !out_item) return EH_HOST_PORT_ERR_INVAL;
    return (xQueuePeek(q->h, out_item, pdMS_TO_TICKS(timeout_ms)) == pdTRUE) ? EH_HOST_PORT_OK : EH_HOST_PORT_ERR_TIMEOUT;
}

uint32_t eh_host_port_queue_count(const eh_host_port_queue_t *q)
{
    if (!q || !q->h) return 0;
    return (uint32_t)uxQueueMessagesWaiting(q->h);
}