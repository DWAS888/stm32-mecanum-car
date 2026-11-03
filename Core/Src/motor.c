/**
 * @file    motor.c
 * @brief   电机驱动模块实现
 */

#include "motor.h"

// 外部引用定时器句柄
extern TIM_HandleTypeDef htim3;

/* ============电机引脚定义============ */

// 电机1: 左前 (AIN1/AIN2)
#define AIN1_PORT  GPIOB
#define AIN1_PIN   GPIO_PIN_9
#define AIN2_PORT  GPIOB
#define AIN2_PIN   GPIO_PIN_8
#define AIN1_CH    TIM_CHANNEL_3

// 电机2: 右前 (BIN1/BIN2)
#define BIN1_PORT  GPIOB
#define BIN1_PIN   GPIO_PIN_7
#define BIN2_PORT  GPIOB
#define BIN2_PIN   GPIO_PIN_6
#define BIN1_CH    TIM_CHANNEL_4

// 电机3: 左后 (CIN1/CIN2)
#define CIN1_PORT  GPIOC
#define CIN1_PIN   GPIO_PIN_9
#define CIN2_PORT  GPIOC
#define CIN2_PIN   GPIO_PIN_8
#define CIN1_CH    TIM_CHANNEL_1

// 电机4: 右后 (DIN1/DIN2)
#define DIN1_PORT  GPIOC
#define DIN1_PIN   GPIO_PIN_7
#define DIN2_PORT  GPIOC
#define DIN2_PIN   GPIO_PIN_6
#define DIN1_CH    TIM_CHANNEL_2

/* ============函数实现============ */

void Motor_Init(void)
{
    // 启动PWM定时器
    HAL_TIM_Base_Start(&htim3);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);  
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);  
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);  
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);  

    // 所有方向引脚设为低电平（初始状态）
    HAL_GPIO_WritePin(AIN1_PORT, AIN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AIN2_PORT, AIN2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BIN1_PORT, BIN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BIN2_PORT, BIN2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(CIN1_PORT, CIN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(CIN2_PORT, CIN2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DIN1_PORT, DIN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DIN2_PORT, DIN2_PIN, GPIO_PIN_RESET);

    // 所有PWM设为0（电机停止）
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);
}

/**
 * @brief 设置电机方向（内部函数）
 *
 * - AIN1=1, AIN2=0  -> 正转
 * - AIN1=0, AIN2=1  -> 反转
 * - AIN1=0, AIN2=0  -> 停止（刹车）
 * - AIN1=1, AIN2=1  -> 停止（刹车）
 */
static void Motor_SetDir(uint8_t motor, uint8_t dir)
{
    if (dir == DIR_FORWARD) {
        // 正转：AIN1=1, AIN2=0
        switch (motor) {
            case MOTOR_FRONT_LEFT:
                HAL_GPIO_WritePin(AIN1_PORT, AIN1_PIN, GPIO_PIN_SET);
                HAL_GPIO_WritePin(AIN2_PORT, AIN2_PIN, GPIO_PIN_RESET);
                break;
            case MOTOR_FRONT_RIGHT:
                HAL_GPIO_WritePin(BIN1_PORT, BIN1_PIN, GPIO_PIN_SET);
                HAL_GPIO_WritePin(BIN2_PORT, BIN2_PIN, GPIO_PIN_RESET);
                break;
            case MOTOR_REAR_LEFT:
                HAL_GPIO_WritePin(CIN1_PORT, CIN1_PIN, GPIO_PIN_SET);
                HAL_GPIO_WritePin(CIN2_PORT, CIN2_PIN, GPIO_PIN_RESET);
                break;
            case MOTOR_REAR_RIGHT:
                HAL_GPIO_WritePin(DIN1_PORT, DIN1_PIN, GPIO_PIN_SET);
                HAL_GPIO_WritePin(DIN2_PORT, DIN2_PIN, GPIO_PIN_RESET);
                break;
        }
    } else {
        // 反转：AIN1=0, AIN2=1
        switch (motor) {
            case MOTOR_FRONT_LEFT:
                HAL_GPIO_WritePin(AIN1_PORT, AIN1_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(AIN2_PORT, AIN2_PIN, GPIO_PIN_SET);
                break;
            case MOTOR_FRONT_RIGHT:
                HAL_GPIO_WritePin(BIN1_PORT, BIN1_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(BIN2_PORT, BIN2_PIN, GPIO_PIN_SET);
                break;
            case MOTOR_REAR_LEFT:
                HAL_GPIO_WritePin(CIN1_PORT, CIN1_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(CIN2_PORT, CIN2_PIN, GPIO_PIN_SET);
                break;
            case MOTOR_REAR_RIGHT:
                HAL_GPIO_WritePin(DIN1_PORT, DIN1_PIN, GPIO_PIN_RESET);
                HAL_GPIO_WritePin(DIN2_PORT, DIN2_PIN, GPIO_PIN_SET);
                break;
        }
    }
}

/**
 * @brief 设置电机PWM速度（内部函数）
**/
static void Motor_SetPWM(uint8_t motor, uint16_t speed)
{
    // 限制速度范围 0-99
    if (speed > 99) speed = 99;

    switch (motor) {
        case MOTOR_FRONT_LEFT:
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, speed);
            break;
        case MOTOR_FRONT_RIGHT:
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, speed);
            break;
        case MOTOR_REAR_LEFT:
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, speed);
            break;
        case MOTOR_REAR_RIGHT:
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, speed);
            break;
    }
}

/**
 * @brief 设置单个电机的速度和方向
 */
void Motor_Set(uint8_t motor, int16_t speed, uint8_t dir)
{
    Motor_SetDir(motor, dir);   
    Motor_SetPWM(motor, speed); 
}

/**
 * @brief 设置电机速度（带符号）
 */
void Motor_SetSpeed(uint8_t motor, int16_t speed)
{
    // 限制范围
    if (speed > 99) speed = 99;
    if (speed < -99) speed = -99;

    if (speed > 0) {
        // 正转
        Motor_Set(motor, speed, DIR_FORWARD);
    } else if (speed < 0) {
        // 反转
        Motor_Set(motor, -speed, DIR_BACKWARD);
    } else {
        // 停止
        Motor_SetPWM(motor, 0);
        // 两个方向引脚都设为0，刹车效果更好
    }
}

/**
 * @brief 紧急刹车
 */
void Motor_Brake(void)
{
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);
}
