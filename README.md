Project Overview
We propos eto build a fully embedded implementation of Tetris on the MSPM0G3507 LaunchPad. 
Unlike software only versions, this project focuses on hardware driven gameplay, realtime control using interrupts, ADCbased joystick input, PWMcontrolled LCD brightness, and multiple external components.
The system uses an **ILI9341 SPI LCD** to render the game grid and falling pieces.

The project demonsrtates embedde systems engineering skills by integrating multiple peripherals sensors, and actuators, and by implementating a multitasking, 
interrupt driven archietecture that goes significantly beyond any lab assignment.

This is not a port of an existing Tetris program -- all logic, rendering, input handling, and hardware control wil lbe written from scratch following CSC202 coding standards.
