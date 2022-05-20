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

Core0Devices core0Devices;
Core1Devices core1Devices;

DriverState driverState;

Action *deserializeAction(const JsonObject &action);

void processActions(const JsonArray &actions);

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

    driverState = READY_FOR_CONFIG;
    mutex_init(&actionCore1MsgMutex);
}

void reconnectMqtt() {
    while (!client.connected()) {
        Logger::printlnCore0("Attempting MQTT connection to ");
        Logger::printCore0(STR(MQTT_SERVER));

        if (client.connect(DEVICE_NAME_STR)) {
            Logger::printlnCore0("...connected");
            String actionTopic = ACTION_TOPIC + String("/") + DEVICE_NAME_STR;
            client.subscribe(actionTopic.c_str());
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
        Logger::printlnCore0("Total allocated space (uordblks):      " + String(mallinfo().uordblks));
        Logger::printlnCore0("Message arrived [");
        Logger::print(topic);
        Logger::print("] ");
        StaticJsonDocument<4048> doc;


        DeserializationError error = deserializeJson(doc, payload, length);
        if (error) {
            Logger::printlnCore0(F("deserializeJson() failed: "));
            Logger::print(error.f_str());
            return;
        }

        Logger::printlnCore0("Deserialize JSON completed");
        if (doc.containsKey("actions")) {
            const JsonArray &actions = doc.getMember("actions").as<JsonArray>();
            processActions(actions);
        } else if (doc.containsKey("configs")) {
            const JsonArray &config = doc.getMember("configs").as<JsonArray>();
            processConfig(config);
        }


        Logger::printlnCore0("Total allocated space (uordblks):      " + String(mallinfo().uordblks));
    } catch (...) {
        Logger::printlnCore0("Error on parse action msg");
    }

}

void processActions(const JsonArray &actions) {
    Logger::printlnCore0("actions: " + String(actions.size()));

    if (mutex_enter_timeout_ms(&actionCore1MsgMutex, 100000)) {
        Logger::printlnCore0("Start Adding action to queue");
        for (JsonObject action : actions) {
            Action *pAction = deserializeAction(action);

            if (pAction->ioType == "NEO") {
                actionCore1Msgs.queueEnqueue(pAction);
            } else {
                actionCore0Msgs.queueEnqueue(pAction);
            }
        }
        mutex_exit(&actionCore1MsgMutex);
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

void processConfig(const JsonArray &actions) {
    Logger::printlnCore0("configs: " + String(actions.size()));

    for (JsonObject action : actions) {

        const String &ioType = action.getMember("ioType").as<String>();
        const String &id = action.getMember("id").as<String>();
        if (ioType == "NEO") {
            const String &neoType = action.getMember("neoType").as<String>();
            const bool &reverse = action.getMember("neoType").as<bool>();
            const int &ledCount = action.getMember("ledCount").as<int>();
            const int &pin = action.getMember("pin").as<int>();
            int neoConfig = NEO_GRB + NEO_KHZ800;
            if (neoType == "NEO_GRBW") neoConfig = NEO_RGBW + NEO_KHZ800;

            core1Devices.neo.push_back(new NeoLight(id, pin, ledCount, neoConfig, reverse));
        } else if (ioType == "RGBW") {
            const String &location = action.getMember("location").as<String>();
            SX1509* io;
            if (location == "EXPANDER_A") io = &core0Devices.ioExpanderA;
            else if (location == "EXPANDER_B") io = &core0Devices.ioExpanderB;
            else if (location == "EXPANDER_C") io = &core0Devices.ioExpanderC;
            else if (location == "EXPANDER_D") io = &core0Devices.ioExpanderD;

            const int &pinR = action.getMember("pinR").as<int>();
            const int &pinG = action.getMember("pinG").as<int>();
            const int &pinB = action.getMember("pinB").as<int>();
            const int &pinW = action.getMember("pinW").as<int>();

            core0Devices.rgbw.push_back(new RgbwLight(id, io, pinR, pinG, pinB, pinW));
        } else if (ioType == "MOTION_SENSOR") {
            const int &pin = action.getMember("pin").as<int>();
            core0Devices.motionSensors.push_back(new MotionSensor(id, pin));
        }
    }
    driverState = WORKING;

    core0Devices.printDevices();
    core1Devices.printDevices();

    String data = R"({"id":")" + String(DEVICE_NAME_STR) + R"(","type":"READY_FOR_INITIAL_STATE"})";
    client.publish(String(SYNC_TOPIC).c_str(), data.c_str());

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

void initDevices() {
    core0Devices.init();
    String data = R"({"id":")" + String(DEVICE_NAME_STR) + R"(","type":"READY_FOR_CONFIG"})";
    Serial.println(data);
    client.publish(String(SYNC_TOPIC).c_str(), data.c_str());
}