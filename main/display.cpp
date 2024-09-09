// display.cpp
// This file contains the functions necessary for displaying text and angle measurements to the OLE screen

#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_BNO080_Arduino_Library.h" //Source: http://librarymanager/All#SparkFun_BNO080
#include <Adafruit_SSD1306.h>
#include <vector>
#include <string>
#include "display.h"

// displayText: display text on the screen
void displayText(std::string message, Adafruit_SSD1306 &display) {
  display.clearDisplay();       // Start with a clear display
  display.setTextSize(1);       // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  display.setCursor(0, 0);      // Start at top-left corner


  // queue text for displaying
  display.println(message.c_str());


  display.display();            // Show the content on the display
  delay(500);
  display.clearDisplay();
}

// displayAngles: display a given set of roll, pitch, and yaw values on screen
void displayAngles(double roll, double pitch, double yaw, Adafruit_SSD1306 &display) {
  display.clearDisplay();       // Start with a clear display
  display.setTextSize(1);       // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  display.setCursor(0, 0);      // Start at top-left corner


  // Display the angles
  display.print("X-axis: ");
  display.println(roll, 2);
  display.print("Y-axis: ");
  display.println(pitch, 2);
  display.print("Z-axis: ");
  display.println(yaw, 2);


  display.display();            // Show the content on the display
}