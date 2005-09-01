/** 
 * Proview   $Id: wb_ldhi.h,v 1.2 2005-09-01 14:57:58 claes Exp $
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

#ifndef wb_ldhi_h
#define wb_ldhi_h

/* wb_ldhi.h 
   This include file contains the basic datastructures and
   data entities internal to LDH.

   This include file allso defines the entry points to ldhi, the
   internal ldh routines.  */

#ifndef __STDIO_LOADED
#include <stdio.h>
#endif

#ifndef __TIME_LOADED
#include <time.h>
#endif

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef pwr_class_h
#include "pwr_class.h"
#endif

#ifndef rt_load_h
#include "rt_load.h"
#endif

#ifndef wb_ldh_h
#include "wb_ldh.h"
#endif

#ifndef wb_ldh_tree_h
#include "wb_ldh_tree.h"
#endif

#define ldhi_cSizBody 65508
#define ldhi_cSizDbName	80
#define ldhi_cMaxUserOix 2147483647

#define ldhi_cAlignInc 3
#define ldhi_dAlign(size) ((size + ldhi_cAlignInc) & ~ldhi_cAlignInc)


typedef struct ldhi_s_NormName		ldhi_sNormName;
typedef struct ldhi_s_SetMember		ldhi_sSetMember;
typedef struct ldhi_s_ObjName		ldhi_sObjName;
typedef struct ldhi_s_ObjBody		ldhi_sObjBody;
typedef struct ldhi_s_ObjLink		ldhi_sObjLink;
typedef struct ldhi_s_OixEntry		ldhi_sOixEntry;
typedef struct ldhi_s_OctxEntry		ldhi_sOctxEntry;
typedef struct ldhi_s_VidEntry		ldhi_sVidEntry;
typedef struct ldhi_s_ObjDbHead		ldhi_sObjDbHead;
typedef struct ldhi_s_Binding		ldhi_sBinding;
typedef struct ldhi_s_ObjHead		ldhi_sObjHead;
typedef struct ldhi_s_Session		ldhi_sSession;
typedef struct ldhi_s_Workbench		ldhi_sWorkbench;
typedef struct ldhi_s_VolHead		ldhi_sVolHead;
typedef struct ldhi_s_VolContext	ldhi_sVolContext;
typedef struct ldhi_s_FreeOix		ldhi_sFreeOix;

typedef char ldhi_tDbName[ldhi_cSizDbName+1];
typedef unsigned long ldhi_tLockSts;
typedef pwr_tStatus (*ldhi_tSesEventCB)(
    void *EditorContext,
    ldh_sEvent *Event
  );

typedef enum {
  ldhi_eDbCallBack__ = 0,
  ldhi_eDbCallBack_AnteCreate,
  ldhi_eDbCallBack_PostCreate,
  ldhi_eDbCallBack_AnteDelete,
  ldhi_eDbCallBack_PostDelete,
  ldhi_eDbCallBack_AnteMove,
  ldhi_eDbCallBack_PostMove,
  ldhi_eDbCallBack_AnteRename,
  ldhi_eDbCallBack_PostRename,
  ldhi_eDbCallBack_AnteAdopt,
  ldhi_eDbCallBack_PostAdopt,
  ldhi_eDbCallBack_AnteUnadopt,
  ldhi_eDbCallBack_PostUnadopt,
  ldhi_eDbCallBack_AnteUpdate,
  ldhi_eDbCallBack_PostUpdate,
  ldhi_eDbCallBack_SyntaxCheck,
  ldhi_eDbCallBack_
} ldhi_eDbCallBack;

typedef union {
  struct {
    pwr_tBit IsCreated : 1;		/* Object is created */
    pwr_tBit IsDeleted : 1;		/* Object is deleted */
    pwr_tBit IsMoved : 1;		/* Object is moved */
    pwr_tBit IsRenamed : 1;		/* Object is renamed */
    pwr_tBit BrotherIsNew : 1;		/* Object has new brother */
    pwr_tBit RBodyIsModified :1;	/* Runtime body is changed */
    pwr_tBit DBodyIsModified :1;	/* Development body is changed */
    pwr_tBit FlagsAreModified : 1;	/* Object flags are changed */
    pwr_tBit FatherIsNew : 1;		/* Object has new brother */
    pwr_tBit IsTouched : 1;		/* Object has been touched but must
					   not necessarily be saved. */
  } f;

  pwr_tBitMask	m;

#define ldhi_mChange_IsCreated		  1
#define ldhi_mChange_IsDeleted		  2
#define ldhi_mChange_IsMoved		  4
#define ldhi_mChange_IsRenamed		  8
#define ldhi_mChange_BrotherIsNew	 16
#define ldhi_mChange_RBodyIsModified     32
#define ldhi_mChange_DBodyIsModified     64
#define ldhi_mChange_FlagsAreModified   128
#define ldhi_mChange_FatherIsNew	256
#define ldhi_mChange_IsTouched		512
#define ldhi_mChange_Created		(1 | 4 | 8 | 16 | 32 | 64 | 128 | 256)
#define ldhi_mChange_HeadIsModified	(4 | 8 | 16 | 128)
#define ldhi_mChange_BodyIsModified	(32 | 64)
#define ldhi_mChange_RtHeadIsModified	(1 | 8 | 256)
} ldhi_mChange;

