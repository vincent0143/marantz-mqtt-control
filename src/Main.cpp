#include "config.hpp"
#include "MarantzRc5Control.hpp"
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#define IR_TX_PIN 5 // pin to use for tx
#define DELAY_TIME 1500

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

MarantzRc5Control marantz_controller;

void setup_wifi()
{
    delay(10);

    // Connect to Wi-Fi
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    randomSeed(micros());

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void GetValuesFromCommaSeperatedString(char* payload, uint8_t length, uint8_t* out)
{
    char payloadBuffer[length];

    for (int i = 0; i < length; i++)
    {
        payloadBuffer[i] = payload[i];
    }
    
    String input = String(payloadBuffer);
    char *token;
    int index = 0;

    char charArray[input.length() + 1];
    input.toCharArray(charArray, input.length() + 1);

    // Extract tokens from the input string
    token = strtok(charArray, ",");
    while (token != NULL) {
        out[index++] = atoi(token);
        token = strtok(NULL, ",");
    }
}

void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");

    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }

    Serial.println();

    if (strncmp((char*)payload, "power_toggle", length) == 0)
    {
        marantz_controller.power_toggle();
    }
    else if(strncmp((char*)payload, "volume_up", length) == 0)
    {
        marantz_controller.volume_up();
    }
    else if(strncmp((char*)payload, "volume_down", length) == 0)
    {
        marantz_controller.volume_down();
    }
    else if(strncmp((char*)payload, "volume_mute", length) == 0)
    {
        marantz_controller.volume_mute();
    }
    else if(strncmp((char*)payload, "input_next", length) == 0)
    {
        marantz_controller.input_next();
    }
    else if(strncmp((char*)payload, "input_previous", length) == 0)
    {
        marantz_controller.input_previous();
    }
    else if(strncmp((char*)payload, "input_phono", length) == 0)
    {
        marantz_controller.input_phono();
    }
    else if(strcmp((char*)topic, "passthrough_rc5") == 0)
    {
        Serial.println("Passing through RC5");

        uint8_t outBuffer[50];
        GetValuesFromCommaSeperatedString((char*)payload, length, &outBuffer[0]);

        Serial.println(outBuffer[0]);
        Serial.println(outBuffer[1]);
        Serial.println(outBuffer[2]);

        marantz_controller.sendRC5(outBuffer[0], outBuffer[1], outBuffer[2]);
    }
    else if(strcmp((char*)topic, "passthrough_rc5x") == 0)
    {
        Serial.println("Passing through RC5X");

        uint8_t outBuffer[50];
        GetValuesFromCommaSeperatedString((char*)payload, length, outBuffer);

        Serial.println(outBuffer[0]);
        Serial.println(outBuffer[1]);
        Serial.println(outBuffer[2]);
        Serial.println(outBuffer[3]);

        marantz_controller.sendRC5_X(outBuffer[0], outBuffer[1], outBuffer[2], outBuffer[3]);
    }
}

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (client.connect(
            clientId.c_str()
#ifndef ENABLE_TEST_BROKER
            , broker_username
            , broker_password
#endif
            ))
        {
            // resubscribe
            client.subscribe("marantz-mqtt-remote");
            client.subscribe("passthrough_rc5");
            client.subscribe("passthrough_rc5x");
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void setup()
{
    Serial.begin(115200);
    pinMode(IR_TX_PIN, OUTPUT);

    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void loop()
{
    if (!client.connected())
    {
        reconnect();
    }
    client.loop();
}