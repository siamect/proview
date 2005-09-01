/* 
 * Proview   $Id: rt_plc_rt.h,v 1.3 2005-09-01 14:57:56 claes Exp $
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

#ifdef OS_ELN
#  include stdlib
#  include math
#else
#  include <stdlib.h>
#  include <math.h>
#endif

#ifndef rt_plc_h
# include "rt_plc.h"
#endif

#ifndef rt_plc_proc_h
# include "rt_plc_proc.h"
#endif

/* Direct link table */
struct plc_rtdbref
{
  void		**Pointer;
  pwr_sAttrRef	AttrRef;
  pwr_tClassId	ObjType;
  pwr_tUInt32	Size;
  pwr_tUInt32	UseCode;
};

#define UC_NORMAL	0
#define UC_READ		1
#define UC_WRITE	2
#define UC_READ2	3
#define UC_WRITE2	4
