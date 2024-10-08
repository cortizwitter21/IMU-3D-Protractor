// Main Program
// This program is meant to be directly run every time the 3D Protractor is turned on.

#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_BNO080_Arduino_Library.h" //Source: http://librarymanager/All#SparkFun_BNO080
#include <Adafruit_SSD1306.h>
#include "calibration.h"
#include "display.h"
#include <vector>
#include <string>



// define screen size
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

// define number of cycles to be run to find the initial calibration offset.
int num_calibrations = 20;

// define breakout components
BNO080 bno08x1; // address 0x4B
BNO080 bno08x2; // address 0x4A
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

//define difference in roll, pitch, and yaw
double roll_diff, pitch_diff, yaw_diff;

// define offsets for roll, pitch and yaw
double roll_offset, pitch_offset, yaw_offset;

// define struct that will house the yaw, pitch, and. roll of each individual IMU
// ypr1 holds information for sensor 1, and ypr2 holds information for sensor 2
struct euler_t {
  double yaw;
  double pitch;
  double roll;
} ypr1;
struct euler_t ypr2; 

void setup()
{
  // Start serial prints
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
  displayText("Initializing 3D Protractor...", display);
  Serial.println("Found OLED display");






  // Check for 1st IMU
  if (bno08x1.begin(0x4B) == false)
  {
    Serial.println("First BNO085 not detected with I2C ADR jumper open");
    while(1);
  }
  Serial.println("Found Sensor 1");

  // Check for 2nd IMU
  if (bno08x2.begin(0x4A) == false)
  {
    Serial.println("Second BNO085 not detected with I2C ADR jumper closed");
    while(1);
  }
  Serial.println("Found Sensor 2");

  // Enable the rotation vectors for both IMUs
  bno08x1.enableRotationVector(50); //Send data update every 50ms
  bno08x2.enableRotationVector(50); //Send data update every 50ms
  displayText("Rotation vector enabled", display);

  // Calibrate each individual IMU
  calibrateSystem(bno08x1, 1, display);
  calibrateSystem(bno08x2, 2, display);

  // Calculate the offsets for each angle measurement
  calibrateAverage(roll_offset, pitch_offset, yaw_offset, bno08x1, bno08x2, num_calibrations, display);
}

void loop()
{
  // Get the roll, pitch, and yaw from sensor 1
  if (bno08x1.dataAvailable() == true)
  {
    ypr1.roll = (bno08x1.getRoll()) * 180.0 / PI; // Convert roll to degrees
    ypr1.pitch = (bno08x1.getPitch()) * 180.0 / PI; // Convert pitch to degrees
    ypr1.yaw = (bno08x1.getYaw()) * 180.0 / PI; // Convert yaw  to degrees


  }

  // Get the roll, pitch, and yaw from sensor 2
  if (bno08x2.dataAvailable() == true)
  {
    ypr2.roll = (bno08x2.getRoll()) * 180.0 / PI; // Convert roll to degrees
    ypr2.pitch = (bno08x2.getPitch()) * 180.0 / PI; // Convert pitch to degrees
    ypr2.yaw = (bno08x2.getYaw()) * 180.0 / PI; // Convert yaw to degrees


  }

  // Calculate the difference in angles, taking the offset into account
  roll_diff = ypr1.roll - ypr2.roll - roll_offset;
  pitch_diff = ypr1.pitch - ypr2.pitch - pitch_offset;
  yaw_diff = ypr1.yaw - ypr2.yaw - yaw_offset;
   
  // Print results on serial monitor
  Serial.print("two IMUs");             Serial.print("\t");
  Serial.print("Yaw:");
  Serial.print(yaw_diff);                Serial.print("\t");
  Serial.print("Pitch:");
  Serial.print(pitch_diff);              Serial.print("\t");
  Serial.print("Roll:");
  Serial.println(roll_diff);

  // Print results on OLED screen
  displayAngles(roll_diff, pitch_diff, yaw_diff, display);

  // Delay for readability
  delay(100);  
}



