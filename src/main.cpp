#include <Wire.h>
#include <module/communication.h>
#include <Arduino.h>

void processSensors();

void moveCore1Msgs();

//#define LED_PIN_V1     10  // virtual digital pin used to drive the virtual LED strip
//#define LED_PIN_P1     11  // physical digital pin used to drive the first physical LED strip
//#define LED_PIN_P2     12  // physical digital pin used to drive the second physical LED strip
//#define LED_COUNT_P1   133  // number of LEDs on the first physical strip
//#define LED_COUNT_P2   98  // number of LEDs on the second physical strip
//#define LED_COUNT_P3   25  // number of LEDs on the second physical strip
//
//WS2812FX ws2812fx_v1 = WS2812FX(LED_COUNT_P1 + LED_COUNT_P2 + LED_COUNT_P3, LED_PIN_V1, NEO_RGBW + NEO_KHZ800);
//WS2812FX ws2812fx_p1 = WS2812FX(1,                           LED_PIN_P1, NEO_RGBW + NEO_KHZ800);
//WS2812FX ws2812fx_p2 = WS2812FX(1,                           LED_PIN_P2, NEO_RGBW + NEO_KHZ800);

//void myCustomShow(void) {
//    ws2812fx_p1.show();
//    ws2812fx_p2.show();
//}

unsigned long reconnectTimeout;
#define RESENT_TIMEOUT   3000

void init() {
    Logger::printlnCore0("\nStart Initializing ");

    Logger::printlnCore0(STR(DEVICE_NAME));

    Wire.begin();

    sleep_ms(1000);
    setupMqtt();
    sleep_ms(1000);
    Logger::printlnCore0("\nCPU " + String(rp2040.f_cpu()));
    core0Devices.init();


    while (driverState != WORKING) {
        if ((millis() > reconnectTimeout) || (reconnectTimeout == 0)) {
            Logger::printlnCore0("Waiting for config...");
            initDevices();
            reconnectTimeout = millis() + RESENT_TIMEOUT;
        }
        mqttService();
    }

    pinMode(LED_BUILTIN, OUTPUT);

}

void setup() {
    rp2040.idleOtherCore();

    Serial.begin(115200);
#ifdef APP_DEBUG
//    while (!Serial);
#endif

    init();



//    ws2812fx_v1.init();
//    ws2812fx_v1.setBrightness(255);
//    ws2812fx_v1.setSegment(0, 0, ws2812fx_v1.getLength()-1, FX_MODE_RAINBOW_CYCLE, RED, 2000, NO_OPTIONS);
//    ws2812fx_v1.start();
//
//    ws2812fx_p1.init();
//    ws2812fx_p1.setPixels(LED_COUNT_P1, ws2812fx_v1.getPixels());
//    ws2812fx_p1.reverse = true;
//    ws2812fx_p2.init();
//    ws2812fx_p2.setPixels(LED_COUNT_P2, ws2812fx_v1.getPixels() + ((LED_COUNT_P3 + LED_COUNT_P1) * ws2812fx_v1.getNumBytesPerPixel()));
//
//    ws2812fx_v1.setCustomShow(myCustomShow);

    rp2040.resumeOtherCore();

    Logger::printlnCore0("configuration finished");
}

void loop() {
    int startMicros = micros();
    mqttService();
    core0Devices.service();

    if (actionCore0Msgs.rear > 0) {
        while (actionCore0Msgs.rear > 0) {
            Action *pAction = actionCore0Msgs.queueDequeue();
            core0Devices.process(pAction);
            delete pAction;
        }
    }
    if (actionCore1Msgs.rear > 0) {
        while (actionCore1Msgs.rear > 0) {
            Action* pAction = actionCore1Msgs.queueDequeue();

            core1Devices.process(pAction);
            delete pAction;
        }
    }

    processSensors();
    core1Devices.service();
    int endMicros = micros();
//    Serial.println("cycle time: " + String(endMicros - startMicros));
}


void processSensors() {
    for (auto &sensor : core0Devices.motionSensors) {
        if (sensor->check() != NOT_CHANGED) {
            Logger::printlnCore0(String(sensor->id) + ": " + String(sensor->currentState));
            publishSensor(sensor->id, sensor->currentState);
        }
    }
}