#ifndef rt_ivol_h
#define rt_ivol_h

/* rt_ivol.h -- Initiate Volumes

   PROVIEW/R
   Copyright (C) 1997 by Comator Process AB.

   .  */

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef co_tree_h
# include "co_tree.h"
#endif

#ifndef co_dbs_h
#include "co_dbs.h"
#endif

#ifndef rt_gdb_h
# include "rt_gdb.h"
#endif

#ifndef rt_vol_h
# include "rt_vol.h"
#endif

#ifndef rt_lst_h
# include "rt_lst.h"
#endif

typedef struct {
  tree_sNode		tree;
  lst_sEntry		ll;
  pwr_tBoolean		isVolRef;
  pwr_tObjName		name;
  pwr_tVolumeId		vid;
  pwr_tTime             time;
  char			filename[256];
  dbs_sVolume		volume;
  dbs_sVolRef		volRef;
  gdb_sVolume		*vp;
  lst_sEntry		cre_lh;
  lst_sEntry		upd_lh;
  lst_sEntry		upd_io_lh;
  tree_sTable		*oid_t;
} ivol_sVolume;

typedef struct {
  pwr_tNodeId		nid;
  pwr_tObjid		nod_oid;
  pwr_tVolumeId		vid;
  pwr_tUInt32		cardinality;
  pwr_tUInt32		bodySize;
  pwr_tUInt32		cvolMaxCount;
  pwr_tUInt32		cvolMinCount;
  pwr_tProjVersion	plcVersion;
  pwr_tProjVersion	rtVersion;
  pwr_tTime		rtCreTime;
} ivol_sNode;

typedef struct {
  tree_sNode		tree;
  gdb_sObject		*op;
  dbs_sObject		oh;
  pwr_tObjectIx		oix;
  vol_mLink		unlink;
  vol_mLink		link;
  gdb_mChange		flags;
  lst_sEntry		obj_ll;
#if 0
  lst_sEntry		seg_lh;
#else
  gdb_sClass		*cp;
  void			*upd_body;
#endif
  pwr_tUInt32		size;
  void			*body;
} ivol_sObject;

typedef struct {
  gdb_sObject		*op;
  pwr_tUInt32		size;
  void			*body;
} ivol_sBody;

typedef struct {
  pwr_tUInt32		offs;
  pwr_tUInt32		size;
  lst_sEntry		seg_ll;
} ivol_sSegment;


/* Function prototypes.  */

void		ivol_BuildNode (pwr_tStatus*, ivol_sNode*, const co_mFormat*);
pwr_tBoolean	ivol_BuildVolume (pwr_tStatus*, gdb_sVolume*);
void		ivol_CopyBody (pwr_tStatus*, void*, void*, gdb_sClass*);
pwr_tBoolean	ivol_DecodeBody (pwr_tStatus*, void*, gdb_sClass*);
pwr_tBoolean	ivol_DeleteVolume (pwr_tStatus*, gdb_sVolume*);
ivol_sBody     *ivol_GetBody (pwr_tStatus*, pwr_tObjid, ivol_sBody*);
pwr_tBoolean	ivol_InitiateVolumeUpdate (pwr_tStatus*, ivol_sVolume*);
gdb_sObject    *ivol_LoadObject (pwr_tStatus*, ivol_sVolume*, dbs_sObject*, pwr_tBitMask);
gdb_sScObject  *ivol_LoadScObject (pwr_tStatus*, ivol_sVolume*, dbs_sScObject*, pwr_tBitMask);
gdb_sVolume    *ivol_LoadVolume (pwr_tStatus*, dbs_sVolume*, const co_mFormat*);
pwr_tBoolean	ivol_RebuildVolume (pwr_tStatus*, ivol_sVolume*, const co_mFormat*);

#endif
