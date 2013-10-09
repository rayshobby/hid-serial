// *******************************
// HID Serial Example
// Echo
// RAYSHOBBY.net
//
// This program opens an HID serial
// channel and shows how to send
// and receive messages from a host
// computer. To use it, you need
// to run the HIDSerialMonitor
// from a host computer. The program
// echos the string (up to 32 characters)
// received from the host computer.
// *******************************

#include <HIDSerial.h>

HIDSerial serial;

unsigned char buffer[32];

void setup() {
  serial.begin();
}

void loop() {
  if(serial.available()) {
    int size = serial.read(buffer);
    if (size!=0) {
      serial.write((const uint8_t*)buffer, size);
      serial.write('\n');
    }
  }
  serial.poll();
}

