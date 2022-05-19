//
// Created by Dawid on 26.02.2022.
//

#include "communication.h"

#define ETH_RESET_PIN 20

const char *ACTION_TOPIC = STR(MQTT_ACTION_TOPIC);
const char *SYNC_TOPIC = STR(MQTT_SYNC_TOPIC);
const char *DEVICE_NAME_STR = STR(DEVICE_NAME);
const char *TRIGGER_TOPIC = STR(MQTT_TRIGGER_TOPIC);

EthernetClient ethClient;
PubSubClient client(ethClient);

mutex_t actionCore1MsgMutex;
Queue actionCore0Msgs(256);
Queue actionCore1Msgs(256);

Action *deserializeAction(const JsonObject &action);

void setupMqtt() {
    Logger::printlnCore0("Start MQTTClient init ");
    Logger::printCore0(DEVICE_NAME_STR);

    pinMode(ETH_RESET_PIN, OUTPUT);
    digitalWrite(ETH_RESET_PIN, HIGH);

    pinMode(USE_THIS_SS_PIN, OUTPUT);
    digitalWrite(USE_THIS_SS_PIN, HIGH);
    Ethernet.init(USE_THIS_SS_PIN);

    IPAddress ip;
    ip.fromString(STR(DEVICE_IP));
    const char *macStr = STR(DEVICE_MAC);

    byte mac[6];
    parseBytes(macStr, '-', mac, 6, 16);

    Ethernet.begin(mac);

    Logger::printlnCore0(F("Connected! IP address: "));
    Logger::printCore0(Ethernet.localIP());

    client.setServer(STR(MQTT_SERVER), 1883);
    client.setCallback(actionTopicCallback);
    sleep_ms(2000);

    reconnectMqtt();

    mutex_init(&actionCore1MsgMutex);
}

void reconnectMqtt() {
    while (!client.connected()) {
        Logger::printlnCore0("Attempting MQTT connection to ");
        Logger::printCore0(STR(MQTT_SERVER));


        if (client.connect(DEVICE_NAME_STR)) {
            Logger::printlnCore0("...connected");
            String data = "Hello from MQTTClient_SSL on " + String(BOARD_NAME);
            String topic = ACTION_TOPIC + String("/") + DEVICE_NAME_STR;
            client.publish(topic.c_str(), data.c_str());
            client.subscribe(topic.c_str());
        } else {
            Logger::printlnCore0("...failed, rc=" + String(client.state()) + " try again in 2 seconds ");
            sleep_ms(2000);
        }
    }
}

void resetENC() {
    digitalWrite(ETH_RESET_PIN, LOW);
    sleep_ms(500);
    digitalWrite(ETH_RESET_PIN, HIGH);
}

void actionTopicCallback(char *topic, byte *payload, unsigned int length) {
    try {
//        int startMicros = millis();
        Logger::printlnCore0("Total allocated space (uordblks):      " + String(mallinfo().uordblks));

        Logger::printlnCore0("Message arrived [");
        Logger::print(topic);
        Logger::print("] ");
        StaticJsonDocument<2048> doc;


        DeserializationError error = deserializeJson(doc, payload, length);
        if (error) {
            Logger::printlnCore0(F("deserializeJson() failed: "));
            Logger::print(error.f_str());
            return;
        }

        const JsonArray &actions = doc.getMember("actions").as<JsonArray>();
        Logger::printlnCore0("actions: " + String(actions.size()));

        if (mutex_enter_timeout_ms(&actionCore1MsgMutex, 100000)) {
            Logger::printlnCore0("Start Adding action to queue");
            for (JsonObject action : actions) {
                Action *pAction = deserializeAction(action);

                if (pAction->ioType == "NEO") {
                    actionCore1Msgs.queueEnqueue(pAction);
//                    addToCore1Mutex(pAction);
                } else {
                    actionCore0Msgs.queueEnqueue(pAction);
                }
            }
            mutex_exit(&actionCore1MsgMutex);
        }

//        Logger::printlnCore0("Wait fot Core 1 change processing finish");
//        if (mutex_enter_timeout_ms(&actionCore1MsgMutex, 1000)) {
//            Logger::printlnCore0("Finish waiting fot Core 1 change processing finish");
//            mutex_exit(&actionCore1MsgMutex);
//        }


        Logger::printlnCore0("Total allocated space (uordblks):      " + String(mallinfo().uordblks));
//        Serial.println("\n" + String(millis() - startMicros));
    } catch (...) {
        Logger::printlnCore0("Error on parse action msg");
    }

}

Action *deserializeAction(const JsonObject &action) {
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
        ioColor.ww = color.getMember("ww").as<uint8_t>();
    }

    auto *pAction = new Action(ioId, ioType, ioAction, ioColor, brightness, animationId, speed, time, stepTime);
    return pAction;
}

void addToCore1Mutex(Action *action) {
    if (mutex_enter_timeout_ms(&actionCore1MsgMutex, 100000)) {
        actionCore1Msgs.queueEnqueue(action);
        mutex_exit(&actionCore1MsgMutex);
    }
}

unsigned long mqttPubTime = 0;

void mqttService() {
    if (!client.connected()) {
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

void parseBytes(const char *str, char sep, byte *bytes, int maxBytes, int base) {
    for (int i = 0; i < maxBytes; i++) {
        bytes[i] = strtoul(str, NULL, base);
        str = strchr(str, sep);
        if (str == NULL || *str == '\0') {
            break;
        }
        str++;
    }
}

void publishSensor(const String &id, bool state) {
    if (client.connected()) {
        StaticJsonDocument<64> doc;

        doc["id"] = id;
        Logger::printlnCore0("state " + String(state));
        doc["state"] = state;

        char buffer[64];
        serializeJson(doc, buffer);

        client.publish(TRIGGER_TOPIC, buffer);
    } else {
        reconnectMqtt();
    }
}