/* 
 * Proview   $Id: rt_mh_log.h,v 1.2 2005-09-01 14:57:56 claes Exp $
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

#ifndef rt_mh_log_h
#define rt_mh_log_h

/* rt_mh_log.h -- Log utilities */

#include <stdio.h>

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef rt_qcom_h
#include "rt_qcom.h"
#endif

#ifndef rt_mh_def_h
#include "rt_mh_def.h"
#endif

#ifndef rt_mh_outunit_h
#include "rt_mh_outunit.h"
#endif

#define mhLog 0


FILE *mh_LogFileOpen (
  qcom_sQid *Address
);


void mh_LogMessage (
  FILE *fp,
  pwr_tStatus sts,
  mh_eMsg Type,
  mh_sMsgInfo *Msg,
  qcom_sQid *Address
);

#endif
