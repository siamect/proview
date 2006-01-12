/* 
 * Proview   $Id: remote_mq.h,v 1.1 2006-01-12 06:39:33 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef remote_mq_h
#define remote_mq_h 

/* remote_mq.h -- Runtime environment - Message Queue
*/


/* Define types used by PAMS  */
#if defined(OS_VMS) || defined(OS_ELN)

#ifndef __INTS_LOADED

#ifdef __DECC
#include <ints.h>
#else
#define __INTS_LOADED 1

typedef char           int8;
typedef unsigned char  uint8;
typedef short          int16;
typedef unsigned short uint16;

#if defined(__alpha) && defined(__osf__)
typedef signed int     int32;
typedef unsigned int   uint32;
#else
typedef long           int32;
typedef unsigned long  uint32;
#endif  /* __alpha && __osf__ */

#endif  /* __DECC             */
#endif  /* __INTS_LOADED      */




/* In PAMS 'queue' is named 'process', so do a fix  */

#ifdef OS_ELN
typedef union {
       unsigned long all;
       struct {
          unsigned short queue;
          unsigned short group;
       } au;
} PAMS_ADDRESS;
#define PAMS_ADDRESS_DEFINED

#else

/* Include p_entry.h otherwise we won't get any prototypes  */

#include "p_entry.h"
#endif

#include pams_c_process
#include pams_c_group
#include pams_c_type_class
#include pams_c_return_status_def
#include pams_c_symbol_def
#include pams_c_entry_point
#include avail_msg_def
#include sbs_msg_def



typedef union {
  unsigned long All;
  struct {
    unsigned short Queue;
    unsigned short Group;
  } au;
} mq_uAddress;

pwr_tStatus mq_AttachQueue (
  mq_uAddress  *RequestedAddress, 
  mq_uAddress *Address
); 

#endif
#endif
