/* 
 * Proview   $Id: rt_vol.h,v 1.5 2006-09-14 14:16:07 claes Exp $
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

#ifndef rt_vol_h
#define rt_vol_h

/* rt_vol.h -- Volume

   This module contains the generic access routines to volumes.
   Loaded volumes can be either fully mounted or structure mounted.
   If the volume is mounted on the owner node, it has to be fully
   mounted. This is also the case if the volume is shared.
   A volume owned by another node can either be structure mounted
   or cahce mounted. Cache mounted volumes are handeled by rt_cvol.  */


#ifdef	OS_ELN
#include $vaxelnc
#endif

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef co_cdh_h
#include "co_cdh.h"
#endif

#ifndef rt_qcom_h
#include "rt_qcom.h"
#endif

#ifndef rt_gdb_h
#include "rt_gdb.h"
#endif

#ifndef rt_mvol_h
#include "rt_mvol.h"
#endif

#ifndef rt_cvol_h
#include "rt_cvol.h"
#endif

typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( init			, 1),
    pwr_Bits( fill			, 3),,,
    pwr_Bits( goAdopt			, 1),
    pwr_Bits( go_fill			, 3),,,

    pwr_Bits( loObjList			, 1),
    pwr_Bits( loOidTab			, 1),
    pwr_Bits( loFamilyTab		, 1),
    pwr_Bits( loParentRef		, 1),
    pwr_Bits( loVolumeRef		, 1),
    pwr_Bits( loMountServer		, 1),
    pwr_Bits( lo_fill			, 2),,

    pwr_Bits( noSibList			, 1),
    pwr_Bits( noCidList			, 1),
    pwr_Bits( noAliasClientList		, 1),
    pwr_Bits( noMountClientList		, 1),
    pwr_Bits( noBody			, 1),
    pwr_Bits( noSub			, 1),
    pwr_Bits( noSan			, 1),
    pwr_Bits( no_fill			, 1),

    pwr_Bits( coTouch			, 1),
    pwr_Bits( co_fill			, 7),,,,,,
  ) b;

#define vol_mLink__			0
#define vol_mLink_init			pwr_Bit(0)
#define vol_mLink_goAdopt		pwr_Bit(4)

#define vol_mLink_loObjList		pwr_Bit(8)
#define vol_mLink_loOidTab		pwr_Bit(9)
#define vol_mLink_loFamilyTab		pwr_Bit(10)
#define vol_mLink_loParentRef		pwr_Bit(11)
#define vol_mLink_loVolumeRef		pwr_Bit(12)
#define vol_mLink_loMountServer		pwr_Bit(13)

#define vol_mLink_noSibList		pwr_Bit(16)
#define vol_mLink_noCidList		pwr_Bit(17)
#define vol_mLink_noAliasClientList	pwr_Bit(18)
#define vol_mLink_noMountClientList	pwr_Bit(19)
#define vol_mLink_noBody		pwr_Bit(20)
#define vol_mLink_noSub			pwr_Bit(21)
#define vol_mLink_noSan			pwr_Bit(22)

#define vol_mLink_coTouch		pwr_Bit(24)
#define vol_mLink_			(~vol_mLink__)

#define vol_mLink_go_all	(15<<4)
#define vol_mLink_lo_all	(255<<8)
#define vol_mLink_no_all	(255<<16)
#define vol_mLink_co_all	(255<<24)

#define	vol_mLink_volumeDelete	0
#define	vol_mLink_rebuild	0
#define	vol_mLink_flush		0

#define	vol_mLink_load		(vol_mLink_init|vol_mLink_loObjList|vol_mLink_loOidTab|vol_mLink_loFamilyTab\
				|vol_mLink_loParentRef|vol_mLink_loVolumeRef)

#define	vol_mLink_swapLoad	(vol_mLink_init|vol_mLink_loVolumeRef|vol_mLink_loOidTab)

#define	vol_mLink_build		(vol_mLink_goAdopt|vol_mLink_noSibList|vol_mLink_loMountServer|vol_mLink_noCidList\
				|vol_mLink_noAliasClientList|vol_mLink_noMountClientList|vol_mLink_noBody)

