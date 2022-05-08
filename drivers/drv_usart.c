/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-09     xiangxistu   first version
 * 2020-05-18     chenyaxing   modify stm32_uart_config
 */

#include "stdlib.h"
#include "drv_common.h"
#include "uart_config.h"

#define DBG_TAG              "drv.usart"

#ifdef DRV_DEBUG
#define DBG_LVL               DBG_LOG
#else
#define DBG_LVL               DBG_INFO
#endif

#include <rtdbg.h>

#ifdef RT_USING_CONSOLE

static UART_HandleTypeDef handle;

/* stm32 config class */
struct stm32_uart_config
{
    const char *name;
    USART_TypeDef *Instance;
    IRQn_Type irq_type;

    const char *tx_pin_name;
    const char *rx_pin_name;
};

static struct stm32_uart_config *_uart_config = RT_NULL;

struct stm32_uart_config uart_config[] =
{
#ifdef BSP_USING_UART1
    UART1_CONFIG,
#endif
#ifdef BSP_USING_UART2
    UART2_CONFIG,
#endif
#ifdef BSP_USING_UART3
    UART3_CONFIG,
#endif
#ifdef BSP_USING_UART4
    UART4_CONFIG,
#endif
#ifdef BSP_USING_UART5
    UART5_CONFIG,
#endif
#ifdef BSP_USING_UART6
    UART6_CONFIG,
#endif
#ifdef BSP_USING_UART7
    UART7_CONFIG,
#endif
#ifdef BSP_USING_UART8
    UART8_CONFIG,
#endif
#ifdef BSP_USING_LPUART1
    LPUART1_CONFIG,
#endif
};

static rt_err_t stm32_uart_clk_enable(struct stm32_uart_config *config)
{
    /* check the parameters */
    RT_ASSERT(IS_UART_INSTANCE(config->Instance));

    /* uart clock enable */
    switch ((uint32_t)config->Instance)
    {
#ifdef BSP_USING_UART1
    case (uint32_t)USART1:
        __HAL_RCC_USART1_CLK_ENABLE();
        break;
#endif /* BSP_USING_UART1 */
#ifdef BSP_USING_UART2
    case (uint32_t)USART2:
        __HAL_RCC_USART2_CLK_ENABLE();
        break;
#endif /* BSP_USING_UART2 */
#ifdef BSP_USING_UART3
    case (uint32_t)USART3:
        __HAL_RCC_USART3_CLK_ENABLE();
        break;
#endif /* BSP_USING_UART3 */
#ifdef BSP_USING_UART4
#if defined(SOC_SERIES_STM32F0) || defined(SOC_SERIES_STM32L0) || \
   defined(SOC_SERIES_STM32G0)
    case (uint32_t)USART4:
        __HAL_RCC_USART4_CLK_ENABLE();
#else
    case (uint32_t)UART4:
        __HAL_RCC_UART4_CLK_ENABLE();
#endif
        break;
#endif /* BSP_USING_UART4 */
#ifdef BSP_USING_UART5
#if defined(SOC_SERIES_STM32F0) || defined(SOC_SERIES_STM32L0) || \
   defined(SOC_SERIES_STM32G0)
    case (uint32_t)USART5:
        __HAL_RCC_USART5_CLK_ENABLE();
#else
    case (uint32_t)UART5:
        __HAL_RCC_UART5_CLK_ENABLE();
#endif
        break;
#endif /* BSP_USING_UART5 */
#ifdef BSP_USING_UART6
    case (uint32_t)USART6:
        __HAL_RCC_USART6_CLK_ENABLE();
        break;
#endif /* BSP_USING_UART6 */
#ifdef BSP_USING_UART7
#if defined(SOC_SERIES_STM32F0)
    case (uint32_t)USART7:
        __HAL_RCC_USART7_CLK_ENABLE();
#else
    case (uint32_t)UART7:
        __HAL_RCC_UART7_CLK_ENABLE();
#endif
        break;
#endif /* BSP_USING_UART7 */
#ifdef BSP_USING_UART8
#if defined(SOC_SERIES_STM32F0)
    case (uint32_t)USART8:
        __HAL_RCC_USART8_CLK_ENABLE();
#else
    case (uint32_t)UART8:
        __HAL_RCC_UART8_CLK_ENABLE();
#endif
        break;
#endif /* BSP_USING_UART8 */
#ifdef BSP_USING_LPUART1
    case (uint32_t)LPUART1:
       __HAL_RCC_LPUART1_CLK_ENABLE();
       break;
#endif /* BSP_USING_LPUART1 */
    default:
        return -RT_ERROR;
    }

    return RT_EOK;
}

