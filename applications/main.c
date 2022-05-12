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
#include "fft_thread.h"
#include "arm_math.h"
#include "oled.h"
#include "oled_thread.h"

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

float32_t signal_for_fft[64];
float32_t fft_in[64];
float32_t data_disp[32];
float32_t in[64],out[64],mag[32];
struct rt_semaphore sem;
struct rt_semaphore sem2;
rt_sem_t dynamic_sem = &sem;
rt_sem_t sem_oled = &sem2;

int main(void)
{
    MX_GPIO_Init();
    MX_ADC1_Init();
    MX_I2C1_Init();
    MX_TIM1_Init();
    MX_USART3_UART_Init();
    oled_init();
    oled_show_string(0, 0, "Stand With Carol!", 1);
    oled_show_string(0, 2, "FUCK A-SOUL!", 2);
    rt_sem_init(dynamic_sem, "SIGNAL_SEM", 1, RT_IPC_FLAG_PRIO);
    rt_sem_init(sem_oled,    "OLED_SEM",   1, RT_IPC_FLAG_PRIO);
    int count = 1;
    HAL_TIM_OC_Start(&htim1, TIM_CHANNEL_1);
    HAL_ADCEx_Calibration_Start(&hadc1,ADC_CALIB_OFFSET,ADC_SINGLE_ENDED);
    HAL_ADC_Start_IT(&hadc1);
    fft_thread_init();
    oled_thread_init();
    fft_test();

    while (count++){
//        LOG_D("Hello RT-Thread!");
        rt_thread_mdelay(1000);
    }
    LOG_D("-----------END-----------");
    return RT_EOK;
}