#define	vol_mLink_swapBuild	(vol_mLink_goAdopt|vol_mLink_loObjList|vol_mLink_loFamilyTab\
				|vol_mLink_loParentRef|vol_mLink_noSibList|vol_mLink_loMountServer|vol_mLink_noCidList\
				|vol_mLink_noAliasClientList|vol_mLink_noMountClientList|vol_mLink_noBody)

#define	vol_mLink_rename	(vol_mLink_loFamilyTab)

#define	vol_mLink_create	(vol_mLink_init|vol_mLink_loObjList|vol_mLink_loOidTab\
				|vol_mLink_noSibList|vol_mLink_loParentRef|vol_mLink_loVolumeRef\
				|vol_mLink_loMountServer|vol_mLink_noCidList|vol_mLink_noAliasClientList\
				|vol_mLink_noMountClientList|vol_mLink_noBody|vol_mLink_noSub|vol_mLink_noSan)

#define	vol_mLink_delete	(vol_mLink_init|vol_mLink_loObjList|vol_mLink_loOidTab|vol_mLink_loFamilyTab\
				|vol_mLink_noSibList|vol_mLink_loParentRef|vol_mLink_loVolumeRef\
				|vol_mLink_loMountServer|vol_mLink_noCidList|vol_mLink_noAliasClientList\
				|vol_mLink_noMountClientList|vol_mLink_noBody|vol_mLink_noSub|vol_mLink_noSan)

#define	vol_mLink_swapDelete	(vol_mLink_init|vol_mLink_goAdopt|vol_mLink_loObjList|vol_mLink_loOidTab|vol_mLink_loFamilyTab\
				|vol_mLink_noSibList|vol_mLink_loParentRef|vol_mLink_loVolumeRef\
				|vol_mLink_loMountServer|vol_mLink_noCidList|vol_mLink_noAliasClientList\
				|vol_mLink_noMountClientList|vol_mLink_noBody|vol_mLink_noSub|vol_mLink_noSan)

#define	vol_mLink_cacheDelete	(vol_mLink_loObjList|vol_mLink_loOidTab|vol_mLink_loFamilyTab\
				|vol_mLink_loParentRef|vol_mLink_loVolumeRef|vol_mLink_loMountServer)

#define	vol_mLink_cacheTrim	vol_mLink_cacheDelete
#define	vol_mLink_cacheMove	(vol_mLink_loFamilyTab)
#define	vol_mLink_cacheFlush	vol_mLink_cacheDelete

} vol_mLink;



/** Link mask for sub classes */

typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( init			, 1),
    pwr_Bits( scList			, 1),
    pwr_Bits( scTab			, 1),
    pwr_Bits( parentRef		        , 1),
    pwr_Bits( volumeRef		        , 1),
    pwr_Bits( classRef		        , 1),
    pwr_Bits( sibList			, 1),
    pwr_Bits( cidList			, 1),

    pwr_Bits( body			, 1),
    pwr_Bits( fill_1			, 7),,,,,,,

    pwr_Bits( fill_2			, 8),,,,,,,,

    pwr_Bits( fill_3			, 8),,,,,,,
  ) b;

#define vol_mLinkSc__			0
#define vol_mLinkSc_init		pwr_Bit(0)
#define vol_mLinkSc_scList		pwr_Bit(1)
#define vol_mLinkSc_scTab		pwr_Bit(2)
#define vol_mLinkSc_parentRef		pwr_Bit(3)
#define vol_mLinkSc_volumeRef		pwr_Bit(4)
#define vol_mLinkSc_classRef		pwr_Bit(5)
#define vol_mLinkSc_sibList		pwr_Bit(6)
#define vol_mLinkSc_cidList		pwr_Bit(7)

#define vol_mLinkSc_body	        pwr_Bit(8)

#define vol_mLinkSc_			(~vol_mLinkSc__)



#define	vol_mLinkSc_load		(vol_mLinkSc_init|vol_mLinkSc_scList|vol_mLinkSc_scTab\
				        |vol_mLinkSc_parentRef|vol_mLinkSc_volumeRef)


#define	vol_mLinkSc_build		(vol_mLinkSc_sibList|vol_mLinkSc_cidList\
				        |vol_mLinkSc_body|vol_mLinkSc_classRef)
} vol_mLinkSc;




typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( privat		, 1),
    pwr_Bits( native		, 1),
    pwr_Bits( fileCached	, 1),
    pwr_Bits( netCached		, 1),
    pwr_Bits( fill0		, 4),,,,

    pwr_Bits( fill1		, 8),,,,,,,,
    pwr_Bits( fill2		, 8),,,,,,,,
    pwr_Bits( fill3		, 8),,,,,,,
  ) b;

#define vol_mLoad__		0
#define vol_mLoad_private	pwr_Bit(0)
#define vol_mLoad_native	pwr_Bit(1)    
#define vol_mLoad_fileCached	pwr_Bit(2)
#define vol_mLoad_netCached	pwr_Bit(3)
#define vol_mLoad_		(~vol_mLoad__)

#define vol_mLoad_build		(vol_mLoad_native)
#define vol_mLoad_rebuild	(vol_mLoad_native)
} vol_mLoad;

/* Translation.
   If 'Alias' is set, then alias clients will be translated.
   If "Mount' is set, then mount clients will be translated.  */

typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( alias		, 1 ),
    pwr_Bits( mount		, 1 ),
    pwr_Bits( fill0		, 6 ),,,,,,

    pwr_Bits( fill1		, 8 ),,,,,,,,
    pwr_Bits( fill2		, 8 ),,,,,,,,
    pwr_Bits( fill3		, 8 ),,,,,,,
  ) b;

#define vol_mTrans__		0
#define vol_mTrans_none		0
#define vol_mTrans_alias	pwr_Bit(0)
#define vol_mTrans_mount	pwr_Bit(1)
#define vol_mTrans_all		(vol_mTrans_alias | vol_mTrans_mount)
#define vol_mTrans_		(~vol_mTrans__)

} vol_mTrans;

/* Function prototypes.  */


gdb_sAliasServer *
vol_AddAliasClient (
  pwr_tStatus		*sts,
  gdb_sObject		*op
);

gdb_sMountServer *
vol_AddMountClient (
  pwr_tStatus           *sts,
  gdb_sObject		*op
);

gdb_sMountServer *
vol_AddMountServer (
  pwr_tStatus           *sts,
  pwr_tObjid		oid
);

mvol_sAttribute *
vol_ArefToAttribute (
  pwr_tStatus		*sts,
  mvol_sAttribute	*ap,
  pwr_sAttrRef		*arp,
  pwr_tBitMask		local,
  pwr_tBitMask		trans
);

void *
vol_AttributeToAddress (
  pwr_tStatus		*sts,
  mvol_sAttribute	*ap
);

char *
vol_AttributeToName (
  pwr_tStatus		*sts,
  mvol_sAttribute	*ap,
  pwr_tBitMask		nametype,
  char			*s
);

pool_tRef
vol_AttributeToReference (
  pwr_tStatus		*sts,
  mvol_sAttribute	*ap
);

void
vol_CancelDlink (
  pwr_tStatus		*sts,
  pwr_tDlid		dlid
);

void
vol_CancelUserDlinks (
  qcom_sQid		user
);

gdb_sObject *
vol_InitiateObject (
  pwr_tStatus		*sts,
  gdb_sObject		*op,
  gdb_sVolume		*vp
);

void
vol_InsertSiblist (
  pwr_tStatus		*sts,
  gdb_sObject		*op,
  gdb_sObject		*p_op
);

gdb_sObject *
vol_LinkObject (
  pwr_tStatus		*sts,
  gdb_sVolume		*vp,
  gdb_sObject		*op,
  pwr_tBitMask		ilink
);

gdb_sScObject *
vol_LinkScObject (
  pwr_tStatus		*sts,
  gdb_sVolume		*vp,
  gdb_sScObject		*op,
  pwr_tBitMask		ilink
);

gdb_sObject *
vol_LoadObject (
  pwr_tStatus		*sts,
  gdb_sVolume		*vp,
  char			*name,
  pwr_tObjid		oid,
  pwr_tClassId		cid,
  pwr_tUInt32		size,
  pwr_tObjid		poid,
  pwr_tBitMask		iflags,
  pwr_tObjid		soid,
  void			*bodyp
);

