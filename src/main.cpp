#include <Arduino.h>
#include "SX1280.h"
#include "SX1280_Regs.h"

SX1280Driver driver;
uint8_t bufferAddr;
#define Sx1280Freq 2400000000
#define Sx1280Bandwidth 203
#define Sx1280CodeRate 5
#define Sx1280PreambleRate 10
#define Sx1280PayloadLength 36
#define Sx1280IntervalTimeout 1000
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(10000);
  Serial.println("AAAAA");
  pinMode(15, OUTPUT);
  Serial.println("TIMEOUT: " + driver.timeout);
  // try
  // {
  //   driver->SX1280Driver::SetFrequencyHz(Sx1280Freq, SX12XX_Radio_All);
  // }
  // catch(const std::exception &e)
  // {
  //   Serial.println("Pre config doesn't work.");
  // }

  

  // try
  // {
  //   driver->SX1280Driver::Config((uint8_t) Sx1280Bandwidth, (uint8_t) SX1280_LORA_SF6, (uint8_t) Sx1280CodeRate,
  //   (uint32_t) Sx1280Freq, (uint8_t) Sx1280PreambleRate, (bool) false, (uint8_t) Sx1280PayloadLength, 
  //   (uint32_t) Sx1280IntervalTimeout, (uint32_t) 0, (uint32_t) 0, (uint32_t) 0);
  // }
  // catch(const std::exception &e)
  // {
  //   Serial.flush();
  //   Serial.print("an issue with config: ");
  //   Serial.println(e.what());
  //   Serial.flush();
  // }

  // bufferAddr = driver->GetRxBufferAddr(SX12XX_Radio_All);
  // Serial.println("LOCATION OF BUFFER: " + bufferAddr);
}

void loop() {
  
  // Serial.println("RX: Beginning to receive data:");
  // digitalWrite(15, HIGH);
  // delay(500);
  // // driver->Begin(Sx1280Freq - 100, Sx1280Freq + 100);
  // Serial.println("RX: Done receiving");
  // digitalWrite(15, LOW);
  // delay(500);
}