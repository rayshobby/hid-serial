// *******************************
// HID Serial Example
// Hello World
// RAYSHOBBY.net
//
// Print "Hello World" once every
// second.
// *******************************

#include <HIDSerial.h>

HIDSerial serial;

void setup() {
  serial.begin();
}

unsigned long t = 0;
void loop() {

  if (t == 0 || millis() > t+1000) {
    serial.println("Hello World!");
    t = millis();
  }
  serial.poll();
  
}

