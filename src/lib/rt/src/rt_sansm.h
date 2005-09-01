/* 
 * Proview   $Id: rt_sansm.h,v 1.2 2005-09-01 14:57:56 claes Exp $
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

#ifndef rt_sansm_h
#define rt_sansm_h

/* rt_sansm.h -- Subscribed alarm (and block) notification, server monitor calls. */

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef rt_net_h
# include "rt_net.h"
#endif

#ifndef rt_qcom_h
# include "rt_qcom.h"
#endif

#ifndef rt_gdb_h
# include "rt_gdb.h"
#endif

void
sansm_Add (
  qcom_sGet		*get
);

void
sansm_Check ();

void
sansm_FlushNode (
  pwr_tStatus		*sts,
  gdb_sNode		*np
);

void
sansm_Remove (
  qcom_sGet		*get
);

pwr_tUInt32
sansm_Update (
  gdb_sNode		*np
);

#endif
