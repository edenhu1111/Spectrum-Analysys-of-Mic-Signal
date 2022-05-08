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
#define THREAD_TIMESLICE        500

static rt_thread_t fft_thread;
arm_rfft_fast_instance_f32 S;

extern rt_sem_t dynamic_sem;
extern float32_t fft_in[64];
float32_t fft_output[64];
extern float32_t data_disp[32];

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

//
/* 线程 1 的入口函数 */
void fft_thread_entry(void *parameter)
{
    arm_rfft_fast_init_f32(&S, 64);   //设置FFT相关参数
    while (1)
    {
        rt_sem_take(dynamic_sem, RT_WAITING_FOREVER);
        arm_rfft_fast_f32(&S, fft_in, fft_output,1);
        rt_sem_release(dynamic_sem);
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
