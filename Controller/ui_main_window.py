# -*- coding: utf-8 -*-
"""
主窗口 UI 定义 - 使用代码创建界面
"""

from PySide6.QtWidgets import (QMainWindow, QWidget, QVBoxLayout, QHBoxLayout,
                               QGroupBox, QLabel, QSlider, QComboBox, QPushButton)
from PySide6.QtCore import Qt


class UiMainWindow(QMainWindow):
    """主窗口 UI 类"""

    def __init__(self):
        super().__init__()
        self.setup_ui()

    def setup_ui(self):
        """设置 UI"""
        self.setWindowTitle("E-Wei Cup Smart Car Controller")
        self.setMinimumSize(900, 700)

        # 中央控件
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        main_layout = QVBoxLayout(central_widget)

        # ===== 1. 串口设置区 =====
        self.group_serial = QGroupBox("Serial Port Settings")
        main_layout.addWidget(self.group_serial)

        layout_serial = QHBoxLayout(self.group_serial)

        # Port
        layout_serial.addWidget(QLabel("Port:"))
        self.combo_port = QComboBox()
        layout_serial.addWidget(self.combo_port)

        # Baud Rate
        layout_serial.addWidget(QLabel("Baud Rate:"))
        self.combo_baud = QComboBox()
        self.combo_baud.addItems(["9600", "19200", "57600", "115200"])
        self.combo_baud.setCurrentText("115200")
        layout_serial.addWidget(self.combo_baud)

        # Buttons
        self.btn_open = QPushButton("Open Serial")
        layout_serial.addWidget(self.btn_open)

        self.btn_close = QPushButton("Close Serial")
        self.btn_close.setEnabled(False)
        layout_serial.addWidget(self.btn_close)

        layout_serial.addStretch()

        # ===== 2. 车辆状态区 =====
        self.group_status = QGroupBox("Vehicle Status")
        main_layout.addWidget(self.group_status)

        layout_status = QVBoxLayout(self.group_status)

        # Throttle
        layout_status.addWidget(QLabel("Throttle:"))
        self.slider_throttle = QSlider(Qt.Horizontal)
        self.slider_throttle.setMinimum(-100)
        self.slider_throttle.setMaximum(100)
        self.slider_throttle.setEnabled(False)  # 只读显示
        layout_status.addWidget(self.slider_throttle)

        self.label_throttle_val = QLabel("0")
        self.label_throttle_val.setAlignment(Qt.AlignCenter)
        layout_status.addWidget(self.label_throttle_val)

        # Direction
        layout_status.addWidget(QLabel("Direction (Left/Right):"))
        self.slider_direction = QSlider(Qt.Horizontal)
        self.slider_direction.setMinimum(-100)
        self.slider_direction.setMaximum(100)
        self.slider_direction.setEnabled(False)
        layout_status.addWidget(self.slider_direction)

        self.label_direction_val = QLabel("0")
        self.label_direction_val.setAlignment(Qt.AlignCenter)
        layout_status.addWidget(self.label_direction_val)

        # Rotation
        layout_status.addWidget(QLabel("Rotation (Spin):"))
        self.slider_rotate = QSlider(Qt.Horizontal)
        self.slider_rotate.setMinimum(-100)
        self.slider_rotate.setMaximum(100)
        self.slider_rotate.setEnabled(False)
        layout_status.addWidget(self.slider_rotate)

        self.label_rotate_val = QLabel("0")
        self.label_rotate_val.setAlignment(Qt.AlignCenter)
        layout_status.addWidget(self.label_rotate_val)

        # Mode / Gear / Turn / Status
        layout_info = QHBoxLayout()

        self.group_mode = QGroupBox("Mode")
        layout_mode = QHBoxLayout(self.group_mode)
        self.label_mode = QLabel("Remote")
        self.label_mode.setAlignment(Qt.AlignCenter)
        layout_mode.addWidget(self.label_mode)
        layout_info.addWidget(self.group_mode)

        self.group_gear = QGroupBox("Gear")
        layout_gear = QHBoxLayout(self.group_gear)
        self.label_gear = QLabel("2")
        self.label_gear.setAlignment(Qt.AlignCenter)
        layout_gear.addWidget(self.label_gear)
        layout_info.addWidget(self.group_gear)

        self.group_turn = QGroupBox("Turn Dir")
        layout_turn = QHBoxLayout(self.group_turn)
        self.label_turn = QLabel("Left")
        self.label_turn.setAlignment(Qt.AlignCenter)
        layout_turn.addWidget(self.label_turn)
        layout_info.addWidget(self.group_turn)

        self.group_connection = QGroupBox("Status")
        layout_conn = QHBoxLayout(self.group_connection)
        self.label_status = QLabel("● Disconnected")
        self.label_status.setAlignment(Qt.AlignCenter)
        layout_conn.addWidget(self.label_status)
        layout_info.addWidget(self.group_connection)

        layout_status.addLayout(layout_info)

        # ===== 3. 机械臂控制区（预留） =====
        self.group_arm = QGroupBox("Arm Control (Reserved)")
        main_layout.addWidget(self.group_arm)

        layout_arm = QVBoxLayout(self.group_arm)
        self.label_arm_placeholder = QLabel("* Servo control will be implemented later *")
        self.label_arm_placeholder.setAlignment(Qt.AlignCenter)
        layout_arm.addWidget(self.label_arm_placeholder)

        # ===== 4. 手柄调试区 =====
        self.group_gamepad = QGroupBox("Gamepad Debug")
        main_layout.addWidget(self.group_gamepad)

        layout_gamepad = QVBoxLayout(self.group_gamepad)
        self.label_gamepad = QLabel("Left Stick: (  0,   0)  Right Stick: (  0,   0)  Buttons: ---")
        layout_gamepad.addWidget(self.label_gamepad)

        # 样式表（深色主题）
        self.apply_dark_style()

    def apply_dark_style(self):
        """应用深色主题"""
        self.setStyleSheet("""
            QMainWindow {
                background-color: #2b2b2b;
                color: #ffffff;
            }
            QGroupBox {
                border: 2px solid #555555;
                border-radius: 5px;
                margin-top: 10px;
                font-weight: bold;
                color: #ffffff;
            }
            QGroupBox::title {
                subcontrol-origin: margin;
                left: 10px;
                padding: 0 5px;
            }
            QLabel {
                color: #ffffff;
            }
            QSlider::groove:horizontal {
                border: 1px solid #555555;
                height: 8px;
                background: #444444;
                border-radius: 4px;
            }
            QSlider::handle:horizontal {
                background: #3498db;
                width: 18px;
                margin: -5px 0;
                border-radius: 9px;
            }
            QSlider::handle:horizontal:disabled {
                background: #3498db;
            }
            QComboBox {
                background-color: #444444;
                border: 1px solid #555555;
                padding: 5px;
                color: #ffffff;
            }
            QComboBox::drop-down {
                border: none;
                width: 20px;
            }
            QPushButton {
                background-color: #3498db;
                border: none;
                padding: 8px 15px;
                color: #ffffff;
                border-radius: 4px;
            }
            QPushButton:hover {
                background-color: #2980b9;
            }
            QPushButton:disabled {
                background-color: #555555;
            }
        """)
