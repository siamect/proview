#ifndef rt_io_rapi_h
#define rt_io_rapi_h

/* rt_io_rapi.h -- General RAPI definitions
*
*
*  modification history
*  --------------------
*  950108,LT  	Ported to vxWorks from INTERAY demo source code.
*  970706 BHo  Ported to Lynx from vxWorks code
*
*/

#ifdef __cplusplus
extern "C" {
#endif


#include "pwr.h"
#include "rt_io_rapisys.h"
#include "rt_io_rapidev.h"
#ifdef OS_LYNX
#include <time.h>
#endif


#define NUM_RAPI		4	                  /* Max number of RAPI structures */
                                          
#define MILLI_DELAY  1000000              /* One millisecond delay in POSIX timespec struct */

/*-------------------------------------------------------------------------------------------*/

#ifdef WIN32                                       /* POSIX types ;-). I'm writing on NT */ 
                                          
/* Hupp !? where's this defined ??? typedef int time_t; */                      
                                          
struct timespec                           
{                                         
   time_t tv_sec;                                  /* Number of seconds */
   long   tv_nsec;                                 /* Number of nanoseconds */
}; 

int nanosleep(struct timespec*, struct timespec*); /* Bogus protype for testcompilation on NT */

#endif   /* OS_LYNX */

/*-------------------------------------------------------------------------------------------*/

#ifndef mSecToTick
/*#define mSecToTick(x) ((int)(x)/15)*/	/* Convert to VxWorks system clock tick */
#define mSecToTick(x) ((int)(x)/20)	      /* Convert to "xx" system clock tick */
#endif

#if 1
#define RAPI_OK			   0
#define RAPI_ERR		  -1
#else
#define RAPI_OK			   1              /* ODD   is OK  in VMS world */
#define RAPI_ERR		   0              /* EVEN  is BAD in VMS world */
#endif

#define RAPI_REMOVE     	0x0F

#define RAPI_FIFO       	0x10
#define RAPI_LATCH      	0x20
#define RAPI_ARRAY      	0x20
#define RAPI_COS        	0x40
#define RAPI_IRQ        	0x80

#define RAPI_DEV_BUSY      0x80
#define RAPI_DEV_UPDATE    0x40
#define RAPI_DEV_COS       0x20
#define RAPI_DEV_FULL      0x10
#define RAPI_DEV_ERROR     0x08

#define RAPI_RESET      	0
#define RAPI_LDEVSCAN   	1
#define RAPI_GDEVSCAN   	2
#define RAPI_DEVINIT    	3
#define RAPI_PAUSE      	4
#define RAPI_RUN        	5

#define RAPI_BUSY       	7

#define RAPI_SCAN_RDY      0
#define RAPI_NEW_SCAN      1
#define RAPI_MOD_SCAN      2
#define RAPI_MOD_RES       3
#define RAPI_DEV_SCAN      4
#define RAPI_DEV_NEXT      5
#define RAPI_DEV_RDY       6

#define RAPI_OUTP_SCAN     0x80
#define RAPI_INP_SCAN      0x40

typedef unsigned char UINT8;

typedef union	{
	volatile pwr_tUInt8  bdata;          /* Byte          */
	volatile pwr_tUInt16 wdata;          /* Word          */
	volatile pwr_tUInt32 ldata;          /* Long          */
	volatile pwr_tUInt8  ffp[2];         /* Fifo pointers */
	volatile pwr_tUInt8  bbuf[4];        /* Byte array    */
	volatile pwr_tUInt16 wbuf[2];        /* Word array    */
} RAPI_DATA;


typedef struct	{
	volatile pwr_tUInt8  clas __attribute__ ((packed));           /* Device class           */
	volatile pwr_tUInt8  type __attribute__ ((packed));           /* Device type            */
	volatile pwr_tUInt8  attr __attribute__ ((packed));           /* Attribute parameter    */
	volatile pwr_tUInt8  node __attribute__ ((packed));           /* Network node address   */
	volatile pwr_tUInt16 addr __attribute__ ((packed));           /* Offset to IO registers */
} RAPI_DEV;


typedef struct	{
	volatile pwr_tUInt8  ctrl __attribute__ ((packed));
	volatile pwr_tUInt8  stat __attribute__ ((packed));
	RAPI_DATA            dreg __attribute__ ((packed));
} RAPI_IOREG;


typedef struct	{
	volatile pwr_tUInt8  rapiId[4] 	__attribute__ ((packed));	/* "RAPI" ID code          */
	volatile pwr_tUInt8  rapiRev	__attribute__ ((packed));	/* RAPI interface revision */
	volatile pwr_tUInt8  modId 	__attribute__ ((packed));       /* Module ID               */
	volatile pwr_tUInt8  modNr 	__attribute__ ((packed));       /* Module ID number        */
	volatile pwr_tUInt8  modRev 	__attribute__ ((packed));       /* Module revision         */
	volatile pwr_tUInt16 devtab 	__attribute__ ((packed));	/* offset to device table  */
	volatile pwr_tUInt16 vectab 	__attribute__ ((packed));       /* offset to vector table  */
	volatile pwr_tUInt8  res[3] 	__attribute__ ((packed));       /* reserved bytes          */
	volatile pwr_tUInt8  crc 	__attribute__ ((packed));       /* Header CRC              */
	volatile pwr_tUInt8  ctrlReg 	__attribute__ ((packed));
	volatile pwr_tUInt8  statReg 	__attribute__ ((packed));
} RAPI_HEADER;

typedef struct	{
	volatile pwr_tUInt8  cosFifoReadPtr	__attribute__ ((packed));
	volatile pwr_tUInt8  cosFifoWritePtr	__attribute__ ((packed));
	volatile pwr_tUInt8  cosFifoBuf[64]	__attribute__ ((packed));
	volatile pwr_tUInt8  irqHostFifoReadPtr	__attribute__ ((packed));
	volatile pwr_tUInt8  irqHostFifoWritePtr __attribute__ ((packed));
	volatile pwr_tUInt8  irqHostFifoBuf[16] __attribute__ ((packed));
	volatile pwr_tUInt8  irqModFifoReadPtr  __attribute__ ((packed));
	volatile pwr_tUInt8  irqModFifoWritePtr __attribute__ ((packed));
	volatile pwr_tUInt8  irqModFifoBuf[16]	__attribute__ ((packed));
	volatile pwr_tUInt16 irqVec1		__attribute__ ((packed));
	volatile pwr_tUInt16 irqVec2		__attribute__ ((packed));
#if DPM_WIDTH == 1			
	volatile pwr_tUInt8  hostMailbox	__attribute__ ((packed));   /* Swapped according to RAPI manual */
	volatile pwr_tUInt8  modMailbox		__attribute__ ((packed));   /* page 3-14						*/
#elif DPM_WIDTH == 2
	volatile pwr_tUInt16 modMailbox		__attribute__ ((packed));
	volatile pwr_tUInt16 hostMailbox	__attribute__ ((packed));
#else
	volatile pwr_tUInt32 modMailbox 	__attribute__ ((packed));
	volatile pwr_tUInt32 hostMailbox	__attribute__ ((packed));
#endif
} RAPI_IMREG;

typedef struct	{
	pwr_tUInt8  *pRapiDPM		__attribute__ ((packed));
	RAPI_HEADER *pRapiHeader	__attribute__ ((packed));
	RAPI_DEV    *pRapiDevtab	__attribute__ ((packed));	
	RAPI_IMREG  *pRapiImReg		__attribute__ ((packed));
} RAPI_MOD;


int 	    get_devsize(int dev);
int 	    rapiInit(pwr_tUInt8 *base, int irq);
int 	    rapiExit(int rapiId);
int 	    rapiModeGet(int rapiId);
int 	    rapiModeSet(int rapiId, int mode);
int 	    rapiDevSearch(int rapiId, int clas, int type, int node, int numb);
RAPI_IOREG* rapiIoRegGet(int rapiId, int dev);
int 	    rapiDevParmSet(int rapiId, int dev, int parm, int val);
int 	    rapiDevParmGet(int rapiId, int dev, int parm, int *val);
int 	    rapiDevAttrBitSet(int rapiId, int dev, int attrBitMsk);
int 	    rapiDevAttrBitClr(int rapiId, int dev, int attrBitMsk);
int 	    rapiDevCosChk(int rapiId, int dev);
int 	    rapiUpdateWait(int rapiId, int dev);
int 	    rapiDevFifoGet(int rapiId, int dev, void *pData, int size);
int 	    rapiDevFifoPut(int rapiId, int dev, void *pData, int size);
int 	    rapiDevCosGet(int rapiId);
int 	    rapiDevIrqGet(int rapiId);
int 	    rapiDevIrqSet(int rapiId, int dev);
int 	    rapiDevSizeGet(int rapiId, int dev);
	    
#ifdef __cplusplus
}
#endif

#endif 