typedef union {
  struct {
    pwr_tBit CanBeLoaded : 1;
    pwr_tBit Load : 1;
    pwr_tBit HeadIsInOldFile : 1;
    pwr_tBit UseOldBody : 1;
    pwr_tBit LoadBody : 1;
    pwr_tBit FreeBody : 1;
    pwr_tBit OldBodyIsWritten : 1;
    pwr_tBit MountClean : 1;
  } f;

  pwr_tBitMask m;

#define ldhi_mLoad_CanBeLoaded		1
#define ldhi_mLoad_Load			2
#define ldhi_mLoad_HeadIsInOldFile	4
#define ldhi_mLoad_UseOldBody		8
#define ldhi_mLoad_LoadBody		16
#define ldhi_mLoad_FreeBody		32
#define ldhi_mLoad_OldBodyIsWritten	64
#define ldhi_mLoad_MountClean		128
} ldhi_mLoad;


typedef enum {
  ldhi_eBid__ = -1,
  ldhi_eBid_RtBody = 0,
  ldhi_eBid_SysBody = 0,
  ldhi_eBid_DevBody = 1,
  ldhi_eBid_NoBody = 2,
  ldhi_eBid_
} ldhi_eBid;

/* A name packed to 4 bytes. */

typedef union {
  pwr_tUInt32		key;
  struct {
    pwr_tUInt16		len;		/* Size of name. */
    char		first;		/* First char of name. */
    char		last;		/* Last char of name. */
  } cont;				
} ldhi_uPackName;

struct ldhi_s_NormName {
  char			*username;	/* User-provided name string */
  char			*namebuf;	/* Buffer containing our string info (malloced) */
  char			*volume;	/* Volume pointer */
  char			*object;
  char			*body;
  pwr_tObjid		parent;		/* Parent objid, or NOBJID */
  char			*parstr;	/* Parameter pointer */
  pwr_tUInt32		index;		/* Index if positive */
  pwr_tUInt32		offset;
  pwr_tUInt32		size;
  pwr_tUInt32		nseg;		/* # of segments in name */
  char			*seg[1];	/* start of each segment. Terminated with an extra NULL entry, i.e. nseg+1 slots needed */
};

#define	ldhi_mNormName_NoPath	1	/* Suppress parent info */
#define	ldhi_mNormName_NoAttr	2	/* Suppress parameter info */
#define	ldhi_mNormName_NoIndex	4	/* Suppress parameter index info */
#define	ldhi_mNormName_NoVolume	8	/* Suppress volume */

#if 0
struct ldhi_s_IdName {
  char			*username;	/* User-provided name string */
  char			*namebuf;	/* Buffer containing our string info (malloced) */
  char			idtype;		/* A, C, O, T, V, X */
  char			*volume;
  char			*body;
  char			*attr;
  pwr_tInt32		index;		/* Index if positive */
  pwr_tUInt32		nseg;		/* # of segments in name */
  char			*seg[1];	/* start of each segment. Terminated with an extra NULL entry, i.e. nseg+1 slots needed */
};
#endif

typedef struct {
  ldh_sTree		*tp;
  int			nRoot;		/* Number of elements in array below.  */
  ldhi_sSetMember	*frlp;
  ldhi_sSetMember	*brlp;
} ldhi_sSet;

struct ldhi_s_SetMember {
  ldh_sTreeNode		treeNode;
  pwr_tBoolean		isTop;
  ldhi_sSetMember	*frlp;
  pwr_tObjid		oldOid;
  ldhi_sObjHead		*oohp;
  pwr_tObjid		newOid;
  ldhi_sObjHead		*nohp;
};

