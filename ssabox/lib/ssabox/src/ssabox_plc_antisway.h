/* 
 * Proview   $Id: ssabox_plc_antisway.h,v 1.3 2006-04-24 13:22:24 claes Exp $
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
 **/

/*********************************************************************
**********************************************************************
*
*       S S A B  O X E L Ö S U N D
*       = = = =  = = = = = = = = =
**********************************************************************
*
* Filename      : 
*
* Description   : This header file is included by the plc modules
*		: and allows the user to include function prototypes, 
*		: defines, typedefs etc.
*                 
*               Date    Sign           Action
* Revision      040923  SSABCJ         First edition.
*               050213  jonas_h        Antisway
*
*
**********************************************************************
**********************************************************************/

#include <stddef.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
//#include "libiberty.h"

#include "pwr_baseclasses.h"
#include "pwr_basecomponentclasses.h"
#include "pwr_ssaboxclasses.h"
#include "rt_plc.h"
#include "co_time.h"
#include "pwr.h"

#include "ssabox_as_ode_solvers.h"
#include "ssabox_as_phasor_methods.h"


void Ssab_AntiSway_init(pwr_sClass_Ssab_AntiSway *object);

void Ssab_AntiSway_exec(plc_sThread *tp, pwr_sClass_Ssab_AntiSway *object);
