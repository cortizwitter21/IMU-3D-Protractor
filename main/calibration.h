#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_BNO080_Arduino_Library.h" //Source: http://librarymanager/All#SparkFun_BNO080
#include <Adafruit_SSD1306.h>
#include <vector>
#include <string>
#include "display.h"

double findMean(std::vector<double> vector_vals);

double findTrueAve(std::vector<double> vector_vals);

void calibrateAverage(double &roll_offset, double &pitch_offset, double &yaw_offset, BNO080 bno08x1, BNO080 bno08x2, int num_calibrations, Adafruit_SSD1306 &display);

void calibrateSystem(BNO080 &my_IMU, int IMU_num, Adafruit_SSD1306 &display);

#endif