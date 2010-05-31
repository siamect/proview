/* rt_io_m_usb_agent.h -- I/O methods for class USB_Agent. */

#ifndef ra_io_m_usb_agent_h
#define ra_io_m_usb_agent_h

#if defined PWRE_CONF_LIBUSB

typedef struct {
  libusb_context *libusb_ctx;
} io_sLocalUSB_Agent;

#endif
#endif
