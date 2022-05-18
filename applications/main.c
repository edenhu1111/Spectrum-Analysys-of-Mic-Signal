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
float32_t fft_output[64];
float32_t data_disp[32];
float32_t in[64],out[64],mag[32];
rt_thread_t fft_thread = RT_NULL;
rt_thread_t oled_t = RT_NULL;
struct rt_semaphore sem;
struct rt_semaphore sem2;
rt_sem_t dynamic_sem = &sem;
rt_sem_t sem_oled = &sem2;

float32_t w_blackman[64] = {
        0                  , 0.000898411345182787,0.00363185302536068,0.00831269905220919,0.0151208395116820,0.0242929144241465,0.0361078947913142,0.0508696326538654,
        0.0688871356179178 , 0.0904534243541280 , 0.115823899816671  , 0.145195177544635  , 0.178685338070416  , 0.216316495397925  , 0.258000501503662 ,  0.303528487108778,
        0.352564792171523  ,0.404645669425416   , 0.459182957545964  , 0.515472724548008  , 0.572708684557557  , 0.630000000000000  , 0.686392904097260 ,  0.740895422426631,
        0.792504343401827  , 0.840233491062902  , 0.883142293404331  , 0.920363618099908  , 0.951129865842472  , 0.974796368984157  , 0.990861237575456 ,  0.998980922638693,
        0.998980922638693  , 0.990861237575456  , 0.974796368984157  , 0.951129865842472  , 0.920363618099908  , 0.883142293404331  , 0.840233491062902 ,  0.792504343401827,
        0.740895422426631  , 0.686392904097260  , 0.630000000000000  , 0.572708684557557  , 0.515472724548008  , 0.459182957545964  , 0.404645669425416 ,  0.352564792171523,
        0.303528487108778  , 0.258000501503662  , 0.216316495397925  , 0.178685338070416  , 0.145195177544635  , 0.115823899816671  , 0.0904534243541280 , 0.0688871356179178,
        0.0508696326538654 , 0.0361078947913142 , 0.0242929144241465 , 0.0151208395116820 , 0.00831269905220919, 0.00363185302536068, 0.000898411345182787 ,   0

};
//float32_t test_spec[25];

int main(void)
{
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_TIM3_Init();
    MX_TIM2_Init();
    MX_ADC1_Init();
    MX_USART3_UART_Init();
    HAL_TIM_Base_Start(&htim3);
    HAL_TIM_Base_Start(&htim2);
    HAL_TIM_OC_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_OC_Start(&htim2, TIM_CHANNEL_1);
    oled_init();
    oled_show_string(0, 0, "Program begins!", 1);
    oled_show_string(0, 2, "Author:EdenHU", 2);
    rt_thread_mdelay(2000);
    oled_clear();
//    for(uint8_t i = 0;i<25;i++){
//        test_spec[i] = 3.3/64*2*(25-i);
//    }
//    oled_show_spectrum(14, 0, test_spec, 25, 3);
    rt_thread_mdelay(1000);
    oled_clear();
    rt_sem_init(dynamic_sem, "SIGNAL_SEM", 1, RT_IPC_FLAG_PRIO);
    rt_sem_init(sem_oled,    "OLED_SEM",   1, RT_IPC_FLAG_PRIO);
    int count = 1;

//    HAL_ADCEx_Calibration_Start(&hadc1,ADC_CALIB_OFFSET,ADC_SINGLE_ENDED);
    HAL_ADC_Start_IT(&hadc1);
    fft_thread_init();
    oled_thread_init();

//    rt_system_scheduler_start();
//    HAL_ADC_Start_IT(&hadc1);


    while (count++){
//        LOG_D("Hello RT-Thread!");
        rt_thread_mdelay(1000);
    }
//    LOG_D("-----------END-----------");
    return RT_EOK;
}
