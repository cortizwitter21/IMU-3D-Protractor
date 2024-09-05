# IMU-3D-Protractor
This repository contains the code used for the 3D Protractor prototype intended for use a Shriner's Hospital for Children. The protractor would contain two housing structures, one for each IMU, which would be placed on each limb via offshoots on the limbs. The device would then calculate the angles the Euler angles between the two limbs, effectively finding the rotation of the joints in the sagittal, frontal, and transverse planes. These values would be displayed on an SSD1306 I2C OLED display.

Before turning on, the device should be sit on a "calibration rail" that forces the protractor into the known 0 position. The device woud then turn on and calibrate its current angle measurements to be 0 degrees rotation in all planes. From there, the device could be used as normal. To recalibrate, one sould simply turn off the device, put it on the calibration rail, and turn it on again.





