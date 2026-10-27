#pragma once

/* Enable minimal feature set for a first UART-host port */
#define EH_HOST_PORT_HAS_TASK              1
#define EH_HOST_PORT_HAS_MUTEX             1
#define EH_HOST_PORT_HAS_COND              1
#define EH_HOST_PORT_HAS_SEM               1
#define EH_HOST_PORT_HAS_SEM_POST_FROM_ISR 1
#define EH_HOST_PORT_HAS_EVENT_GROUP       1
#define EH_HOST_PORT_HAS_QUEUE             1
#define EH_HOST_PORT_HAS_TIMER             1
#define EH_HOST_PORT_HAS_TIME              1
#define EH_HOST_PORT_HAS_MEM               1
#define EH_HOST_PORT_HAS_DMA               0
#define EH_HOST_PORT_HAS_LOG               1

#define EH_HOST_PORT_HAS_GPIO              1
#define EH_HOST_PORT_HAS_GPIO_INTR         0
#define EH_HOST_PORT_HAS_GPIO_HOLD         0

#define EH_HOST_PORT_HAS_POWER             1
#define EH_HOST_PORT_HAS_POWER_SAVE        0
#define EH_HOST_PORT_HAS_WAKEUP_REASON     0

#define EH_HOST_PORT_HAS_NETIF             1

/* Transport selection for a UART-only first port */
#define EH_HOST_TRANSPORT_BUS_SDIO         0
#define EH_HOST_TRANSPORT_BUS_SPI          0
#define EH_HOST_TRANSPORT_BUS_SPI_HD       0
#define EH_HOST_TRANSPORT_BUS_UART          1

/* Default task stack size for FreeRTOS-based STM32 */
#define EH_HOST_PORT_DFLT_TASK_STACK       4096

/* Optional compatibility for older code paths */
#define MAX_UART_BUFFER_SIZE 2048