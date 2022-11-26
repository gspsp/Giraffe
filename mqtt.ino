VKML cmdData;
void connectMQTT() {
    MQTT.setServer("broker.galasp.cn", 1883);
    MQTT.setCallback(mqttCallback);
    if (MQTT.connect((sn + "-" + sk).c_str(), NULL, NULL)) {
        Serial.println("Connected to MQTT Broker");
        println("MQTT Connected!");
    } else {
        Serial.println("MQTT Broker connection failed");
        println("MQTT Broker connection failed");
        Serial.print(MQTT.state());
        println((String) MQTT.state());
        delay(200);
    }
    fastLoopTasks.add([]() {
        MQTT.loop();
    });
    MQTT.publish(("from-" + sn + "-" + sk).c_str(), "我他妈来啦！");
    MQTT.subscribe(("to-" + sn + "-" + sk).c_str());
}

void mqttCallback(char *topic, byte *payload, unsigned int length) {
    char message[5] = {0x00};
    for (int i = 0; i < length; i++)
        message[i] = (char) payload[i];
    message[length] = 0x00;
    cmdData.text = String(message);
    String cmd = cmdData.get("cmd");
    Serial.println(cmdData.text);
    println(cmdData.text);
    if (cmd == "reset") {

    } else if (cmd == "hello") {
        MQTT.publish(("from-" + sn + "-" + sk).c_str(), "我在！");
    } else if (cmd == "turn") {
        int angle = cmdData.get("angle").toInt();
        conf.set("angle", (String) angle);
        servo.write(angle);
        MQTT.publish(("from-" + sn + "-" + sk).c_str(), ("已转至" + String(angle) + "度！").c_str());
    }
    mqttCallbackTasks.run();
}