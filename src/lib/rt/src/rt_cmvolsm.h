/* 
 * Proview   $Id: rt_cmvolsm.h,v 1.2 2005-09-01 14:57:55 claes Exp $
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

/* rt_cmvolsm.c -- Cached volumes
   This module contains the meta cache handling routines for the server monitor.  */


#ifndef rt_cmvolsm_h
#define rt_cmvolsm_h

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef rt_qcom_h
# include "rt_qcom.h"
#endif

void
cmvolsm_GetCclass (
  qcom_sGet		*get
  );

void
cmvolsm_GetGclass (
  qcom_sGet		*get
  );

#endif
