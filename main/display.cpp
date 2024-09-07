#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_BNO080_Arduino_Library.h" //Source: http://librarymanager/All#SparkFun_BNO080
#include <Adafruit_SSD1306.h>
#include <vector>
#include <string>
#include "display.h"

void displayText(std::string message, Adafruit_SSD1306 &display) {
  display.clearDisplay();
  display.setTextSize(1);       // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  display.setCursor(0, 0);      // Start at top-left corner


  // Display the text
  display.println(message.c_str());


  display.display();            // Show the content on the display
  delay(1000);
}

// Display a given set of roll, pitch, and yaw values on screen
void displayAngles(double roll, double pitch, double yaw, Adafruit_SSD1306 &display) {
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