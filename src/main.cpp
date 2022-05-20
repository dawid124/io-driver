#include <module/communication.h>
#include <Arduino.h>

void processSensors();

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


void init() {
    Logger::printlnCore0("\nStart Initializing ");
    Logger::printlnCore0(STR(DEVICE_NAME));

    sleep_ms(1000);
    setupMqtt();
    sleep_ms(1000);
    initDevices();

    Logger::printlnCore0("Waiting for config...");
    while (driverState != WORKING) {
        mqttService();
    }

    pinMode(LED_BUILTIN, OUTPUT);
}
void setup()
{
    rp2040.idleOtherCore();

    Serial.begin(115200);
//    while (!Serial);
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
//int lastTime = millis();
void loop()
{

//    int startMicros = millis();
    mqttService();
//    Serial.println("\n" + String(millis() - startMicros));
    core0Devices.service();

    if (actionCore0Msgs.rear > 0) {
        while (actionCore0Msgs.rear > 0) {
            Action *pAction = actionCore0Msgs.queueDequeue();
            core0Devices.process(pAction);
            delete pAction;
        }
    }

    processSensors();


//if ((millis() - lastTime) > 10) {
//    Serial.println("\nTime: " + String(millis() - lastTime));
//}
//    lastTime = millis();
}

void processSensors() {
    for (auto &sensor : core0Devices.motionSensors) {
        if (sensor->check() != NOT_CHANGED) {
            Logger::printlnCore0(String(sensor->id) + ": " + String(sensor->currentState));
            publishSensor(sensor->id, sensor->currentState);
        }
    }
}

void loop1() {
//    ws2812fx_v1.service();
    core1Devices.service();

    if (mutex_enter_timeout_ms(&actionCore1MsgMutex, 0)) {

        while (actionCore1Msgs.rear > 0) {
//            Logger::printlnCore1("Start Processing: ");
            Action* pAction = actionCore1Msgs.queueDequeue();
            core1Devices.process(pAction);
            delete pAction;
        }
//        if (actionCore1Msgs.rear > 0) {
//            Action* pAction = actionCore1Msgs.queueDequeue();
//            core1Devices.process(pAction);
//            delete pAction;
//        }

//        if (!actionCore1Msgs.empty()) {
//
//            Action *pAction = actionCore1Msgs.front();
//            core1Devices.process(pAction);
//
//            delete pAction;
//            actionCore1Msgs.erase(actionCore1Msgs.begin());
//        }
        mutex_exit(&actionCore1MsgMutex);
    }


}