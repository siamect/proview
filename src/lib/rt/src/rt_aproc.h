/* 
 * Proview   $Id: rt_aproc.h,v 1.3 2005-09-01 14:57:55 claes Exp $
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

#ifndef rt_aproc_h
#define rt_aproc_h

#if defined __cplusplus
extern "C" {
#endif

#include "pwr.h"
#include "rt_errh.h"

pwr_tStatus	aproc_RegisterObject(pwr_tOid);
pwr_tStatus	aproc_TimeStamp( float scantime, float maxdelay);

#if defined __cplusplus
}
#endif

#endif


