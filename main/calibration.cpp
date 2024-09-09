// calibration.cpp
// This file contains the functions for the calibration of each individual IMU as well as the
// offset calculator

#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_BNO080_Arduino_Library.h" //Source: http://librarymanager/All#SparkFun_BNO080
#include <Adafruit_SSD1306.h>
#include <vector>
#include <string>
#include "calibration.h"

// define struct that will house the yaw, pitch, and. roll of each individual IMU
// ypr_1 holds information for sensor 1, and ypr_2 holds information for sensor 2
struct euler_cal {
  double yaw;
  double pitch;
  double roll;
} ypr_1;
struct euler_cal ypr_2;

// findMean: Given a vector of doubles, find the mean of the values
double findMean(std::vector<double> vector_vals) {
  double sum = 0;

  // Add all of the numbers in the vector
  for (int i = 0; i < vector_vals.size(); i++) {
    sum += vector_vals.at(i);
  }
  // Divide by the size of the vector and return this value
  return sum / vector_vals.size();
}

// findTrueAve: find the average value in a vector of doubles excluding outliers
double findTrueAve(std::vector<double> vector_vals) {
 
  double average;
  double std_error;
  double sum_of_squared_diff = 0.0;
  double variance;
  std::vector<double> new_vector;

  // Find the mean of the values in the vector
  average = findMean(vector_vals);


  // Calculate the sum of the squared differences and the variance
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

// calibrateAverage: calculate the offset for the pitch, roll, and yaw
void calibrateAverage(double &roll_offset, double &pitch_offset, double &yaw_offset, BNO080 bno08x1, BNO080 bno08x2, int num_calibrations, Adafruit_SSD1306 &display)
{


  double roll_diff, pitch_diff, yaw_diff;
  // Define vectors that will store the various angle value differences measured
  std::vector<double> roll_vals;
  std::vector<double> pitch_vals;
  std::vector<double> yaw_vals;
 

  // For a set amount of test cases
  for (int i = 0; i < num_calibrations; i++) {
    // Display the test case number
    std::string test_string = std::string("Running Test ") + std::to_string(i);
    displayText(test_string, display);
    // Find the pitch, yaw, and roll of bno08x1
    if (bno08x1.dataAvailable() == true)
    {
      ypr_1.roll = (bno08x1.getRoll()) * 180.0 / PI; // Convert roll to degrees
      ypr_1.pitch = (bno08x1.getPitch()) * 180.0 / PI; // Convert pitch to degrees
      ypr_1.yaw = (bno08x1.getYaw()) * 180.0 / PI; // Convert yaw / heading to degrees
    }

    // Find the pitch, yaw, and roll of bno08x2
    if (bno08x2.dataAvailable() == true)
    {
      ypr_2.roll = (bno08x2.getRoll()) * 180.0 / PI; // Convert roll to degrees
      ypr_2.pitch = (bno08x2.getPitch()) * 180.0 / PI; // Convert pitch to degrees
      ypr_2.yaw = (bno08x2.getYaw()) * 180.0 / PI; // Convert yaw / heading to degrees
    }

    // Store differences of angles in their respective vectors
    roll_vals.push_back(ypr_1.roll - ypr_2.roll);
    pitch_vals.push_back(ypr_1.pitch - ypr_2.pitch);
    yaw_vals.push_back(ypr_1.yaw - ypr_2.yaw);
  }

  // Find true averages of the vectors and store as offsets
  roll_offset = findTrueAve(roll_vals);
  pitch_offset = findTrueAve(pitch_vals);
  yaw_offset = findTrueAve(yaw_vals);
}

// calibrateSystem: calibrate individual sensors
void calibrateSystem(BNO080 &my_IMU, int IMU_num, Adafruit_SSD1306 &display) {
  my_IMU.calibrateAll(); // Turn on cal for Accel, Gyro, and Mag


  // Enable Game Rotation Vector output
  my_IMU.enableGameRotationVector(100); //Send data update every 100ms


  // Enable Magnetic Field output
  my_IMU.enableMagnetometer(100); //Send data update every 100ms

  int counter = 0;
  byte accuracy;
  byte sensor_accuracy;

  // While sensor accuracy is not high enough or calibration has not been attempted enough times
  while(sensor_accuracy < 2 | counter < 10) {
    // Acquire calibration information if data is available
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
    // Otherwise, display that the IMU is waiting for data
    else {
      displayText("Waiting for IMU data", display);
    }
  }
  // If the sensor accuracy is low, display this to the user
  if (sensor_accuracy < 2) {
    std::string bad_message = std::string("Sensor ") + std::to_string(IMU_num) + " calibrated but with low calibration accuracy";
    displayText(bad_message, display);
  }
  // Otherwise, let the user know calibration was successful
  else {
    std::string good_message = std::string("Sensor ") + std::to_string(IMU_num) + " calibrated successfully!";
    displayText(good_message, display);
  }
  // Save calibration data
  my_IMU.saveCalibration();
}
