/*****************************************************************************/
/*                                                                           */
/*                               SOFTING AG                                  */
/*                        Richard-Reitzner-Allee 6                           */
/*                              D-85540 Haar                                 */
/*                      Phone: (++49)-(0)89-45656-0                          */
/*                      Fax:   (++49)-(0)89-45656-399                        */
/*                                                                           */
/*                    Copyright (C) SOFTING AG 1995-2003                     */
/*                            All Rights Reserved                            */
/*                                                                           */
/*****************************************************************************/
/*****************************************************************************/
/*                       PROFIBUS WINDOWS NT DRIVERS                         */
/*                                                                           */
/*  Filename    : PB_IOCTL.H                                                 */
/*  Version     : 5.26.0.01.release                                          */
/*  Date        : 30-April-2003                                              */
/*  Author      : SOFTING AG                                                 */
/*                                                                           */
/*  Description : This file contains the global declarations                 */
/*                to access the PROFIBUS Drivers for Linux                   */
/*                                                                           */
/*****************************************************************************/

#ifndef PB_IOCTL_H
#define PB_IOCTL_H



/*****************************************************************************/
/* Definitions of IO Control codes                                           */
/*****************************************************************************/

// IOCTL-Magic for Softing-PROFIBUS is 0x69
#define IOCTL_PROFI_GET_DPRAM      0x6901
#define IOCTL_PROFI_SET_TIMEOUT    0x6902
#define IOCTL_PROFI_OPTIMIZED_MODE 0x6903
#define IOCTL_PROFI_SERIAL_NUMBER  0x6904
#define IOCTL_PROFI_STATUS_DATA    0x6905
#define IOCTL_PROFI_GET_DATA_IMAGE 0x6906
#define IOCTL_PROFI_SET_SLAVE_IN   0x6907
#define IOCTL_PROFI_DP_OPERATE     0x6908
#define IOCTL_PROFI_SET_DEBUGLEVEL 0x6909

#ifdef __KERNEL__
extern int pb_ioctl( struct inode *inode, struct file *file, unsigned int cmd,
    unsigned long arg );
#endif

/*****************************************************************************/
/* Structure for data image description                                      */
/*****************************************************************************/

typedef struct _StatusAndData {
  void *StatusImage;
  int StatusLength;
  void *Data;
  int DataLength;
} StatusAndData;

typedef struct _NTIoctl {
  void *InBuf;
  int InBufLength;
  void *OutBuf;
  int OutBufLength;
} NTIoctl;

typedef struct _DataImage {
  int id;
  void *Buf;
  unsigned short Length; // sizeof( Buf )
} DataImage;

/*****************************************************************************/
/* Operation Mode as (FMS/DPV1-Master or DP-Slave)                           */
/*****************************************************************************/

#define FMS_DPV1_MASTER_MODE       0x00                      // FMS/DPV1-Master
#define DPV1_MASTER_MODE           0x01                       // for future use
#define DPV1_SLAVE_MODE            0x02                             // DP-Slave

#endif
