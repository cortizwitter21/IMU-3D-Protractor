# IMU-3D-Protractor
This repository contains the code used for the 3D Protractor prototype intended for use a Shriner's Hospital for Children. The protractor would contain two housing structures, one for each IMU, which would be placed on each limb via offshoots on the limbs. The device would then calculate the angles the Euler angles between the two limbs, effectively finding the rotation of the joints in the sagittal, frontal, and transverse planes. These values would be displayed on an SSD1306 I2C OLED display.

Before turning on, the device should be sit on a "calibration rail" that forces the protractor into the known 0 position. The device woud then turn on and calibrate its current angle measurements to be 0 degrees rotation in all planes. From there, the device could be used as normal. To recalibrate, one sould simply turn off the device, put it on the calibration rail, and turn it on again.

## Necessary Hardware

- 2 Adafruit 9-DOF Orientation IMU Fusion Breakout - BNO085

- 1 SSD 1306 I2C OLED Display

- 1 Adafruit ItsyBitsy M4

- Wires

- 2 Protoboards (one for each IMU to allow free motion between them)

- 1 Battery Pack


## Folders

**main**: contains all code necessary for the 3D Protractor. Files include the following:

- main.ino: main program for functioning 3D protractor

- calibration.h/calibration.cpp: contains functions relating to calibration, including calibration of each sensor and calibration of the relative angle.

- display.h/display.cpp: contains functions relating to displaying information on the SSD1306 OLED display.

**Wiring and Assembly**: contains wiring diagram as well as pictures of preliminary CAD designs for housing

**CircuitPython Attempt**: contains code written when initially trying to use a Raspberry Pi Pico W. This route was abandoned since we could not find a library that addressed our needs (see Key Findings During Experimentation), but for the sake of reproducibility and documentation, the code is still included. However, the majority of it is not functional


## Key Findings During Experimentation:
1. For Arduino, the Adafruit_BNO08x library (the official Adafruit library) does **not** support 2 BNO08x IMUs simultaneously, therefore should not be used for this project.
2. The bno08x library from the most recent adafruit-circuitpython-bundle does not support the change of address, meaning our resources did not allow us to continue with this version.



