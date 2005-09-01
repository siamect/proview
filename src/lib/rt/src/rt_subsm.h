/* 
 * Proview   $Id: rt_subsm.h,v 1.2 2005-09-01 14:57:56 claes Exp $
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

#ifndef rt_subsm_h
#define rt_subsm_h

/* rt_subsm.h -- Object data subscription, server side */

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef rt_qcom_h
#include "rt_qcom.h"
#endif

#ifndef rt_gdb_h
#include "rt_gdb.h"
#endif

#ifndef rt_sub_h
#include "rt_sub.h"
#endif

void
subsm_ActivateBuffer (
  sub_sBuffer		*bp,
  pwr_tUInt32		dt
);

void
subsm_Add (
  qcom_sGet		*get
);

void
subsm_FlushNode (
  pwr_tStatus		*sts,
  gdb_sNode		*np
);

void
subsm_Remove (
  qcom_sGet		*get
);

pwr_tBoolean
subsm_SendBuffer (
  sub_sBuffer		*bp
);

#endif
