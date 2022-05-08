/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-05-03     RT-Thread    first version
 */

#include <rtthread.h>
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "signal_sem.h"
#include "fft_thread.h"
#include "arm_math.h"

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

//uint16_t audio_signal[64];
float32_t signal_for_fft[64];
float32_t data_disp[32];
struct rt_semaphore sem;
rt_sem_t dynamic_sem = &sem;

int main(void)
{
    MX_GPIO_Init();
    MX_ADC1_Init();
    MX_I2C1_Init();
    MX_TIM1_Init();
    MX_USART3_UART_Init();
    sem_init(dynamic_sem);
    int count = 1;
    HAL_TIM_OC_Start(&htim1, TIM_CHANNEL_1);
    HAL_ADCEx_Calibration_Start(&hadc1,ADC_CALIB_OFFSET,ADC_SINGLE_ENDED);
    HAL_ADC_Start_IT(&hadc1);
    fft_thread_init();
    while (count++)
    {
        LOG_D("Hello RT-Thread!");
        rt_thread_mdelay(1000);
    }
    LOG_D("-----------END-----------");
    return RT_EOK;
}