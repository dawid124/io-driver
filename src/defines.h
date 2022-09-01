#ifndef defines_h
#define defines_h

#include <stdio.h>
#include <Arduino.h>
#include "rp2040.h"
#include <Wire.h>
#include <SPI.h>
#include "EthernetENC.h"
#include <PubSubClient.h>
#include "pico/stdlib.h"
#include <ArduinoJson.h>
#include "pico/multicore.h"

//#define APP_DEBUG



#include "objects/comunication/Action.h"
#define ST(A) #A

#define STR(A) ST(A)

#define USE_THIS_SS_PIN       17
#define SHIELD_TYPE           "ENC28J60 using EthernetENC Library"
#endif    //defines_h
