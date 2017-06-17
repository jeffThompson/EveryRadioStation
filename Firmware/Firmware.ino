
#include <Si4703_Breakout.h>
#include <Wire.h>

/*
EVERY RADIO STATION: FIRMWARE
Jeff Thompson | 2017 | jeffreythompson.org

Control software for "Every Radio Station". This project was
supported by a residency at Wave Farm.

HARDWARE
Setup for 8 radios, 8 of these setups total
+ Arduino Pro Mini 3.3V
+ TCDA9548 I2C multiplexer (Adafruit)
+ Si4973 FM radio breakout (Sparkfun)

*/

const int startChannel = 971;     // 3-digit value for station (991 = 99.1FM)
const int volume =       15;      // 0-15

const int numRadios =    2;       // how many radios attached to this board?

const int SDIO =         A4;      // I2C pins on Arduino
const int SCLK =         A5;
const int TCA_ADDR =     0x70;    // I2C address for TCA9548

// all the radios
// first value is the digital pin that radio's reset
// pin is connected to (must be unique for each radio)
Si4703_Breakout radios[numRadios] = {
  Si4703_Breakout(2, SDIO, SCLK), 
  Si4703_Breakout(3, SDIO, SCLK), 
//  Si4703_Breakout(4, SDIO, SCLK), 
//  Si4703_Breakout(5, SDIO, SCLK), 
//  Si4703_Breakout(6, SDIO, SCLK), 
//  Si4703_Breakout(7, SDIO, SCLK), 
//  Si4703_Breakout(8, SDIO, SCLK), 
//  Si4703_Breakout(9, SDIO, SCLK), 
};


void setup() {

  // start up all the radios
  for (int i=0; i<numRadios; i++) {
    selectOutput(i);                              // select I2C output channel
    radios[i].powerOn();                          // turn it on
    radios[i].setChannel(startChannel + (i*2));   // set freq (FM moves by 2, after 99.5 is 99.7, etc)
    radios[i].setVolume(volume);                  // turn it up
  }
}


void loop() {
  // nothin'
}


// function to select I2C output channel on TCA9548
void selectOutput(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(TCA_ADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}


