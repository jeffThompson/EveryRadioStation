
#include <Si4703_Breakout.h>
#include <Wire.h>

/*
EVERY RADIO STATION: FIRMWARE
Jeff Thompson | 2017 | jeffreythompson.org

Control software for "Every Radio Station". This project was
supported by a residency at Wave Farm.

NOTE ON FREQUENCIES
Installations in the Americas, Europe, Australia, Africa, and Asia
(except Japan) can all be accounted for in code below. Installations
in Japan, some of Eastern Europe, and Russia may need manual changes
to this code in order to work, or may not be possible at all.

HARDWARE
Setup for 8 radios per section, 12 of these setups total*:

  + Custom-milled 3.3V and ground power bus PCB
  + Arduino Pro Mini 3.3V
  + TCDA9548 I2C multiplexer (Adafruit)
  + Si4973 FM radio breakout (Sparkfun)
  + Tymphany TC9FD18 speaker (Parts Express)

  * the final section has only 7 radios, for a total of 95

*/

const int whichSection =   1;       // which section of radios? (0–11)

const boolean debug =      true;    // debug over serial? (turn off for final install)

const int SDIO =           A4;      // I2C pins on Arduino
const int SCLK =           A5;

// all the radios
// first value is the digital pin that radio's reset
// pin is connected to (must be unique for each radio)
Si4703_Breakout radios[8] = {
  Si4703_Breakout(2, SDIO, SCLK), 
  Si4703_Breakout(3, SDIO, SCLK), 
  Si4703_Breakout(4, SDIO, SCLK), 
  Si4703_Breakout(5, SDIO, SCLK), 
  Si4703_Breakout(6, SDIO, SCLK), 
  Si4703_Breakout(7, SDIO, SCLK), 
  Si4703_Breakout(8, SDIO, SCLK), 
  Si4703_Breakout(9, SDIO, SCLK)
};

// optional: use pre-determined volumes for each radio
// can be used to lessen the volume of really loud stations
// values in adjustment array should be -15 – +15
const boolean useVolumeAdjustments = false;
const int[8] volumeAdjustments = {
  0, 0, 0, 0, 0, 0, 0, 0
};

// jumper pins to set volume
// if neither connected, will be set to max volume
// via: http://forum.arduino.cc/index.php?topic=20131.msg148010#msg148010
const int volTestPin =     11;      // pin to test against, in between the low and medium pins
                                    // (means you can't accidentally connect it wrong)
const int lowVolPin =      10;      // if connected, set to low volume
const int medVolPin =      12;      // ditto to medium volume

const boolean tuneEvenStations = false;   // tune even-numbered stations (ex 88.8)
                                          // really only for European installs, leave alone otherwise

const boolean delayStart = true;          // delay start so they all come on in one nice row?
                                          // super not-necessary, but fun
const int startupDelay =   540;           // millis to delay for each set to begin


void setup() {

  if (debug) {
    Serial.begin(9600);
    Serial.println("EVERY RADIO STATION: INITIALIZING");
    Serial.print("Section: ");
    Serial.println(whichSection);    
  }

  // find starting channel for this section
  // is a 3-digit integer value (991 = 99.1FM)
  // can also account for running up even-numbered stations (ie 88.8) if desired
  int startChannel = 875 + ((whichSection) * 16);
  if (tuneEvenStations) startChannel = 876 + ((whichSection) * 16);
  if (debug) {
    Serial.print("Start channel: ");
    Serial.println(startChannel);
  }

  // set # of radios, depending on which section we're in
  int numRadios = 8;                        // most sections have 8 radios
  if (whichSection == 11)  numRadios = 7;   // final section only has 7
  if (debug) {
    Serial.print("# radios: ");
    Serial.println(numRadios);
  }

  // setup for volume jumper pins
  // set volume accordingly (range of 0–15)
  pinMode(volTestPin, OUTPUT);
  digitalWrite(volTestPin, LOW);      // set LOW, if other pins go LOW too we know they're connected
  pinMode(lowVolPin, INPUT_PULLUP);   // enable using internal pullup resistor
  pinMode(medVolPin, INPUT_PULLUP);
  int volume = checkVolumeJumpers();
  if (debug) {
    Serial.print("Volume: ");
    Serial.println(volume);
  }

  // delay starting, so they all come on in a nice long row
  if (delayStart) {
    delay(whichSection * startupDelay);
  }

  // start up all the radios in this section
  if (debug) {
    Serial.println("Starting radios...");
    if (useVolumeAdjustments) {
      Serial.println("(using individual volume adjustments)");
    }
  }
  for (int i=0; i<numRadios; i++) {
    selectOutput(i);                          // select I2C output channel
    radios[i].powerOn();                      // turn it on
    int channel = startChannel + (i*2);       // FM moves by 2, after 99.5 is 99.7, etc
    radios[i].setChannel(channel);
    if (debug) {
      Serial.print("- freq: ");
      Serial.print(channel);
    }
    
    // set volume, either to value set by jumper
    // or adjusted individually
    int vol = volume;
    if (useVolumeAdjustments) {
      vol += volumeAdjustments[i];
      vol = constrain(vol, 0,15);
    }
    radios[i].setVolume(vol);
    if (debug) {
      Serial.print(", vol: ");
      Serial.println(vol);
    }
  }

  // that's it
  if (debug) Serial.println("\nALL READY!");
}


void loop() {
  // nothin'
}


// test if any of the volume jumpers are connected
// return a new volume (0–15)
int checkVolumeJumpers() {
  if (digitalRead(lowVolPin) == LOW) {
    return 5;
  }
  else if (digitalRead(medVolPin) == LOW) {
    return 10;
  }
  return 15;
}


// function to select I2C output channel on TCA9548
void selectOutput(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(0x70);
  Wire.write(1 << i);
  Wire.endTransmission();  
}


