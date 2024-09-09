# IMU-3D-Protractor
This repository contains the code used for the 3D Protractor prototype intended for use at Shriners Hospitals for Children to measure joint angles in all three medical planes (sagittal, frontal, and transverse) for motion analysis calibration. The protractor would contain two housing structures, one for each IMU, which would be connected via wires contained in a covering similar to a goose neck. Each housing structure would be placed on a limb on either end of the joint via offshoots on the limbs. The device would then calculate the Euler angles between the two limbs, effectively finding the rotation of a given joint in the three planes. These values would be displayed on an SSD1306 I2C OLED display.

Before turning on, the device should be placed on a "calibration rail" that forces the protractor into the known 0 degree position for all planes. The device woud then turn on and calibrate its current rotational angle measurements to be 0 degrees in all planes. From there, the device could be used as intended. To recalibrate, one sould simply turn off the device via a switch, put it on the calibration rail, and turn it on again.

## Necessary Hardware

- 2 Adafruit 9-DOF Orientation IMU Fusion Breakout - BNO085

- 1 SSD 1306 I2C OLED Display

- 1 Adafruit ItsyBitsy M4

- Wires

- 2 Protoboards (one for each IMU to allow free motion between them)

- 1 Battery Pack For 3.3V

- 1 Switch


## Folders

**main**: contains all code necessary for the 3D Protractor. Files include the following:

- main.ino: main program for functioning 3D protractor

- calibration.h/calibration.cpp: contains functions relating to calibration, including calibration of each sensor and calibration of the relative angle (calculating the necessary offset).

- display.h/display.cpp: contains functions relating to displaying information on the SSD1306 OLED display.

**Wiring and Assembly**: contains the wiring diagram, pictures of preliminary CAD designs for housing, and a sketch of the design.

**CircuitPython Attempt**: contains code written when initially trying to use a Raspberry Pi Pico W. This route was abandoned since we could not find a library that addressed our needs (see Notes for Future Iterations), but for the sake of reproducibility and documentation, the code is still included. However, the majority of it is not functional


## Notes for Future Iterations:
1. For Arduino, the Adafruit_BNO08x library (the official Adafruit library) does **not** support 2 BNO08x IMUs simultaneously, therefore should not be used for this project.
2. The bno08x library from the most recent adafruit-circuitpython-bundle does not support change of address for the IMUs, meaning our resources did not allow us to continue with this version.
