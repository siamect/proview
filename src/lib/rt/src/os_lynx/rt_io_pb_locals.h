#define IO_MAXCHAN 32
 
typedef struct {
  int  Pb_fp;
} io_sAgentLocal;

typedef struct {
  int initialized;
} io_sRackLocal;

typedef struct {
  void *input_area;
  void *output_area;
  short byte_swap;
  int initialized;
  int scancount[IO_MAXCHAN];
} io_sCardLocal;
