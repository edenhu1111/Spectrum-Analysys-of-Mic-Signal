/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-05-12     EdenHU       the first version
 */
#include "oled_thread.h"
#include <rtthread.h>
#include "arm_math.h"
#include "oled.h"
#include "rthw.h"

extern rt_thread_t oled_t;
extern rt_sem_t sem_oled;
extern float32_t data_disp[32];

static void oled_thread_entry(){
//    register rt_base_t level;

    while(1){
//        level = rt_hw_interrupt_disable();
        oled_clear();
        rt_sem_take(sem_oled, RT_WAITING_FOREVER);
        rt_enter_critical();
        oled_show_spectrum(14, 0, &data_disp[3], 25, 3);
        rt_exit_critical();
        rt_sem_release(sem_oled);

//        rt_hw_interrupt_enable(level);
        rt_thread_mdelay(100);
    }
}

int oled_thread_init(void){
    oled_t = rt_thread_create("oled_disp", oled_thread_entry, RT_NULL,
            1024, 10 , 10);
    if (oled_t != RT_NULL){
        rt_thread_startup(oled_t);
    }else{
        return -1;
    }
    return 0;
}
