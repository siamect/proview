#ifndef wb_ldhdb_h
#define wb_ldhdb_h

/* wb_ldhdb.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

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
