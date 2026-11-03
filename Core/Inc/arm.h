/**
 * @file    arm.h
 * @brief   机械臂控制模块头文件
 *
 * 舵机配置：
 * - 舵机1 (PWM1): PE13, TIM1_CH3 - 大范围活动
 * - 舵机2 (PWM2): PE14, TIM1_CH4 - 大范围活动
 * - 舵机3 (PWM3): PE9,  TIM1_CH1 - 开合（只有两个状态）
 *
 * PWM周期：10ms (TIM1配置: Prescaler=83, Period=19999)
 * 舵机脉冲宽度范围：约500us~2000us，对应角度0°~150°
 */

#ifndef __ARM_H
#define __ARM_H

#include "main.h"

// 舵机编号定义
#define ARM_SERVO1     1    // 舵机1 (PE13, TIM1_CH3)
#define ARM_SERVO2     2    // 舵机2 (PE14, TIM1_CH4)
#define ARM_SERVO3     3    // 舵机3 (PE9,  TIM1_CH1) - 开合

// 舵机角度范围定义（可根据实际调试修改）
#define SERVO1_MIN_ANGLE    0      // 舵机1最小角度
#define SERVO1_MAX_ANGLE    150    // 舵机1最大角度
#define SERVO1_INIT_ANGLE   75     // 舵机1初始角度（中间位置）

#define SERVO2_MIN_ANGLE    0      // 舵机2最小角度
#define SERVO2_MAX_ANGLE    150    // 舵机2最大角度
#define SERVO2_INIT_ANGLE   75     // 舵机2初始角度（中间位置）

// 舵机3开合角度（只有两个状态）
#define SERVO3_OPEN_ANGLE   120    // 张开角度
#define SERVO3_CLOSE_ANGLE  30     // 闭合角度
#define SERVO3_INIT_ANGLE   SERVO3_OPEN_ANGLE  // 初始张开

// PWM脉冲宽度对应的CCR值（基于TIM1: 2MHz时钟）
#define SERVO_PWM_MIN       1000   // 500us -> CCR=1000 (约0°)
#define SERVO_PWM_MAX       4000   // 2000us -> CCR=4000 (约150°)

/**
 * @brief 机械臂初始化
 */
void Arm_Init(void);

/**
 * @brief 设置舵机角度
 * @param servo  舵机编号 (ARM_SERVO1, ARM_SERVO2, ARM_SERVO3)
 * @param angle  角度值 (0-150)
 */
void Arm_SetAngle(uint8_t servo, uint8_t angle);

/**
 * @brief 设置舵机3开合状态
 * @param state  0=闭合, 1=张开
 */
void Arm_SetGripper(uint8_t state);

/**
 * @brief 获取舵机当前角度
 * @param servo  舵机编号
 * @return 当前角度
 */
uint8_t Arm_GetAngle(uint8_t servo);

/**
 * @brief 机械臂控制更新（根据蓝牙数据）
 */
void Arm_Update(void);

#endif