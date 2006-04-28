/* 
 * Proview   $Id: rt_plc_macro.h,v 1.4 2006-04-28 04:59:58 claes Exp $
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

#if defined OS_ELN
#include $mutex
#endif

#include <math.h>
#include <float.h>

#include "pwr.h"
#include "pwr_systemclasses.h"
#include "pwr_baseclasses.h"
#include "co_time.h"
#include "rt_plc.h"
#include "rt_plc_io.h"
#include "rt_plc_timer.h"
#include "rt_plc_data.h"
#include "rt_plc_macro_logic.h"
#include "rt_plc_macro_io.h"
#include "rt_plc_macro_grafcet.h"
#include "rt_plc_macro_sup.h"
#include "rt_plc_macro_div.h"
#include "rt_plc_macro_mps.h"
#include "rt_plc_macro_string.h"
#include "rt_plc_macro_time.h"
#include "ra_plc_user.h"

