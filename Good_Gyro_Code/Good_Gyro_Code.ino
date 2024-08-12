
#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_BNO080_Arduino_Library.h" // Click here to get the library: http://librarymanager/All#SparkFun_BNO080
#include <Adafruit_SSD1306.h>

BNO080 bno08x1; //Open I2C ADR jumper - goes to address 0x4B
BNO080 bno08x2; //Closed I2C ADR jumper - goes to address 0x4A

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

float droll, dpitch, dyaw;

struct euler_t {
  float yaw;
  float pitch;
  float roll;
} ypr1;

struct euler_t ypr2;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("BNO085 Reading Angles");

  Wire.begin();
  Wire.setClock(400000); //Increase I2C data rate to 400kHz


  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    while (1);
  }
  display.display();
  delay(2000);  // Pause for 2 seconds
  display.clearDisplay();

  //When a large amount of time has passed since we last polled the sensors
  //they can freeze up. To un-freeze it is easiest to power cycle the sensor.

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

  Serial.println(F("Rotation vector enabled"));
  Serial.println(F("Output in form i, j, k, real, accuracy"));
}

void displayText(float roll, float pitch, float yaw) {
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

  droll = ypr1.roll - ypr2.roll;
    dpitch = ypr1.pitch - ypr2.pitch;
    dyaw = ypr1.yaw - ypr2.yaw;
    
    Serial.print("two IMUs");             Serial.print("\t");
    Serial.print("Yaw:");
    Serial.print(dyaw);                Serial.print("\t");
    Serial.print("Pitch:");
    Serial.print(dpitch);              Serial.print("\t");
    Serial.print("Roll:");
    Serial.println(droll);

    displayText(droll, dpitch, dyaw);

    delay(100);  // Delay for readability
}
