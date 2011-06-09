/* ra_io_m_onewire.h -- I/O methods for class OneWire. */

#ifndef ra_io_m_onewire_h
#define ra_io_m_onewire_h

typedef struct {
  FILE *value_fp;
  int interval_cnt;
} io_sLocalDS18B20;

typedef struct {
  FILE *value_fp;
  int interval_cnt;
} io_sLocalAiDevice;

#endif
