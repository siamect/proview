/* 
 * Proview   $Id: rt_dlinux.h,v 1.2 2005-09-01 14:57:55 claes Exp $
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

/* rt_dlinux.h -- Encode VMS-data
   Functions to convert data to loaded in file
   from Linux to VAX format */


#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef pwr_class_h
#include "pwr_class.h"
#endif

#if defined(OS_LINUX)
 extern pwr_tBoolean (*dlinux[pwr_eTix_])();
#endif
