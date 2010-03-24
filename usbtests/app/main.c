/*
 * main.c
 *
 *  Created on: 2010-03-23
 *      Author: Karol Maciaszek
 */

#include <stdio.h>
#include <string.h>
#include <libusb-1.0/libusb.h>

int main(int argc, char** argv) {
	libusb_device** devs;
	libusb_device* dev;
	ssize_t cnt;
	int i;

	libusb_init(NULL);

	cnt = libusb_get_device_list(NULL, &devs);

	printf("Number of devices: %d\n", cnt);

	for (i = 0; i < cnt; i++) {
		struct libusb_device_descriptor desc;

		dev = devs[i];

		int r = libusb_get_device_descriptor(dev, &desc);
		printf("%d: (%d) vendor=%04x product=%04x bus=%d, device=%d\n", i, r, desc.idVendor, desc.idProduct, libusb_get_bus_number(dev), libusb_get_device_address(dev));



	}

	libusb_exit(NULL);
}
