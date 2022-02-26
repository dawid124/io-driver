#include <module/communication.h>
#include <WS2812FX.h>


#define LED_PIN_C2     22
#define LED_COUNT_P1   1
#define STATUS_CHECK_INTERVAL   2000

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
int i = 0;
int colorIndexT1 = 0;

uint32_t colorsT1[] = {WHITE, ULTRAWHITE, BLUE, PURPLE};
long colorsLengthT1 = (sizeof(colorsT1)/sizeof(*colorsT1));
bool state = false;
void loop1()
{

    if ((millis() > change_color_timeout) || (change_color_timeout == 0))
    {
        state = !state;
        digitalWrite(LED_BUILTIN, state);
        ws2812fx_c2.setColor(colorsT1[colorIndexT1]);
        colorIndexT1++;
        if (colorIndexT1 >= colorsLengthT1) colorIndexT1 = 0;

        change_color_timeout = millis() + STATUS_CHECK_INTERVAL;
    }
    ws2812fx_c2.service();
}