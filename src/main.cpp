#include <module/communication.h>
#include <Arduino.h>
#include <objects/iot/stack/Core0Devices.h>

#include <objects/change/RgbwChange.h>
#include <objects/change/NeoChange.h>
#include <objects/iot/stack/Core1Devices.h>

Core0Devices core0Devices;
Core1Devices core1Devices;

void processSensors();

void init() {
    Logger::printlnCore0("\nStart Initializing ");
    Logger::printlnCore0(STR(DEVICE_NAME));

    sleep_ms(1000);
    setupMqtt();

    sleep_ms(1000);
    core0Devices.init();

    core0Devices.setupDevices();
    core1Devices.setupDevices();

    pinMode(LED_BUILTIN, OUTPUT);
}
void setup()
{
    rp2040.idleOtherCore();

    Serial.begin(115200);
//    while (!Serial);

    init();

    rp2040.resumeOtherCore();
    Serial.println("configuration finished");
}

void loop()
{
//    int startMicros = millis();
    mqttService();
    core0Devices.service();

    if (actionCore0Msgs.rear > 0) {
        Action *pAction = actionCore0Msgs.queueDequeue();
        core0Devices.process(pAction);
        delete pAction;
    }

    processSensors();
//    Serial.println("\n" + String(millis() - startMicros));
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