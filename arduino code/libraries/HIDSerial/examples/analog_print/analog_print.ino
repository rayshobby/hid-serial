// *******************************
// HID Serial Example
// Print analog sensor values
// RAYSHOBBY.net
//
// This program opens an HID serial
// channel and shows how to send
// and receive messages from a host
// computer. To use it, you need
// to run the HIDSerialMonitor
// from a host computer. The program
// repeatedly prints out the value
// on a specific analog pin. Typing
// 'a?' (where ? is a number between
// 0 to 5) in the serial monitor
// changes the selected pin.
// *******************************

#include <HIDSerial.h>

HIDSerial serial;

unsigned long t = 0;
unsigned char buffer[32];
unsigned char adcCh = 0;

void setup() {
  serial.begin();
}

void loop() {

    int value;
    if (millis() > t + 2000)
    {
        t = millis();
        serial.write('A');  
        serial.print(adcCh);
        serial.print(" = ");
        serial.println(analogRead(adcCh));
    }
    
    if(serial.available()) {
        int size = serial.read(buffer);
        if (size!=0) {
            //serial.write((const uint8_t*)buffer, size);
            if(buffer[0]=='a' || buffer[0]=='A') {
                if(buffer[1]>='0'&&buffer[1]<='5') {
                    adcCh = buffer[1]-'0';
                    serial.write('A');
                    serial.print(adcCh);
                    serial.println(" selected.");
                }
            }
        }
    }
    serial.poll();
}

