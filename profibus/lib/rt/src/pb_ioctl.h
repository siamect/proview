/*****************************************************************************/
/*                                                                           */
/*                    SOFTING Industrial Automation GmbH                     */
/*                        Richard-Reitzner-Allee 6                           */
/*                              D-85540 Haar                                 */
/*                      Phone: (++49)-(0)89-45656-0                          */
/*                      Fax:   (++49)-(0)89-45656-399                        */
/*                                                                           */
/*         Copyright (C) SOFTING Industrial Automation GmbH 1995-2012        */
/*                            All Rights Reserved                            */
/*                                                                           */
/*****************************************************************************/
/*****************************************************************************/
/*                          PROFIBUS LINUX DRIVERS                           */
/*                                                                           */
/*  Filename    : PB_IOCTL.H                                                 */
/*  Version     : 5.45.0.00.release                                          */
/*  Date        : December-2009                                              */
/*  Author      : SOFTING                                                    */
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
#define IOCTL_PROFI_GET_DPRAM 0x6901
#define IOCTL_PROFI_SET_TIMEOUT 0x6902
#define IOCTL_PROFI_OPTIMIZED_MODE 0x6903
#define IOCTL_PROFI_SERIAL_NUMBER 0x6904
#define IOCTL_PROFI_STATUS_DATA 0x6905
#define IOCTL_PROFI_GET_DATA_IMAGE 0x6906
#define IOCTL_PROFI_SET_SLAVE_IN 0x6907
#define IOCTL_PROFI_DP_OPERATE 0x6908
#define IOCTL_PROFI_SET_DEBUGLEVEL 0x6909

#ifdef __KERNEL__
#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 32)
long pb_ioctl(struct file* file, unsigned int cmd, unsigned long arg);
#else
int pb_ioctl(struct inode* inode, struct file* file, unsigned int cmd,
             unsigned long arg);
#endif
#endif

/*****************************************************************************/
/* Structure for data image description                                      */
/*****************************************************************************/

// #pragma pack(push,1)

typedef struct _StatusAndData
{
  void* StatusImage;
  __s32 StatusLength;
  void* Data;
  __s32 DataLength;
} __attribute__((aligned(8))) StatusAndData;

typedef struct _NTIoctl
{
  void* InBuf;
  __s32 InBufLength;
  void* OutBuf;
  __s32 OutBufLength;
} __attribute__((aligned(8))) NTIoctl;

typedef struct _DataImage
{
  __s32 id;
  void* Buf;
  __u16 Length; // sizeof( Buf )
} __attribute__((aligned(8))) DataImage;
// #pragma pack(pop)

/*****************************************************************************/
/* Operation Mode as (FMS/DPV1-Master or DP-Slave)                           */
/*****************************************************************************/

#define FMS_DPV1_MASTER_MODE 0x00 // FMS/DPV1-Master
#define DPV1_MASTER_MODE 0x01     // for future use
#define DPV1_SLAVE_MODE 0x02      // DP-Slave

#endif
