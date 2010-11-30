/* ra_io_m_motioncontrol_usb.h -- I/O methods for class MotionControl_USBIO. */

#ifndef ra_io_m_motioncontrol_usb_h
#define ra_io_m_motioncontrol_usb_h

#define USB_MAX_CARDS 50

typedef struct {
  int USB_Handle[USB_MAX_CARDS];
  unsigned int snum[USB_MAX_CARDS];
} io_sLocalUSB;

#endif
