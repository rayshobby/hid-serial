#ifndef HIDSerial_h
#define HIDSerial_h

#include "Arduino.h"
#include "Print.h"

#define HIDSERIAL_INBUFFER_SIZE 32

class HIDSerial : public Print {
public:
  HIDSerial();
  size_t write(uint8_t);  // write one character
  size_t write(const uint8_t *buffer, size_t size); // write a string

  static void poll();
  static unsigned char available();
  static unsigned char read(unsigned char *buffer);
  static void begin();
private:
  size_t write8(const uint8_t *buffer, size_t size);  // write up to 8 characters
};

#endif
