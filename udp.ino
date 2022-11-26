void udpServerLoop() {
    int packetSize = Udp.parsePacket();
    if (packetSize) {
        Serial.printf("Received packet of size %d from %s:%d\n    (to %s:%d, free heap = %d B)\n",
                      packetSize,
                      Udp.remoteIP().toString().c_str(), Udp.remotePort(),
                      Udp.destinationIP().toString().c_str(), Udp.localPort(),
                      ESP.getFreeHeap());

        // read the packet into udpPacketBuffer
        int n = Udp.read(udpPacketBuffer, UDP_TX_PACKET_MAX_SIZE);
        udpPacketBuffer[n] = 0;
        Serial.println("Contents:");
        Serial.println(udpPacketBuffer);
        //验证是否为配置信息
        vkml.text = udpPacketBuffer;
        if (vkml.get("cmd") == "set") {
            ssid = vkml.get("ssid"), pwd = vkml.get("pwd"), sn = vkml.get("sn"), sk = vkml.get("sk");
            //回复ok
            Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
            Udp.write("201");
            Udp.endPacket();
            //打印信息
            Serial.println(ssid);
            Serial.println(pwd);
            Serial.println(sn);
            Serial.println(sk);
            //尝试连接
            Serial.println("正在尝试连接到WiFi");
            WiFi.begin(ssid.c_str(), pwd.c_str());
            int count = 0;
            while (WiFi.status() != WL_CONNECTED) {
                count++;
                if (count > 25) {
                    Serial.println("连接失败！");
                    //回复403
                    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
                    Udp.write("403");
                    Udp.endPacket();
                    return;
                }
                delay(500);
                Serial.print(F("."));
            }
            //配置成功回复200
            Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
            Udp.write("200");
            Udp.endPacket();
            conf.set("status", "already");
            conf.set("ssid", ssid);
            conf.set("pwd", pwd);
            conf.set("sn", sn);
            conf.set("sk", sk);
            delay(1000);
            resetFunc();
        }
        // send a reply, to the IP address and port that sent us the packet we received
    }
}
