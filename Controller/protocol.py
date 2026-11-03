# -*- coding: utf-8 -*-
"""
通信协议定义
帧格式：[帧头 0xAA][命令字][数据 1][数据 2][帧尾 0x55]
"""

# 帧头帧尾
FRAME_HEADER = 0xAA
FRAME_TAIL = 0x55

# 命令字定义
CMD_JOYSTICK = 0x01    # 摇杆数据
CMD_MODE = 0x02        # 模式切换
CMD_GEAR = 0x03        # 档位切换
CMD_STOP = 0x04        # 紧急停止
CMD_TURN_DIR = 0x05    # 转向方向 (0=左，1=右)
CMD_ARM_SERVO1 = 0x06  # 舵机 1 角度 (0-180) - 预留
CMD_ARM_SERVO2 = 0x07  # 舵机 2 角度 (0-180) - 预留
CMD_ARM_GRIPPER = 0x08 # 抓夹 (0=开，1=合) - 预留

# 工作模式
MODE_REMOTE = 0        # 遥控模式
MODE_LINE_FOLLOW = 1   # 循迹模式

# 档位
GEAR_STOP = 0
GEAR_LOW = 1
GEAR_MEDIUM = 2
GEAR_HIGH = 3


def pack_frame(cmd, d1, d2):
    """打包一帧数据"""
    return bytes([FRAME_HEADER, cmd, d1 & 0xFF, d2 & 0xFF, FRAME_TAIL])


def pack_joystick(forward, side):
    """打包摇杆数据 (forward/side: -100~+100)"""
    # 负数转补码
    d1 = forward & 0xFF
    d2 = side & 0xFF
    return pack_frame(CMD_JOYSTICK, d1, d2)


def pack_mode(mode):
    """打包模式切换命令"""
    return pack_frame(CMD_MODE, mode, 0)


def pack_gear(gear):
    """打包档位命令 (0-3)"""
    return pack_frame(CMD_GEAR, gear, 0)


def pack_stop():
    """打包急停命令"""
    return pack_frame(CMD_STOP, 0, 0)


def pack_turn_dir(direction):
    """打包转向命令 (0=左，1=右)"""
    return pack_frame(CMD_TURN_DIR, direction, 0)


def pack_servo1(angle):
    """打包舵机 1 角度 (0-180) - 预留"""
    return pack_frame(CMD_ARM_SERVO1, angle, 0)


def pack_servo2(angle):
    """打包舵机 2 角度 (0-180) - 预留"""
    return pack_frame(CMD_ARM_SERVO2, angle, 0)


def pack_gripper(state):
    """打包抓夹命令 (0=开，1=合) - 预留"""
    return pack_frame(CMD_ARM_GRIPPER, state, 0)