gdb_sVolume *
vol_LoadVolume (
  pwr_tStatus		*sts,
  pwr_tVolumeId		vid,
  char			*name,
  pwr_tClassId		cid,
  pwr_tNodeId		nid,
  pwr_tBitMask		iload
);

gdb_sVolume *
vol_MountVolume (
  pwr_tStatus		*sts,
  pwr_tVolumeId		vid
);

mvol_sAttribute *
vol_NameToAttribute (
  pwr_tStatus		*sts,
  mvol_sAttribute	*ap,
  cdh_sParseName	*pn,
  pwr_tBitMask		local,
  pwr_tBitMask		trans
);

gdb_sObject *
vol_NameToParentObject (
  pwr_tStatus		*sts,
  cdh_sParseName	*pn,
  pwr_tBitMask		lo_flags,
  pwr_tBitMask		trans
);

gdb_sObject *
vol_NameToObject (
  pwr_tStatus		*sts,
  cdh_sParseName	*pn,
  pwr_tBitMask		lo_flags,
  pwr_tBitMask		trans
);

char *
vol_ObjectToAddress (
  pwr_tStatus		*sts,
  gdb_sObject		*op
);

char *
vol_ObjectToName (
  pwr_tStatus		*sts,
  gdb_sObject		*op,
  pwr_tBitMask		nametype,
  char			*buff
);

pwr_tObjid
vol_Oid (
  pwr_tStatus		*sts,
  gdb_sVolume		*vp,
  pwr_tClassId		cid
);

gdb_sObject *
vol_OidToObject (
  pwr_tStatus		*sts,
  pwr_tObjid		oid,
  pwr_tBitMask		in_db,
  pwr_tBitMask		trans,
  cvol_eHint		hint		/* Give a hint to the cache handler.  */
);

gdb_sObject *
vol_OixToObject (
  pwr_tStatus		*sts,
  pwr_tVolumeId		vid,
  pwr_tObjectIx		oix,
  pwr_tBitMask		local,
  pwr_tBitMask		trans,
  cvol_eHint		hint		/* Give a hint to the cache handler.  */
);

void
vol_PropagateAlarmLevel (
  pwr_tStatus		*sts,
  gdb_sObject		*op,
  pwr_tBoolean		l_maxa,
  pwr_tUInt32		old_maxa,
  pwr_tUInt32		new_maxa,
  pwr_tBoolean		propagate_alias
);

void
vol_PropagateBlockLevel (
  pwr_tStatus		*sts,
  gdb_sObject		*op,
  pwr_tBoolean		l_maxb,
  pwr_tUInt32		omblvl,
  pwr_tUInt32		nmblvl,
  pwr_tBoolean		propagate_alias
);

void
vol_RemoveMountClient (
  pwr_tStatus           *sts,
  gdb_sObject		*op
);

gdb_sMountServer *
vol_RemoveMountServer (
  pwr_tStatus           *sts,
  pwr_tObjid		oid
);

void
vol_RemoveSiblist (
  pwr_tStatus		*sts,
  gdb_sObject		*op,
  gdb_sObject		*pop
);

void
vol_SetAlarmLevel (
  pwr_tStatus		*sts,
  gdb_sObject		*op,
  pwr_tUInt32		alvl
);

void
vol_SetBlockLevel (
  pwr_tStatus		*sts,
  gdb_sObject		*op,
  pwr_tUInt32		blvl
);

gdb_sObject *
vol_FamilyToObject (
  pwr_tStatus		*sts,
  char			*name,
  pwr_tObjid		poid
);

gdb_sObject *
vol_TranslateObject (
  pwr_tStatus		*sts,
  gdb_sObject		*op,
  pwr_tBitMask		in_db,
  pwr_tBitMask		trans
);

void
vol_UnlinkObject (
  pwr_tStatus		*sts,
  gdb_sVolume		*vp,
  gdb_sObject		*op,
  pwr_tBitMask		link
);

void
vol_UpdateAlarm (
  pwr_tStatus		*sts,
  gdb_sObject		*op,
  net_sAlarm		al
);

pwr_tDisableAttr
vol_ArefDisabled (
  pwr_tStatus 		*sts,
  pwr_sAttrRef		*arp
);
#endif
