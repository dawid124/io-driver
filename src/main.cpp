#include <module/communication.h>
#include <Arduino.h>
#include <objects/iot/IotDevices.h>
#include <arduino-timer.h>
#include <objects/change/RgbwChange.h>

#define STATUS_LED_INTERVAL   2000

IotDevices devices;
Timer<16, millis, RgbwChange*> timer0;

#define CHANGE_MILLIS 30

void setup()
{
    rp2040.idleOtherCore();

    Serial.begin(115200);
//    while (!Serial);

    Serial.print("\nStart Initializing ");
    Serial.println(STR(DEVICE_NAME));

    sleep_ms(1000);
    setupMqtt();
    sleep_ms(1000);
    devices.init();
    devices.setupDevices();
    devices.printDevices();

    pinMode(LED_BUILTIN, OUTPUT);

    pinMode(28, INPUT);
    digitalWrite(28, LOW);
    pinMode(27, INPUT);
    digitalWrite(27, LOW);
    pinMode(26, INPUT);
    digitalWrite(26, LOW);
    pinMode(22, INPUT);
    digitalWrite(22, LOW);
    pinMode(21, INPUT);
    digitalWrite(21, LOW);


//    while (true) {
//        PinStatus read = digitalRead(28);
//
//        if (read == HIGH) {
//            Serial.println("HIGH 28");
//        }
//
//        read = digitalRead(27);
//
//        if (read == HIGH) {
//            Serial.println("HIGH 27");
//        }
//
//        read = digitalRead(26);
//
//        if (read == HIGH) {
//            Serial.println("HIGH 26");
//        }
//
//        read = digitalRead(22);
//
//        if (read == HIGH) {
//            Serial.println("HIGH 22");
//        }
//
//        read = digitalRead(21);
//
//        if (read == HIGH) {
//            Serial.println("HIGH 21");
//        }
//    }


    rp2040.resumeOtherCore();
    Serial.println("configuration finished");
}

bool rgbwChange(RgbwChange *change) {
    return change->nextStep();
}

void processRgbwAction(const Action &action, RgbwLight& light) {

    Color color = action.color;
    int stepTime = CHANGE_MILLIS;
    if (action.stepTime > 0) {
        stepTime = action.stepTime;
    }
    int steps = (int) action.time / stepTime;
    timer0.every(stepTime, rgbwChange, new RgbwChange(color, steps, &light));
}

void processActionCore0(const Action &action) {
    if (action.ioType == "RGBW") {
        RgbwLight *light = devices.getRgbwById(action.ioId);
        processRgbwAction(action, *light);
    } else if (action.ioType == "RGB") {

    } else if (action.ioType == "BLIND") {

    } else if (action.ioType == "HEATING") {

    }
}

void loop()
{
    mqttService();
    timer0.tick();

    if (!actionCore0Msgs.empty()) {
        for (auto &action : actionCore0Msgs) {
            processActionCore0(action);
        }
        actionCore0Msgs.erase(actionCore0Msgs.begin());
    }
}


unsigned long change_color_timeout = 0;
bool state = false;

void picoLight();

void processActionCore1(const Action &action) {
    NeoLight *neo = devices.getNeoById(action.ioId);
    if (action.action == "ON") {
        neo->ws2812fx.start();
    } else if (action.action == "OFF") {
        neo->ws2812fx.stop();
    } else if (action.action == "CHANGE") {
        neo->ws2812fx.setBrightness(action.brightness);
        neo->ws2812fx.setMode(action.animationId);
        neo->ws2812fx.setSpeed(action.speed);
        neo->ws2812fx.setColor(action.color.r, action.color.g, action.color.b, action.color.w);
        if (!neo->ws2812fx.isRunning()) {
            neo->ws2812fx.start();
        }
    }
}

void loop1()
{
    picoLight();

    bool ms = mutex_enter_timeout_ms(&actionCore1MsgMutex, 0);
    if (ms) {
        if (!actionCore1Msgs.empty()) {
            for (auto &action : actionCore1Msgs) {
                processActionCore1(action);
            }
            actionCore1Msgs.erase(actionCore1Msgs.begin());
        }
        mutex_exit(&actionCore1MsgMutex);
    }

    for (auto const& neo: devices.neo) {
//        Serial.println(String(neo->ws2812fx.isRunning()));
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