struct ldhi_s_ObjName {
  pwr_tUInt16		len;
  pwr_tObjName		data;
  ldhi_uPackName	packname;
};

struct ldhi_s_ObjBody {
  pwr_tUInt32		size;		/* Body size. */
  char			*body;		/* Pointer to body. */
  int			exist;
};

struct ldhi_s_ObjLink {
  ldhi_sObjHead		*fwhp;		/* Pointer to forward header. */
  ldhi_sObjHead		*bwhp;		/* Pointer to backward header. */
};

struct ldhi_s_OixEntry {
  ldh_sTreeNode		treeNode;
  pwr_tObjectIx		oix;
  ldhi_sObjHead		*ohp;
};

struct ldhi_s_OctxEntry {
  ldh_sTreeNode		treeNode;
  ldh_sObjContext	octx;
};

struct ldhi_s_VidEntry {
  ldh_sTreeNode		treeNode;
  pwr_tVolumeId		vid;
  ldhi_sVolHead		*vhp;
};

struct ldhi_s_ObjDbHead {
    pwr_tOid          oid;
    pwr_tCid          cid;
  ldhi_sObjName		name;		/* Name of object segment */
  ldhi_sObjName		normname;	/* Name of object segment */
  pwr_tOid		poid;
  pwr_tOid		bwsoid;		/* Previous object among siblings. */
  pwr_tOid		fwsoid;		/* Next object among siblings. */
    pwr_tTime time;
  pwr_tGeneration	rh_gen;		/* Runtime header generation */
  pwr_tGeneration	dh_gen;
  pwr_tGeneration	b_gen[2];
  pwr_tTime	b_time[2];
  pwr_tUInt32		b_siz[2];
  pwr_mClassDef		flags;
  char			dbkey[8];	/* DBKEY to object */
};

struct ldhi_s_Binding {
  ldh_sTreeNode		treeNode;
  pwr_tString80		methodName;
  pwr_tStatus		(*method)();
};

struct ldhi_s_ObjHead {
  ldhi_sObjHead		*hdlst;		/* Next head in database headlist. */
  ldhi_mChange		change;		/* Object change mask. */
  ldhi_sObjDbHead	db;		/* Reflects the current state of the object. */
  ldhi_sObjDbHead	dbcpy;		/* Reflects the database state of the object. */
  ldhi_sVolHead		*vhp;
  ldhi_sObjHead		*fahp;		/* Pointer to objhd of father or NULL */
  ldhi_sObjHead		*chhp;		/* Pointer to objhd of first child or NULL */
  ldhi_sObjHead		*tmphp;		/* If ClassDef, pointer to template objhd. */
  ldhi_sObjHead		*chp;		/* Pointer to class objhd. */
  ldhi_sObjLink		silst;		/* Sibling list. */
  ldhi_sObjBody		body[2];	/* List of all bodies owned by object. */

  ldhi_mLoad		load;		/* Object load flags. */
  pwr_tStatus		(*dbCallBack[ldhi_eDbCallBack_])();
  ldhi_sSetMember	*smp;
};

struct ldhi_s_Session {
  clock_t		time;
  ldhi_sSession	*seslst;	/* List of sessions. */
  ldhi_sWorkbench	*wb;
  ldhi_sVolHead		*vhp;
  ldhi_sVolContext	*volctx;
  ldh_sTree		*oixtab;	/* Table of modified objects */
  ldh_sTree		*octxtab;	/* Table of object contexts */
  ldh_eAccess		access;
  ldh_eUtility		utility;
  ldh_sEvent		*fep;
  ldh_sEvent		*lep;
  void			*editorContext;
  ldhi_tSesEventCB	sendThisSession;
  ldhi_tSesEventCB	sendOtherSession;
};

struct ldhi_s_Workbench {
  ldh_sTree		*vidtab;	/* Volume table */
#if 0
  ldhi_sObjTree		*treeroot;	/* Root to tree of volumes,  */
  ldhi_sObjHead		*rootlst;	/* List of root volume objects.  */
#endif
  ldh_sTree		*bindtab;	/* Binding table.  */
  ldhi_tDbName		wbname;
  ldhi_sObjHead		*hdlst;		/* List of all heads to be read/written to DB.  */
  ldhi_sObjBody		*bdlst;		/* List of all bodies to be read/written to DB.  */
  ldh_sEvent		*fep;		/* First free event */
  ldh_sEvent		*lep;		/* Last free event */
  ldhi_sSet		*sep;
};

