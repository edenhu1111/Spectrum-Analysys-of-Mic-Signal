/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-05-12     EdenHU       the first version
 */
#include "main.h"
#ifndef __OLED_H_
#define __OLED_H_

extern I2C_HandleTypeDef hi2c1;
extern unsigned char bmp1[];
void oled_full(uint8_t data); //全屏填充
void oled_init(void); //初始化
void oled_show_char(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);//单字节
void oled_show_string(uint8_t x, uint8_t y, char ch[], uint8_t TextSize); //输出字符串
void oled_show_chinese(uint8_t x,uint8_t y,uint8_t no); //输出汉字
void oled_show_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t bmp[]);//输出图像
void oled_clear(); //清屏

#endif
