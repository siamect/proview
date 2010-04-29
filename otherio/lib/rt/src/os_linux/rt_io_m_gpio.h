/* ra_io_m_motioncontrol_usb.h -- I/O methods for class MotionControl_USBIO. */

#ifndef ra_io_m_gpio_h
#define ra_io_m_gpio_h

#define GPIO_MAX_CHANNELS 32

typedef struct {
  FILE *value_fp[GPIO_MAX_CHANNELS];
  unsigned int number[GPIO_MAX_CHANNELS];
  int interval_cnt;
  int has_read_method;
} io_sLocalGPIO_Module;

#endif
