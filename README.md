# CAR_SECOND — STM32 麦轮小车

基于 STM32F407VGT6 的麦克纳姆轮小车工程（CubeMX + Keil MDK），支持 Python 手柄上位机遥控。

## 硬件

| 项 | 说明 |
|---|---|
| MCU | STM32F407VGT6 (Cortex-M4, 168MHz) |
| 底盘 | 4 × 麦轮 + 直流减速电机（TIM PWM 驱动） |
| 传感器 | 循迹模块（`line_follow.c`） |
| 通信 | UART（与上位机通信，协议见 `Controller/protocol.py`） |

## 目录结构

```
CAR_SECOND/
├── CAR_SECOND.ioc        # CubeMX 配置（引脚/时钟/外设）
├── Core/                 # 应用代码
│   ├── motor.c           # 电机 PWM 控制
│   ├── mecanum.c         # 麦轮运动学（vx/vy/vw → 4 轮转速）
│   ├── line_follow.c     # 循迹逻辑
│   └── ...
├── Drivers/              # ST HAL 库 + CMSIS
├── MDK-ARM/              # Keil 工程 (CAR_SECOND.uvprojx)
└── Controller/           # Python 上位机
    ├── gamepad_reader.py # 游戏手柄输入读取
    └── protocol.py       # 通信协议定义
```

## 项目简介

本项目用于大一嵌入式学习和循迹抓取小车制作和比赛练习。

设想采用蓝牙连接、串口通信、手柄遥控车辆行进以及机械臂转动抓取；预期可实现基础循迹、断点停止、手柄遥控。

实际调试结果：手柄控制未完成，且有待优化实现闭环控制。

## 如何编译

1. 用 STM32CubeMX 打开 `CAR_SECOND.ioc`（如需重新生成代码）
2. 用 Keil MDK 打开 `MDK-ARM/CAR_SECOND.uvprojx`
3. F7 编译，下载烧录

## License

MIT
