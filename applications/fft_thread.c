/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-05-08     EdenHU       the first version
 */
#include <rtthread.h>
#include "fft_thread.h"
#include "arm_math.h"

#define THREAD_PRIORITY         10
#define THREAD_STACK_SIZE       1024
#define THREAD_TIMESLICE        100

static rt_thread_t fft_thread;
arm_rfft_fast_instance_f32 S;
extern rt_sem_t dynamic_sem;
extern float32_t signal_for_fft[64];
float32_t fft_output[128];
extern float32_t data_disp[32];
/* 线程 1 的入口函数 */
void fft_thread_entry(void *parameter)
{
    arm_rfft_fast_init_f32(&S, 64);   //设置FFT相关参数
    while (1)
    {
        rt_sem_take(dynamic_sem, RT_WAITING_FOREVER);
        arm_rfft_fast_f32(&S, signal_for_fft, fft_output,0);
        arm_cmplx_mag_squared_f32(fft_output, data_disp, 32);//只取前32个（对称性）
    }
}


/* 线程示例 */
int fft_thread_init(void)
{
    /* 创建线程 1，名称是 thread1，入口是 thread1_entry*/

    fft_thread = rt_thread_create("fft",fft_thread_entry, RT_NULL,
                                    THREAD_STACK_SIZE,
                                    THREAD_PRIORITY, THREAD_TIMESLICE);

    /* 如果获得线程控制块，启动这个线程 */
    if (fft_thread != RT_NULL)
        rt_thread_startup(fft_thread);

    return 0;
}
