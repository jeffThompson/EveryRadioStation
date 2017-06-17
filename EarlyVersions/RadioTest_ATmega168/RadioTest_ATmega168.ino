
#include <Si4703_Breakout.h>
#include <Wire.h>

int resetPin = 2;
int SDIO = A4;
int SCLK = A5;
int ledPin = 13;

Si4703_Breakout radio(resetPin, SDIO, SCLK);

void setup() {
  pinMode(ledPin, OUTPUT);
  radio.powerOn();
  radio.setChannel(971);
}

void loop() {
  radio.setVolume(15);
  digitalWrite(ledPin, HIGH);
  delay(1000);
  radio.setVolume(0);
  digitalWrite(ledPin, LOW);
  delay(1000);

  
}

