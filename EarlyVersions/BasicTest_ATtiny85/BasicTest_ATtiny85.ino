
/*
BASIC TEST
Jeff Thompson | 2017 | jeffreythompson.org

1.8-2.7k pullup instead

Test code for using the Si4703 FM radio breakout with
ATtiny chip. This example is intended for ATtiny85, so
you may have to tweak your SDIO and SCLK pins, depending
on what chip you use.

REQUIRED LIBRARIES
Requires the TinyWireM library from Adafruit, which lets
you use I2C on ATtiny chips:
https://github.com/adafruit/TinyWireM

IC2
+ Data connection = SDA
    Should be on pin PB0, which is pin #5 on the ATtiny85, aka D0
+ Clock connection = SCK
    Should be on pin PB2, which is pin #7, aka D2

HOOKUP
Note: you must (MUST!) have pullup resistors on the SDA
and SCK lines, otherwise you will have problems, Sparkfun
suggests a 4.7k for each, like so:

    D0                  D2
    |   4.7k            |   4.7k
    +---[[[[---> 3V     +---[[[[---> 3V
    |                   |
   SDA                 SCK

Connect the rest:
+ VCC to your 3V supply, and out to the Si4703
+ GND to your supply and the radio
+ reset pin from D1 to the radio

MORE DETAILS ON ATTINY85
For more info on hooking up and using the ATtiny85
+ http://highlowtech.org/?p=1695
+ https://32414320wji53mwwch1u68ce-wpengine.netdna-ssl.com/wp-content/
  uploads/2011/05/Attiny-Master.gif
 
*/

#include <TinyWireM.h>
#include <Si4703_ATtiny.h>

//#define GPIO_ADDR 0x3F


int SDA =   0;       // I2C data pin on ATtiny85 (pin 
int SCK =   2;       // I2C clock pin on ATtiny85
int reset = 1;       // can probably be any digital pin

Si4703_ATtiny radio(reset, SDA, SCK);


void setup() {
  TinyWireM.begin();    // need this?
  
  // turn on radio
  // (creates connection with device)
  radio.powerOn();
//
//  // set a channel
//  // should be a three-digit integer
//  // for the station's frequency
//  // (ie 93.7FM = 937)
  radio.setChannel(937);
}

void loop() {
  radio.setVolume(15);
  delay(1000);
  radio.setVolume(0);
  delay(1000);
}


//
//  // play the radio for 5 seconds
//  delay(5000);
//
//  // seek channels up five times,
//  // listen for a second
//  for (int i=0; i<5; i++) {
//    radio.seekUp();
//    delay(1000);
//  }
//
//  // fade out volume programatically
//  for (int i=15; i>0; i-=1) {
//    radio.setVolume(i);
//    delay(200);
//  }
//}


//void loop() {
//  // nothing here :)
//}


