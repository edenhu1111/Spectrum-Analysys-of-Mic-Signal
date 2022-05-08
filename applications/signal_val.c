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
#include "signal_sem.h"

int sem_init(rt_sem_t sem)
{
    rt_sem_init(sem, "SIGNAL_SEM", 0, RT_IPC_FLAG_PRIO);
    if (sem == RT_NULL)
    {
        rt_kprintf("create dynamic semaphore failed.\n");
        return -1;
    }
    return 0;
}
