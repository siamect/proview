#ifndef wb_trv_h
#define wb_trv_h

/* wb_trv.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#ifndef wb_ldh_h
#include "wb_ldh.h"
#endif

typedef enum {
  trv_eDepth_Deep,
  trv_eDepth_Children,
  trv_eDepth_Self
} trv_eDepth;
  
typedef int (*trv_tBcFunc)( pwr_sAttrRef *, void *, void *, void *, void *, void*);

typedef struct {
	  ldh_tSesContext	ldhses;
	  pwr_tObjid		hierobjid;
	  pwr_tClassId		*classid;
	  char			*name;
	  pwr_tVolumeId		*volume;
	} *trv_ctx;

int trv_get_rtnodes (
  ldh_tSesContext ldhses,
  unsigned long	*rtnode_count,
  pwr_tObjid	**rtnodelist
);

int trv_get_plcpgms (
  ldh_tSesContext ldhses,
  unsigned long	*plc_count,
  pwr_tObjid	**plclist
);

int trv_get_plc_window ( 
  ldh_tSesContext ldhses,
  pwr_tObjid	  plc,
  pwr_tObjid	  *window
);

int trv_get_window_windows (
  ldh_tSesContext ldhses,
  pwr_tObjid	window,
  unsigned long	*wind_count,
  pwr_tObjid	**windlist
);

int trv_get_window_objects (
  ldh_tSesContext ldhses,
  pwr_tObjid	window,
  unsigned long	*object_count,
  pwr_tObjid	**objectlist
);

int trv_get_window_connections (
  ldh_tSesContext ldhses,
  pwr_tObjid	window,
  unsigned long	*object_count,
  pwr_tObjid	**objectlist
);

int trv_get_parentlist (
  ldh_tSesContext ldhses,
  pwr_tObjid	object,
  unsigned long	*object_count,
  pwr_tObjid	**objectlist
);

int trv_get_objects_hier_class_name (
  ldh_tSesContext ldhses,
  pwr_tObjid	hierobjdid,
  pwr_tClassId	*classid,
  char		*name,
  int		(*backcall)(),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
);

/* cpp call to trv_get_objects_hier_class_namn */
int trv_get_objects_hcn (
  ldh_tSesContext ldhses,
  pwr_tObjid	hierobjdid,
  pwr_tClassId	*classid,
  char		*name,
  int		(*backcall)( pwr_tObjid, void *, void *, void *, void *, void*),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
);

int trv_get_objects_class (
  ldh_tSesContext ldhses,
  pwr_tClassId	classid,
  int		(*backcall)(),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
);

int trv_get_children_class_name (
  ldh_tSesContext ldhses,
  pwr_tObjid	parentobjdid,
  pwr_tClassId	*classid,
  char		*name,
  int		(*backcall)(),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
);

int trv_get_class_hier (
  ldh_tSesContext ldhses,
  pwr_tObjid	hierobjdid,
  char		*name,
  pwr_tClassId	*classid,
  int		(*backcall)(),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
);


int trv_get_docobjects ( 
  ldh_tSesContext ldhses,
  pwr_tObjid	hierobjdid,
  int		(*backcall)(),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
);

int trv_object_search(
  trv_ctx	trvctx,
  int		(*backcall)(),
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
);

int	trv_create_ctx( 
  trv_ctx		*trvctx,
  ldh_tSesContext	ldhses,
  pwr_tObjid		hierobjid,
  pwr_tClassId		*classid,
  char			*name,
  pwr_tVolumeId		*volume
);

int	trv_delete_ctx( 
  trv_ctx		trvctx
);

int trv_get_attrobjects (
  ldh_tSesContext ldhses,
  pwr_tOid	oid,
  pwr_tCid	*cid,
  char		*name,
  trv_eDepth	depth,
  trv_tBcFunc	backcall,
  void		*arg1,
  void		*arg2,
  void		*arg3,
  void		*arg4,
  void		*arg5
);

#endif

