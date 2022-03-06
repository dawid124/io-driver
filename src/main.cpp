#include <module/communication.h>
#include <Arduino.h>
#include <objects/iot/IotDevices.h>

#define STATUS_LED_INTERVAL   2000

IotDevices devices;

void setup()
{
    rp2040.idleOtherCore();

    Serial.begin(115200);
    while (!Serial);

    Serial.print("\nStart Initializing ");
    Serial.println(STR(DEVICE_NAME));

    sleep_ms(1000);
    setupMqtt();
    devices.init();
    devices.setupDevices();
    devices.printDevices();

    devices.rgbw.at(0)->on(255,0,0,0);
    
    Serial.println(devices.neo.at(0)->id);

    pinMode(LED_BUILTIN, OUTPUT);


    rp2040.resumeOtherCore();
    Serial.println("configuration finished");
}



void loop()
{
//    sleep_ms(2000);
//    devices.rgbw.at(0)->on(255,0,0,0);
//    sleep_ms(2000);
//    devices.rgbw.at(0)->on(0,0,0,0);
    mqttService();

    if (!actionCore0Msgs.empty()) {
        for (auto &action : actionCore0Msgs) {
            Serial.println("id: " + action.ioId);
            Serial.println("type: " + action.ioType);
            Serial.println("color r: " + String(action.color.r));
        }
        actionCore0Msgs.erase(actionCore0Msgs.begin());
    }
}


unsigned long change_color_timeout = 0;
bool state = false;

void picoLight();

void loop1()
{
    picoLight();

    bool ms = mutex_enter_timeout_ms(&actionCore1MsgMutex, 0);
    if (ms) {
        if (!actionCore1Msgs.empty()) {
            for (auto &action : actionCore1Msgs) {
                Serial.println("id: " + action.ioId);
                Serial.println("type: " + action.ioType);
            }
            actionCore1Msgs.erase(actionCore1Msgs.begin());
        }
        mutex_exit(&actionCore1MsgMutex);
    }

    for (auto const& neo: devices.neo) {
        neo->service();
    }
}

void picoLight() {
    if ((millis() > change_color_timeout) || (change_color_timeout == 0))
    {
        state = !state;
        digitalWrite(LED_BUILTIN, state);
        change_color_timeout = millis() + STATUS_LED_INTERVAL;
    }
}
