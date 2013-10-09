// *******************************
// HID Serial Example
// Button press
// RAYSHOBBY.net
//
// This program opens an HID serial
// channel and shows the status of
// the pushbutton as it's pressed
// or released. To use it, you need
// to run the HIDSerialMonitor
// from a host computer.
// *******************************

#include <HIDSerial.h>

#define BUTTON 4
HIDSerial serial;

void setup() {
  serial.begin();
  pinMode(BUTTON, INPUT);
  digitalWrite(BUTTON, HIGH);
}

void loop() {
  static int last = HIGH;
  static int count = 0;
  int b = digitalRead(BUTTON);
  if(b!=last) {
    serial.print((b==LOW) ? "button down :" : "button up.");
    if (b==LOW) {
      count ++;
      serial.println(count);
    } else {
      serial.println("");
    }
    last = b;
    delay(50);
  }
  serial.poll();
}
  
