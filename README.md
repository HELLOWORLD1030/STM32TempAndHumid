# STM32测量环境温湿度

## 使用

下载本例程，使用MDK或是任何你熟悉的IDE打开，使用ST-Link，JTag，串口等方式，下载编译好的程序到stm32开发板中

## 芯片

主控：STM32F103C8T6

温度传感器：DS18B20

湿度传感器：DHT11

显示屏：LCD1602

## 文件结构

├───CORE   #stm32的启动代码
├───FWLIB
│   └───STM32F10x_FWLib #STM32库函数
│       ├───inc
│       └───src
├───HARDWARE # 外设及外部中断
│   ├───ADC
│   ├───DHT11
│   ├───DS18B20
│   ├───EXTI
│   ├───ITEMP
│   ├───KEY
│   ├───LCD
│   ├───LCD1602
│   └───LED
├───MALLOC 
├───OBJ #编译后的文件
├───SYSTEM #系统函数，延时，串口等
│   ├───delay
│   ├───sys
│   └───usart
├───UCOSII #UCOSii库函数
│   ├───CONFIG
│   ├───CORE
│   └───PORT
├───USER #工程文件所在
│   └───DebugConfig
└───USMART