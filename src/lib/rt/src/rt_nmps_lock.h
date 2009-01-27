/* 
 * Proview   $Id: rt_nmps_lock.h,v 1.1 2008-01-25 14:32:44 claes Exp $
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

#ifndef	rt_nmps_lock_h
#define rt_nmps_lock_h

#if defined __cplusplus
extern "C" {
#endif

#include "pwr.h"
#include "rt_sect.h"

#define nmps_cName_Lock  "/tmp/pwr_nmps_lock"
#define nmps_Lock sect_Lock( NULL, nmps_locksect, (sect_sMutex *)nmps_locksect->base);
#define nmps_Unlock sect_Unlock( NULL, nmps_locksect, (sect_sMutex *)nmps_locksect->base);

extern sect_sHead *nmps_locksect;

void nmps_create_lock( pwr_tStatus *sts);
void nmps_delete_lock( pwr_tStatus *sts);
void nmps_unlink_lock( pwr_tStatus *sts);

#if defined __cplusplus
}
#endif
#endif
