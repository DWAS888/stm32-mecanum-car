/**
 * @file    mecanum.h
 * @brief   麦轮运动学模块头文件
 *
 * 麦轮布局：X形
 * 1(左前)     2(右前)
 *     \     /
 *      \   /
 *       \ /
 *       / \
 *      /   \
 *     /     \
 * 3(左后)     4(右后)
 */

#ifndef __MECANUM_H
#define __MECANUM_H

#include "main.h"

// 速度档位定义
#define SPEED_STOP   0
#define SPEED_LOW    30    // 低速档
#define SPEED_MEDIUM 50    // 中速档
#define SPEED_HIGH   70    // 高速档

/**
 * @brief 麦轮运动学模块初始化
 */
void Mecanum_Init(void);

/**
 * @brief 设置整体运动速度（前进/后退）
 * @param speed 速度值 -99 到 +99，正=前进，负=后退
 *
 * @example Mecanum_MoveForward(50);   // 前进50%速度
 *           Mecanum_MoveForward(-30);  // 后退30%速度
 *           Mecanum_MoveForward(0);    // 停止
 */
void Mecanum_MoveForward(int16_t speed);

/**
 * @brief 左右平移
 * @param speed 速度值 -99 到 +99，正=右移，负=左移
 *
 * @example Mecanum_MoveLeftRight(40);  // 右移
 *           Mecanum_MoveLeftRight(-50); // 左移
 */
void Mecanum_MoveLeftRight(int16_t speed);

/**
 * @brief 原地旋转
 * @param speed 速度值 -99 到 +99，正=顺时针，负=逆时针
 *
 * @example Mecanum_Rotate(30);   // 顺时针旋转
 *           Mecanum_Rotate(-40);  // 逆时针旋转
 */
void Mecanum_Rotate(int16_t speed);

/**
 * @brief 斜向移动（结合前进+左右）
 * @param forward_speed 前后速度 -99到+99
 * @param side_speed    左右速度 -99到+99
 *
 * @example Mecanum_MoveDiagonal(50, 50);   // 右前斜向
 *           Mecanum_MoveDiagonal(30, -30); // 左前斜向
 */
void Mecanum_MoveDiagonal(int16_t forward_speed, int16_t side_speed);

/**
 * @brief 设置速度档位
 * @param gear 档位 (1=低速, 2=中速, 3=高速, 0=停止)
 *
 * @example Mecanum_SetGear(2);  // 切换到中速档
 */
void Mecanum_SetGear(uint8_t gear);

/**
 * @brief 获取当前速度档位
 * @return 当前档位值 (0-3)
 */
uint8_t Mecanum_GetGear(void);

/**
 * @brief 停止所有电机
 */
void Mecanum_Stop(void);

#endif
