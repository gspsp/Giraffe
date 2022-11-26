VKML dhtData;

void dhtSetup() {
    dht.begin();
    dhtData.set("report", "dht");
    slowLoopTasks.add([]() {
        dhtLoop();
    });
}

void dhtLoop() {
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
    }

    if (dhtData.get("humidity") != String(h) && dhtData.get("temperature") != String(t)) {
        dhtData.set("humidity", String(h));
        dhtData.set("temperature", String(t));
        println(String("DHT-") + F("H:") + String(h) + "," + F("T:") + String(t));
        Serial.println(dhtData.text);
        MQTT.publish(("from-" + sn + "-" + sk).c_str(), dhtData.text.c_str());
    }
}