struct ldhi_s_FreeOix {
  pwr_tObjectIx		next;
  pwr_tObjectIx		high;
};

struct ldhi_s_VolHead {
  pwr_tVolumeId		vid;
  ldh_sTree		*oixtab;	/* Oix table of volume.  */
  ldhi_sObjHead		*ohp;		/* Root volume object header.  */
  ldhi_sVolContext	*volctx;	/* Attached volume context.  */
  ldhi_sFreeOix		freeOix;
};

struct ldhi_s_VolContext {
  ldhi_sVolHead		*vhp;		/* Volume of this attachment.  */
  ldhi_sObjHead		*hdlst;		/* List of all heads to be read/written to DB. */
  ldhi_sObjBody		*bdlst;		/* List of all bodies to be read/written to DB. */
  ldhi_sWorkbench	*wb;
  ldhi_sSession	*utility[ldh_eUtility_];	/* Session lists */
};

extern struct ldhi_s_Workbench ldhi_gWB;



/*============================================================================*\
  Function prototypes to exported functions.
\*============================================================================*/

pwr_tStatus
ldhi_AllocNormName (
  pwr_tObjid		parent,
  char			*instr,
  ldhi_sNormName	**nname
);

pwr_tStatus
ldhi_BindMethod (
  char			*className,
  char			*methodName,
  pwr_tStatus		(*entry)()
);

pwr_tStatus
ldhi_BodyNameToBid (
  char			*name,
  ldhi_eBid		*bid
);

pwr_tStatus
ldhi_ConnectObject (
  ldhi_sSession		*sp,
  ldhi_sObjHead		*ohp
);

pwr_tStatus
ldhi_CopyObjectBody (
  ldhi_sSession		*sp,
  ldhi_sObjHead		*ohp,
  ldhi_sObjHead		*shp,
  ldhi_sObjHead		*clbhp
);

pwr_tStatus
ldhi_CopyObjectFamily (
  ldhi_sSession		*sp,
  ldhi_sObjHead		*oohp,
  ldhi_sObjHead		**nohp,
  ldh_sTree		*rtp,
  int			istop
);

pwr_tStatus
ldhi_CreateObjectBody (
  ldhi_sSession		*sp,
  ldhi_sObjHead		*ohp,
  ldhi_sObjHead		*clbhp
);

pwr_tStatus
ldhi_DeleteObjectTree (
  ldhi_sSession		*sp,
  ldhi_sObjHead		*ohp
);

pwr_tStatus
ldhi_EvalClassDef (
  ldhi_sSession		*sp,
  ldhi_sObjHead		*chp
);

pwr_tStatus
ldhi_EvalObjectBodyDef (
  ldhi_sSession		*sp,
  ldhi_sObjHead		*obhp
);

pwr_tStatus
ldhi_EvalParamDef (
  ldhi_sSession		*sp,
  ldhi_sObjHead		*php,
  int			offset,
  int			*csize,
  int			index
);

ldhi_eDbCallBack
ldhi_FindDbCallBack (
  pwr_tString32		name
);

pwr_tStatus
ldhi_FindSiblingByName (
  ldhi_sSession		*sp,
  ldhi_sObjHead		*ohp,
  ldhi_sObjName		*nn,
  ldhi_sObjHead		**shp
);

pwr_tStatus
ldhi_FixObjectReferences (
  ldhi_sSession 	*sp,
  ldhi_sObjHead 	*ohp,
  ldhi_sObjHead 	*fphp,
  ldh_sTree 		*rtp,
  ldhi_sSetMember 	*smp,
  void *bp,
  pwr_tUInt32 		offset
);

void
ldhi_FreeNormName (
  ldhi_sNormName	*nname
);

pwr_tStatus
ldhi_FreeObjectBodies (
  ldhi_sSession		*sp,
  ldhi_sObjHead		*ohp
);

pwr_tStatus
ldhi_GetObjectBody (
  ldhi_sSession		*sp,
  ldhi_sObjHead		*ohp,
  ldhi_eBid		bid,
  ldhi_sObjBody		**obp
);

pwr_tStatus
ldhi_GetUniqueOix (
  ldhi_sSession		*sp,
  pwr_tObjectIx		*oix
);

pwr_tStatus
ldhi_GetUniqueObjectName (
  ldhi_sSession		*sp,
  pwr_tObjectIx		oix,
  char			*name
);

pwr_tStatus
ldhi_AdoptObject (
  ldhi_sSession		*sp,
  ldhi_sObjHead		*ohp,		/* Object head to be inserted. */
  ldhi_sObjHead		*dhp,		/* Object head of destination. */
  ldh_eDest		dest
);

