#pragma once

#include <calibration.ino>

double findMean(std::vector<double> vector_vals);

double findTrueAve(std::vector<double> vector_vals);

void calibrateAverage(double &roll_offset, double &pitch_offset, double &yaw_offset, BNO080 bno08x1, BNO080 bno08x2, int num_calibrations);

void calibrateSystem(BNO080 &my_IMU, int IMU_num);