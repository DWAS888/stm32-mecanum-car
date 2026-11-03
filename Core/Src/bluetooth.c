/**
 * @file    bluetooth.c
 * @brief   蓝牙通信模块实现
 */

#include "bluetooth.h"
#include "usart.h"
#include <string.h>

/* ============全局变量============ */

// 接收缓冲区
static uint8_t rx_buffer[BT_RX_BUFFER_SIZE];
static uint16_t rx_index = 0;

// 摇杆数据（-100到+100）
static int16_t joystick_forward = 0;  // 前后
static int16_t joystick_side = 0;     // 左右

// 右摇杆数据（用于机械臂控制）
static int16_t right_stick_x = 0;     // 右摇杆X轴
static int16_t right_stick_y = 0;     // 右摇杆Y轴

// 当前模式和档位
static WorkMode_t current_mode = MODE_REMOTE;
static uint8_t current_gear = 0;

// 循迹转向方向（0=左转, 1=右转）
static uint8_t turn_direction = 0;

// 舵机3开合状态（0=闭合, 1=张开, 0xFF=无效）
static uint8_t gripper_state = 0xFF;

/* ============内部函数============ */

/**
 * @brief 解析接收到的数据帧
 */
static void ParseFrame(uint8_t *frame, uint16_t len)
{
    // 检查最小长度
    if (len < 5) return;

    // 检查帧头帧尾
    if (frame[0] != FRAME_HEADER || frame[4] != FRAME_TAIL) {
        return;
    }

    // 获取命令字
    uint8_t cmd = frame[1];
    uint8_t data1 = frame[2];
    uint8_t data2 = frame[3];

    // 根据命令字处理
    switch (cmd) {
        case CMD_JOYSTICK:
            // 摇杆数据：data1=前后(-100~+100), data2=左右(-100~+100)
            // 有符号数
						joystick_forward = (int8_t)data1 ;
            joystick_side = (int8_t)data2 ;
            break;

        case CMD_MODE:
            // 模式切换：0=遥控, 1=循迹
            if (data1 == 0) {
                current_mode = MODE_REMOTE;
            } else if (data1 == 1) {
                current_mode = MODE_LINE_FOLLOW;
            }
            break;

        case CMD_GEAR:
            // 档位切换：0=停止, 1=低速, 2=中速, 3=高速
            if (data1 <= 3) {
                current_gear = data1;
            }
            break;

        case CMD_STOP:
            // 紧急停止
            joystick_forward = 0;
            joystick_side = 0;
            current_gear = 0;
            break;

        case CMD_TURN_DIR:
            // 转向方向设置：0=左转, 1=右转
            if (data1 <= 1) {
                turn_direction = data1;
            }
            break;

        case CMD_RIGHT_STICK:
            // 右摇杆数据（机械臂控制）：data1=X轴, data2=Y轴
            right_stick_x = (int8_t)data1;
            right_stick_y = (int8_t)data2;
            break;

        case CMD_Gripper:
            // 舵机3开合：0=闭合, 1=张开
            if (data1 <= 1) {
                gripper_state = data1;
            }
            break;

        default:
            // 未知命令
            break;
    }
}

/* ============ 外部函数 ============ */

/**
 * @brief 蓝牙模块初始化
 */
void Bluetooth_Init(void)
{
    // 启动串口中断接收（接收1个字节到rx_buffer）
    HAL_UART_Receive_IT(&huart2, &rx_buffer[0], 1);
}

/**
* @brief 处理接收到的数据（状态机接收）
 */
void Bluetooth_Process(void)
{
    // 在主循环中调用
    // 可以添加超时检测
}

/**
 * @brief 串口接收回调函数（HAL库自动调用）
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart2) {  
        rx_index++;

        if (rx_index >= BT_RX_BUFFER_SIZE) {
            rx_index = 0;
        }

        HAL_UART_Receive_IT(&huart2, &rx_buffer[rx_index], 1);

        if (rx_buffer[rx_index] == FRAME_TAIL && rx_index >= 4) {

            for (int i = 0; i <= rx_index - 4; i++) {
                if (rx_buffer[i] == FRAME_HEADER) {
                    ParseFrame(&rx_buffer[i], 5);
                    break;
                }
            }
            // 重置索引
            rx_index = 0;
        }
    }
}

/**
 * @brief 摇杆前后值
 */
int16_t Bluetooth_GetForward(void)
{
    return joystick_forward;
}

/**
 * @brief 摇杆左右值
 */
int16_t Bluetooth_GetSide(void)
{
    return joystick_side;
}

/**
 * @brief 当前工作模式
 */
WorkMode_t Bluetooth_GetMode(void)
{
    return current_mode;
}

/**
 * @brief 当前档位
 */
uint8_t Bluetooth_GetGear(void)
{
    return current_gear;
}

/**
 * @brief 循迹转向方向
 * @return 0=左转, 1=右转
 */
uint8_t Bluetooth_GetTurnDirection(void)
{
    return turn_direction;
}

/**
 * @brief 右摇杆X轴值
 */
int16_t Bluetooth_GetRightStickX(void)
{
    return right_stick_x;
}

/**
 * @brief 右摇杆Y轴值
 */
int16_t Bluetooth_GetRightStickY(void)
{
    return right_stick_y;
}

/**
 * @brief 获取舵机3开合状态
 * @return 0=闭合, 1=张开, 0xFF=无效
 */
uint8_t Bluetooth_GetGripper(void)
{
    uint8_t state = gripper_state;
    gripper_state = 0xFF;  // 清除，下次返回无效值
    return state;
}

/**
 * @brief 发送数据到蓝牙
 */
void Bluetooth_Send(uint8_t *data, uint16_t len)
{
    HAL_UART_Transmit(&huart2, data, len, 1000);
}
