#include "rxtx_common.h"
#include "LowPassFilter.h"

#include "crc.h"
#include "telemetry_protocol.h"
#include "telemetry.h"
#include "stubborn_sender.h"
#include "stubborn_receiver.h"

#include "lua.h"
#include "msp.h"
#include "msptypes.h"
#include "PFD.h"
#include "options.h"
#include "dynpower.h"
#include "MeanAccumulator.h"
#include "freqTable.h"

#include "rx-serial/SerialIO.h"
#include "rx-serial/SerialNOOP.h"
#include "rx-serial/SerialCRSF.h"
#include "rx-serial/SerialSBUS.h"
#include "rx-serial/SerialSUMD.h"
#include "rx-serial/SerialAirPort.h"
#include "rx-serial/SerialHoTT_TLM.h"
#include "rx-serial/SerialMavlink.h"
#include "rx-serial/SerialTramp.h"
#include "rx-serial/SerialSmartAudio.h"

#include "FreeRTOSConfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "rx-serial/devSerialIO.h"
#include "devLED.h"
#include "devLUA.h"
#include "devWIFI.h"
#include "devButton.h"
#include "devServoOutput.h"
#include "devVTXSPI.h"
#include "devAnalogVbat.h"
#include "devSerialUpdate.h"
#include "devBaro.h"
#include "devMSPVTX.h"
#include "devThermal.h"

#if defined(PLATFORM_ESP8266)
#include <user_interface.h>
#include <FS.h>
#elif defined(PLATFORM_ESP32)
#include <SPIFFS.h>
#include "esp_task_wdt.h"
#endif

extern void setWifiUpdateMode();
void reconfigureSerial();

uint8_t getLq();

static inline void checkGeminiMode();

static uint8_t minLqForChaos();

void ICACHE_RAM_ATTR getRFlinkInfo();

void SetRFLinkRate(uint8_t index, bool bindMode); // Set speed of RF link

bool ICACHE_RAM_ATTR HandleFHSS();

void ICACHE_RAM_ATTR LinkStatsToOta(OTA_LinkStats_s * const ls);

bool ICACHE_RAM_ATTR HandleSendTelemetryResponse();

int32_t ICACHE_RAM_ATTR HandleFreqCorr(bool value);

void ICACHE_RAM_ATTR updatePhaseLock();

void ICACHE_RAM_ATTR HWtimerCallbackTick(); // this is 180 out of phase with the other callback, occurs mid-packet reception

//////////////////////////////////////////////////////////////
// flip to the other antenna
// no-op if GPIO_PIN_ANT_CTRL not defined
static inline void switchAntenna();

static void ICACHE_RAM_ATTR updateDiversity();

void ICACHE_RAM_ATTR HWtimerCallbackTock();

void LostConnection(bool resumeRx);

void ICACHE_RAM_ATTR TentativeConnection(unsigned long now);

void GotConnection(unsigned long now);

typedef struct {
    bool isSmoothing;
    std::function<void(uint32_t*)> smoothFunction;
    bool isDiffOutputting;
    std::function<void()> outputFunction;
} paramStruct;

static void ICACHE_RAM_ATTR ProcessRfPacket_RC(OTA_Packet_s const * const otaPktPtr);

void ICACHE_RAM_ATTR OnELRSBindMSP(uint8_t* newUid4);

static void ICACHE_RAM_ATTR ProcessRfPacket_MSP(OTA_Packet_s const * const otaPktPtr);

static void ICACHE_RAM_ATTR updateSwitchModePendingFromOta(uint8_t newSwitchMode);

static bool ICACHE_RAM_ATTR ProcessRfPacket_SYNC(uint32_t const now, OTA_Sync_s const * const otaSync);

bool ICACHE_RAM_ATTR ProcessRFPacket(SX12xxDriverCommon::rx_status const status);

bool ICACHE_RAM_ATTR RXdoneISR(SX12xxDriverCommon::rx_status const status);

void ICACHE_RAM_ATTR TXdoneISR();

void UpdateModelMatch(uint8_t model);

void SendMSPFrameToFC(uint8_t *mspData);

/**
 * Process the assembled MSP packet in MspData[]
 **/
void MspReceiveComplete();

static void setupSerial();

#if defined(PLATFORM_ESP32)
static void serial1Shutdown();

static void setupSerial1();

void reconfigureSerial1();
#else
    void setupSerial1();
    void reconfigureSerial1();
#endif

static void serialShutdown();

void reconfigureSerial();

static void setupConfigAndPocCheck();

static void setupTarget();

static void setupBindingFromConfig();

static void setupRadio();

static void updateTelemetryBurst();

/* If not connected will rotate through the RF modes looking for sync
 * and blink LED
 */
static void cycleRfMode(unsigned long now);

static void EnterBindingMode();

static void ExitBindingMode();

static void updateBindingMode(unsigned long now);

void EnterBindingModeSafely();

static void checkSendLinkStatsToFc(uint32_t now);

static void debugRcvrLinkstats();

static void debugRcvrSignalStats(uint32_t now);

static void updateSwitchMode();

static void CheckConfigChangePending();

void resetConfigAndReboot();

void elrsLoop(void *pvParameters);

void elrsSetup(void *pvParameters);

struct bootloader {
    uint32_t key;
    uint32_t reset_type;
};

void reset_into_bootloader(void);