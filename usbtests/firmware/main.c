#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include "pcf8583.h"
#include "twi.h"
#include "usbdrv.h"

PROGMEM char usbHidReportDescriptor[52] = { /* USB report descriptor, size must match usbconfig.h */
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,                    // USAGE (Mouse)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x09, 0x01,                    //   USAGE (Pointer)
    0xA1, 0x00,                    //   COLLECTION (Physical)
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM
    0x29, 0x03,                    //     USAGE_MAXIMUM
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x75, 0x05,                    //     REPORT_SIZE (5)
    0x81, 0x03,                    //     INPUT (Const,Var,Abs)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x09, 0x38,                    //     USAGE (Wheel)
    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
    0x25, 0x7F,                    //     LOGICAL_MAXIMUM (127)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x81, 0x06,                    //     INPUT (Data,Var,Rel)
    0xC0,                          //   END_COLLECTION
    0xC0,                          // END COLLECTION
};

static char led = 0xff;
static pcf8583TimeStruct time;


void twiPlayground() {
/*	uint8_t request[1];
	uint8_t response[1];

	*request = 0x02;

	if (twiSyncMTMR(0xa0, request, 1, response, 1) == TWI_OK)
		led = response[0];
	else
		led = 0x55;*/
}

void readSecond() {
	if (pcf8583GetTime(0xa0, &time) != PCF8583_OK) {
		led = 0xaa;
		return;
	}
}

void executeCommand(uchar command) {
	switch (command) {
		case 0x00:
			twiPlayground();
			break;
		case 0x01:
			readSecond();
			break;
		default:
			led = command;
	}
}

usbMsgLen_t usbFunctionSetup(uchar data[8]) {
	usbRequest_t* request = (usbRequest_t *)data;

	if (request->bRequest == 0x04) {
//		led = 0x00;
		return USB_NO_MSG;
	}

    return 0;
}

uchar usbFunctionRead(uchar *data, uchar len) {
	data[0] = time.milisecond;
	data[1] = time.second;
	data[2] = time.minute;
	data[3] = time.hour;
	data[4] = time.date;
	data[5] = time.month;
	data[6] = time.year;
	data[7] = time.weekday;

	return 8;
}

uchar usbFunctionWrite(uchar *data, uchar len) {
	unsigned int i;

	for (i = 0; i < len; i++)
		executeCommand(data[i]);

	return len;
}

int main(void) {
	DDRB = 0xFF;

	twiInit(100000);

	usbInit();
	
	usbDeviceDisconnect();
	uchar i = 0;
	while (--i) {
//		wdt_reset();
		_delay_ms(1);
	}
	
	usbDeviceConnect();
	sei();

//	DBG1(0x01, 0, 0);

	while (1) {
//		wdt_reset();
		usbPoll();
		PORTB = led;
	}

	
}
