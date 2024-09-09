// calibration.h
// This file contains the functions for the calibration of each individual IMU as well as the
// offset calculator

#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_BNO080_Arduino_Library.h" //Source: http://librarymanager/All#SparkFun_BNO080
#include <Adafruit_SSD1306.h>
#include <vector>
#include <string>
#include "display.h"

// Given a vector of doubles, find the mean of the values
double findMean(std::vector<double> vector_vals);

// Find the average value in a vector of doubles excluding outliers
double findTrueAve(std::vector<double> vector_vals);

// Calculate the offset for the pitch, roll, and yaw
void calibrateAverage(double &roll_offset, double &pitch_offset, double &yaw_offset, BNO080 bno08x1, BNO080 bno08x2, int num_calibrations, Adafruit_SSD1306 &display);

// Calibrate individual sensors
void calibrateSystem(BNO080 &my_IMU, int IMU_num, Adafruit_SSD1306 &display);

#endif