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
#include "rthw.h"

#define THREAD_PRIORITY         10
#define THREAD_STACK_SIZE       1024
#define THREAD_TIMESLICE        10

extern rt_thread_t fft_thread;
static arm_rfft_fast_instance_f32 S;

extern rt_sem_t dynamic_sem;
extern rt_sem_t sem_oled;
extern float32_t fft_in[64];
extern float32_t fft_output[64];
extern float32_t data_disp[32];
extern float32_t w_blackman[64];


//
/* 线程 1 的入口函数 */
static void fft_thread_entry(void *parameter)
{
//    register rt_base_t level;
    arm_rfft_fast_init_f32(&S, 64);   //设置FFT相关参数

    while (1){

        rt_sem_take(dynamic_sem, RT_WAITING_FOREVER);
        arm_rfft_fast_f32(&S, fft_in, fft_output,1);
        rt_sem_release(dynamic_sem);

        rt_sem_take(sem_oled, RT_WAITING_FOREVER);
        arm_cmplx_mag_squared_f32(fft_output, data_disp, 32);//只取前32个（对称性）
        rt_sem_release(sem_oled);
        rt_thread_mdelay(100);
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
    if (fft_thread != RT_NULL){
        rt_thread_startup(fft_thread);
    }
    else {
        return -1;
    }

    return 0;
}

int fft_test(void){
    extern float32_t in[64],out[64],mag[32];
    arm_rfft_fast_init_f32(&S, 64);
    for(int i=0;i<64;i++){
        in[i] = 10*arm_sin_f32(6.28*i/32*5) * w_blackman[i];
    }
    arm_rfft_fast_f32(&S, in, out, 1);
    arm_cmplx_mag_f32(out, mag, 32);
    for(int i = 0;i<64;i++){
        rt_kprintf("f% \n",mag[i]);
    }

    return 0;
}
MSH_CMD_EXPORT(fft_test,test fft);
