
#define QBUS_IOCTL_PHYSBADLA 	1

typedef struct {
	unsigned int	Address;
	unsigned int	Data;
} qbus_io_read;

typedef struct {
        unsigned int    Address;
        unsigned int    Data;
} qbus_io_write;

typedef struct {
        unsigned int    PhysBadla;
} qbus_io_ioctl;

