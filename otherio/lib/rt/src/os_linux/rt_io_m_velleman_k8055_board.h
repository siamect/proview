/* rt_io_m_velleman_k8055_board.h -- I/O methods for class velleman_k8055_board. */

#ifndef ra_io_m_velleman_k8055_board_h
#define ra_io_m_velleman_k8055_board_h

#if defined PWRE_CONF_LIBUSB

typedef struct {
  libusb_device_handle *libusb_device;
} io_sLocal_K8055;

#endif
#endif
