/* 
 * Proview   $Id: rt_mvol.h,v 1.8 2008-10-31 12:51:31 claes Exp $
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

#ifndef rt_mvol_h
#define rt_mvol_h

/* rt_mvol.h -- Meta Volumes  */


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
  mvol_eList_objectfirst,
  mvol_eList_objectnext,
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
  pwr_tOName	       	name;	/**< Full attribute name */
} mvol_sAttribute;

/* Function prototypes.  */


mvol_sAttribute *
mvol_AnameToAttribute (
  pwr_tStatus		*sts,
  mvol_sAttribute	*ap,
  pwr_tClassId		cid,
  cdh_sParseName	*pn,
  gdb_sObject           *op
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

void
mvol_ClassListAttrRef (
  pwr_tStatus		*sts,
  pwr_tClassId		cid,
  pwr_sAttrRef		*iarp,
  pwr_sAttrRef		*oarp,
  mvol_eList		list
);

gdb_sClass *
mvol_LinkClass (
  pwr_tStatus		*sts,
  gdb_sClass		*cp,
  pwr_tBitMask		ilink
);

gdb_sClass *
mvol_LinkSubClassToAttribute (
  pwr_tStatus		*sts,
  gdb_sClass		*cp
);

gdb_sClass *
mvol_LinkObject (
  pwr_tStatus		*sts,
  gdb_sVolume		*vp,
  gdb_sObject		*op,
  pwr_tBitMask		ilink
);

gdb_sClass *
mvol_LinkScObject (
  pwr_tStatus		*sts,
  gdb_sVolume		*vp,
  gdb_sScObject		*scp
);

gdb_sClass *
mvol_NameToClass (
  pwr_tStatus		*sts,
  const char	       	*name
);

void 
mvol_BuildCatt ( 
pwr_tStatus *sts
);

#endif
