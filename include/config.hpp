#pragma once

//#define ENABLE_TEST_BROKER

const char* ssid = "<ssid>";
const char* password = "<pass>";

#ifdef ENABLE_TEST_BROKER
//Broker without auth
const char* mqtt_server = "10.1.x.x";
#else
//Broker with auth
const char* mqtt_server = "10.1.x.x";
const char* broker_username = "username";
const char* broker_password = "password";
#endif
