# Frequency Sweeping and Detection with ESP32

This repository contains code for frequency sweeping and detection implemented on an ESP32 microcontroller. The code utilizes ultrasonic sensors for frequency measurement and infrared sensors for inter-robot communication.

## Overview

The main objective of this project is to perform frequency sweeping and detection using an ESP32 microcontroller. The code allows the microcontroller to sweep across a range of frequencies, detect the frequency of received signals, and communicate with other robots using infrared sensors.

## Features
* Frequency sweeping across a specified range
* Frequency detection using ultrasonic sensors
* Inter-robot communication using infrared sensors
* Coarse and fine frequency sweeping modes
* Constant frequency mode
* Automatic adjustment of sweeping parameters based on detected frequencies

## Hardware Requirements
* ESP32 microcontroller
* Ultrasonic sensor for frequency measurement
* Infrared sensor for inter-robot communication
* LED for visual indication (optional)

## Usage
1. Connect the ultrasonic and infrared sensors to the ESP32 microcontroller as per the hardware setup.
2. Upload the provided code (sweep_detection.ino) to the ESP32 microcontroller using the Arduino IDE or any compatible development environment.
3. Power on the microcontroller and observe the frequency sweeping and detection behavior.
4. Monitor the serial output for frequency measurements and communication status.

## Code Structure
* sweep_detection.ino: Main Arduino sketch file containing the setup and loop functions.
* swarm_robot.h: Header file defining the SwarmRobot class for frequency sweeping and detection.
* swarm_robot.cpp: Implementation file for the SwarmRobot class methods.

## Contributing

Contributions to this project are welcome! If you find any bugs or have suggestions for improvements, please feel free to open an issue or submit a pull request.
