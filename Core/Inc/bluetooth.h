/**
 * @file    bluetooth.h
 * @brief   蓝牙通信模块头文件
 *
 * 通信协议（帧头帧尾）：
 * [帧头][命令字][数据1][数据2][帧尾]
 *
 * 帧头：0xAA
 * 帧尾：0x55
 *
 * 命令字定义：
 * 0x01 - 摇杆数据（data1=前后-100~100, data2=左右-100~100）
 * 0x02 - 模式切换（data1=0遥控, 1循迹）
 * 0x03 - 档位切换（data1=0停止, 1低速, 2中速, 3高速）
 * 0x04 - 紧急停止
 */

#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "main.h"

// 接收缓冲区大小
#define BT_RX_BUFFER_SIZE 64

// 命令字定义
#define CMD_JOYSTICK    0x01    // 摇杆数据（左摇杆：小车控制）
#define CMD_MODE        0x02    // 模式切换
#define CMD_GEAR        0x03    // 档位切换
#define CMD_STOP        0x04    // 紧急停止
#define CMD_TURN_DIR    0x05    // 转向方向设置（循迹用，data1=0左转, 1右转）
#define CMD_RIGHT_STICK 0x06    // 右摇杆数据（机械臂控制：data1=X轴, data2=Y轴）
#define CMD_Gripper     0x07    // 舵机3开合（data1=0闭合, 1张开）

// 帧头帧尾
#define FRAME_HEADER    0xAA
#define FRAME_TAIL      0x55

// 工作模式
typedef enum {
    MODE_REMOTE = 0,    // 遥控模式
    MODE_LINE_FOLLOW    // 循迹模式
} WorkMode_t;

/**
 * @brief 蓝牙模块初始化
 */
void Bluetooth_Init(void);

/**
 * @brief 接收数据处理（在主循环中调用）
 * @note  会解析接收到的数据并更新摇杆值、模式等
 */
void Bluetooth_Process(void);

/**
 * @brief 获取摇杆前后值
 * @return 前后值 -100 到 +100（+100=最前, -100=最后）
 */
int16_t Bluetooth_GetForward(void);

/**
 * @brief 获取摇杆左右值
 * @return 左右值 -100 到 +100（+100=最右, -100=最左）
 */
int16_t Bluetooth_GetSide(void);

/**
 * @brief 获取当前工作模式
 * @return MODE_REMOTE 或 MODE_LINE_FOLLOW
 */
WorkMode_t Bluetooth_GetMode(void);

/**
 * @brief 获取当前档位
 * @return 0=停止, 1=低速, 2=中速, 3=高速
 */
uint8_t Bluetooth_GetGear(void);

/**
 * @brief 获取循迹转向方向
 * @return 0=左转, 1=右转
 */
uint8_t Bluetooth_GetTurnDirection(void);

/**
 * @brief 获取右摇杆X轴值（用于机械臂控制）
 * @return X轴值 -100 到 +100
 */
int16_t Bluetooth_GetRightStickX(void);

/**
 * @brief 获取右摇杆Y轴值（用于机械臂控制）
 * @return Y轴值 -100 到 +100
 */
int16_t Bluetooth_GetRightStickY(void);

/**
 * @brief 获取舵机3开合状态
 * @return 0=闭合, 1=张开, 0xFF=无效（未收到命令）
 */
uint8_t Bluetooth_GetGripper(void);

/**
 * @brief 发送数据到蓝牙（可选）
 * @param data 数据指针
 * @param len  数据长度
 */
void Bluetooth_Send(uint8_t *data, uint16_t len);

#endif
