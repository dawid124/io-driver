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
    Serial.print("\nStart Initializing ");
    Serial.println(STR(DEVICE_NAME));

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
    mqttService();
    core0Devices.service();

    if (!actionCore0Msgs.empty()) {
        for (auto &action : actionCore0Msgs) {
            core0Devices.process(action);
        }
        actionCore0Msgs.erase(actionCore0Msgs.begin());
    }

    processSensors();
}

void processSensors() {
    for (auto &sensor : core0Devices.motionSensors) {
        if (sensor->check() != NOT_CHANGED) {
            Serial.println(String(sensor->id) + ": " + String(sensor->currentState));
            publishSensor(sensor->id, sensor->currentState);
        }
    }
}

void loop1() {
    core1Devices.service();

    bool ms = mutex_enter_timeout_ms(&actionCore1MsgMutex, 0);
    if (ms) {
        if (!actionCore1Msgs.empty()) {
            for (auto &action : actionCore1Msgs) {
                core1Devices.process(action);
            }
            actionCore1Msgs.erase(actionCore1Msgs.begin());
        }
        mutex_exit(&actionCore1MsgMutex);
    }
}