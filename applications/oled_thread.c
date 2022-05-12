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

static rt_thread_t oled_t;
extern rt_sem_t sem_oled;
extern float32_t data_disp[32];

static void oled_thread_entry(){

}

int oled_thread_init(void){
    oled_t = rt_thread_create("oled_disp", oled_thread_entry, RT_NULL,
            1024, 10, 500);
    if (oled_t != RT_NULL){
        rt_thread_startup(oled_t);
    }else{
        return -1;
    }
    return 0;
}