static rt_err_t stm32_gpio_clk_enable(GPIO_TypeDef *gpiox)
{
    /* check the parameters */
    RT_ASSERT(IS_GPIO_ALL_INSTANCE(gpiox));

    /* gpio ports clock enable */
    switch ((uint32_t)gpiox)
    {
#if defined(__HAL_RCC_GPIOA_CLK_ENABLE)
    case (uint32_t)GPIOA:
        __HAL_RCC_GPIOA_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOB_CLK_ENABLE)
    case (uint32_t)GPIOB:
        __HAL_RCC_GPIOB_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOC_CLK_ENABLE)
    case (uint32_t)GPIOC:
        __HAL_RCC_GPIOC_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOD_CLK_ENABLE)
    case (uint32_t)GPIOD:
        __HAL_RCC_GPIOD_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOE_CLK_ENABLE)
    case (uint32_t)GPIOE:
        __HAL_RCC_GPIOE_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOF_CLK_ENABLE)
    case (uint32_t)GPIOF:
        __HAL_RCC_GPIOF_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOG_CLK_ENABLE)
    case (uint32_t)GPIOG:
        __HAL_RCC_GPIOG_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOH_CLK_ENABLE)
    case (uint32_t)GPIOH:
        __HAL_RCC_GPIOH_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOI_CLK_ENABLE)
    case (uint32_t)GPIOI:
        __HAL_RCC_GPIOI_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOJ_CLK_ENABLE)
    case (uint32_t)GPIOJ:
        __HAL_RCC_GPIOJ_CLK_ENABLE();
        break;
#endif
#if defined(__HAL_RCC_GPIOK_CLK_ENABLE)
    case (uint32_t)GPIOK:
        __HAL_RCC_GPIOK_CLK_ENABLE();
        break;
#endif
    default:
        return -RT_ERROR;
    }

    return RT_EOK;
}

static int up_char(char * c)
{
    if ((*c >= 'a') && (*c <= 'z'))
    {
        *c = *c - 32;
    }
    return 0;
}

static void get_pin_by_name(const char* pin_name, GPIO_TypeDef **port, uint16_t *pin)
{
    int pin_num = atoi((char*) &pin_name[2]);
    char port_name = pin_name[1];
    up_char(&port_name);
    up_char(&port_name);
    *port = ((GPIO_TypeDef *) ((uint32_t) GPIOA
            + (uint32_t) (port_name - 'A') * ((uint32_t) GPIOB - (uint32_t) GPIOA)));
    *pin = (GPIO_PIN_0 << pin_num);
}

static uint16_t stm32_get_pin(GPIO_TypeDef *pin_port, rt_uint32_t pin_num)
{
    return (uint16_t)((16 * (((rt_base_t)pin_port - (rt_base_t)GPIOA_BASE)/(0x0400UL))) + (__rt_ffs(pin_num) - 1));
}

