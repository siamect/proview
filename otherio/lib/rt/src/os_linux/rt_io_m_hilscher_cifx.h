

typedef struct {
  unsigned long channel;
  unsigned long board;
  CIFXHANDLE chan;
  unsigned int diag_cnt;
  unsigned int diag_interval;
  unsigned int dev_init;
  unsigned int dev_init_cnt;
  unsigned int dev_init_limit;
  int input_area_size;
  int output_area_size;
  void *input_area;
  void *output_area;
} io_sLocalHilscher_cifX_Master;

typedef struct {
  int byte_ordering;
  int float_representation;
} io_sLocalHilscher_cifX_Device;

