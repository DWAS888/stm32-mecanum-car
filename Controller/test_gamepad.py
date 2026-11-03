# -*- coding: utf-8 -*-
"""
手柄测试脚本 - 检测轴索引和按钮编号
运行后推动摇杆、按按钮，观察输出
"""

import pygame

pygame.init()
pygame.joystick.init()

if pygame.joystick.get_count() == 0:
    print("未检测到手柄！")
    exit()

joystick = pygame.joystick.Joystick(0)
joystick.init()
print(f"手柄名称: {joystick.get_name()}")
print(f"轴数量: {joystick.get_numaxes()}")
print(f"按钮数量: {joystick.get_numbuttons()}")
print(f"十字键数量: {joystick.get_numhats()}")
print("-" * 50)
print("请推动摇杆、按按钮，观察输出...")
print("-" * 50)

try:
    while True:
        pygame.event.pump()

        # 读取所有轴
        axes = []
        for i in range(joystick.get_numaxes()):
            val = joystick.get_axis(i)
            if abs(val) > 0.1:  # 只显示有变化的
                axes.append(f"轴{i}={val:.2f}")

        # 读取所有按钮
        buttons = []
        for i in range(joystick.get_numbuttons()):
            if joystick.get_button(i):
                buttons.append(f"Btn{i}")

        # 读取十字键
        hats = []
        for i in range(joystick.get_numhats()):
            hat = joystick.get_hat(i)
            if hat != (0, 0):
                hats.append(f"Hat{i}={hat}")

        # 输出
        if axes or buttons or hats:
            output = " | ".join(axes + buttons + hats)
            print(output)

        pygame.time.delay(50)

except KeyboardInterrupt:
    print("\n测试结束")
    pygame.quit()