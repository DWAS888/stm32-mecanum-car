/**
 * @file    mecanum.c
 * @brief   麦轮运动学模块实现
 */

#include "mecanum.h"
#include "motor.h"

/**
 *
 * X形布局
 * 电机1(左前)  电机2(右前)
 *      \       /
 *       \     /
 *        \   /
 *        /   \
 *       /     \
 *      /       \
 * 电机3(左后)  电机4(右后)
 *
 * 前进：    所有电机正转
 * 后退：    所有电机反转
 * 左移：    1,4反转; 2,3正转
 * 右移：    1,4正转; 2,3反转
 * 顺时针：  1,3反转; 2,4正转
 * 逆时针：  1,3正转; 2,4反转
 */

/* ============ 全局变量 ============ */

static uint8_t current_gear = 0;

static const int16_t gear_speed[] = {0, 30, 50, 70};


/**
 * @brief 麦轮模块初始化
 */
void Mecanum_Init(void)
{
    current_gear = 0;  // 初始停止
}

/**
 * @brief 前进/后退
 *
 * 【X形麦轮前进方向】
 */
void Mecanum_MoveForward(int16_t speed)
{
    // 应用当前档位限制
    int16_t max_speed = gear_speed[current_gear];
    if (max_speed > 0) {
        if (speed > max_speed) speed = max_speed;
        if (speed < -max_speed) speed = -max_speed;
    }

    Motor_SetSpeed(MOTOR_FRONT_LEFT, speed); 
    Motor_SetSpeed(MOTOR_FRONT_RIGHT, speed);
    Motor_SetSpeed(MOTOR_REAR_LEFT, speed);  
    Motor_SetSpeed(MOTOR_REAR_RIGHT, speed); 
}

/**
 * @brief 左右平移
 * 左移：1,4反转; 2,3正转
 * 右移：1,4正转; 2,3反转
 */
void Mecanum_MoveLeftRight(int16_t speed)
{
    int16_t max_speed = gear_speed[current_gear];
    if (max_speed > 0) {
        if (speed > max_speed) speed = max_speed;
        if (speed < -max_speed) speed = -max_speed;
    }

    // speed > 0 = 右移, speed < 0 = 左移
    Motor_SetSpeed(MOTOR_FRONT_LEFT, -speed);
    Motor_SetSpeed(MOTOR_FRONT_RIGHT, speed);
    Motor_SetSpeed(MOTOR_REAR_LEFT, speed);  
    Motor_SetSpeed(MOTOR_REAR_RIGHT, -speed);
}

/**
 * @brief 原地旋转
 *
 * 【原地旋转】
 *
 * 顺时针：左边电机反转，右边电机正转
 * 逆时针：左边电机正转，右边电机反转
 */
void Mecanum_Rotate(int16_t speed)
{
    int16_t max_speed = gear_speed[current_gear];
    if (max_speed > 0) {
        if (speed > max_speed) speed = max_speed;
        if (speed < -max_speed) speed = -max_speed;
    }

    // speed > 0 = 顺时针, speed < 0 = 逆时针
    Motor_SetSpeed(MOTOR_FRONT_LEFT, -speed);
    Motor_SetSpeed(MOTOR_FRONT_RIGHT, speed);
    Motor_SetSpeed(MOTOR_REAR_LEFT, -speed); 
    Motor_SetSpeed(MOTOR_REAR_RIGHT, speed); 
}

/**
 * @brief 斜向移动
 *
 * 斜向移动 = 前进 + 左右
 *
 * 把前进速度和左右速度叠加到每个电机上
 *
 * 电机1速度 = 前进速度 - 左右速度
 * 电机2速度 = 前进速度 + 左右速度
 * 电机3速度 = 前进速度 + 左右速度
 * 电机4速度 = 前进速度 - 左右速度
 */
void Mecanum_MoveDiagonal(int16_t forward_speed, int16_t side_speed)
{
    int16_t max_speed = gear_speed[current_gear];
    if (max_speed > 0) {
        if (forward_speed > max_speed) forward_speed = max_speed;
        if (forward_speed < -max_speed) forward_speed = -max_speed;
        if (side_speed > max_speed) side_speed = max_speed;
        if (side_speed < -max_speed) side_speed = -max_speed;
    }

    int16_t m1 = forward_speed - side_speed;
    int16_t m2 = forward_speed + side_speed;
    int16_t m3 = forward_speed + side_speed;
    int16_t m4 = forward_speed - side_speed;

    // 限制到 -99 到 +99
    if (m1 > 99) m1 = 99; if (m1 < -99) m1 = -99;
    if (m2 > 99) m2 = 99; if (m2 < -99) m2 = -99;
    if (m3 > 99) m3 = 99; if (m3 < -99) m3 = -99;
    if (m4 > 99) m4 = 99; if (m4 < -99) m4 = -99;

    Motor_SetSpeed(MOTOR_FRONT_LEFT, m1);
    Motor_SetSpeed(MOTOR_FRONT_RIGHT, m2);
    Motor_SetSpeed(MOTOR_REAR_LEFT, m3);
    Motor_SetSpeed(MOTOR_REAR_RIGHT, m4);
}

/**
 * @brief 设置速度档位
 */
void Mecanum_SetGear(uint8_t gear)
{
    if (gear > 3) gear = 3;
    current_gear = gear;
}

/**
 * @brief 获取当前档位
 */
uint8_t Mecanum_GetGear(void)
{
    return current_gear;
}

/**
 * @brief 停止所有电机
 */
void Mecanum_Stop(void)
{
    Motor_SetSpeed(MOTOR_FRONT_LEFT, 0);
    Motor_SetSpeed(MOTOR_FRONT_RIGHT, 0);
    Motor_SetSpeed(MOTOR_REAR_LEFT, 0);
    Motor_SetSpeed(MOTOR_REAR_RIGHT, 0);
}
