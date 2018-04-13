/*
  Dummy for libusb-1.0
 */
#include <sys/types.h>
#include <stdint.h>
#include "rt_io_base.h"

struct libusb_context;
struct libusb_device;
struct libusb_device_handle;
typedef struct libusb_context libusb_context;
typedef struct libusb_device libusb_device;
typedef struct libusb_device_handle libusb_device_handle;
struct libusb_device_descriptor {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint16_t bcdUSB;
	uint8_t  bDeviceClass;
	uint8_t  bDeviceSubClass;
	uint8_t  bDeviceProtocol;
	uint8_t  bMaxPacketSize0;
	uint16_t idVendor;
	uint16_t idProduct;
	uint16_t bcdDevice;
	uint8_t  iManufacturer;
	uint8_t  iProduct;
	uint8_t  iSerialNumber;
	uint8_t  bNumConfigurations;
};

int libusb_init(libusb_context **ctx) {return io_cLibDummy;}
void libusb_exit(libusb_context *ctx) {}
ssize_t libusb_get_device_list(libusb_context *ctx,
			       libusb_device ***list) {return 0;}
void libusb_free_device_list(libusb_device **list, int unref_devices) {}
int libusb_open(libusb_device *dev, libusb_device_handle **handle) {return io_cLibDummy;}
void libusb_close(libusb_device_handle *dev_handle) {}
int libusb_interrupt_transfer(libusb_device_handle *dev_handle,
			      unsigned char endpoint, unsigned char *data, int length,
			      int *actual_length, unsigned int timeout) {return 0;}
int libusb_kernel_driver_active(libusb_device_handle *dev, int interface) {return 0;}
int libusb_detach_kernel_driver(libusb_device_handle *dev, int interface) {return 0;}
int libusb_claim_interface(libusb_device_handle *dev, int iface) {return 0;}
int libusb_get_device_descriptor(libusb_device *dev,
				 struct libusb_device_descriptor *desc) {return 0;}





