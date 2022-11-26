void waterSetup() {
    pinMode(12, OUTPUT);
    mqttCallbackTasks.add([]() {
        waterMqttCallback();
    });
}

void waterMqttCallback() {
    String cmd = cmdData.get("cmd");
    if (cmd == "waterStart") {
        digitalWrite(12, HIGH);
        MQTT.publish(("from-" + sn + "-" + sk).c_str(), "已开始浇水！");
    } else if (cmd == "waterStop") {
        digitalWrite(12, LOW);
        MQTT.publish(("from-" + sn + "-" + sk).c_str(), "已停止浇水！");
    }
}