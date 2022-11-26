void connectWiFi() {
    println("Connecting to "+ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), pwd.c_str());
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(F("."));
        print(".");
    }
    Serial.println();
    Serial.println("WiFi connected!");
    println("WiFi connected!");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("Way IP: ");
    Serial.println(WiFi.gatewayIP().toString());
    Serial.println(WiFi.hostname());
}