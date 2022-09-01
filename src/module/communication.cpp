//
// Created by Dawid on 26.02.2022.
//

#include <objects/iot/blind/Blind.h>
#include <objects/iot/switch/Switch.h>
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

int timeMillis = millis();

Action *deserializeAction(const JsonObject &action, int nowTime);

void processActions(const JsonArray &actions);

SX1509 *parseLocation(const JsonObject &action);

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
    Logger::printlnCore0("MAC: " + String(macStr));
    byte mac[6];
    parseBytes(macStr, '-', mac, 6, 16);

//    Ethernet.begin(mac, IPAddress(192, 168, 0, 121));
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
        Logger::printlnCore0("Message arrived [" + String(topic) + "] length: [" + String(length) + "]");
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
        } else if (doc.containsKey("configs") && driverState == READY_FOR_CONFIG) {
            const JsonArray &config = doc.getMember("configs").as<JsonArray>();
            processConfig(config);

            Logger::printlnCore0("Finish process config");
        }




//        Logger::printlnCore0("Total allocated space (uordblks):      " + String(mallinfo().uordblks));


    } catch (...) {
        Logger::printlnCore0("Error on parse action msg");
    }
    Serial.print("\nTime: " + String(millis() - timeMillis));
    timeMillis = millis();
}

void processActions(const JsonArray &actions) {
    Logger::printlnCore0("actions: " + String(actions.size()));

    Logger::printlnCore0("Start Adding action to queue");

    int nowTime = millis();

    for (JsonObject action : actions) {
        Action *pAction = deserializeAction(action, nowTime);

        if (core1Devices.idTypeMap[pAction->ioId] == "NEO") {
            actionCore1Msgs.queueEnqueue(pAction);
        } else {
            actionCore0Msgs.queueEnqueue(pAction);
        }
    }

}

Action *deserializeAction(const JsonObject &action, int nowTime) {
    const String &ioId = action.getMember("id").as<String>();
    auto *pAction = new Action(ioId);

    if (action.containsKey("a")) pAction->action = action.getMember("a").as<String>();
    if (action.containsKey("b")) pAction->brightness = action.getMember("b").as<uint8_t>();

    if (action.containsKey("an")) pAction->animationId = action.getMember("an").as<int>();
    if (action.containsKey("ssm")) pAction->staticSubMode = action.getMember("ssm").as<int>();
    if (action.containsKey("s")) pAction->speed = action.getMember("s").as<int>();

    if (action.containsKey("t")) pAction->time = action.getMember("t").as<int>();
    if (action.containsKey("st")) pAction->stepTime = action.getMember("st").as<int>();

    if (action.containsKey("d")) {
        int delay = action.getMember("d").as<int>();
        pAction->delay = nowTime + delay;
    } else {
        pAction->delay = 0;
    }

    if (action.containsKey("c")) {
        Color ioColor(0, 0, 0, 255);
        const String &color = action.getMember("c").as<String>();
        int r, g, b, w, ww;
        sscanf(color.c_str(), "%02x%02x%02x%02x%02x", &r, &g, &b, &w, &ww);

        ioColor.r = (uint8_t) r;
        ioColor.g = (uint8_t) g;
        ioColor.b = (uint8_t) b;
        ioColor.w = (uint8_t) w;
        ioColor.ww = (uint8_t) ww;

        pAction->color = ioColor;
    }
    return pAction;
}

void processConfig(const JsonArray &actions) {
    Logger::printlnCore0("configs: " + String(actions.size()));
    try {
        for (JsonObject action : actions) {
            const String &ioType = action.getMember("ioType").as<String>();
            const String &id = action.getMember("id").as<String>();
            Logger::printlnCore0("id: " + String(id));
            if (ioType == "NEO") {
                const String &neoType = action.getMember("neoType").as<String>();
                const bool &reverse = action.getMember("neoType").as<bool>();
                const int &ledCount = action.getMember("ledCount").as<int>();
                const int &pin = action.getMember("pin").as<int>();
                int neoConfig = NEO_GRB + NEO_KHZ800;
                if (neoType == "NEO_GRBW") neoConfig = NEO_RGBW + NEO_KHZ800;

                core1Devices.neo.push_back(new NeoLight(id, pin, ledCount, neoConfig, reverse));
                core1Devices.idTypeMap[id] = "NEO";
            } else if (ioType == "RGBW" || ioType == "CCT" || ioType == "RGBWW") {
                SX1509 *io = parseLocation(action);

                const int &pinR = action.getMember("pinR").as<int>();
                const int &pinG = action.getMember("pinG").as<int>();
                const int &pinB = action.getMember("pinB").as<int>();
                const int &pinW = action.getMember("pinW").as<int>();
                const int &pinWw = action.getMember("pinWW").as<int>();

                auto *x = new RgbwLight(id, io, pinR, pinG, pinB, pinW, pinWw);
                core0Devices.rgbw.push_back(x);
                core0Devices.idTypeMap[id] = "RGBW";
            } else if (ioType == "MOTION_SENSOR") {
                const int &pin = action.getMember("pin").as<int>();
                const int &reverse = action.getMember("reverse").as<bool>();
                core0Devices.motionSensors.push_back(new MotionSensor(id, pin, reverse));
            } else if (ioType == "BLIND") {
                const int &upPin = action.getMember("pinUp").as<int>();
                const int &downPin = action.getMember("pinDown").as<int>();

                SX1509 *io = parseLocation(action);

                core0Devices.blinds.push_back(new Blind(id, upPin, downPin, io));
                core0Devices.idTypeMap[id] = "BLIND";
            } else if (ioType == "SWITCH") {
                const int &pin = action.getMember("pin").as<int>();
                const String &type = action.getMember("type").as<String>();

                SX1509 *io = parseLocation(action);

                core0Devices.switches.push_back(new Switch(id, pin, type, io));
                core0Devices.idTypeMap[id] = "SWITCH";
            }
        }
    } catch (...) {
       Serial.printf("Error");
    }
    driverState = WORKING;

    core0Devices.printDevices();
    core1Devices.printDevices();

    String data = R"({"id":")" + String(DEVICE_NAME_STR) + R"(","type":"READY_FOR_INITIAL_STATE"})";
    client.publish(String(SYNC_TOPIC).c_str(), data.c_str());

}

SX1509 *parseLocation(const JsonObject &action) {
    const String &location = action.getMember("location").as<String>();
    SX1509 *io;
    if (location == "EXPANDER_A") io = &core0Devices.ioExpanderA;
    else if (location == "EXPANDER_B") io = &core0Devices.ioExpanderB;
    else if (location == "EXPANDER_C") io = &core0Devices.ioExpanderC;
    else if (location == "EXPANDER_D") io = &core0Devices.ioExpanderD;
    else io = nullptr;
    return io;
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
    String data = R"({"id":")" + String(DEVICE_NAME_STR) + R"(","type":"READY_FOR_CONFIG"})";
    client.publish(String(SYNC_TOPIC).c_str(), data.c_str());
}