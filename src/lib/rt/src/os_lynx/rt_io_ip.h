#ifndef rt_io_ip_h
#define rt_io_ip_h
/* rt_io_ip.h -- IP structures.

   PROVIEW/R
   Copyright (C) 1997 by Comator Process AB.

*/
#ifdef __cplusplus
extern "C" {
#endif

// Valid for Big endian only
typedef struct {
  unsigned char		filler_0;
  unsigned char		ascii0;  	// "I"
  unsigned char		filler_1;
  unsigned char		ascii1;     	// "P"
  unsigned char		filler_2;
  unsigned char		ascii2;         // "A"
  unsigned char		filler_3;
  unsigned char		frequence;	// "C" = 8 MHz, "H" = 32 MHz
  unsigned char		filler_4;
  unsigned char		manId;		// Manuacturer ID
  unsigned char		filler_5;
  unsigned char		modelNo;	// Model number
  unsigned char		filler_6;
  unsigned char		revision;	// Revision
  unsigned char		filler_7;
  unsigned char		reserved1;	// Reserved
  unsigned char		filler_8;
  unsigned char		driverIdLow;	// Driver ID, low byte
  unsigned char		filler_9;
  unsigned char		driverIdHigh;	// Driver ID, high byte
  unsigned char		filler_10;
  unsigned char		usedBytes;	// No of bytes used
  unsigned char		filler_11;
  unsigned char		crc;		// CRC
  unsigned char		userSpace[32];
} io_sIpId; 


#ifdef __cplusplus
}
#endif

#endif
