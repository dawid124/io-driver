//
// Created by Dawid on 26.02.2022.
//

#include "communication.h"

#define ETH_RESET_PIN 20

const char* ACTION_TOPIC = STR(MQTT_ACTION_TOPIC);

EthernetClient  ethClient;
PubSubClient    client(ethClient);

mutex_t actionCore1MsgMutex;
std::vector<Action> actionCore0Msgs;
std::vector<Action> actionCore1Msgs;

void setupMqtt() {
    Serial.print("\nStart MQTTClient init ");

    pinMode(ETH_RESET_PIN, OUTPUT);
    digitalWrite(ETH_RESET_PIN, HIGH);

    pinMode(USE_THIS_SS_PIN, OUTPUT);
    digitalWrite(USE_THIS_SS_PIN, HIGH);
    Ethernet.init(USE_THIS_SS_PIN);

    IPAddress ip;
    ip.fromString(STR(DEVICE_IP));
    const char* macStr = STR(DEVICE_MAC);

    byte mac[6];
    parseBytes(macStr, '-', mac, 6, 16);

    Ethernet.begin(mac);

    Serial.print(F("Connected! IP address: "));
    Serial.println(Ethernet.localIP());

    client.setServer(STR(MQTT_SERVER), 1883);
    client.setCallback(actionTopicCallback);
    sleep_ms(2000);

    reconnectMqtt();

    mutex_init(&actionCore1MsgMutex);
}

void reconnectMqtt()
{
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection to ");
        Serial.println(STR(MQTT_SERVER));

        if (client.connect(STR(DEVICE_NAME))) {
            Serial.println("...connected");
            String data = "Hello from MQTTClient_SSL on " + String(BOARD_NAME);
            client.publish(ACTION_TOPIC, data.c_str());
            client.subscribe(ACTION_TOPIC);
        } else {
            Serial.println("...failed, rc=" + String(client.state()) + " try again in 2 seconds ");
            sleep_ms(2000);
        }
    }
}

void resetENC() {
    digitalWrite(ETH_RESET_PIN, LOW);
    sleep_ms(500);
    digitalWrite(ETH_RESET_PIN, HIGH);
}

Action deserializeAction(const JsonObject &action);

void actionTopicCallback(char* topic, byte* payload, unsigned int length) {
    try {
        Serial.print("Message arrived [");
        Serial.print(topic);
        Serial.print("] ");
//
//        for (unsigned int i = 0; i < length; i++)
//        {
//            Serial.print((char)payload[i]);
//        }
//
//        Serial.println();

        StaticJsonDocument<1024> doc;


        DeserializationError error =  deserializeJson(doc, payload, length);
        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return;
        }

        const JsonArray &actions = doc.getMember("actions").as<JsonArray>();
        Serial.println("actions: " + String(actions.size()));
        for (JsonObject action : actions) {
            Action actionObj = deserializeAction(action);

            if (actionObj.ioType == "NEO") {
                addToCore1Mutex(actionObj);
            } else  {
                actionCore0Msgs.push_back(actionObj);
            }
        }
    } catch (...) {
        Serial.println("Error on parse action msg");
    }

}

Action deserializeAction(const JsonObject &action) {
    const String &ioId = action.getMember("ioId").as<String>();
    const String &ioType = action.getMember("ioType").as<String>();
    const String &ioAction = action.getMember("action").as<String>();
    const uint8_t &brightness = action.getMember("brightness").as<uint8_t>();
    const int &animationId = action.getMember("animationId").as<int>();
    const int &speed = action.getMember("speed").as<int>();
    const int &time = action.getMember("time").as<int>();
    const int &stepTime = action.getMember("stepTime").as<int>();

    Color ioColor;
    const JsonObject &color = action.getMember("color").as<JsonObject>();
    if (!color.isNull()) {
        ioColor.r = color.getMember("r").as<uint8_t>();
        ioColor.g = color.getMember("g").as<uint8_t>();
        ioColor.b = color.getMember("b").as<uint8_t>();
        ioColor.w = color.getMember("w").as<uint8_t>();
    }

    Action actionObj(ioId, ioType, ioAction, ioColor, brightness, animationId, speed, time, stepTime);
    return actionObj;
}

void addToCore1Mutex(const Action& action) {
    if (mutex_enter_timeout_ms(&actionCore1MsgMutex, 100000)) {
        actionCore1Msgs.push_back(action);
        mutex_exit(&actionCore1MsgMutex);
    }
}

unsigned long mqttPubTime = 0;
void mqttService() {
    if (!client.connected())
    {
        reconnectMqtt();
    }

    client.loop();

//    if ((millis() > mqttPubTime) || (mqttPubTime == 0))
//    {
//        testMillis = millis();
//        client.publish(ACTION_TOPIC,"{\"actions\": [ {\"ioId\": \"io-1\", \"type\": \"core1\"} ]}");
//        mqttPubTime = millis() + 4000;
//    }
}

void parseBytes(const char* str, char sep, byte* bytes, int maxBytes, int base) {
    for (int i = 0; i < maxBytes; i++) {
        bytes[i] = strtoul(str, NULL, base);
        str = strchr(str, sep);
        if (str == NULL || *str == '\0') {
            break;
        }
        str++;
    }
}
