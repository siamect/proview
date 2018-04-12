/* ra_io_m_nodave.h -- I/O methods for libnodave. */

#ifndef rt_io_m_nodave_h
#define rt_io_m_nodave_h


typedef struct {
  _daveOSserialType fds;
  daveInterface *di;
  daveConnection *dc;
  int status;
  int reset_inputs;
  char *input_area;
  char *output_area;
  unsigned int input_size;
  unsigned int output_size;
} io_sRackLocal;

typedef struct {
  int status;
  int interval_cnt;
  int has_read_method;
  unsigned int input_size;
  unsigned int output_size;
  int op_read;
  int op_write;
  char *input_area;
  char *output_area;
} io_sCardLocal;

#endif
