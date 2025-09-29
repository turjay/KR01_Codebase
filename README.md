# 🏎️ KR-01 Codebase

## Project Overview

This repository contains the **Vehicle Control Unit (VCU)** software developed for the **KR-01** electric vehicle, prepared for the FS Romania competition.

## System Architecture

The system is built around a custom-designed Vehicle Control Unit (VCU) based on the **STM32F407VGT7** microcontroller. Sensor data—including throttle, brake, speed, and gyroscope readings—is acquired through a combination of **analog-to-digital converters (ADC)**, **digital GPIOs**, and **CANopen communication**. The microcontroller processes these signals in real time, applies application-specific logic, and logs selected data to an onboard **microSD card** using a **FATFS-based SPI interface**. In parallel, relevant information is transmitted via **UART** to a **Nextion HMI display**, providing the driver with a live data interface. Additionally, the VCU handles control of safety-critical and state-based systems such as the **Ready-to-Drive (R2D)** signal via dedicated output lines and internal state machines.

## Sensor Modules

| Module     | Description                             | Interface         |
|------------|-----------------------------------------|-------------------|
| APPS       | Accelerator Pedal Position Sensor       | Analog (0–5V)     |
| BPPS       | Brake Pedal Position Sensor             | Analog (0–5V)     |
| GYRO       | Analog X & Y Gyroscope                  | Analog (0–10V)    |
| Speed      | Hall-effect-based Speed Sensor          | Digital GPIO      |
| Battery    | Libat Battery Telemetry Module          | CANopen           |
| Display    | Nextion HMI Display                     | UART              |
