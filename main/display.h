// display.h
// This file contains the functions necessary for displaying text and angle measurements to the OLE screen

#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <vector>
#include <string>

// Display text on the screen
void displayText(std::string message, Adafruit_SSD1306 &display);

// Display angles on the screen
void displayAngles(double roll, double pitch, double yaw, Adafruit_SSD1306 &display);

#endif