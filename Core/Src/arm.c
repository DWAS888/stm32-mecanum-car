/**
 * @file    arm.c
 * @brief   机械臂控制模块实现
 */

#include "arm.h"
#include "bluetooth.h"
#include "tim.h"

// 外部引用定时器句柄
extern TIM_HandleTypeDef htim1;

// 当前舵机角度
static uint8_t servo1_angle = SERVO1_INIT_ANGLE;
static uint8_t servo2_angle = SERVO2_INIT_ANGLE;
static uint8_t servo3_angle = SERVO3_INIT_ANGLE;

// 舵机3开合状态
static uint8_t gripper_state = 1;  // 1=张开, 0=闭合

/* ============ 内部函数 ============ */

/**
 * @brief 角度转PWM CCR值
 * @param angle  角度 (0-150)
 * @return CCR值 (1000-4000)
 *
 * 线性映射：angle -> CCR
 * 0°   -> 1000 (500us)
 * 150° -> 4000 (2000us)
 */
static uint16_t AngleToCCR(uint8_t angle)
{
    if (angle > 150) angle = 150;

    // 线性映射: CCR = 1000 + (angle / 150) * 3000
    return SERVO_PWM_MIN + (uint32_t)angle * (SERVO_PWM_MAX - SERVO_PWM_MIN) / 150;
}

/* ============ 外部函数 ============ */

/**
 * @brief 机械臂初始化
 */
void Arm_Init(void)
{
    // 启动TIM1 PWM
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);   // 舵机3 (PE9)
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);   // 舵机1 (PE13)
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);   // 舵机2 (PE14)

    // 设置初始角度
    Arm_SetAngle(ARM_SERVO1, SERVO1_INIT_ANGLE);
    Arm_SetAngle(ARM_SERVO2, SERVO2_INIT_ANGLE);
    Arm_SetAngle(ARM_SERVO3, SERVO3_INIT_ANGLE);

    gripper_state = 1;  // 初始张开
}

/**
 * @brief 设置舵机角度
 */
void Arm_SetAngle(uint8_t servo, uint8_t angle)
{
    uint16_t ccr = AngleToCCR(angle);

    switch (servo) {
        case ARM_SERVO1:
            servo1_angle = angle;
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, ccr);
            break;

        case ARM_SERVO2:
            servo2_angle = angle;
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, ccr);
            break;

        case ARM_SERVO3:
            servo3_angle = angle;
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, ccr);
            break;

        default:
            break;
    }
}

/**
 * @brief 设置舵机3开合状态
 */
void Arm_SetGripper(uint8_t state)
{
    gripper_state = state;

    if (state == 0) {
        // 闭合
        Arm_SetAngle(ARM_SERVO3, SERVO3_CLOSE_ANGLE);
    } else {
        // 张开
        Arm_SetAngle(ARM_SERVO3, SERVO3_OPEN_ANGLE);
    }
}

/**
 * @brief 获取舵机当前角度
 */
uint8_t Arm_GetAngle(uint8_t servo)
{
    switch (servo) {
        case ARM_SERVO1:
            return servo1_angle;
        case ARM_SERVO2:
            return servo2_angle;
        case ARM_SERVO3:
            return servo3_angle;
        default:
            return 0;
    }
}

/**
 * @brief 机械臂控制更新（根据蓝牙数据）
 *
 * 使用右摇杆控制舵机1和舵机2：
 * - 右摇杆X轴 (-100~+100) -> 舵机1角度
 * - 右摇杆Y轴 (-100~+100) -> 舵机2角度
 */
void Arm_Update(void)
{
    // 获取右摇杆数据
    int16_t stick_x = Bluetooth_GetRightStickX();
    int16_t stick_y = Bluetooth_GetRightStickY();

    // 舵机1：右摇杆X轴控制
    // X=-100 -> 最小角度, X=+100 -> 最大角度
    if (abs(stick_x) > 15) {  // 死区阈值
        uint8_t angle1 = SERVO1_INIT_ANGLE + (stick_x * (SERVO1_MAX_ANGLE - SERVO1_MIN_ANGLE) / 200);
        if (angle1 < SERVO1_MIN_ANGLE) angle1 = SERVO1_MIN_ANGLE;
        if (angle1 > SERVO1_MAX_ANGLE) angle1 = SERVO1_MAX_ANGLE;
        Arm_SetAngle(ARM_SERVO1, angle1);
    }

    // 舵机2：右摇杆Y轴控制
    // Y=-100 -> 最小角度, Y=+100 -> 最大角度
    if (abs(stick_y) > 15) {  // 死区阈值
        uint8_t angle2 = SERVO2_INIT_ANGLE + (stick_y * (SERVO2_MAX_ANGLE - SERVO2_MIN_ANGLE) / 200);
        if (angle2 < SERVO2_MIN_ANGLE) angle2 = SERVO2_MIN_ANGLE;
        if (angle2 > SERVO2_MAX_ANGLE) angle2 = SERVO2_MAX_ANGLE;
        Arm_SetAngle(ARM_SERVO2, angle2);
    }

    // 舵机3开合：LB/RB按钮控制（在蓝牙模块解析）
    uint8_t gripper = Bluetooth_GetGripper();
    if (gripper != 0xFF) {  // 有效值
        Arm_SetGripper(gripper);
    }
}