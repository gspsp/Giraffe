#include "conf.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "vkml.h"
#include <Series.h>
#include <Ticker.h>
#include <DNSServer.h>
#include <WiFiUdp.h>
#include <Servo.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

DHT dht(13, DHT11);//D7引脚

Adafruit_SSD1306 display(128, 64, &Wire, -1);

Servo servo;

VKML vkml;

WiFiUDP Udp;
char udpPacketBuffer[UDP_TX_PACKET_MAX_SIZE + 1];

DNSServer dnsServer;

Ticker ticker;

CONF conf;
WiFiClient wifiClient;
PubSubClient MQTT(wifiClient);

void (*resetFunc)(void) = 0;

String ssid = conf.get("ssid"), pwd = conf.get("pwd"), sn = conf.get("sn"), sk = conf.get("sk");

Series fastLoopTasks;
Series slowLoopTasks;
Series mqttCallbackTasks;
bool slowLoopReady = false;

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println();
    screenSetUp();

    conf.set("status", "already");
    conf.set("ssid", "4001");
    conf.set("pwd", "123456789");
    conf.set("sn", "4001");
    conf.set("sk", "light");


    //检测是否被设置过
    if (conf.get("status") == "") {
        WiFi.mode(WIFI_AP_STA);
        //没有配网，开启热点
        Serial.print("Setting soft-AP ... ");
        Serial.println(WiFi.softAP("rabbit-chameleon", "") ? "Ready" : "Failed!");
        Serial.println("IP address: ");
        Serial.println(WiFi.softAPIP());
        //开启dns服务器
        dnsServer.start(53, "rabbit.com", WiFi.softAPIP());
        fastLoopTasks.add([]() {
            dnsServer.processNextRequest();
        });
        //开启udp
        Udp.begin(8888);
        fastLoopTasks.add([]() {
            udpServerLoop();
        });
    } else {
        //已经配网连接wifi
        connectWiFi();
        //开启mqtt
        connectMQTT();
        slowLoopTasks.add([]() {
            //定时检测wifi连接状态
            if (WiFi.status() != WL_CONNECTED) {
                connectWiFi();
            }
            //定时检测mqtt连接状态
            if (!MQTT.connected()) {
                connectMQTT();
            }
        });
        servoSetup();
        dhtSetup();
        smeSetup();
        waterSetup();
    }
    pinMode(16, INPUT);
    ticker.attach(1, []() {
        slowLoopReady = true;
    });
}

void loop() {
    fastLoopTasks.run();
    if (slowLoopReady) {
        //slowLoop
        slowLoopTasks.run();
        slowLoopReady = false;
    }
}