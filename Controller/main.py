# -*- coding: utf-8 -*-
"""
主程序入口 - E 唯杯小车上位机
"""

import sys
import time
from PySide6.QtWidgets import QApplication
from PySide6.QtCore import QTimer

from ui_main_window import UiMainWindow
from gamepad_reader import GamepadReader
from serial_comm import SerialComm
import protocol


class SmartCarController(UiMainWindow):
    """智能车控制器主类"""

    def __init__(self):
        super().__init__()

        # 初始化模块
        self.gamepad = GamepadReader()
        self.serial = SerialComm()

        # 当前状态
        self.current_gear = 2  # 中速
        self.turn_direction = 0  # 0=左，1=右
        self.last_gear_time = 0
        self.last_turn_time = 0

        # 连接信号槽
        self.connect_signals()

        # 初始化手柄
        if self.gamepad.connect():
            self.label_gamepad.setText("手柄已连接")
        else:
            self.label_gamepad.setText("未检测到手柄")

        # 刷新串口列表
        self.refresh_ports()

        # 定时器：读取手柄
        self.gamepad_timer = QTimer()
        self.gamepad_timer.timeout.connect(self.update_gamepad)
        self.gamepad_timer.start(50)  # 50ms = 20Hz

        # 定时器：更新 UI
        self.ui_timer = QTimer()
        self.ui_timer.timeout.connect(self.update_ui_display)
        self.ui_timer.start(100)  # 100ms = 10Hz

    def connect_signals(self):
        """连接信号槽"""
        self.btn_open.clicked.connect(self.on_open_serial)
        self.btn_close.clicked.connect(self.on_close_serial)
        self.combo_port.currentTextChanged.connect(self.on_port_changed)

    def refresh_ports(self):
        """刷新串口列表"""
        self.combo_port.clear()
        ports = self.serial.get_available_ports()
        self.combo_port.addItems(ports)

    def on_open_serial(self):
        """打开串口按钮"""
        port = self.combo_port.currentText()
        baud = int(self.combo_baud.currentText())

        if self.serial.open(port, baud):
            self.label_status.setText("● Connected")
            self.btn_open.setEnabled(False)
            self.btn_close.setEnabled(True)
            self.combo_port.setEnabled(False)
            self.combo_baud.setEnabled(False)

    def on_close_serial(self):
        """关闭串口按钮"""
        self.serial.close()
        self.label_status.setText("● Disconnected")
        self.btn_open.setEnabled(True)
        self.btn_close.setEnabled(False)
        self.combo_port.setEnabled(True)
        self.combo_baud.setEnabled(True)

    def on_port_changed(self):
        """串口改变"""
        pass

    def update_gamepad(self):
        """更新手柄状态并发送控制命令"""
        if not self.gamepad.connected:
            return

        self.gamepad.update()

        # 打印调试信息
        print(f"摇杆: L({self.gamepad.left_stick_x}, {self.gamepad.left_stick_y}) R({self.gamepad.right_stick_x}, {self.gamepad.right_stick_y})")

        # 检测按钮按下（只响应瞬间）
        if self.gamepad.is_btn_a_pressed():
            self.toggle_mode()
        if self.gamepad.is_btn_b_pressed():
            self.send_stop()
        if self.gamepad.is_btn_x_pressed():
            self.change_gear(-1)
        if self.gamepad.is_btn_y_pressed():
            self.change_gear(1)
        if self.gamepad.is_dpad_left_pressed():
            self.set_turn_direction(0)
        if self.gamepad.is_dpad_right_pressed():
            self.set_turn_direction(1)

        # 发送摇杆数据
        if self.serial.is_open:
            forward = self.gamepad.left_stick_y
            side = self.gamepad.left_stick_x

            # 发送协议帧
            frame = protocol.pack_joystick(forward, side)
            self.serial.send_frame(frame)

    def update_ui_display(self):
        """更新界面显示"""
        # 更新手柄状态显示
        status_str = self.gamepad.get_status_string()
        self.label_gamepad.setText(status_str)

        # 更新滑杆显示（手柄输入）
        throttle_val = -self.gamepad.left_stick_y  # 反向
        direction_val = self.gamepad.left_stick_x
        rotate_val = self.gamepad.right_stick_x

        self.slider_throttle.setValue(throttle_val)
        self.slider_direction.setValue(direction_val)
        self.slider_rotate.setValue(rotate_val)

        # 更新数值标签
        self.label_throttle_val.setText(str(throttle_val))
        self.label_direction_val.setText(str(direction_val))
        self.label_rotate_val.setText(str(rotate_val))

        # 调试打印
        print(f"UI更新: Throttle={throttle_val}, Direction={direction_val}, Rotate={rotate_val}")

    def toggle_mode(self):
        """切换模式"""
        # 切换模式逻辑（可选：显示在界面上）
        current_mode = "Remote" if self.label_mode.text() == "Remote" else "Line Follow"
        new_mode = "Line Follow" if current_mode == "Remote" else "Remote"
        self.label_mode.setText(new_mode)

        # 发送模式切换命令
        if self.serial.is_open:
            mode_val = protocol.MODE_REMOTE if new_mode == "Remote" else protocol.MODE_LINE_FOLLOW
            frame = protocol.pack_mode(mode_val)
            self.serial.send_frame(frame)

        print(f"模式切换：{new_mode}")

    def change_gear(self, delta):
        """改变档位"""
        # 防止频繁切换
        current_time = time.time()
        if current_time - self.last_gear_time < 0.3:
            return

        new_gear = self.current_gear + delta
        if new_gear < 0:
            new_gear = 0
        elif new_gear > 3:
            new_gear = 3

        self.current_gear = new_gear
        self.label_gear.setText(str(new_gear))
        self.last_gear_time = current_time

        # 发送档位命令
        if self.serial.is_open:
            frame = protocol.pack_gear(new_gear)
            self.serial.send_frame(frame)

        print(f"档位：{new_gear}")

    def set_turn_direction(self, direction):
        """设置转向方向"""
        current_time = time.time()
        if current_time - self.last_turn_time < 0.3:
            return

        self.turn_direction = direction
        self.label_turn.setText("Left" if direction == 0 else "Right")
        self.last_turn_time = current_time

        # 发送转向命令
        if self.serial.is_open:
            frame = protocol.pack_turn_dir(direction)
            self.serial.send_frame(frame)

        print(f"转向方向：{'Left' if direction == 0 else 'Right'}")

    def send_stop(self):
        """发送急停命令"""
        if self.serial.is_open:
            frame = protocol.pack_stop()
            self.serial.send_frame(frame)
            print("急停!")

    def closeEvent(self, event):
        """关闭窗口时清理"""
        self.gamepad_timer.stop()
        self.ui_timer.stop()
        self.serial.close()
        self.gamepad.close()
        event.accept()


def main():
    """主函数"""
    app = QApplication(sys.argv)

    # 创建控制器
    controller = SmartCarController()
    controller.show()

    sys.exit(app.exec())


if __name__ == "__main__":
    main()
