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
		libusb_device_handle* devhandle;
		unsigned char manufacturer[256];
		unsigned char product[256];

		dev = devs[i];

		int r = libusb_get_device_descriptor(dev, &desc);

		libusb_open(dev, &devhandle);

		libusb_get_string_descriptor_ascii(devhandle, desc.iManufacturer, manufacturer, sizeof(manufacturer));
		libusb_get_string_descriptor_ascii(devhandle, desc.iProduct, product, sizeof(product));

		printf("%d: (%d) vendor=0x%04x (%d) product=0x%04x (%d) bus=%d device=%d manufacturer=%s product=%s\n", i, r, desc.idVendor, desc.idVendor, desc.idProduct, desc.idProduct, libusb_get_bus_number(dev), libusb_get_device_address(dev), manufacturer, product);




		libusb_close(devhandle);
	}

	libusb_exit(NULL);
	return 0;
}
