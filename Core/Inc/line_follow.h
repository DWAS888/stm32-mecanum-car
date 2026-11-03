/**
 * @file    line_follow.h
 * @brief   循迹模块头文件
 *
 * 四路循迹传感器布局（从左到右）：
 *
 *    [PD11]  [PD10]  [PD9]  [PD8]
 *     最左    左     右    最右
 *
 * 传感器输出：0=检测到黑线, 1=检测到白色
 */

#ifndef __LINE_FOLLOW_H
#define __LINE_FOLLOW_H

#include "main.h"

// 传感器引脚定义
#define SENSOR_LEFT_OUT    GPIO_PIN_11   // PD11 最左
#define SENSOR_LEFT        GPIO_PIN_10   // PD10 左
#define SENSOR_RIGHT       GPIO_PIN_9    // PD9  右
#define SENSOR_RIGHT_OUT   GPIO_PIN_8    // PD8  最右

#define SENSOR_PORT        GPIOD

// 循迹状态
typedef enum {
    LINE_LOST = -2,       // 丢线（完全偏离）
    LINE_FAR_LEFT = -1,   // 严重偏左
    LINE_CENTER = 0,      // 居中
    LINE_LEFT = 1,        // 轻微偏左
    LINE_RIGHT = 2,       // 轻微偏右
    LINE_FAR_RIGHT = 3,   // 严重偏右
    LINE_ALL_BLACK = 4,   // 全黑（十字/选择弯/T型）
} LineState_t;

/**
 * @brief 循迹模块初始化
 */
void LineFollow_Init(void);

/**
 * @brief 读取所有传感器状态
 * @return 4位二进制数，bit0=最右, bit3=最左
 */
uint8_t LineFollow_ReadSensors(void);

/**
 * @brief 获取当前循迹状态
 * @return LINE_CENTER, LINE_LEFT, LINE_RIGHT 等
 */
LineState_t LineFollow_GetState(void);

/**
 * @brief 循迹控制函数（在主循环调用）
 */
void LineFollow_Update(void);

/**
 * @brief 设置循迹速度
 * @param speed 前进速度 0-99
 */
void LineFollow_SetSpeed(uint8_t speed);

/**
 * @brief 获取循迹速度
 * @return 当前速度值 0-99
 */
uint8_t LineFollow_GetSpeed(void);

#endif
