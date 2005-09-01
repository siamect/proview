/* 
 * Proview   $Id: rt_mh.h,v 1.2 2005-09-01 14:57:55 claes Exp $
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

#ifndef rt_mh_h
#define rt_mh_h

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
