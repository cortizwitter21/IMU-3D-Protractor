#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_BNO080_Arduino_Library.h" //Source: http://librarymanager/All#SparkFun_BNO080
#include <Adafruit_SSD1306.h>
#include <vector>
#include <string>
#include "calibration.h"

// Find the mean of a vector of values
double findMean(std::vector<double> vector_vals) {
  double sum = 0;

  // Add all of the numbers in the vector
  for (int i = 0; i < vector_vals.size(); i++) {
    sum += vector_vals.at(i);
  }
  // Divide by the size of the vector
  return sum / vector_vals.size();
}

// Find the average value in a vector excluding outliers
double findTrueAve(std::vector<double> vector_vals) {
 
  double average;
  double std_error;
  double sum_of_squared_diff = 0.0;
  double variance;
  std::vector<double> new_vector;

  average = findMean(vector_vals);


    // Calculate the sum of the squared differences from the mean
  for (int i = 0; i < vector_vals.size(); i++) {
      sum_of_squared_diff += pow(vector_vals.at(i) - average, 2);
  }
  variance = sum_of_squared_diff / vector_vals.size();

  // Find the standard error
  std_error = sqrt(variance);

  // If a value is more than 2 standard deviations away from the mean, it is an outlier, so remove it
  for (int i = 0; i < vector_vals.size(); i++) {
    if (abs(average - vector_vals.at(i)) <= 2*std_error) {
      new_vector.push_back(vector_vals.at(i));
    }
  }
  // Find mean of the vector excluding outliers
  return findMean(new_vector);

}

// Calculate the offset for the pitch, roll, and yaw
void calibrateAverage(double &roll_offset, double &pitch_offset, double &yaw_offset, BNO080 bno08x1, BNO080 bno08x2, int num_calibrations)
{


  double roll_diff, pitch_diff, yaw_diff;
  // Define vectors that will store the various angle value differences measured
  std::vector<double> roll_vals;
  std::vector<double> pitch_vals;
  std::vector<double> yaw_vals;
 


  for (int i = 0; i < num_calibrations; i++) {
    // Find pitch, yaw, and roll of bno08x1
    if (bno08x1.dataAvailable() == true)
    {
      ypr1.roll = (bno08x1.getRoll()) * 180.0 / PI; // Convert roll to degrees
      ypr1.pitch = (bno08x1.getPitch()) * 180.0 / PI; // Convert pitch to degrees
      ypr1.yaw = (bno08x1.getYaw()) * 180.0 / PI; // Convert yaw / heading to degrees


    }

    // Find pitch, yaw, and roll of bno08x2
    if (bno08x2.dataAvailable() == true)
    {
      ypr2.roll = (bno08x2.getRoll()) * 180.0 / PI; // Convert roll to degrees
      ypr2.pitch = (bno08x1.getPitch()) * 180.0 / PI; // Convert pitch to degrees
      ypr2.yaw = (bno08x2.getYaw()) * 180.0 / PI; // Convert yaw / heading to degrees


    }

    // Store differences of angles in their respective vectors
    roll_vals[i] = ypr1.roll - ypr2.roll;
    pitch_vals[i] = ypr1.pitch - ypr2.pitch;
    yaw_vals[i] = ypr1.yaw - ypr2.yaw;


  }

  // Find true averages of the vectors and store as offsets
  roll_offset = findTrueAve(roll_vals);
  pitch_offset = findTrueAve(pitch_vals);
  yaw_offset = findTrueAve(yaw_vals);
}

// Calibrate individual sensors
void calibrateSystem(BNO080 &my_IMU, int IMU_num) {
  my_IMU.calibrateAll(); //Turn on cal for Accel, Gyro, and Mag


  //Enable Game Rotation Vector output
  my_IMU.enableGameRotationVector(100); //Send data update every 100ms


  //Enable Magnetic Field output
  my_IMU.enableMagnetometer(100); //Send data update every 100ms

  int counter = 0;
  byte accuracy;
  byte sensor_accuracy;

  while(sensor_accuracy < 2 | counter < 10) {
    if (my_IMU.dataAvailable() == true)
    {
      float x = my_IMU.getMagX();
      float y = my_IMU.getMagY();
      float z = my_IMU.getMagZ();
      accuracy = my_IMU.getMagAccuracy();

      float quatI = my_IMU.getQuatI();
      float quatJ = my_IMU.getQuatJ();
      float quatK = my_IMU.getQuatK();
      float quatReal = my_IMU.getQuatReal();
      sensor_accuracy = my_IMU.getQuatAccuracy();

      Serial.print(x, 2);
      Serial.print(F(","));
      Serial.print(y, 2);
      Serial.print(F(","));
      Serial.print(z, 2);
      Serial.print(F(","));
      Serial.print(accuracy);
      Serial.print(F(","));

      Serial.print("\t");

      Serial.print(quatI, 2);
      Serial.print(F(","));
      Serial.print(quatJ, 2);
      Serial.print(F(","));
      Serial.print(quatK, 2);
      Serial.print(F(","));
      Serial.print(quatReal, 2);
      Serial.print(F(","));
      Serial.print(sensor_accuracy);
      Serial.print(F(","));

      Serial.println();
      counter+=1;
    }
  }
  if (sensor_accuracy < 2) {
    std::string bad_message = std::string("Sensor ") + std::to_string(IMU_num) + "offset, but with low calibration accuracy";
    displayText(bad_message);
  }
  else {
    std::string good_message = std::string("Sensor ") + std::to_string(IMU_num) + "offset successfully!";
    displayText(good_message);
  }
}