static rt_err_t stm32_gpio_configure(struct stm32_uart_config *config)
{
#define UART_IS_TX        (1U<<7)
#define UART_IS_RX        (0U)

    rt_uint16_t tx_pin_num = 0;
    int index = 0, tx_af_num = 0, rx_af_num = 0;
    uint8_t uart_num = 0;
    GPIO_TypeDef *tx_port;
    GPIO_TypeDef *rx_port;
    uint16_t tx_pin;
    uint16_t rx_pin;
    get_pin_by_name(config->rx_pin_name, &rx_port, &rx_pin);
    get_pin_by_name(config->tx_pin_name, &tx_port, &tx_pin);
    
   struct gpio_uart_af {
       /* index get by GET_PIN */
       uint16_t pin_index;
       struct {
           /* when is TX the bit7 is 1, bit6-bit0 is uart number (1-8)  */
           uint8_t uart_num;
           uint8_t af_num;
       } afs[1];
   };

   static const struct gpio_uart_af uart_afs[] =
     {
         { .pin_index = GET_PIN(A,  0), .afs[0] = {.uart_num = UART_IS_TX|4, .af_num =  8}},
         { .pin_index = GET_PIN(A,  1), .afs[0] = {.uart_num = UART_IS_RX|4, .af_num =  8}},
         { .pin_index = GET_PIN(A,  2), .afs[0] = {.uart_num = UART_IS_TX|2, .af_num =  7}},
         { .pin_index = GET_PIN(A,  3), .afs[0] = {.uart_num = UART_IS_RX|2, .af_num =  7}},
         { .pin_index = GET_PIN(A,  9), .afs[0] = {.uart_num = UART_IS_TX|1, .af_num =  7}},
         { .pin_index = GET_PIN(A, 10), .afs[0] = {.uart_num = UART_IS_RX|1, .af_num =  7}},
         { .pin_index = GET_PIN(A, 11), .afs[0] = {.uart_num = UART_IS_TX|4, .af_num =  6}},
         { .pin_index = GET_PIN(A, 12), .afs[0] = {.uart_num = UART_IS_RX|4, .af_num =  6}},
         { .pin_index = GET_PIN(A, 15), .afs[0] = {.uart_num = UART_IS_RX|7, .af_num = 11}},
         { .pin_index = GET_PIN(A,  8), .afs[0] = {.uart_num = UART_IS_RX|7, .af_num = 11}},
         { .pin_index = GET_PIN(B,  4), .afs[0] = {.uart_num = UART_IS_TX|7, .af_num = 11}},
         { .pin_index = GET_PIN(B,  3), .afs[0] = {.uart_num = UART_IS_RX|7, .af_num = 11}},
         { .pin_index = GET_PIN(B,  5), .afs[0] = {.uart_num = UART_IS_TX|5, .af_num = 14}},
         { .pin_index = GET_PIN(B,  6), .afs[0] = {.uart_num = UART_IS_RX|5, .af_num = 14}},
         { .pin_index = GET_PIN(B,  6), .afs[0] = {.uart_num = UART_IS_TX|1, .af_num =  7}},
         { .pin_index = GET_PIN(B,  7), .afs[0] = {.uart_num = UART_IS_RX|1, .af_num =  7}},
         { .pin_index = GET_PIN(B,  9), .afs[0] = {.uart_num = UART_IS_TX|4, .af_num =  8}},
         { .pin_index = GET_PIN(B,  8), .afs[0] = {.uart_num = UART_IS_RX|4, .af_num =  8}},
         { .pin_index = GET_PIN(B, 10), .afs[0] = {.uart_num = UART_IS_TX|3, .af_num =  7}},
         { .pin_index = GET_PIN(B, 11), .afs[0] = {.uart_num = UART_IS_RX|3, .af_num =  7}},
         { .pin_index = GET_PIN(B, 13), .afs[0] = {.uart_num = UART_IS_TX|5, .af_num = 14}},
         { .pin_index = GET_PIN(B, 12), .afs[0] = {.uart_num = UART_IS_RX|5, .af_num = 14}},
         { .pin_index = GET_PIN(B, 14), .afs[0] = {.uart_num = UART_IS_TX|1, .af_num =  4}},
         { .pin_index = GET_PIN(B, 15), .afs[0] = {.uart_num = UART_IS_RX|1, .af_num =  4}},
         { .pin_index = GET_PIN(C,  6), .afs[0] = {.uart_num = UART_IS_TX|6, .af_num =  7}},
         { .pin_index = GET_PIN(C,  7), .afs[0] = {.uart_num = UART_IS_RX|6, .af_num =  7}},
         { .pin_index = GET_PIN(C, 10), .afs[0] = {.uart_num = UART_IS_TX|3, .af_num =  7}},
         { .pin_index = GET_PIN(C, 11), .afs[0] = {.uart_num = UART_IS_RX|3, .af_num =  7}},
         { .pin_index = GET_PIN(C, 10), .afs[0] = {.uart_num = UART_IS_TX|4, .af_num =  8}},
         { .pin_index = GET_PIN(C, 11), .afs[0] = {.uart_num = UART_IS_RX|4, .af_num =  8}},
         { .pin_index = GET_PIN(C, 12), .afs[0] = {.uart_num = UART_IS_RX|5, .af_num =  8}},
         { .pin_index = GET_PIN(D,  2), .afs[0] = {.uart_num = UART_IS_TX|5, .af_num =  8}},
         { .pin_index = GET_PIN(D,  1), .afs[0] = {.uart_num = UART_IS_TX|4, .af_num =  8}},
         { .pin_index = GET_PIN(D,  0), .afs[0] = {.uart_num = UART_IS_RX|4, .af_num =  8}},
         { .pin_index = GET_PIN(D,  5), .afs[0] = {.uart_num = UART_IS_TX|2, .af_num =  7}},
         { .pin_index = GET_PIN(D,  6), .afs[0] = {.uart_num = UART_IS_RX|2, .af_num =  7}},
         { .pin_index = GET_PIN(D,  8), .afs[0] = {.uart_num = UART_IS_TX|3, .af_num =  7}},
         { .pin_index = GET_PIN(D,  9), .afs[0] = {.uart_num = UART_IS_RX|3, .af_num =  7}},
         { .pin_index = GET_PIN(E,  1), .afs[0] = {.uart_num = UART_IS_TX|8, .af_num =  8}},
         { .pin_index = GET_PIN(E,  0), .afs[0] = {.uart_num = UART_IS_RX|8, .af_num =  8}},
         { .pin_index = GET_PIN(E,  8), .afs[0] = {.uart_num = UART_IS_TX|7, .af_num =  7}},
         { .pin_index = GET_PIN(E,  7), .afs[0] = {.uart_num = UART_IS_RX|7, .af_num =  7}},
         { .pin_index = GET_PIN(F,  7), .afs[0] = {.uart_num = UART_IS_TX|7, .af_num =  7}},
         { .pin_index = GET_PIN(F,  6), .afs[0] = {.uart_num = UART_IS_RX|7, .af_num =  7}},
         { .pin_index = GET_PIN(G, 14), .afs[0] = {.uart_num = UART_IS_TX|6, .af_num =  7}},
         { .pin_index = GET_PIN(G,  9), .afs[0] = {.uart_num = UART_IS_RX|6, .af_num =  7}},
         { .pin_index = GET_PIN(H, 13), .afs[0] = {.uart_num = UART_IS_TX|4, .af_num =  8}},
         { .pin_index = GET_PIN(H, 14), .afs[0] = {.uart_num = UART_IS_RX|4, .af_num =  8}},
         { .pin_index = GET_PIN(G,  8), .afs[0] = {.uart_num = UART_IS_TX|8, .af_num =  8}},
         { .pin_index = GET_PIN(G,  9), .afs[0] = {.uart_num = UART_IS_RX|8, .af_num =  8}},
      };

   /* get tx/rx pin index */
   uart_num = config->name[4] - '0';
   tx_pin_num = stm32_get_pin(tx_port, tx_pin);

   for (index = 0; index < sizeof(uart_afs) / sizeof(struct gpio_uart_af); index = index + 2)
   {
       if (uart_afs[index].pin_index == tx_pin_num)
       {
           if(uart_afs[index].afs[0].uart_num == (uart_num|UART_IS_TX))
           {
               tx_af_num = uart_afs[index].afs[0].af_num;
               rx_af_num = uart_afs[index + 1].afs[0].af_num;
           }
       }
   }

   /* gpio Init */
   GPIO_InitTypeDef GPIO_InitStruct = {0};

   /* gpio ports clock enable */
   stm32_gpio_clk_enable(tx_port);
   if (tx_port != rx_port)
   {
       stm32_gpio_clk_enable(rx_port);
   }

    /* rx pin initialize */
    GPIO_InitStruct.Pin = tx_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
#if defined(SOC_SERIES_STM32L0) || defined(SOC_SERIES_STM32F0) || defined(SOC_SERIES_STM32G0) || defined(SOC_SERIES_STM32H7)
    GPIO_InitStruct.Alternate = tx_af_num;
#endif
    HAL_GPIO_Init(tx_port, &GPIO_InitStruct);

    /* rx pin initialize */
    GPIO_InitStruct.Pin = rx_pin;
#if defined(SOC_SERIES_STM32L0) || defined(SOC_SERIES_STM32F0) || defined(SOC_SERIES_STM32G0) || defined(SOC_SERIES_STM32H7)
    GPIO_InitStruct.Alternate = rx_af_num;
#endif
    HAL_GPIO_Init(rx_port, &GPIO_InitStruct);

    return RT_EOK;
}

