#include <Arduino.h>
#include "SX1280.h"

SX1280Driver *driver;
#define Sx1280Freq 2400000000
#define Sx1280Bandwidth 203
#define Sx1280CodeRate 5
#define Sx1280PreambleRate 10
#define Sx1280PayloadLength 36
#define Sx1280IntervalTimeout 0
void setup() {
  // put your setup code here, to run once:
  pinMode(15, OUTPUT);
  driver = SX1280Driver::instance;
  driver->SetRxTimeoutUs(0);
  driver->SetFrequencyHz(Sx1280Freq, SX12XX_Radio_All);
  driver->SetFrequencyReg(Sx1280Freq, SX12XX_Radio_All);
  driver->Config(Sx1280Bandwidth, SX1280_LORA_SF6, Sx1280CodeRate,
    Sx1280Freq, Sx1280PreambleRate, false, Sx1280PayloadLength, 
    Sx1280IntervalTimeout);
}

void loop() {
  ESP_LOGI("RX", "Beginning to receive data:");
  digitalWrite(15, HIGH);
  delay(100);
  // driver->Begin(Sx1280Freq - 100, Sx1280Freq + 100);
  ESP_LOGI("RX", "Done receiving.");
  digitalWrite(15, LOW);
  delay(100);
}