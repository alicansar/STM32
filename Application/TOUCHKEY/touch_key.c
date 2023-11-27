//
// Created by sclliang on 2023/11/27.
//

#include "touch_key.h"
#include "delay/delay.h"

TIM_HandleTypeDef TOUCH_TIM_InitStruct;

/**
* @berif    触摸按键初始化函数
* @param    None
**/
void touch_key_init(void) {
    TOUCH_TIM_InitStruct.Instance = TIM2;
    TOUCH_TIM_InitStruct.Init.CounterMode = TIM_COUNTERMODE_UP;
    TOUCH_TIM_InitStruct.Init.Period = 0xffffff;
    TOUCH_TIM_InitStruct.Init.Prescaler = 83;
    HAL_TIM_IC_Init(&TOUCH_TIM_InitStruct);

    TIM_IC_InitTypeDef icConfig = {0};
    icConfig.ICFilter = 0;
    icConfig.ICPolarity = TIM_ICPOLARITY_RISING;
    icConfig.ICPrescaler = TIM_ICPSC_DIV1;
    icConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
    HAL_TIM_IC_ConfigChannel(&TOUCH_TIM_InitStruct, &icConfig, TIM_CHANNEL_1);

    HAL_TIM_IC_Start(&TOUCH_TIM_InitStruct, TIM_CHANNEL_1);

}

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        __HAL_RCC_TIM2_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitTypeDef TOUCH_KEY_GPIO_InitStruct = {0};
        TOUCH_KEY_GPIO_InitStruct.Pin = GPIO_PIN_5;
        TOUCH_KEY_GPIO_InitStruct.Pull = GPIO_NOPULL;
        TOUCH_KEY_GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        TOUCH_KEY_GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        TOUCH_KEY_GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
        HAL_GPIO_Init(GPIOA, &TOUCH_KEY_GPIO_InitStruct);

    }
}

/**
* @berif    获取触摸按键由低电平跳变到高电平时间，没有触摸时，时间为95us左右，按下触摸按键后时间为200us+
* @param    None
* @return   触摸按键由低电平跳变到高电平时间
**/
uint32_t touch_key_reset(void) {
    GPIO_InitTypeDef TOUCH_KEY_GPIO_InitStruct = {0};
    TOUCH_KEY_GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    TOUCH_KEY_GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    TOUCH_KEY_GPIO_InitStruct.Pin = GPIO_PIN_5;
    TOUCH_KEY_GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA, &TOUCH_KEY_GPIO_InitStruct);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    delay_ms(10);
    TOUCH_KEY_GPIO_InitStruct.Pull = GPIO_NOPULL;
    TOUCH_KEY_GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    TOUCH_KEY_GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    TOUCH_KEY_GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &TOUCH_KEY_GPIO_InitStruct);
    TIM2->SR = 0;
    TIM2->CNT = 0;
    while (__HAL_TIM_GET_FLAG(&TOUCH_TIM_InitStruct, TIM_FLAG_CC1) == RESET) {
        if (TIM2->CNT == 0xffff00) {
            break;
        }
    }


    return TIM2->CNT;

}


