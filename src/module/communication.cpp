//
// Created by Dawid on 26.02.2022.
//

#include "communication.h"

const char* mqttServer = "192.168.0.100";
const char* TOPIC     = "sensors";
const IPAddress IP = IPAddress(192, 168, 0, 101);
const IPAddress DNS = IPAddress(192, 168, 0, 1);
byte MAC[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

EthernetClient  ethClient;
PubSubClient    client(mqttServer, 1883, ethClient);

void setupMqtt() {
    Serial.print("\nStart MQTTClient init ");

    pinMode(USE_THIS_SS_PIN, OUTPUT);
    digitalWrite(USE_THIS_SS_PIN, HIGH);
    Ethernet.init(USE_THIS_SS_PIN);

    Ethernet.begin(MAC, IP, DNS);

    Serial.print(F("Connected! IP address: "));
    Serial.println(Ethernet.localIP());

    client.setServer(mqttServer, 1883);
    client.setCallback(actionTopicCallback);
    sleep_ms(2000);

    reconnectMqtt();
}

void reconnectMqtt()
{
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection to ");
        Serial.println(mqttServer);

        if (client.connect(STR(DEVICE_NAME)))
        {
            Serial.println("...connected");
            String data = "Hello from MQTTClient_SSL on " + String(BOARD_NAME);
            client.publish(TOPIC, data.c_str());
            client.subscribe(TOPIC);
        }
        else
        {
            Serial.print("...failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            sleep_ms(5000);
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

    aJsonObject* root = aJson.parse((char*) payload);
    aJsonObject *value1 = aJson.getObjectItem(root, "value1");

    Serial.println(value1->valueint);
}

void mqttService() {
    if (!client.connected())
    {
        reconnectMqtt();
    }

    client.loop();
}
