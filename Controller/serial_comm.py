# -*- coding: utf-8 -*-
"""
串口通信模块
"""

import serial
import serial.tools.list_ports


class SerialComm:
    """串口通信类"""

    def __init__(self):
        self.serial = None
        self.is_open = False

    def get_available_ports(self):
        """获取可用串口列表"""
        ports = serial.tools.list_ports.comports()
        return [port.device for port in ports]

    def open(self, port, baudrate=115200):
        """打开串口"""
        try:
            self.serial = serial.Serial(port, baudrate, timeout=0.1)
            self.is_open = True
            print(f"串口 {port} 已打开，波特率 {baudrate}")
            return True
        except Exception as e:
            print(f"打开串口失败：{e}")
            return False

    def close(self):
        """关闭串口"""
        if self.serial and self.is_open:
            self.serial.close()
            self.is_open = False
            print("串口已关闭")

    def send(self, data):
        """发送数据"""
        if self.serial and self.is_open:
            self.serial.write(data)

    def receive(self):
        """接收数据"""
        if self.serial and self.is_open:
            return self.serial.read_all()
        return b""

    def send_frame(self, frame):
        """发送一帧数据"""
        self.send(frame)
