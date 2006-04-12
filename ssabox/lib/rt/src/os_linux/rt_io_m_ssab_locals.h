#define BFB_SERVICE_READ 1
#define BFB_SERVICE_WRITE 2

struct bfb_item {
	pwr_tUInt16	address;
	pwr_tUInt16	data;
};

struct bfb_buf {
	pwr_tUInt16	service;
	pwr_tUInt16	length;
	struct bfb_item	item[256];
};

typedef struct {
	int 		Qbus_fp;	/* File pointer for local qbus device */
	int		s;		/* Socket for remote qbus node */
	struct sockaddr_in my_addr;
	struct sockaddr_in rem_addr;
	struct bfb_buf	in;		/* Data area for remote qbus node */
	struct bfb_buf	out;		/* -"- */
	int		in_items;
	int		out_items;
} io_sRackLocal;
