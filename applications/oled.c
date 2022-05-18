/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-05-12     EdenHU       the first version
 */
#include <rtthread.h>
#include "oled.h"
#include "main.h"
#include "oledfont.h"
#include "bmp.h"

#define OLED_ADD 0x78 //设备地址0x78
/*---------------------------------*/

/*---------------------------------*/
static void oled_cmd(uint8_t i2c_cmd) //发送指令
{
    static uint8_t *cmd;
    cmd = &i2c_cmd;
    HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, 0X00,
                      I2C_MEMADD_SIZE_8BIT, cmd, 1,
                      100);
}
static void oled_data(uint8_t i2c_data) //发送数据
{
    static uint8_t *data;
    data = &i2c_data;
        HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, 0X40,
                          I2C_MEMADD_SIZE_8BIT, data, 1,
                          100);
}
static void oled_origin(uint8_t x,uint8_t y) //设置起始点位置
{
    oled_cmd(0xb0+y);
    oled_cmd(((x&0xf0)>>4)|0x10);
    oled_cmd((x&0x0f)|0x01);
}
void oled_full(uint8_t data) //全屏填充(全亮或全灭)
{
    uint8_t i,n;
    for(i=0;i<8;i++)
    {
        oled_cmd(0xb0+i);
        oled_cmd(0x00);
        oled_cmd(0x10);
        for(n=0;n<128;n++)
        {
            oled_data(data);
        }
    }
}
void oled_clear() //清屏
{
    oled_full(0x00);
}
void oled_init(void) //初始化
{
    HAL_Delay(100);

    oled_cmd(0xAE);//--display off
    oled_cmd(0x20);//---set low column address
    oled_cmd(0x10);//---set high column address
    oled_cmd(0xb0);//--set start line address
    oled_cmd(0xc8); // contract control
    oled_cmd(0x00);//--128
    oled_cmd(0x10);//set segment remap
    oled_cmd(0x40);//Com scan direction
    oled_cmd(0x81);//--normal / reverse
    oled_cmd(0xff);//--set multiplex ratio(1 to 64)
    oled_cmd(0xa1);//--1/32 duty
    oled_cmd(0xa6);//-set display offset
    oled_cmd(0xa8);//
    oled_cmd(0x3F);//set osc division
    oled_cmd(0xa4);
    oled_cmd(0xd3);//Set Pre-Charge Period
    oled_cmd(0x00);//
    oled_cmd(0xd5);//set com pin configuartion
    oled_cmd(0xf0);//
    oled_cmd(0xd9);//set Vcomh
    oled_cmd(0x22);//
    oled_cmd(0xda);
    oled_cmd(0x12);
    oled_cmd(0xdb);//set charge pump enable
    oled_cmd(0x20);//
    oled_cmd(0x8d);
    oled_cmd(0x14);
    oled_cmd(0xaf);//--turn on oled panel

    oled_clear();
}
void oled_show_char(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size) //单字节
{
    unsigned char c=0,i=0;
        c=chr-' ';//得到偏移后的值
        if(x>128-1){
            x=0;
            y=y+2;
        }
        if(Char_Size == 16){
            oled_origin(x,y);
            for(i=0;i<8;i++)
                oled_data(F8X16[c*16+i]);
            oled_origin(x,y+1);
            for(i=0;i<8;i++)
                oled_data(F8X16[c*16+i+8]);
        }
            else {
                oled_origin(x,y);
                for(i=0;i<6;i++)
                oled_data(F6x8[c][i]);
            }
}
void oled_show_string(uint8_t x, uint8_t y, char ch[], uint8_t TextSize) //输出字符串 （x y开始坐标;ch字符串;textsize字体大小)
{
    uint8_t c = 0,i = 0,j = 0;
    switch(TextSize){
    case 1:{
        while(ch[j] != '\0'){
            c = ch[j] - 32;
            if(x > 126){
                x = 0;
                y++;
            }
            oled_origin(x, y);
            for(i = 0;i < 6; i++)
                oled_data(F6x8[c][i]);
            x += 6;
            j++;
        }
    }break;
    case 2:{
        while(ch[j] != '\0'){
            c = ch[j] - 32;
            if(x > 120){
                x = 0;
                y++;
            }
            oled_origin(x,y);
            for(i=0;i<8;i++)
                oled_data(F8X16[c*16+i]);
            oled_origin(x, y+1);
            for(i=0;i<8;i++)
                oled_data(F8X16[c*16+i+8]);
            x += 8;
            j++;
        }
    }break;
    }
}
void oled_show_chinese(uint8_t x,uint8_t y,uint8_t no)
{
    uint8_t t;
    oled_origin(x,y);
    for(t=0;t<32;t++)
    {
        oled_data(Hzk[2*no][t]);
    }
    oled_origin(x,y+1);
    for(t=0;t<32;t++)
    {
        oled_data(Hzk[2*no+1][t]);
    }
}
void oled_show_bmp(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t bmp[])
{
    uint8_t j = 0;
    uint8_t x,y;
    if(y1%8==0)
        y1=y1/8;
    else
        y1=y1/8+1;

    for(y=y0;y<y1;y++)
    {
        oled_origin(x0,y);
        for(x=x0;x<x1;x++)
        {
            oled_data(bmp[j++]);
        }
    }

}
//USER CODE by Eden HU
void oled_show_spectrum(uint8_t x0,uint8_t y0,
                                    float_t spec[],uint8_t length,uint8_t bar_size){
    static uint8_t y1 = 8;
    static uint8_t num;
    uint8_t disp_ctrl_array[length][y1-y0];
    for(uint8_t i = 0;i < length;i++){
        num = (1.0 - spec[i]*20/3.3) * (y1-y0) * 8;
        for(uint8_t j = 0;j < y1-y0;j++){
            disp_ctrl_array[i][j] = (0xffffffffffffffff << (num)) >> ((j-1)*8);
        }
    }

    for(uint8_t y = y0;y < y1; y++){
        oled_origin(x0,y);

        for(uint8_t x = 0;x < length;x = x + 1){
            for(uint8_t count = 0;count < bar_size;count++){
                oled_data(disp_ctrl_array[x][y-y0]);
            }
            oled_data(0x00);

        }

    }

}