pwr_tStatus
ldhi_MapAttribute (
  ldhi_sSession		*sp,
  pwr_sAttrRef		*aref,
  void			**ap
);

pwr_tStatus
ldhi_MarkObject (
  ldhi_sSession		*sp,
  pwr_tBitMask		change,
  ldhi_sObjHead		*ohp
);

pwr_tStatus
ldhi_MarkObjectAsSaved (
  ldhi_sSession		*sp,
  ldhi_sObjHead		*ohp
);

pwr_tStatus
ldhi_MarkObjectForSave (
  ldhi_sSession		*sp,
  ldhi_sObjHead		*ohp
);

pwr_tStatus
ldhi_NormNamePretty (
  ldhi_sNormName	*nn,
  pwr_tUInt32		attr,
  char			*namebuf,
  pwr_tInt32		namebuflen
);

pwr_tStatus
ldhi_NormNamePrettylen (
  ldhi_sNormName	*nn,
  pwr_tUInt32		attr
);

pwr_tStatus
ldhi_SetNormObjName (
  ldhi_sObjName		*nn,
  char			*name
);

pwr_tStatus
ldhi_NormalizePathName (
  char			*outbuf,
  char			*instr,
  long			bufsiz
);

pwr_tStatus
ldhi_ObjidToOh (
  ldhi_sWorkbench	*wb,
  pwr_tObjid		objid,
  ldhi_sObjHead		**objhdp
);

pwr_tStatus
ldhi_RemoveObjhd (
  ldhi_sSession		*sp,
  ldhi_sObjHead		*ohp,
  int			mark
);

pwr_tStatus
ldhi_RevertObject (
  ldhi_sSession		*sp,
  ldhi_sObjHead		*ohp
);

pwr_tStatus
ldhi_SetObjName (
  ldhi_sObjName		*nn,
  char			*name
);

pwr_tStatus
ldhi_UnadoptObject (
  ldhi_sSession		*sp,
  ldhi_sObjHead		*ohp,
  int			mark
);

pwr_tStatus
ldhi_UnNatObjectName (
  char			*outbuf,
  char			*instr,
  unsigned long int	bufsiz
);

pwr_tStatus
ldhi_StringToObjid (
  char			*s,
  pwr_tObjid		*oid
);

char *
ldhi_TrimName (
  char			*name
);

pwr_tStatus
ldhi_VolumeNameToOh (
  ldhi_sSession		*sp,
  char			*name,
  ldhi_sObjHead		**ohp
);

char *
ldhi_ArefToAname (
  ldhi_sSession		*sp,
  ldhi_sObjHead		*ohp,
  pwr_sAttrRef		*arp
);

char *
ldhi_PathName (
  ldhi_sObjHead		*ohp,
  int			volume
);

ldhi_sObjHead *
ldhi_AllocObjHead (
			void
);

void
ldhi_FreeObjHead (
  ldhi_sObjHead		*ohp
);

ldhi_sVolHead *
ldhi_AllocVolHead (
			void
);

void
ldhi_FreeVolHead(ldhi_sVolHead *vhp);

pwr_tStatus
ldhi_AssignOix (
  ldhi_sSession		*sp,
  pwr_tObjectIx		*oix,
  pwr_tClassId		cid,
  ldhi_sObjHead		*chp,
  ldhi_sObjHead		*fhp,	/* Father object header.  */
  char			*name
);

pwr_tStatus
ldhi_NewSet(ldhi_sSession *sp, ldhi_sSet **sep, pwr_sAttrRef *arp, ldh_eDest dest);

pwr_tStatus
ldhi_CheckSetDestination (
  ldhi_sSession         *sp,
  ldhi_sSet             *sep,
  pwr_tObjid            doid,
  ldh_eDest             dest,
  pwr_tBoolean          self,
  ldhi_sObjHead         **dhp
);

pwr_tStatus
ldhi_PopulateSet (
  ldhi_sSession 	*sp,
  ldhi_sSet 		*sep
);


pwr_tStatus
ldhi_CopySet (
  ldhi_sSet 		*sep,
  pwr_tBoolean		replace
);

pwr_tStatus
ldhi_InstalSet (
);

pwr_tStatus
ldhi_DeleteSet (
  ldhi_sSet 		*sep,
  pwr_tBoolean		deleteOld,
  pwr_tBoolean		deleteNew
);


#endif
