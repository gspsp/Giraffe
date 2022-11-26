VKML smeData;

void smeSetup() {
    smeData.set("report", "sme");
    slowLoopTasks.add([]() {
        smeLoop();
    });
}

void smeLoop() {
    int sensorValue = 1024 - analogRead(A0);
    String h = (String) map(sensorValue, 0, 730, 0, 100);
    if (smeData.get("humidity") != h) {
        smeData.set("humidity", h);
        println(String("SME-") + F("H:") + h);
        Serial.println(smeData.text);
        MQTT.publish(("from-" + sn + "-" + sk).c_str(), smeData.text.c_str());
    }
}