__WEAK void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    /* if this uart is shell function */
    if(huart == &handle)
    {
        stm32_gpio_configure(_uart_config);
    }
}

static rt_err_t stm32_configure(struct stm32_uart_config *config)
{
    stm32_uart_clk_enable(config);

    handle.Instance          = config->Instance;
    handle.Init.BaudRate     = 115200;
    handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    handle.Init.Mode         = UART_MODE_TX_RX;
    handle.Init.OverSampling = UART_OVERSAMPLING_16;
    handle.Init.WordLength   = UART_WORDLENGTH_8B;
    handle.Init.StopBits     = UART_STOPBITS_1;
    handle.Init.Parity       = UART_PARITY_NONE;

    if (HAL_UART_Init(&handle) != HAL_OK)
    {
        return -RT_ERROR;
    }

    return RT_EOK;
}

int rt_hw_usart_init(void)
{
    _uart_config = &uart_config[0];
    stm32_configure(_uart_config);

    return RT_EOK;
}
INIT_BOARD_EXPORT(rt_hw_usart_init);

void rt_hw_console_output(const char *str)
{
    rt_size_t i = 0, size = 0;
    char a = '\r';

    size = rt_strlen(str);
    for (i = 0; i < size; i++)
    {
        if (*(str + i) == '\n')
        {
            HAL_UART_Transmit(&handle, (uint8_t *)&a, 1, 1);
        }
        HAL_UART_Transmit(&handle, (uint8_t *)(str + i), 1, 1);
    }
}

#ifdef RT_USING_FINSH
char rt_hw_console_getchar(void)
{
    int ch = -1;

    if (__HAL_UART_GET_FLAG(&handle, UART_FLAG_RXNE) != RESET)
    {
#if defined(SOC_SERIES_STM32L4) || defined(SOC_SERIES_STM32F7) || defined(SOC_SERIES_STM32F0) \
    || defined(SOC_SERIES_STM32L0) || defined(SOC_SERIES_STM32G0) || defined(SOC_SERIES_STM32H7) \
    || defined(SOC_SERIES_STM32G4)
        ch = handle.Instance->RDR & 0xff;
#else
        ch = handle.Instance->DR & 0xff;
#endif
    }
    else
    {
        if(__HAL_UART_GET_FLAG(&handle, UART_FLAG_ORE) != RESET)
        {
            __HAL_UART_CLEAR_OREFLAG(&handle);
        }
        rt_thread_mdelay(10);
    }
    return ch;
}
#endif /* RT_USING_FINSH */
#endif /* RT_USING_CONSLONE */
