#ifndef rt_mh_h
#define rt_mh_h

/* rt_mh.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996,1999 by Mandator AB.

   <Description>.  */

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef pwr_class_h
#include "pwr_class.h"
#endif

#ifndef rt_mh_msg_h
#include "rt_mh_msg.h"
#endif

#ifdef __DECC
#pragma member_alignment save
#pragma nomember_alignment
#endif

typedef union {
  pwr_tUInt32	All;
  struct { pwr_Endian_4 (
    pwr_Field(pwr_tUInt16,  Status),
    pwr_Field(pwr_tUInt16,  Prio),,
  ) } Event;
} mh_uEventInfo;

#ifdef __DECC
#pragma member_alignment restore
#endif

#endif
