/*
 * main.c
 *
 *  Created on: 2010-03-23
 *      Author: Karol Maciaszek
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libusb-1.0/libusb.h>

#define DEV_VENDOR_ID 0x16c0
#define DEV_PRODUCT_ID 0x03e8
#define DEV_MANUFACTURER_NAME "czystybeton.pl"
#define DEV_PRODUCT_NAME "Napierdalator"

/**
 * Searches for USB device.
 * Returns 0 on success or negative integer on failure.
 *
 * @author Karol Maciaszek
 * @param handle Once found, usb device handler will be there
 * @return int
 */
int get_device(libusb_device_handle** handle) {
	libusb_device** devs;
	ssize_t devices_count;
	int i;

	devices_count = libusb_get_device_list(NULL, &devs);

	printf("Looking for device...\n");

	for (i = 0; i < devices_count; i++) {
		struct libusb_device_descriptor descriptor;
		unsigned char manufacturer[256];
		unsigned char product[256];
		libusb_device_handle* tmp_handle;

		if (libusb_get_device_descriptor(devs[i], &descriptor) < 0) {
			printf("Cannot get device #%d descriptor!\n", i);
			continue;
		}

		if (descriptor.idVendor != DEV_VENDOR_ID || descriptor.idProduct != DEV_PRODUCT_ID)
			continue;

		if (libusb_open(devs[i], &tmp_handle) < 0) {
			printf("Cannot open device %04x:%04x!\n", descriptor.idVendor, descriptor.idProduct);
			continue;
		}

		if (libusb_get_string_descriptor_ascii(tmp_handle, descriptor.iManufacturer, manufacturer, sizeof(manufacturer)) < 0) {
			printf("Cannot get manufacturer string from device %04x:%04x!\n", descriptor.idVendor, descriptor.idProduct);
			libusb_close(tmp_handle);
			continue;
		}

		if (libusb_get_string_descriptor_ascii(tmp_handle, descriptor.iProduct, product, sizeof(product)) < 0) {
			printf("Cannot get product string from device %04x:%04x!\n", descriptor.idVendor, descriptor.idProduct);
			libusb_close(tmp_handle);
			continue;
		}

		if (strncmp(DEV_MANUFACTURER_NAME, (char *)manufacturer, 256) != 0 || strncmp(DEV_PRODUCT_NAME, (char *)product, 256) != 0) {
			libusb_close(tmp_handle);
			continue;
		}

		// device found
		printf("Device found at %04d:%04d\n", libusb_get_bus_number(devs[i]), libusb_get_device_address(devs[i]));
		printf("Device info: %s / %s [%04x:%04x]\n", manufacturer, product, descriptor.idVendor, descriptor.idProduct);

		*handle = tmp_handle;
		libusb_free_device_list(devs, devices_count);
		return 0;
	}

	// device not found
	libusb_free_device_list(devs, devices_count);
	return -1;
}

int main(int argc, char** argv) {
	libusb_device_handle* handle;

	libusb_init(NULL);

	if (get_device(&handle) < 0) {
		printf("Unable to find device!\n");
		return 0;
	}

	if (argc != 2) {
		printf("Wrong parameter count\n");
		return 0;
	}

	unsigned int reqType[1];
	if (sscanf(argv[1], "0x%02x", reqType) == 0) {
		printf("Invalid hex value\n");
		return 0;
	}

	printf("%d\n", reqType[0]);

	int ret = libusb_control_transfer(handle, LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_OUT, 0x04, 0 ,0, (unsigned char*)reqType, 1, 10000);

	printf("Write: %d\n", ret);

	unsigned char data[4];
	ret = libusb_control_transfer(handle, LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_IN, 0x04, 0 ,0, data, 4, 10000);

	printf("Read: %d (%02d:%02d:%02d.%02d)\n", ret, data[3], data[2], data[1], data[0]);

	libusb_close(handle);

	libusb_exit(NULL);
	return 0;
}
