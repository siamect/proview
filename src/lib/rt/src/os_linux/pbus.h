#ifndef __PBDRVR__
#define __PBDRVR__

#define INIT_MODE		0x00
#define CONFIG_MODE		0x05
#define COMM_MODE		0x08

#define H_READY_MASK 		0x874B2D1E
#define C_READY_MASK 		0x78B4D2E1

#define IDLE			0x00
#define BUSY			0x01

#define D_DATA_IDLE		0x00
#define D_DATA_APPLY		0xF0
#define D_DATA_BUSY		0x0F

// Macros for defining ioctl commands

#define PB_IOCTL_MAGIC 'c'

#define PB_IOCTL_READCMIDESC            _IO(PB_IOCTL_MAGIC, 1)
#define PB_IOCTL_READLOCALREG           _IO(PB_IOCTL_MAGIC, 2)
#define PB_IOCTL_WRITECMIDESC           _IO(PB_IOCTL_MAGIC, 3)
#define PB_IOCTL_HWRESET                _IO(PB_IOCTL_MAGIC, 4)
#define PB_IOCTL_CMI_INIT               _IO(PB_IOCTL_MAGIC, 5)
#define PB_IOCTL_READDATADESC           _IO(PB_IOCTL_MAGIC, 6)
#define PB_IOCTL_CMI_WRITE              _IO(PB_IOCTL_MAGIC, 7)
#define PB_IOCTL_READ_SDB               _IO(PB_IOCTL_MAGIC, 8)
#define PB_IOCTL_READ_DB                _IO(PB_IOCTL_MAGIC, 9)
#define PB_IOCTL_CMI_READ               _IO(PB_IOCTL_MAGIC, 10)
#define PB_IOCTL_READ_IRQ_VALUES        _IO(PB_IOCTL_MAGIC, 11)
#define PB_IOCTL_READ_FIRST_SLAVE       _IO(PB_IOCTL_MAGIC, 12)
#define PB_IOCTL_WRITE_FIRST_SLAVE      _IO(PB_IOCTL_MAGIC, 13)
#define PB_IOCTL_READVERSION            _IO(PB_IOCTL_MAGIC, 14)
#define PB_IOCTL_READSERIAL             _IO(PB_IOCTL_MAGIC, 15)

#define ERROR_DESCR_LENGTH            32 

typedef struct {
  unsigned int 		h_ready_mask;
  unsigned int 		h_base_address;
  unsigned char 	h_id;
  unsigned char 	h_int_enable;
  unsigned char 	h_address_swap_mode;
  unsigned char 	h_state;
  unsigned int 		h_param_addr;
  unsigned int 		h_data_addr;
  unsigned short 	h_param_size;
  unsigned short 	h_data_size;
  unsigned char 	h_sema;
  unsigned char 	h_ret_val;
  unsigned char 	h_head;
  unsigned char 	h_tail;
  unsigned int 		h_data_descr_addr;

  unsigned int 		c_ready_mask;
  unsigned int 		c_base_address;
  unsigned char 	c_id;
  unsigned char 	c_int_enable;
  unsigned char 	c_address_swap_mode;
  unsigned char 	c_state;
  unsigned int 		c_param_addr;
  unsigned int 		c_data_addr;
  unsigned short 	c_param_size;
  unsigned short 	c_data_size;
  unsigned char 	c_sema;
  unsigned char 	c_ret_val;
  unsigned char 	c_head;
  unsigned char 	c_tail;
  unsigned int 		c_data_descr_addr;
}T_CMI_DESCRIPTOR;

typedef struct {
  unsigned int		reg[21];
} T_LOCALREG;

typedef struct {
  unsigned char		d_id;
  unsigned char		dummy;
  unsigned char 	d_sema_c;
  unsigned char 	d_sema_h;
  unsigned short	d_data_size;
  unsigned int		d_data_addr;
} T_DATA_DESCR;

typedef struct {
  T_PROFI_SERVICE_DESCR *sdb_ptr;
  USIGN8		*data_ptr;
  USIGN16		*data_len_ptr;
  USIGN16		*retval_ptr;
} cmi_request_access_struct;

typedef struct {
  USIGN8		data_id;		// Id of data structure
  USIGN16		offset;			// Offset in data area
  USIGN8		*data_ptr;		// Pointer to data to write/to be read
  USIGN16		*retval_ptr;		// Pointer to return value
} cmi_data_access_struct;

#endif
