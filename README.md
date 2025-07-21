# 🏎️ KOU Racing - VCU Project

## Project Overview

This repository contains the **Vehicle Control Unit (VCU)** software developed for the **KR-02** electric vehicle, prepared for the FS Alpe Adria competition.

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

## Project Goals (Current Sprint)

- [x] Finalize APPS mapping and PWM control logic
- [x] Refactor and modularize common utility code
- [ ] Implement gyro sensor data mapping and filtering
- [ ] Develop Nextion display communication layer
- [ ] Parse and use battery telemetry from Makcon (LIBAT)
- [ ] Display sensor data on Nextion screen
- [ ] Implement brake light logic based on BPPS
- [ ] Integrate and finalize R2D signal management
- [ ] Final testing with full sensor + motor integration

## Tools & Platforms

- **Microcontroller:** STM32F407VGT7 (STM32F4 series)
- **Software:** STM32CubeIDE, VSCode, KiCad, MakconX
- **Protocols:** CANopen, UART, SPI, GPIO, ADC

## Future Work & Planned Features

The following items are planned as part of the long-term development roadmap for OpenVCU:

- **LTE-based Telemetry Integration**  
  Real-time telemetry system for Android devices using LTE modules for wireless data transmission.

- **Advanced Data Logging System**  
  Structured and timestamped logging with session management and improved SD card handling.

- **Migration to Rust Language**  
  Transition of critical modules to the **Rust programming language** for enhanced memory safety and concurrency.

- **Regenerative (Electronic) Braking System**  
  Dynamic motor braking based on vehicle state and pedal input.

- **Modular Firmware Framework**  
  Reusable, hardware-agnostic modules for easier testing and support for other STM32 platforms.

- **Bootloader & OTA Update System**  
  CAN-based bootloader with support for **over-the-air (OTA)** or SD card firmware updates.

- **Hardware-in-the-Loop (HIL) Simulation**  
  Simulated environment to validate VCU behavior using virtual sensor/ECU input.

- **Dashboard UI for Engineering Debug**  
  A cross-platform debug interface (e.g., Qt or web-based) for visualizing real-time sensor and system data.
