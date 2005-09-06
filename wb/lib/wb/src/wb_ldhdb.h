/* 
 * Proview   $Id: wb_ldhdb.h,v 1.3 2005-09-06 10:43:31 claes Exp $
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

#ifndef wb_ldhdb_h
#define wb_ldhdb_h

#ifndef wb_ldhi_h
#include "wb_ldhi.h"
#endif

pwr_tStatus
ldhdb_LoadObjectHeaders (
  ldhi_sWorkbench	*wb
);

pwr_tStatus
ldhdb_StoreObjects (
  ldhi_sSession		*sp
);

pwr_tStatus
ldhdb_GetOix (
  int			vid,
  ldhi_sFreeOix		*freeOix
);

pwr_tStatus
ldhdb_NextOix (
  int			vid,
  ldhi_sFreeOix		*freeOix
);

pwr_tStatus
ldhdb_SetOix (
  int			vid,
  int			oix
);

pwr_tStatus
ldhdb_AddVolume (
  int			vid
);

pwr_tStatus
ldhdb_RemoveVolume (
  int			vid
);

pwr_tStatus
ldhdb_GetObjectBodies (
  ldhi_sObjHead		*ohp
);

#endif
