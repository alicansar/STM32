//
// Created by sclliang on 2023/11/27.
//

#ifndef F407ZG_HAL_CLION_TOUCH_KEY_H
#define F407ZG_HAL_CLION_TOUCH_KEY_H

#include "sys/sys.h"

void touch_key_init(void);      //触摸按键初始化函数

uint32_t touch_key_reset(void); //获取是否处于触摸状态，当处于触摸状态时返回值大于200，反之在95左右

#endif //F407ZG_HAL_CLION_TOUCH_KEY_H
