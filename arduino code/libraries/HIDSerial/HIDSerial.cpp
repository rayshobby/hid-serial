#include "HIDSerial.h"

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */
#include <avr/eeprom.h>
#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include "usbdrv.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

static uchar received = 0;
static uchar outBuffer[8];
static uchar inBuffer[HIDSERIAL_INBUFFER_SIZE];
static uchar reportId = 0;
static uchar bytesRemaining;
static uchar* pos;

PROGMEM const char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = {    /* USB report descriptor */
    0x06, 0x00, 0xff,              // USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    // USAGE (Vendor Usage 1)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x09, 0x00,                    //   USAGE (Undefined)  
    0x82, 0x02, 0x01,              //   INPUT (Data,Var,Abs,Buf)
    0x95, HIDSERIAL_INBUFFER_SIZE, //   REPORT_COUNT (32)
    0x09, 0x00,                    //   USAGE (Undefined)        
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)
    0xc0                           // END_COLLECTION
};

/* usbFunctionRead() is called when the host requests a chunk of data from
 * the device. For more information see the documentation in usbdrv/usbdrv.h.
 */
uchar   usbFunctionRead(uchar *data, uchar len)
{
    return 0;
}

/* usbFunctionWrite() is called when the host sends a chunk of data to the
 * device. For more information see the documentation in usbdrv/usbdrv.h.
 */
uchar   usbFunctionWrite(uchar *data, uchar len)
{
    if (reportId == 0) {
        int i;
        if(len > bytesRemaining)
            len = bytesRemaining;
        bytesRemaining -= len;
        //int start = (pos==inBuffer)?1:0;
        for(i=0;i<len;i++) {
            if (data[i]!=0) {
                *pos++ = data[i];
             }
        }
        if (bytesRemaining == 0) {
            received = 1;
            *pos++ = 0;
            return 1;
        } else {
            return 0;
        }
    } else {
        return 1;
    }
}

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
    usbRequest_t    *rq = (usbRequest_t *)data;
    reportId = rq->wValue.bytes[0];
    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS){    /* HID class request */
        if(rq->bRequest == USBRQ_HID_GET_REPORT){
          /* wValue: ReportType (highbyte), ReportID (lowbyte) */
            /* since we have only one report type, we can ignore the report-ID */
            return USB_NO_MSG;  /* use usbFunctionRead() to obtain data */
        }else if(rq->bRequest == USBRQ_HID_SET_REPORT){
            /* since we have only one report type, we can ignore the report-ID */
            pos = inBuffer;
            bytesRemaining = rq->wLength.word;
            if(bytesRemaining > sizeof(inBuffer))
                bytesRemaining = sizeof(inBuffer);
            return USB_NO_MSG;  /* use usbFunctionWrite() to receive data from host */
        }
    }else{
        /* ignore vendor type requests, we don't use any */
    }
    return 0;
}

HIDSerial::HIDSerial() 
{

}

void HIDSerial::begin()
{
  uchar i;
  cli();
  usbDeviceDisconnect(); 
  i = 0;
  while(--i){             /* fake USB disconnect for > 250 ms */
    _delay_ms(1);
  }
  usbDeviceConnect();
  usbInit();
  sei();
  
  received = 0;
}

void HIDSerial::poll()
{
    usbPoll();
}

uchar HIDSerial::available()
{
    return received;
}

uchar HIDSerial::read(uchar *buffer)
{
    if(received == 0) return 0;
    int i;
    for(i=0;inBuffer[i]!=0&&i<HIDSERIAL_INBUFFER_SIZE;i++)
    {
        buffer[i] = inBuffer[i];
    }
    inBuffer[0] = 0;
    buffer[i] = 0;
    received = 0;
    return i;
}

// write one character
size_t HIDSerial::write(uint8_t data)
{
  while(!usbInterruptIsReady()) {
    usbPoll();
  }
  memset(outBuffer, 0, 8);
  outBuffer[0] = data;
  usbSetInterrupt(outBuffer, 8);
  return 1;
}

// write up to 8 characters
size_t HIDSerial::write8(const uint8_t *buffer, size_t size)
{
  unsigned char i;
  while(!usbInterruptIsReady()) {
    usbPoll();
  }
  memset(outBuffer, 0, 8);
  for(i=0;i<size && i<8; i++) {
    outBuffer[i] = buffer[i];
  }
  usbSetInterrupt(outBuffer, 8);
  return (i);
}

// write a string
size_t HIDSerial::write(const uint8_t *buffer, size_t size)
{
  size_t count = 0;
  unsigned char i;
  for(i=0; i< (size/8) + 1; i++) {
    count += write8(buffer+i*8, (size<(count+8)) ? (size-count) : 8);
  }
  return count;
}
