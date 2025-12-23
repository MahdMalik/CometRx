#include <stdio.h>
#include <Arduino.h>
#include "rx_api.h"

static TaskHandle_t elrsDeviceTask = NULL;

// this is the channel data you'll be modifying
void cleanBits(uint32_t *ChannelData)
{
    // the chanenl has 16 bits. Anyways go crazy, transform those bits however youd like.
}
// this is the channel data you'll be modifying
void performSpecialOutput()
{
    
}



void setupELRS(bool isSmoothing, bool isSpecialOutput)
{
    paramStruct params;
    params.isSmoothing = isSmoothing;
    params.smoothFunction = cleanBits;
    params.isDiffOutputting = isSpecialOutput;
    params.outputFunction = performSpecialOutput;
    
    delay(3000);
    xTaskCreatePinnedToCore(elrsSetup, "elrsTask", 32768, (void*) &params, 5, &elrsDeviceTask, 0);
}

void setup()
{
    bool wantSmoothing = true;
    bool wantDiffOutput = true;
    
    setupELRS(wantSmoothing, wantDiffOutput);

    printf("Main: hello world from core %d!\n", xPortGetCoreID() );
}

void loop()
{
    // printf("Main: world from core %d!\n", xPortGetCoreID() );
}