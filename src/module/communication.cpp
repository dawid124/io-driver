//
// Created by Dawid on 26.02.2022.
//

#include "communication.h"

const char* ACTION_TOPIC = STR(MQTT_ACTION_TOPIC);

EthernetClient  ethClient;
PubSubClient    client(STR(MQTT_SERVER), 1883, ethClient);

mutex_t actionCore1MsgMutex;
std::vector<Action> actionCore0Msgs;
std::vector<Action> actionCore1Msgs;

void setupMqtt() {
    Serial.print("\nStart MQTTClient init ");

    pinMode(USE_THIS_SS_PIN, OUTPUT);
    digitalWrite(USE_THIS_SS_PIN, HIGH);
    Ethernet.init(USE_THIS_SS_PIN);

    IPAddress ip;
    ip.fromString(STR(DEVICE_IP));
    const char* macStr = STR(DEVICE_MAC);

    byte mac[6];
    parseBytes(macStr, '-', mac, 6, 16);

    Ethernet.begin(mac, ip);

    Serial.print(F("Connected! IP address: "));
    Serial.println(Ethernet.localIP());

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

        if (client.connect(STR(DEVICE_NAME)))
        {
            Serial.println("...connected");
            String data = "Hello from MQTTClient_SSL on " + String(BOARD_NAME);
            client.publish(ACTION_TOPIC, data.c_str());
            client.subscribe(ACTION_TOPIC);
        }
        else
        {
            Serial.print("...failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 2 seconds");
            sleep_ms(2000);
        }
    }
}

void actionTopicCallback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");

    for (unsigned int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }

    Serial.println();

    StaticJsonDocument<256> doc;


    DeserializationError error =  deserializeJson(doc, payload, length);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }

    const JsonArray &actions = doc.getMember("actions").as<JsonArray>();
    for (JsonObject action : actions) {
        const String &ioId = action.getMember("ioId").as<String>();
        const String &type = action.getMember("type").as<String>();

        Action actionObj(ioId, type);
        if (type == "core0") {
            actionCore0Msgs.push_back(actionObj);
        } else if (type == "core1") {
            addToCore1Mutex(actionObj);
        }
    }
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

    if ((millis() > mqttPubTime) || (mqttPubTime == 0))
    {
//        client.publish(ACTION_TOPIC,"{\"actions\": [ {\"ioId\": \"io-1\", \"type\": \"type-1\"} ]}");
        mqttPubTime = millis() + 2000;
    }
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
