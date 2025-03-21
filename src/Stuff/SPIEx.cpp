#include "SPIEx.h"
#include "SPI.h"

#include "esp32-hal-spi.h"
#define PLATFORM_ESP32
#if defined(PLATFORM_ESP32)
    #include <soc/spi_struct.h>
#endif

void ICACHE_RAM_ATTR SPIExClass::_transfer(uint8_t cs_mask, uint8_t *data, uint32_t size, bool reading)
{
#if defined(PLATFORM_ESP32)
    if (bus() != nullptr) {
        Serial.printf("bus() returned: %p\n", (void*)bus());
    }
    else
    {
        Serial.println("bus failed oh no");
        return;
    }
    spi_t * spiThing = bus();
    if(spiThing == nullptr)
    {
        Serial.println("spiThing... is a null pointer?");
        return;
    }
    else
    {
        Serial.printf("spiThing isn't a nullptr. It returned: %p\n", (void*) spiThing);
    }
    spi_dev_t* spi =  (spi_dev_t*) (spiThing->dev);
    // spi_dev_t *spi = *(reinterpret_cast<spi_dev_t**>(bus()));
    // if(spi == nullptr)
    // {
    //     Serial.println("NULL POINTER GRAHHH");
    //     return;
    // }
    // else
    // {
    //     // Serial.printf("SPI pointer lower 16 bits: 0x%04X\n", (uint32_t)spi & 0xFFFF);
    //     // Serial.printf("Live bitlen reaction: %d", spi->addr);
    // }
    // wait for SPI to become non-busy
    // char* thingToPrint = (char*) malloc(strlen("We're gonna find this for the while bit: ") * sizeof(char) + sizeof(int) + 10);
    // int usrInt = (int) spi->cmd.usr;
    // sprintf(thingToPrint, "We're gonna find this for the while bit: %d", usrInt);
    // Serial.println(thingToPrint);
    // free(thingToPrint);

    // while(spi->cmd.usr) {}
    // Set the CS pins which we want controlled by the SPI module for this operation
    // spiDisableSSPins(bus(), ~cs_mask);

//     spiEnableSSPins(bus(), cs_mask);
// #if defined(PLATFORM_ESP32_S3) || defined(PLATFORM_ESP32_C3)
//     spi->ms_dlen.ms_data_bitlen = (size*8)-1;
// #else
//     spi->mosi_dlen.usr_mosi_dbitlen = ((size * 8) - 1);
//     spi->miso_dlen.usr_miso_dbitlen = ((size * 8) - 1);
// #endif

//     // write the data to the SPI FIFO
//     const uint32_t words = (size + 3) / 4;
//     auto * const wordsBuf = reinterpret_cast<uint32_t *>(data);
//     for(int i=0; i<words; i++)
//     {
//         spi->data_buf[i] = wordsBuf[i]; //copy buffer to spi fifo
//     }

// #if defined(PLATFORM_ESP32_S3) || defined(PLATFORM_ESP32_C3)
//     spi->cmd.update = 1;
//     while (spi->cmd.update) {}
// #endif
//     // start the SPI module
//     spi->cmd.usr = 1;

//     if (reading)x    
//     {
//         // wait for SPI write to complete
//         while(spi->cmd.usr) {}

//         for(int i=0; i<words; i++)
//         {
//             wordsBuf[i] = spi->data_buf[i]; //copy spi fifo to buffer
//         }
//     }
//     end();
#elif defined(PLATFORM_ESP8266)
    // we support only one hardware controlled CS pin, so theres nothing to do to configure it at this point

    // wait for SPI to become non-busy
    while(SPI1CMD & SPIBUSY) {}

    // Set in/out Bits to transfer
    const uint32_t mask = ~((SPIMMOSI << SPILMOSI) | (SPIMMISO << SPILMISO));
    const auto bits = (size * 8) - 1;
    SPI1U1 = ((SPI1U1 & mask) | ((bits << SPILMOSI) | (bits << SPILMISO)));

    // write the data to the SPI FIFO
    volatile uint32_t * const fifoPtr = &SPI1W0;
    const uint8_t outSize = ((size + 3) / 4);
    uint32_t * const dataPtr = (uint32_t*) data;
    for(int i=0; i<outSize; i++)
    {
        fifoPtr[i] = dataPtr[i];
    }

    // start the SPI module
    SPI1CMD |= SPIBUSY;

    if (reading)
    {
        // wait for SPI write to complete
        while(SPI1CMD & SPIBUSY) {}

        // read data from the FIFO back to the application buffer
        for(int i=0; i<outSize; i++)
        {
            dataPtr[i] = fifoPtr[i];
        }
    }
#endif
}

#if defined(PLATFORM_ESP32_S3) || defined(PLATFORM_ESP32_C3)
SPIExClass SPIEx(FSPI);
#elif defined(PLATFORM_ESP32)
SPIExClass SPIEx(FSPI);
#else
SPIExClass SPIEx(VSPI);
#endif
