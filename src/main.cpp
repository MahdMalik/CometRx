#include <Arduino.h>
#include "SX1280.h"
#include "SX1280_Regs.h"
#include <string>

uint8_t bufferAddr;
#define Sx1280Freq 2400000000
#define Sx1280Bandwidth 203
#define Sx1280CodeRate 5
#define Sx1280PreambleRate 10
#define Sx1280PayloadLength 36
#define Sx1280IntervalTimeout 1000

SX1280Driver* theDriver;

void setup() {
  // put your setup code here, to run once:
  delay(1000);
  pinMode(15, OUTPUT);
  new SX1280Driver();
  theDriver = SX1280Driver::instance;
  try
  {
    theDriver->SX1280Driver::SetFrequencyHz(Sx1280Freq, SX12XX_Radio_All);
    Serial.println("Frequency set successfully.");
  }
  catch(const std::exception &e)
  {
    Serial.println("Pre config doesn't work.");
  }

  try
  {
    theDriver->SX1280Driver::Config((uint8_t) Sx1280Bandwidth, (uint8_t) SX1280_LORA_SF6, (uint8_t) Sx1280CodeRate,
    (uint32_t) Sx1280Freq, (uint8_t) Sx1280PreambleRate, (bool) false, (uint8_t) Sx1280PayloadLength, 
    (uint32_t) Sx1280IntervalTimeout, (uint32_t) 0, (uint32_t) 0, (uint32_t) 0);
    Serial.println("Config worked.");
  }
  catch(const std::exception &e)
  {
    Serial.print("an issue with config: ");
    Serial.println(e.what());
    Serial.flush();
  }

  bufferAddr = theDriver->GetRxBufferAddr(SX12XX_Radio_All);
  char* str = (char*)malloc(sizeof(char) * strlen("LOCATION OF BUFFER: ") + sizeof(uint8_t) + 10);
  sprintf(str, "LOCATION OF BUFFER: %d", bufferAddr);
  Serial.println(str);
  free(str);
}

void loop() 
{  
  Serial.println("RX: Beginning to receive data:");
  digitalWrite(15, HIGH);
  delay(500);
  theDriver->Begin(Sx1280Freq - 100, Sx1280Freq + 100);
  Serial.println("RX: Done receiving");
  digitalWrite(15, LOW);
  delay(500);
}