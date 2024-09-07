#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <vector>
#include <string>

void displayText(std::string message, Adafruit_SSD1306 &display);

void displayAngles(double roll, double pitch, double yaw, Adafruit_SSD1306 &display);

#endif