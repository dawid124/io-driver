#include <module/communication.h>
#include <Arduino.h>
#include <WS2812FX.h>

#define LED_PIN_C2     22
#define LED_COUNT_P1   1
#define STATUS_LED_INTERVAL   2000

WS2812FX ws2812fx_c2 = WS2812FX(LED_COUNT_P1, LED_PIN_C2, NEO_RGBW + NEO_KHZ800);


void setupNeo() {
    Serial.println("Start setup Neo");
    ws2812fx_c2.begin();
    ws2812fx_c2.setBrightness(10);
    ws2812fx_c2.setSpeed(1000);
    ws2812fx_c2.setColor(0,0,255,0);
    ws2812fx_c2.setMode(FX_MODE_RAINBOW_CYCLE);
    ws2812fx_c2.start();
    Serial.println("Finish setup Neo");
}


void setup()
{
    rp2040.idleOtherCore();

    Serial.begin(115200);
    while (!Serial);

    Serial.print("\nStart Initializing ");
    Serial.println(STR(DEVICE_NAME));

    setupMqtt();
    setupNeo();
    pinMode(LED_BUILTIN, OUTPUT);

    rp2040.resumeOtherCore();
}



void loop()
{
    mqttService();
}


unsigned long change_color_timeout = 0;
bool state = false;

void loop1()
{
    bool ms = mutex_enter_timeout_ms(&actionCore1MsgMutex, 0);
    if (!ms) {
        Serial.println("loop1 - mutex lock");
    }
    if (ms) {
        if (!actionCore1Msgs.empty()) {
            for (auto &action : actionCore1Msgs) {
                Serial.println("id: " + action.ioId);
                Serial.println("type: " + action.type);
            }
            actionCore1Msgs.erase(actionCore1Msgs.begin());
//            delete &message;
        }
        mutex_exit(&actionCore1MsgMutex);
    }

    if ((millis() > change_color_timeout) || (change_color_timeout == 0))
    {
        state = !state;
        digitalWrite(LED_BUILTIN, state);
        change_color_timeout = millis() + STATUS_LED_INTERVAL;
    }

    ws2812fx_c2.service();
}