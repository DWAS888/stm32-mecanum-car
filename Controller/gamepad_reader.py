# -*- coding: utf-8 -*-
"""
手柄读取模块 - 使用 pygame 库
"""

import pygame


class GamepadReader:
    """手柄读取器"""

    def __init__(self):
        self.left_stick_x = 0      # 左摇杆 X (-100~+100)
        self.left_stick_y = 0      # 左摇杆 Y (-100~+100)
        self.right_stick_x = 0     # 右摇杆 X (-100~+100)
        self.right_stick_y = 0     # 右摇杆 Y (-100~+100)

        # 按钮状态
        self.btn_a = False
        self.btn_b = False
        self.btn_x = False
        self.btn_y = False
        self.btn_lb = False
        self.btn_rb = False
        self.btn_start = False
        self.btn_back = False
        self.dpad_up = False
        self.dpad_down = False
        self.dpad_left = False
        self.dpad_right = False

        # 按钮上一帧状态（用于检测按下瞬间）
        self.prev_btn_a = False
        self.prev_btn_b = False
        self.prev_btn_x = False
        self.prev_btn_y = False
        self.prev_dpad_left = False
        self.prev_dpad_right = False

        # 死区阈值
        self.deadzone = 0.15

        pygame.init()
        pygame.joystick.init()

        self.joystick = None
        self.connected = False

    def connect(self):
        """连接手柄"""
        if pygame.joystick.get_count() > 0:
            self.joystick = pygame.joystick.Joystick(0)
            self.joystick.init()
            self.connected = True
            print(f"手柄已连接：{self.joystick.get_name()}")
            return True
        return False

    def update(self):
        """更新手柄状态"""
        if not self.connected:
            return

        pygame.event.pump()

        # 保存上一帧状态
        self.prev_btn_a = self.btn_a
        self.prev_btn_b = self.btn_b
        self.prev_btn_x = self.btn_x
        self.prev_btn_y = self.btn_y
        self.prev_dpad_left = self.dpad_left
        self.prev_dpad_right = self.dpad_right

        # 读取摇杆 (pygame 返回 -1.0~+1.0)
        if self.joystick.get_numaxes() >= 4:
            lx = self.joystick.get_axis(0)
            ly = self.joystick.get_axis(1)
            rx = self.joystick.get_axis(2)
            ry = self.joystick.get_axis(3) if self.joystick.get_numaxes() > 3 else 0

            # 应用死区
            lx = self._apply_deadzone(lx)
            ly = self._apply_deadzone(ly)
            rx = self._apply_deadzone(rx)
            ry = self._apply_deadzone(ry)

            # 转换到 -100~+100
            self.left_stick_x = int(lx * 100)
            self.left_stick_y = int(ly * 100)
            self.right_stick_x = int(rx * 100)
            self.right_stick_y = int(ry * 100)

        # 读取按钮
        if self.joystick.get_numbuttons() > 0:
            self.btn_a = self.joystick.get_button(0)
        if self.joystick.get_numbuttons() > 1:
            self.btn_b = self.joystick.get_button(1)
        if self.joystick.get_numbuttons() > 2:
            self.btn_x = self.joystick.get_button(2)
        if self.joystick.get_numbuttons() > 3:
            self.btn_y = self.joystick.get_button(3)
        if self.joystick.get_numbuttons() > 4:
            self.btn_lb = self.joystick.get_button(4)
        if self.joystick.get_numbuttons() > 5:
            self.btn_rb = self.joystick.get_button(5)
        if self.joystick.get_numbuttons() > 7:
            self.btn_back = self.joystick.get_button(7)
        if self.joystick.get_numbuttons() > 6:
            self.btn_start = self.joystick.get_button(6)

        # 读取十字键 (不同手柄可能不同)
        if self.joystick.get_numhats() > 0:
            hat = self.joystick.get_hat(0)
            self.dpad_up = hat[1] > 0
            self.dpad_down = hat[1] < 0
            self.dpad_left = hat[0] < 0
            self.dpad_right = hat[0] > 0

    def _apply_deadzone(self, value):
        """应用死区"""
        if abs(value) < self.deadzone:
            return 0
        # 重新映射到 -1~+1
        if value > 0:
            return (value - self.deadzone) / (1 - self.deadzone)
        else:
            return (value + self.deadzone) / (1 - self.deadzone)

    def is_btn_a_pressed(self):
        """检测 A 按钮是否刚按下"""
        return self.btn_a and not self.prev_btn_a

    def is_btn_b_pressed(self):
        """检测 B 按钮是否刚按下"""
        return self.btn_b and not self.prev_btn_b

    def is_btn_x_pressed(self):
        """检测 X 按钮是否刚按下"""
        return self.btn_x and not self.prev_btn_x

    def is_btn_y_pressed(self):
        """检测 Y 按钮是否刚按下"""
        return self.btn_y and not self.prev_btn_y

    def is_dpad_left_pressed(self):
        """检测十字键左是否刚按下"""
        return self.dpad_left and not self.prev_dpad_left

    def is_dpad_right_pressed(self):
        """检测十字键右是否刚按下"""
        return self.dpad_right and not self.prev_dpad_right

    def get_status_string(self):
        """获取状态字符串"""
        if not self.connected:
            return "未连接"

        btns = []
        if self.btn_a: btns.append("A")
        if self.btn_b: btns.append("B")
        if self.btn_x: btns.append("X")
        if self.btn_y: btns.append("Y")
        if self.btn_lb: btns.append("LB")
        if self.btn_rb: btns.append("RB")
        if self.dpad_up: btns.append("↑")
        if self.dpad_down: btns.append("↓")
        if self.dpad_left: btns.append("←")
        if self.dpad_right: btns.append("→")

        btn_str = " ".join(btns) if btns else "---"

        return f"左摇杆：({self.left_stick_x:4}, {self.left_stick_y:4})  " \
               f"右摇杆：({self.right_stick_x:4}, {self.right_stick_y:4})  " \
               f"按键：{btn_str}"

    def close(self):
        """关闭"""
        pygame.quit()
