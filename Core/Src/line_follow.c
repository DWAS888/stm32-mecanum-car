/**
 * @file    line_follow.c
 * @brief   循迹模块实现
 */

#include "line_follow.h"
#include "mecanum.h"
#include "bluetooth.h"

/* ============全局变量============ */

static uint8_t line_speed = 25;  // 光滑地面用较低速度

// 上一次有效状态
static LineState_t last_valid_state = LINE_CENTER;

// 丢线计数器
static uint16_t lost_count = 0;

// 全白计数器（用于断点判断）
static uint16_t white_count = 0;

// 全黑计数器（用于十字/选择弯/T型判断）
static uint16_t black_count = 0;

// 路径进度标志
static uint8_t passed_cross = 0;
static uint8_t passed_turn = 0; 

/* ============内部函数声明============ */

static void ExecuteTurn(void);



void LineFollow_Init(void)
{
    last_valid_state = LINE_CENTER;
    lost_count = 0;
    white_count = 0;
    black_count = 0;
    passed_cross = 0;
    passed_turn = 0;
}

/**
 * @brief 读取所有传感器状态
 * 返回4位二进制，bit3=最左, bit0=最右
 * 0=黑, 1=白
 */
uint8_t LineFollow_ReadSensors(void)
{
    uint8_t s3, s2, s1, s0;

    s3 = HAL_GPIO_ReadPin(SENSOR_PORT, SENSOR_LEFT_OUT);    // 最左
    s2 = HAL_GPIO_ReadPin(SENSOR_PORT, SENSOR_LEFT);        // 左
    s1 = HAL_GPIO_ReadPin(SENSOR_PORT, SENSOR_RIGHT);       // 右
    s0 = HAL_GPIO_ReadPin(SENSOR_PORT, SENSOR_RIGHT_OUT);   // 最右

    return (s3 << 3) | (s2 << 2) | (s1 << 1) | s0;
}

/**
 * @brief 获取当前循迹状态
 */
LineState_t LineFollow_GetState(void)
{
    uint8_t sensors = LineFollow_ReadSensors();

    // 快丢线检测：只有最左或最右检测到
    if (sensors == 0b1000) return LINE_FAR_LEFT;
    if (sensors == 0b0001) return LINE_FAR_RIGHT;

    switch (sensors) {
        case 0b0110:  
            return LINE_CENTER;

        case 0b0100: 
        case 0b1100: 
            return LINE_LEFT;

        case 0b0010: 
        case 0b0011: 
            return LINE_RIGHT;

        case 0b1111:
            return LINE_LOST;

        case 0b0000:
            return LINE_ALL_BLACK;

        default:
            return LINE_LOST;
    }
}

/**
 * @brief 处理全白状态（断点）
 */
static void HandleWhiteLine(void)
{
    white_count++;

    if (white_count > 10) {
        // 确认是断点，保持直行
        Mecanum_MoveForward(line_speed);
        return;
    }

    // 刚检测到全白，根据上次状态调整
    switch (last_valid_state) {
        case LINE_CENTER:
        case LINE_LEFT:
        case LINE_RIGHT:
            Mecanum_MoveForward(line_speed);
            break;

        case LINE_FAR_LEFT:
            Mecanum_MoveLeftRight(20);  
            break;

        case LINE_FAR_RIGHT:
            Mecanum_MoveLeftRight(-20); 
            break;

        default:
            Mecanum_Stop();
            break;
    }
}

/**
 * @brief 处理全黑状态（十字/选择弯/T型）
 */
static void HandleBlackLine(void)
{
    black_count++;

    // 阈值需根据实际调试（主循环频率影响）
    // 如果主循环有HAL_Delay(1)，black_count > 10表示约10ms全黑
    if (black_count > 10) {
        if (!passed_cross) {
            // 未过十字 → 十字停止点
            Mecanum_Stop();
            HAL_Delay(4000);  // 停4秒
            black_count = 0;
            passed_cross = 1;  // 标记已过十字

        } else if (!passed_turn) {
            // 已过十字 + 未过选择弯 → 选择直角弯
            ExecuteTurn();
            black_count = 0;

        } else {
            // 已过选择弯 → T型停止点
            Mecanum_Stop();
            while (1);  // 永久停止
        }
    }
}

/**
 * @brief 执行选择直角弯
 */
static void ExecuteTurn(void)
{
    uint8_t dir = Bluetooth_GetTurnDirection();

    Mecanum_Stop();
    HAL_Delay(100);

    if (dir == 0) {
        Mecanum_MoveLeftRight(-30);
    } else {
        Mecanum_MoveLeftRight(30);
    }

    HAL_Delay(500);

    passed_turn = 1;
}

/**
 * @brief 循迹控制更新
 */
void LineFollow_Update(void)
{
    uint8_t sensors = LineFollow_ReadSensors();
    LineState_t state = LineFollow_GetState();

    // 检测全白（断点）
    if (sensors == 0b1111) {
        HandleWhiteLine();
        return;
    }
    white_count = 0;

    // 检测全黑（十字/选择弯/T型）
    if (sensors == 0b0000) {
        HandleBlackLine();
        return;
    }
    black_count = 0;

    // 正常循迹
    if (state != LINE_LOST) {
        last_valid_state = state;
        lost_count = 0;
    }

    switch (state) {
        case LINE_CENTER:
            Mecanum_MoveForward(line_speed);
            break;

        case LINE_LEFT:  
            Mecanum_MoveDiagonal(line_speed, 8);
            break;

        case LINE_RIGHT:
            Mecanum_MoveDiagonal(line_speed, -8);
            break;

        case LINE_FAR_LEFT:
            Mecanum_MoveLeftRight(20);
            break;

        case LINE_FAR_RIGHT:
            Mecanum_MoveLeftRight(-20);
            break;

        case LINE_LOST:
            switch (last_valid_state) {
                case LINE_CENTER:
                case LINE_LEFT:
                case LINE_RIGHT:
                    Mecanum_MoveForward(line_speed);
                    lost_count++;
                    if (lost_count > 100) {
                        Mecanum_Stop();
                    }
                    break;

                case LINE_FAR_LEFT:
                    Mecanum_MoveLeftRight(25);
                    break;

                case LINE_FAR_RIGHT:
                    Mecanum_MoveLeftRight(-25);
                    break;

                default:
                    Mecanum_Stop();
                    break;
            }
            break;

        case LINE_ALL_BLACK:
            // 不会到这里，前面已处理
            break;
    }
}

/**
 * @brief 设置循迹速度
 */
void LineFollow_SetSpeed(uint8_t speed)
{
    if (speed > 99) speed = 99;
    line_speed = speed;
}

/**
 * @brief 获取循迹速度
 */
uint8_t LineFollow_GetSpeed(void)
{
    return line_speed;
}