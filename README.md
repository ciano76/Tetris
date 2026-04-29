<div style="display: flex; align-items: center; gap: 20px;">
  <img src="./documentation/MCC_logo_left_color_rgb.png" alt="Monroe Community College Logo" width="150">
  <div style="font-size: 2em; font-weight: bold;">
    Tetris Final Project - CSC-202 - MCC
  </div>
</div>
<hr style="border: none; border-bottom: 4px solid #FFF; width: 100%; margin-top: 0;">


# Project Overview
We propose to build a fully embedded implementation of Tetris on the MSPM0G3507 LaunchPad. 
Unlike software only versions, this project focuses on hardware driven gameplay, realtime control using interrupts, ADCbased joystick input, PWMcontrolled LCD brightness, and multiple external components.
The system uses an **ILI9341 SPI LCD** to render the game grid and falling pieces.

The project demonsrtates embedde systems engineering skills by integrating multiple peripherals sensors, and actuators, and by implementating a multitasking, 
interrupt driven archietecture that goes significantly beyond any lab assignment.

This is not a port of an existing Tetris program -- all logic, rendering, input handling, and hardware control wil lbe written from scratch following CSC202 coding standards.

# User Story
A player interacts with the system using a 4way joystick and three push buttons. The
ILI9341 LCD displays the Tetris grid and falling pieces. The joystick moves pieces left
and right, holding down accelerates the drop, and holding up pauses the game. Two
rotation buttons rotate the piece left or right. A dedicated Start/Pause button controls the
game state.
A brightness wheel (potentiometer) allows the player to adjust the LCD backlight in real
time. A buzzer provides audio feedback for line clears or gameover.
The system responds instantly to user input through hardware interrupts and
timerdriven updates.

# Hardware Components (10+ Required)

## External Components
1. ILI9341 LCD (SPI)
2. 4way joystick (ADC)
3. Rotate Left button
4. Rotate Right button
5. Start/Pause button
6. Potentiometer (LCD brightness)
7. Buzzer (audio feedback)
8. Bargraph LED (stack height indicator)
9. Servo motor (physical “piece drop” indicator)
10. DC motor (simulated conveyor motion)
11. Ultrasonic sensor (detects “top out” or proximity events)
    
## Internal MSPM0G3507 Peripherals
- SPI
- ADC
- GPIO
- Interrupts
- SysTick
- Timers
- PWM
- UART (optional debugging)

# System Architecture
## Display Subsystem
- SPI driven ILI9341 LCD
- Renders grid, pieces, score, and game state
## Input Subsystem
- Joystick (ADC) for movement and drop control
- Buttons (GPIO interrupts) for rotation and start/pause
## Feedback Subsystem
- Buzzer for events

## Control Logic
- Timer ISR for gravity tick
- Button ISRs for rotation and pause
- ADC sampling for joystick and brightness wheel
- State machine for RUNNING, PAUSED, GAME_OVER, FAST_DROP

# Software Design Requirements
## EventDriven I/O
-  Buttons use GPIO interrupts
- Timer ISR drives piece descent
- ADC used for joystick and brightness control
## Multitasking
- Nonblocking main loop
- Flags set by ISRs
- State machine controlling gameplay
## BitMasking
- Grid stored as bitfields
- Collision detection uses masks
## Error Handling
- Invalid moves prevented
- Sensor faults trigger safe states
## Modular Code
- lcd.c, input.c, game_logic.c, hardware.c, main.c

## Originality & Compliance
This project:
- Is not a lab repeat
- Is not a softwareonly game
- Uses multiple hardware components
- Uses multiple MSPM0 peripherals
- Uses interrupts, timers, ADC, PWM, SPI, GPIO, bitmasking
- Is fully original code, written from scratch
- Demonstrates real embeddedsystems engineering
