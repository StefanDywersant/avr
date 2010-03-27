#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include "usbdrv.h"
//#include "oddebug.h"

#define SCL_CLOCK 100000L

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

void twiPlayground() {
	// send start condition
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	led = 0x01;

	// wait for TWINT flag set
	while (!(TWCR & (1 << TWINT)));
	led = 0x02;

	// check value of TWI status register
	if ((TWSR & 0xf8) != 0x08) {
//		led = TWSR;
		led = 0xff;
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
		return;
	}
	led = 0x03;

	// send address
	TWDR = 0xa0 | 0x00;
	TWCR = (1 << TWINT) | (1 << TWEN);
	led = 0x04;

	// wait for TWINT flag set
	while (!(TWCR & (1 << TWINT)));
	led = 0x05;

	// check value of TWI status register
	if ((TWSR & 0xf8) != 0x18) {
//		led = TWSR;
		led = 0xfe;
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
		return;
	}
	led = 0x06;

	// load data into TWDR register
	TWDR = 0x02;
	TWCR = (1 << TWINT) | (1 << TWEN);
	led = 0x07;

	// wait for TWINT flag set
	while (!(TWCR & (1 << TWINT)));
	led = 0x08;

	// check value of TWI status register
	if ((TWSR & 0xf8) != 0x28) {
//		led = TWSR;
		led = 0xfd;
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
		return;
	}
	led = 0x09;

	// *********

	// send start condition
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	led = 0x0a;

	// wait for TWINT flag set
	while (!(TWCR & (1 << TWINT)));
	led = 0x0b;

	// check value of TWI status register
	if ((TWSR & 0xf8) != 0x10) {
//		led = TWSR;
		led = 0xfc;
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
		return;
	}
	led = 0x0c;

	// send address
	TWDR = 0xa0 | 0x01;
	TWCR = (1 << TWINT) | (1 << TWEN);
	led = 0x0d;

	// wait for TWINT flag set
	while (!(TWCR & (1 << TWINT)));
	led = 0x0e;

	// check value of TWI status register
	if ((TWSR & 0xf8) != 0x40) {
//		led = TWSR;
		led = 0xfb;
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
		return;
	}
	led = 0x0f;

	// load data into TWDR register
	TWDR = 0x02;
	TWCR = (1 << TWINT) | (1 << TWEN);
	led = 0x10;

	// wait for TWINT flag set
	while (!(TWCR & (1 << TWINT)));
	led = 0x11;

	// check value of TWI status register
	if ((TWSR & 0xf8) != 0x28) {
//		led = TWSR;
//		led = 0xfa;
		led = TWDR;
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
		return;
	}
	led = 0x12;

	// transmit stop condition
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	led = 0x13;
}

void executeCommand(uchar command) {
	switch (command) {
		case 0x00:
			twiPlayground();
			break;
		default:
			led = command;
	}
}

usbMsgLen_t usbFunctionSetup(uchar data[8]) {
	usbRequest_t* request = (usbRequest_t *)data;

	if (request->bRequest == 0x04) {
		led = 0x00;
		return USB_NO_MSG;
	}

    return 0;
}

uchar usbFunctionRead(uchar *data, uchar len) {
	led=data[0];

	return len;
}

uchar usbFunctionWrite(uchar *data, uchar len) {
	unsigned int i;

	for (i = 0; i < len; i++)
		executeCommand(data[i]);

	return len;
}

int main(void) {
	DDRB = 0xFF;
	PORTC = 0x03;

	TWSR = 0x00;
	TWBR = ((F_CPU / SCL_CLOCK) - 16) / 2;
	TWCR = (1 << TWEN) | (1 << TWIE);

//	wdt_enable(WDTO_1S);

//	DBG1(0x00, 0, 0);

//	odDebugInit();
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
