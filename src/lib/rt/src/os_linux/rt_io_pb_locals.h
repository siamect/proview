#define IO_MAXCHAN 32

#define PB_MODULE_STATE_NOTINIT 0
#define PB_MODULE_STATE_OPERATE 1

#define PB_SLAVE_STATE_NOTINIT 0
#define PB_SLAVE_STATE_STOPPED 1
#define PB_SLAVE_STATE_OPERATE 2

#define PB_MASTER_STATE_NOTINIT 0
#define PB_MASTER_STATE_STOPPED 1
#define PB_MASTER_STATE_CLEARED 2
#define PB_MASTER_STATE_OPERATE 3

#define PB_STALLACTION_NONE	0
#define PB_STALLACTION_RESET 	1
#define PB_STALLACTION_BREAK 	2

#define PB_NUMREP_UNSIGNEDINT	0
#define PB_NUMREP_SIGNEDINT	1
#define PB_NUMREP_FLOATIEEE	2
#define PB_NUMREP_FLOATVAX	3
#define PB_NUMREP_FLOATINTEL	4

#define PB_BYTEORDERING_LE	0
#define PB_BYTEORDERING_BE	1

#define PB_ORIENTATION_BYTE	8
#define PB_ORIENTATION_WORD	16
#define PB_ORIENTATION_DWORD	32

typedef struct {
  int  Pb_fp;
} io_sAgentLocal;

typedef struct {
  int initialized;
} io_sRackLocal;

typedef struct {
  void *input_area;
  void *output_area;
  int scancount[IO_MAXCHAN];
} io_sCardLocal;
