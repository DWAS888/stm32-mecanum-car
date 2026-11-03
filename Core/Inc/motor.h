/**
 *
 * 这个模块控制4个直流电机
 * 电机1: 左前  (TIM3_CH3 = PB0, 方向: PB9/PB8 = AIN1/AIN2)
 * 电机2: 右前  (TIM3_CH4 = PB1, 方向: PB7/PB6 = BIN1/BIN2)
 * 电机3: 左后  (TIM3_CH1 = PB4, 方向: PC9/PC8 = CIN1/CIN2)
 * 电机4: 右后  (TIM3_CH2 = PB5, 方向: PC7/PC6 = DIN1/DIN2)
 */

#ifndef __MOTOR_H
#define __MOTOR_H

#include "main.h"

// 电机编号定义
#define MOTOR_FRONT_LEFT   1   // 左前电机
#define MOTOR_FRONT_RIGHT  2   // 右前电机
#define MOTOR_REAR_LEFT    3   // 左后电机
#define MOTOR_REAR_RIGHT   4   // 右后电机

// 方向定义
#define DIR_FORWARD  1   // 正转
#define DIR_BACKWARD 0   // 反转

/**
 * @brief 初始化电机模块
 * @note  启动PWM定时器，设置所有电机初始状态为停止
 */
void Motor_Init(void);

/**
 * @brief 设置单个电机的速度和方向
 * @param motor  电机编号 (MOTOR_FRONT_LEFT 等)
 * @param speed  速度值 0-99 (0=停止, 99=全速)
 * @param dir    方向 (DIR_FORWARD 或 DIR_BACKWARD)
 *
 * @example Motor_Set(MOTOR_FRONT_LEFT, 50, DIR_FORWARD);  // 左前电机50%速度正转
 */
void Motor_Set(uint8_t motor, int16_t speed, uint8_t dir);

/**
 * @brief 设置单个电机的速度（带符号，正=正转，负=反转）
 * @param motor  电机编号
 * @param speed  速度值 -99 到 +99
 *              正数=正转，负数=反转，0=停止
 *
 * @example Motor_SetSpeed(MOTOR_FRONT_LEFT, 50);   // 正转50%
 *           Motor_SetSpeed(MOTOR_FRONT_LEFT, -30);  // 反转30%
 *           Motor_SetSpeed(MOTOR_FRONT_LEFT, 0);    // 停止
 */
void Motor_SetSpeed(uint8_t motor, int16_t speed);

/**
 * @brief 紧急刹车，停止所有电机
 */
void Motor_Brake(void);

#endif


