#ifndef rt_mvol_h
#define rt_mvol_h

/* rt_mvol.h -- Meta Volumes

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   .  */


#ifdef	OS_ELN
#include $vaxelnc
#endif

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef co_cdh_h
#include "co_cdh.h"
#endif

#ifndef rt_gdb_h
#include "rt_gdb.h"
#endif

#ifndef rt_cvol_h
#include "rt_cvol.h"
#endif

typedef enum {
  mvol_eList__ = 0,
  mvol_eList_first,
  mvol_eList_last,
  mvol_eList_next,
  mvol_eList_prev,
  mvol_eList_
} mvol_eList;

typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( fill	, 32),,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
  ) b;

#define mvol_mLink__		0
#define mvol_mLink_		(~mvol_mLink__)

} mvol_mLink;


typedef struct {
  gdb_sObject		*op;
  gdb_sClass		*cp;
  gdb_sObject		*cop;
  gdb_sObject		*bop;
  pwr_sClassDef		*cdef;
  pwr_sObjBodyDef	*bdef;
  gdb_sObject		*aop;
  pwr_sParam		*adef;
  pwr_tAix		aix;
  pwr_tUInt32		idx;		/* Index within one attribute, -1 denotes no index.  */
  pwr_tUInt32		offs;
  pwr_tUInt32		size;
  pwr_tUInt32		elem;
  pwr_tTypeId		tid;
  pwr_mAttrRef		flags;
} mvol_sAttribute;

/* Function prototypes.  */


mvol_sAttribute *
mvol_AnameToAttribute (
  pwr_tStatus		*sts,
  mvol_sAttribute	*ap,
  pwr_tClassId		cid,
  cdh_sParseName	*pn
);

mvol_sAttribute *
mvol_ArefToAttribute (
  pwr_tStatus		*sts,
  mvol_sAttribute	*ap,
  pwr_sAttrRef		*arp,
  pwr_tClassId		cid
);

pwr_sAttrRef *
mvol_AttributeToAref (
  pwr_tStatus		*sts,
  mvol_sAttribute	*ap,
  pwr_sAttrRef		*arp
);

gdb_sObject *
mvol_ClassList (
  pwr_tStatus		*sts,
  pwr_tClassId		cid,
  pwr_tObjid		oid,
  mvol_eList		list
);

gdb_sClass *
mvol_LinkClass (
  pwr_tStatus		*sts,
  gdb_sClass		*cp,
  pwr_tBitMask		ilink
);

gdb_sClass *
mvol_LinkObject (
  pwr_tStatus		*sts,
  gdb_sVolume		*vp,
  gdb_sObject		*op,
  pwr_tBitMask		ilink
);

gdb_sClass *
mvol_NameToClass (
  pwr_tStatus		*sts,
  char			*name
);

#endif
