#include <stdio.h>
#include <Arduino.h>
#include "rx_api.h"


static TaskHandle_t elrsDeviceTask = NULL;

void setupELRS()
{
    delay(3000);
    xTaskCreatePinnedToCore(elrsSetup, "elrsTask", 32768, NULL, 5, &elrsDeviceTask, 0);
}

void setup()
{
    setupELRS();

    printf("Main: hello world from core %d!\n", xPortGetCoreID() );
}

void loop()
{
    // printf("Main: world from core %d!\n", xPortGetCoreID() );
}
