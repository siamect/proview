#define BFB_SERVICE_READ 1
#define BFB_SERVICE_WRITE 2

struct bfb_item {
	pwr_tUInt16	address;
	pwr_tUInt16	data;
};

struct bfb_buf {
	pwr_tUInt16	service;
	pwr_tUInt16	length;
	struct bfb_item	item[350];
};

typedef struct {
	/* Elements for Qbus local I/O */
	int 		Qbus_fp;	/* File pointer for local qbus device */
	int		s;		/* Socket for remote qbus node */
	/* Elements for Qbus ethernet I/O */
	struct sockaddr_in my_addr;	/* My socket */
	struct sockaddr_in rem_addr;	/* Remote socket */
	struct bfb_buf	read_area;	/* Data area for the last inputs read (i.e. ack of read request) */
	struct bfb_buf	write_area;	/* Data area for the last outputs read (i.e. ack of write request) */
	struct bfb_buf	read_req;	/* Data area for the read request */
	struct bfb_buf	write_req;	/* Data area for the write request */
	int		next_read_req_item;	/* Index for next position in read request area */
	int		next_write_req_item;	/* Index for next position in write request area */
} io_sRackLocal;
