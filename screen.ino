void screenSetUp() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;); // Don't proceed, loop forever
    }
    //Adafruit_GFX
    display.display();
    delay(1000);
    display.clearDisplay();
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(WHITE);        // Draw white text
    display.setTextWrap(false);
}

String lines[8];
int firstLineIndex = -7;
int lastLineIndex = 0;
bool endlLock = true;

void println(String line) {
    if (!endlLock) {
        lastLineIndex++;
        lastLineIndex %= 8;
        firstLineIndex++;
        firstLineIndex %= 8;
    }
    endlLock = true;
    lines[lastLineIndex] = line;
    display.clearDisplay();
    display.setCursor(0, 0);
    for (int i = 0; i < 8; i++) {
        display.println(lines[(max(firstLineIndex, 0) + i) % 8]);
    }
    display.display();
    lastLineIndex++;
    lastLineIndex %= 8;
    firstLineIndex++;
    firstLineIndex %= 8;
}

void print(String line) {
    endlLock = false;
    lines[lastLineIndex] += line;
    display.clearDisplay();
    display.setCursor(0, 0);
    for (int i = 0; i < 8; i++) {
        display.println(lines[(max(firstLineIndex, 0) + i) % 8]);
    }
    display.display();
}