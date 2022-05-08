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
#include "main.h"

#define ADC_PRIORITY 10
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        5

ALIGN(RT_ALIGN_SIZE)
static char t_adc_stack[1024];
static struct rt_thread t_adc;



/* ADC进程入口函数  */
