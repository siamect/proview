/* 
 * Proview   $Id: wb_ldhld.h,v 1.3 2005-09-06 10:43:31 claes Exp $
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

#ifndef wb_ldhld_h
#define wb_ldhld_h

/* wb_ldhld.h
   This module contains functions to create load data files.  */

#ifndef pwr_class_h
#include "pwr_class.h"
#endif
#ifndef wb_ldh_h
#include "wb_ldh.h"
#endif

/*============================================================================*\
  Exported functions.
\*============================================================================*/

// Fix
pwr_tStatus ldhld_CreateLoadFile (
  ldh_tSession Session
);

pwr_tStatus ldhld_Diff (
  char *NodeName,
  pwr_tProjVersion NewDnoVersion,
  char *DiffFileName,
  char *DECnetNodeName
);
#endif



