
#include <Si4703_Breakout.h>
#include <Wire.h>

int resetPin = 2;
int SDIO = A4;
int SCLK = A5;

Si4703_Breakout radio(resetPin, SDIO, SCLK);


void setup() {
  Serial.begin(9600);
  
  // turn on radio
  // (creates connection with device)
  Serial.println("Turning radio on...");
  radio.powerOn();

  // set a channel
  // should be a three-digit integer
  // for the station's frequency
  // (ie 93.7FM = 937)
  Serial.println("Setting initial station...");
  radio.setChannel(939);

  // set the volume (int from 0-15)
  Serial.println("Volume up...");
  radio.setVolume(15);

//  // play the radio for 5 seconds
//  Serial.println("Playing for 10 seconds...");
//  delay(10000);
//
//  // seek channels up five times,
//  // listen for a second
//  Serial.println("Seeking to next 5 stations...");
//  for (int i=0; i<5; i++) {
//    Serial.println("- seeking up...");
//    radio.seekUp();
//    delay(10000);
//  }
//
//  // fade out volume programatically
//  Serial.println("Fading out volume...");
//  for (int i=15; i>0; i-=1) {
//    radio.setVolume(i);
//    delay(200);
//  }
//  Serial.println("DONE");
}

void loop() {
  radio.setVolume(15);
  delay(5000);
  radio.setVolume(0);
  delay(5000);
}

