// Main Program
// This program is meant to be directly run every time the 3D Protractor is turned on.


#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_BNO080_Arduino_Library.h" //Source: http://librarymanager/All#SparkFun_BNO080
#include <Adafruit_SSD1306.h>
#include <calibration.h>
#include <vector>
#include <string>



// define screen size
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

// define number of cycles will be run to find the initial calibration offset.
int num_calibrations = 20;

// define breakout components
BNO080 bno08x1; // address 0x4B
BNO080 bno08x2; // address 0x4A
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

//define difference in roll, pitch, and yaw
double roll_diff, pitch_diff, yaw_diff;

// define struct that will house the yaw, pitch, and. roll of each individual IMU
struct euler_t {
  double yaw;
  double pitch;
  double roll;
} ypr1;


struct euler_t ypr2;

void displayText(std::string message) {
  display.clearDisplay();
  display.setTextSize(1);       // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  display.setCursor(0, 0);      // Start at top-left corner


  // Display the text
  display.println(message.c_str());


  display.display();            // Show the content on the display
  delay(1000);
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Initializing 3D Protractor...");


  Wire.begin();
  Wire.setClock(400000); //Increase I2C data rate to 400kHz



  // Check for OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    while (1);
  }
  displayText("Initializing 3D Protractor...");







  //Start 2 sensors
  if (bno08x1.begin(0x4B) == false)
  {
    Serial.println("First BNO080 not detected with I2C ADR jumper open. Check your jumpers and the hookup guide. Freezing...");
    while(1);
  }


  if (bno08x2.begin(0x4A) == false)
  {
    Serial.println("Second BNO080 not detected with I2C ADR jumper closed. Check your jumpers and the hookup guide. Freezing...");
    while(1);
  }


  bno08x1.enableRotationVector(50); //Send data update every 50ms
  bno08x2.enableRotationVector(50); //Send data update every 50ms


  display.println(F("Rotation vector enabled"));
  delay(1000);
  display.display();
  display.clearDisplay();
}

// Display a given set of roll, pitch, and yaw values on screen
void displayAngles(double roll, double pitch, double yaw) {
  display.clearDisplay();
  display.setTextSize(1);       // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  display.setCursor(0, 0);      // Start at top-left corner


  // Display the angles
  display.print("Roll: ");
  display.println(roll, 2);
  display.print("Pitch: ");
  display.println(pitch, 2);
  display.print("Yaw: ");
  display.println(yaw, 2);


  display.display();            // Show the content on the display
}

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


void loop()
{
  //Look for reports from the IMU
  if (bno08x1.dataAvailable() == true)
  {
    ypr1.roll = (bno08x1.getRoll()) * 180.0 / PI; // Convert roll to degrees
    ypr1.pitch = (bno08x1.getPitch()) * 180.0 / PI; // Convert pitch to degrees
    ypr1.yaw = (bno08x1.getYaw()) * 180.0 / PI; // Convert yaw / heading to degrees


  }


  if (bno08x2.dataAvailable() == true)
  {
    ypr2.roll = (bno08x2.getRoll()) * 180.0 / PI; // Convert roll to degrees
    ypr2.pitch = (bno08x1.getPitch()) * 180.0 / PI; // Convert pitch to degrees
    ypr2.yaw = (bno08x2.getYaw()) * 180.0 / PI; // Convert yaw / heading to degrees


  }


  roll_diff = ypr1.roll - ypr2.roll;
    pitch_diff = ypr1.pitch - ypr2.pitch;
    yaw_diff = ypr1.yaw - ypr2.yaw;
   
    Serial.print("two IMUs");             Serial.print("\t");
    Serial.print("Yaw:");
    Serial.print(yaw_diff);                Serial.print("\t");
    Serial.print("Pitch:");
    Serial.print(pitch_diff);              Serial.print("\t");
    Serial.print("Roll:");
    Serial.println(roll_diff);


    displayAngles(roll_diff, pitch_diff, yaw_diff);


    delay(100);  // Delay for readability
}



