/**
  ******************************************************************************
  * @file    main.c
  * @brief   Main program body
  ******************************************************************************
  */

#include "stm32f10x.h"

void GPIO_Config(void);

int main(void)
{
    // 系统初始化（时钟等，已由启动文件调用 SystemInit）
    
    GPIO_Config(); // 用户自定义的 GPIO 初始化

    // 主循环
    while (1)
    {
        // 示例：翻转某个 LED
        GPIOC->ODR ^= GPIO_Pin_13;
        for (volatile int i = 0; i < 500000; i++); // 简单延时
    }
}

// 用户自定义 GPIO 初始化函数
void GPIO_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}