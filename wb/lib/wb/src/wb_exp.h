/* 
 * Proview   $Id: wb_exp.h,v 1.3 2005-09-06 10:43:31 claes Exp $
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

#ifndef wb_exp_h
#define wb_exp_h

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef wb_ldh_h
#include "wb_ldh.h"
#endif

pwr_tStatus exp_window (
  ldh_tWBContext	ldhwb,
  ldh_tSesContext	ldhses,
  char			*window_str,
  char			*output
);
pwr_tStatus exp_plcpgm (
  ldh_tSesContext	ldhses,
  char			*plcpgm_str,
  char			*output
);

#endif
