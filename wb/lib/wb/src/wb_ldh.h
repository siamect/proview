#ifndef wb_ldh_h
#define wb_ldh_h

/* wb_ldh.h -- local data handler

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   This include file contains definitions and function prototypes
   needed to use LDH.  */

#ifndef pwr_class_h
#include "pwr_class.h"
#endif

#ifndef co_cdh_h
#include "co_cdh.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define ldh_cWbdbVersionStr	  "V2.7.0"
#define ldh_cWBVol  (0 + ((pwr_tVolumeId)254 << 24) + (254 << 16) + (254 << 8) + 254) 
#define ldh_cProjectVolume  (0 + ((pwr_tVolumeId)254 << 24) + (254 << 16) + (254 << 8) + 253)

#if 0
#ifndef 0
typedef void		   	*ldh_tSession;
typedef void		      	*ldh_tWorkbench;
typedef void		     	*ldh_tVolContext;
#else
typedef struct ldhi_s_Session  	  *ldh_tSession;
typedef struct ldhi_s_Workbench	  *ldh_tWorkbench;
typedef struct ldhi_s_VolContext  *ldh_tVolContext;
#endif
#endif

typedef void *ldh_tSession;
typedef ldh_tSession ldh_tSesContext;    
typedef void *ldh_tWorkbench;
typedef ldh_tWorkbench ldh_tWBContext;    
typedef void *ldh_tVolume;
typedef ldh_tVolume ldh_tVolContext;

typedef struct ldh_s_Event	ldh_sEvent;
typedef struct ldh_s_MenuItem	ldh_sMenuItem;
typedef struct ldh_s_MenuCall	ldh_sMenuCall;
typedef struct ldh_s_ObjContext	ldh_sObjContext;
typedef struct ldh_s_ObjInfo	ldh_sObjInfo;
typedef struct ldh_s_VolumeInfo	ldh_sVolumeInfo;
typedef struct ldh_s_ParDef	ldh_sParDef;
typedef struct ldh_s_RefInfo	ldh_sRefInfo;
typedef struct ldh_s_RefUsage	ldh_sRefUsage;
typedef struct ldh_s_SessInfo	ldh_sSessInfo;
typedef struct ldh_s_LCB	ldh_sLCB;

typedef union ldh_m_MenuFlags	ldh_mMenuFlags;

typedef enum {
  ldh_eAccess__ = 0,
  ldh_eAccess_ReadOnly,
  ldh_eAccess_ReadWrite,
  ldh_eAccess_
} ldh_eAccess;

typedef enum {
  ldh_eDest__ = 0,
  ldh_eDest_IntoFirst,	/* Object inserted as first son. */
  ldh_eDest_IntoLast,	/* Object inserted as last son. */
  ldh_eDest_After,	/* Object inserted after destination object.*/
  ldh_eDest_Before,	/* Object inserted before destination object. */
  ldh_eDest_
} ldh_eDest;

typedef enum {
  /* The objid-range is divided into 3 groups. */
  ldh_eDId__ = -1,
  ldh_eDId_System = 0,	/* System objects. */
  ldh_eDId_Base = 1,	/* Base objects. */
  ldh_eDId_User = 2,	/* User objects. */
  ldh_eDId_
} ldh_eDId;

typedef enum {
  ldh_eEvent__ = 0,
  ldh_eEvent_ObjectCreated,
  ldh_eEvent_ObjectCopied,
  ldh_eEvent_ObjectDeleted,
  ldh_eEvent_ObjectMoved,
  ldh_eEvent_ObjectRenamed,

  ldh_eEvent_AttributeModified,
  ldh_eEvent_BodyModified,

  ldh_eEvent_SessionReverted,
  ldh_eEvent_SessionSaved,

  ldh_eEvent_MenuMethodCalled,
  ldh_eEvent_ObjectTreeCopied,

  ldh_eEvent_
} ldh_eEvent;

typedef enum {
  ldh_eMenuItem__ = 0,
  ldh_eMenuItem_Button,
  ldh_eMenuItem_Cascade,
  ldh_eMenuItem_Separator,
  ldh_eMenuItem_
} ldh_eMenuItem;

typedef enum {
  ldh_eMenuSet__ = 0,
  ldh_eMenuSet_Attribute,
  ldh_eMenuSet_Class,
  ldh_eMenuSet_Many,
  ldh_eMenuSet_None,
  ldh_eMenuSet_Object,
  ldh_eMenuSet_
} ldh_eMenuSet;

typedef enum {
  ldh_eName_Object      = cdh_mName_object,
  ldh_eName_Default     = cdh_mName_object,
  ldh_eName_Hierarchy   = cdh_mName_pathStrict,
  ldh_eName_Path        = cdh_mName_path,
  ldh_eName_VolPath     = cdh_mName_volumeStrict,
  ldh_eName_Volume      = cdh_mName_volume,
  ldh_eName_Objid       = cdh_mName_form_id | cdh_mName_idString | cdh_mName_volume | cdh_mName_object,
  ldh_eName_ObjectIx    = cdh_mName_form_id | cdh_mName_idString | cdh_mName_object,
  ldh_eName_OixString   = cdh_mName_form_id | cdh_mName_object,
  ldh_eName_VolumeId    = cdh_mName_form_id | cdh_mName_idString | cdh_mName_volume,
  ldh_eName_VidString   = cdh_mName_form_id | cdh_mName_volume,
  ldh_eName_Aref        = cdh_mName_object | cdh_mName_attribute,
  ldh_eName_ArefVol     = cdh_mName_volume | cdh_mName_object | cdh_mName_attribute,
  ldh_eName_ArefExport  = cdh_mName_form_id | cdh_mName_volume | cdh_mName_object | cdh_mName_attribute
} ldh_eName;
#if 0
typedef enum {		/* Object and attribute name types. */
  ldh_eName__ = 0,
  ldh_eName_Object,	/*		   Cobj		*/

  ldh_eName_Hierarchy,	/*	 Aobj-Bobj-Cobj
			    _O0.123.34.63:1234567890	*/

  ldh_eName_Path,	/*	 Aobj-Bobj		*/

  ldh_eName_VolPath,	/*  Avol:Aobj-Bobj-Cobj
			    _O0.123.34.63:1234567890	*/

  ldh_eName_Volume,	/*  Avol
			    _V0.123.34.63		*/

  ldh_eName_Objid,	/*  _O0.123.34.63:1234567890	*/

  ldh_eName_VolumeId,	/*  _V0.123.34.63		*/

  ldh_eName_VidString,	/*  0.123.34.63			*/

  ldh_eName_ObjectIx,	/*  _X1234567890		*/

  ldh_eName_OixString,	/*  1234567890			*/

  ldh_eName_ClassId,	/*  _C0.1:34			*/

  ldh_eName_TypeId,	/*  _T0.1:0.34.1    Classbody
			    _T0.1:1.0.34    Type
			    _T0.1:1.1.12    Typedef
			    _T0.1:1.2.123   Struct
			    _T0.1:1.3.56    Union
			    _T0.1:1.4.53    Enum
			    _T0.1:1.5.36    Bitmask	*/

  ldh_eName_Aref,	/*       Aobj-Bobj-Cobj.FilterAttribute[2]
			    _A0.123.34.63:1234567890(pwrb:Class-Ai-RtBody)FilterAttribute[2]
			    _A0.123.34.63:1234567890(_T0.1:0.34.1)[60.4]  */

  ldh_eName_ArefVol,	/*  Avol:Aobj-Bobj-Cobj.FilterAttribute[2]
			    _A0.123.34.63:1234567890(pwrb:Class-Ai-RtBody)FilterAttribute[2]
			    _A0.123.34.63:1234567890(_T0.1:0.34.1)[60.4]  */

  ldh_eName_ArefExport, /*  _A0.123.34.63:1234567890(pwrb:Class-Ai-RtBody)FilterAttribute[2] */

  ldh_eName_ArefId,	/*  _A0.123.34.63:1234567890(_T0.1:0.34.1)[60.4]  */

  ldh_eName_Default,	/*  Tries to give the most intuitive name.  */
  ldh_eName_
} ldh_eName;
#endif

typedef enum {
  ldh_eUtility__ = 0,
  ldh_eUtility_Navigator,
  ldh_eUtility_Configurator,
  ldh_eUtility_Loader,
  ldh_eUtility_PlcEditor,
  ldh_eUtility_Pwr,
  ldh_eUtility_Login,
  ldh_eUtility_CreateVolume,
  ldh_eUtility_
} ldh_eUtility;

union ldh_m_MenuFlags {
  struct {
    pwr_tBit Sensitive : 1;
  } f;

  pwr_tBitMask	m;

#define ldh_mMenuFlags_Sensitive  1
};

struct ldh_s_RefUsage {
  unsigned int		Total;
  unsigned int		Used;
  unsigned int		Errors;
};

struct ldh_s_RefInfo {
  ldh_sRefUsage		ObjRef;
  ldh_sRefUsage		ObjXRef;
  ldh_sRefUsage		AttrRef;
  ldh_sRefUsage		AttrXRef;
};

struct ldh_s_Event {
  ldh_eEvent		Event;
  pwr_tObjid		Object;
  ldh_eDest		Dest;
  pwr_tObjid		OldParent;
  pwr_tObjid		OldLsibling;
  pwr_tObjid		OldRsibling;
  pwr_tObjid		NewParent;
  pwr_tObjid		NewLsibling;
  pwr_tObjid		NewRsibling;
  ldh_sEvent		*nep;
  ldh_sEvent		*pep;
};

struct ldh_s_MenuItem {
  pwr_tString40		Name;
  pwr_tUInt32		Level;
  ldh_eMenuItem		Item;
  ldh_mMenuFlags	Flags;
  pwr_tStatus		(*Method)();
  pwr_tObjid		MenuObject;
};

struct ldh_s_MenuCall {
  void			*EditorContext;
  void			*WindowContext;
  ldh_eMenuSet		PointedSet;
  ldh_tSession	PointedSession;
  pwr_sAttrRef		Pointed;
  ldh_eMenuSet		SelectedSet;
  ldh_tSession	SelectedSession;
  pwr_sAttrRef		*Selected;
  pwr_tUInt32		SelectCount;
  ldh_sMenuItem		*ItemList;
  pwr_tUInt32		ItemCount;
  pwr_tUInt32		ChosenItem;
};

struct ldh_s_SessInfo {
  ldh_eAccess		Access;
  ldh_eDId		DidRange;
  ldh_eUtility		Utility;
  pwr_tBoolean		Empty;
};

struct ldh_s_ObjContext {
  pwr_tObjid		objid;
  int			active;
  void			*modctx;
};

struct ldh_s_ObjInfo {
  pwr_tVolumeId		Volume;
};

struct ldh_s_VolumeInfo {
  pwr_tVolumeId		Volume;
  pwr_tClassId		Class;
  pwr_tProjVersion	Version;
};

struct ldh_s_ParDef {
  pwr_tObjName		ParName;
  unsigned long		ParLevel;
  pwr_eClass		ParClass;
  pwr_uParDef		*Par;
};

struct ldh_s_LCB {
  struct {
    pwr_tBoolean	Load;
    pwr_tUInt32		RowNr;
    char		String[256];
  } Row[259];
};


/*============================================================================*\
  Function prototypes to exported functions.
\*============================================================================*/

pwr_tStatus
ldh_AttachVolume (
  ldh_tWorkbench	Workbench,
  pwr_tVolumeId		vid,
  ldh_tVolContext	*VolContext
);

pwr_tStatus
ldh_SAttachVolume (
  ldh_tWorkbench	Workbench,
  pwr_tVolumeId		vid,
  ldh_tVolContext	*VolContext
);

pwr_tStatus
ldh_AttrRefToName (
  ldh_tSession	Session,
  pwr_sAttrRef		*AttrRef,
  ldh_eName		NameType,
  char			**Name,
  int			*Size
);

pwr_tStatus ldh_CallMenuMethod (
  ldh_sMenuCall *CallStruct,
  int Index
);

pwr_tStatus ldh_ChangeObjectName (
  ldh_tSession Session,
  pwr_tObjid object,
  char *newname
);

pwr_tStatus ldh_CheckAttrXRef (
  ldh_tSession   Session,
  pwr_sAttrRef	    *PattrRef
);

pwr_tStatus ldh_CheckAttrXRefs (
  ldh_tSession   Session,
  pwr_sAttrRef	    *PattrRef,
  pwr_sAttrRef	    *SattrRef
);

pwr_tStatus ldh_CheckObjXRef (
  ldh_tSession   Session,
  pwr_tObjid	    Pobject,
  pwr_tObjName	    PattrName
);

pwr_tStatus ldh_CheckObjXRefs (
  ldh_tSession   Session,
  pwr_tObjid	    Pobject,
  pwr_tObjName	    PattrName,
  pwr_tObjid	    Sobject,
  pwr_tObjName	    SattrName
);

pwr_tStatus
ldh_ClassIdToName (
  ldh_tSession	Session,
  pwr_tClassId		Class,
  char			*namebuf,
  int			maxsize,
  int			*size
);

pwr_tStatus ldh_ClassNameToId (
  ldh_tSession Session,
  pwr_tClassId *Class,
  char *name
);

pwr_tStatus ldh_ClassNameToObjid (
  ldh_tSession Session,
  pwr_tObjid *objid,
  char *name
);

pwr_tStatus ldh_CloseSession (
  ldh_tSession Session
);

pwr_tStatus ldh_CloseWB (
  ldh_tWorkbench workbench
);

pwr_tStatus ldh_ConnectObjects (
  ldh_tSession Session,
  pwr_tObjid src_object,
  pwr_tObjid dst_object
);

pwr_tStatus ldh_ConnectObjectByXRef (
  ldh_tSession Session,
  pwr_tObjid srcdid		/* objid of source object. */
);

pwr_tStatus ldh_Copy (
  ldh_tSession	Session,
  pwr_sAttrRef		*aref
);

pwr_tStatus ldh_CopyObject (
  ldh_tSession Session,
  pwr_tObjid *object,
  char *name,
  pwr_tObjid source,
  pwr_tObjid destination,
  ldh_eDest destination_code
);

pwr_tStatus ldh_CopyObjectTrees (
  ldh_tSession   Session,
#if 0 // def __cplusplus
  pwr_sAttrRef	    (*AttrRef)[1],
#else
  pwr_sAttrRef	    *AttrRef,
#endif
  pwr_tObjid	    destobject,
  ldh_eDest	    dest,
  pwr_tBoolean	    Self
);

pwr_tStatus ldh_CreateObject (
  ldh_tSession Session,
  pwr_tObjid *object,
  char *name,
  pwr_tClassId Class,
  pwr_tObjid destination,
  ldh_eDest destination_code
);

pwr_tStatus
ldh_CreateVolume (
  ldh_tWorkbench    Workbench,
  ldh_tSession   *Session,
  pwr_tVolumeId	    Volume,
  char		    *Name,
  pwr_tClassId	    Class
);

pwr_tStatus ldh_Cut (
  ldh_tSession	session,
  pwr_sAttrRef		*aref
);

pwr_tStatus
ldh_DefineObject (
  ldh_tSession	Session,
  pwr_tObjid		*Objid,
  char			*Name,
  pwr_tObjectIx		ObjectIx,
  pwr_tClassId		Class,
  pwr_tObjid		DestinationObject,
  ldh_eDest		DestinationCode,
  int			CreateBody	/* Are bodies to be created or not ? */
);

pwr_tStatus ldh_DefineObjectBody (
  ldh_tSession Session,
  pwr_tObjid object,
  char *bodyname,
  char *body,
  int size
);
pwr_tStatus ldh_DeleteObject (
  ldh_tSession Session,
  pwr_tObjid object
);
pwr_tStatus ldh_DeleteObjectTree (
  ldh_tSession Session,
  pwr_tObjid object
);

pwr_tStatus
ldh_DeleteVolume (
  ldh_tWorkbench	Workbench,
  pwr_tVolumeId		Volume
);

pwr_tStatus
ldh_DetachVolume (
  ldh_tWorkbench	Workbench,
  ldh_tVolContext	VolContext
);

pwr_tStatus ldh_DisconnectObjects (
  ldh_tSession Session,
  pwr_tObjid srcobject
);
pwr_tStatus ldh_EvalObject (
  ldh_tSession Session,
  pwr_tObjid object
);
pwr_tStatus ldh_GetAttrDef (
  ldh_tSession Session,
  pwr_tClassId Class,
  char *BodyName,
  char *AttrName,
  ldh_sParDef *AttrDef
);
pwr_tStatus ldh_GetAttrRef (
  ldh_tSession Session,
  pwr_tObjid Object,
  char *AttrName,
  pwr_sAttrRef *AttrRef
);
pwr_tStatus ldh_GetAttrXRefDef (
  ldh_tSession Session,
  pwr_sAttrRef *AttrRef,
  pwr_sAttrXRef *AttrXRef
);
pwr_tStatus ldh_GetChild (
  ldh_tSession Session,
  pwr_tObjid object,
  pwr_tObjid *new_object
);
pwr_tStatus ldh_GetClassBody (
  ldh_tSession Session,
  pwr_tClassId Class,
  char *bodyname,
  pwr_tClassId *bodyclass,
  char **body,
  int *size
);
pwr_tStatus ldh_GetClassList (
  ldh_tSession Session,
  pwr_tClassId Class,
  pwr_tObjid *objid
);
pwr_tStatus ldh_GetMenu (
  ldh_sMenuCall *CallStruct
);
pwr_tStatus ldh_GetNextObject (
  ldh_tSession Session,
  pwr_tObjid object,
  pwr_tObjid *new_object
);
pwr_tStatus ldh_GetNextSibling (
  ldh_tSession Session,
  pwr_tObjid object,
  pwr_tObjid *new_object
);
pwr_tStatus ldh_GetObjectBody (
  ldh_tSession Session,
  pwr_tObjid object,
  char *bodyname,
  void **body,
  int *size
);
pwr_tStatus ldh_GetObjectBodyDef (
  ldh_tSession Session,
  pwr_tClassId Class,
  char *bodyname,
  int maxlev,
#if 0 // def __cplusplus
  ldh_sParDef (**bodydef)[1],
#else
  ldh_sParDef **bodydef,
#endif
  int *rows
);
pwr_tStatus ldh_GetObjectBuffer (
  ldh_tSession Session,
  pwr_tObjid object,
  char *bodyname,
  char *buffername,
  pwr_eClass *bufferclass,
  char **buff,
  int *size
);
pwr_tStatus ldh_GetObjectClass (
  ldh_tSession Session,
  pwr_tObjid object,
  pwr_tClassId *Class
);
pwr_tStatus ldh_GetObjectContext (
  ldh_tSession Session,
  pwr_tObjid object,
  ldh_sObjContext **object_context
);
pwr_tStatus ldh_GetObjectNode (
  ldh_tSession Session,
  pwr_tObjid object,
  pwr_tObjid *wnode,
  pwr_tObjid *anode
);
pwr_tStatus ldh_GetObjectInfo (
  ldh_tSession Session,
  pwr_tObjid object,
  ldh_sObjInfo *Info
);
pwr_tStatus ldh_GetObjectPar (
  ldh_tSession Session,
  pwr_tObjid object,
  char *bodyname,
  char *parname,
  char **buff,
  int *size
);
pwr_tStatus ldh_GetObjXRefDef (
  ldh_tSession Session,
  pwr_sAttrRef *AttrRef,
  pwr_sObjXRef *ObjXRef
);
pwr_tStatus ldh_GetParent (
  ldh_tSession Session,
  pwr_tObjid object,
  pwr_tObjid *new_object
);
pwr_tStatus ldh_GetPreviousObject (
  ldh_tSession Session,
  pwr_tObjid object,
  pwr_tObjid *new_object
);
pwr_tStatus ldh_GetPreviousSibling (
  ldh_tSession Session,
  pwr_tObjid object,
  pwr_tObjid *new_object
);
pwr_tStatus ldh_GetReferenceInfo (
  ldh_tSession Session,
  pwr_tObjid object,
  ldh_sRefInfo *RefInfo
);
pwr_tStatus ldh_GetRootList (
  ldh_tSession Session,
  pwr_tObjid *object
);
pwr_tStatus ldh_GetSessionInfo (
  ldh_tSession Session,
  ldh_sSessInfo *Info
);
pwr_tStatus ldh_GetUniqueObjectName (
  ldh_tSession Session,
  pwr_tObjid objid,
  char *Name
);

pwr_tStatus
ldh_GetVolumeList (
  ldh_tWorkbench	Workbench,
  pwr_tVolumeId		*Volume
);

pwr_tStatus
ldh_GetNextVolume (
  ldh_tWorkbench	Workbench,
  pwr_tVolumeId		Volume,
  pwr_tVolumeId		*NewVolume
);

pwr_tStatus
ldh_GetPreviousVolume (
  ldh_tWorkbench	Workbench,
  pwr_tVolumeId		Volume,
  pwr_tVolumeId		*NewVolume
);

pwr_tStatus
ldh_VolumeIdToName (
  ldh_tWorkbench	Workbench,
  pwr_tVolumeId		Volume,
  char			*Name,
  int			MaxSize,
  int			*Size
);

pwr_tStatus
ldh_VolumeNameToId (
  ldh_tWorkbench	Workbench,
  char			*Name,
  pwr_tVolumeId		*Volume
);

pwr_tStatus
ldh_GetVolumeClass (
  ldh_tWorkbench	Workbench,
  pwr_tVolumeId		Volume,
  pwr_tClassId		*Class
);

pwr_tStatus
ldh_IsOkCopyObject (
  ldh_tSession	Session,
  pwr_tObjid		Object,
  pwr_tObjid		DestinationObject,
  ldh_eDest		DestinationCode
);

pwr_tStatus
ldh_IsOkCreateObject (
  ldh_tSession	Session,
  pwr_tClassId		Class,
  pwr_tObjid		DestinationObject,
  ldh_eDest		DestinationCode
);

pwr_tStatus
ldh_IsOkMoveObject (
  ldh_tSession	Session,
  pwr_tObjid		Object,
  pwr_tObjid		DestinationObject,
  ldh_eDest		DestinationCode
);

pwr_tStatus
ldh_MoveObject (
  ldh_tSession	session,
  pwr_tObjid	object,
  pwr_tObjid	destinationObject,
  ldh_eDest	destinationCode
);

pwr_tStatus
ldh_NameToAttrRef (
  ldh_tSession	session,
  char		*attrName,
  pwr_sAttrRef	*attrRef
);

pwr_tStatus
ldh_NameToObjid (
  ldh_tSession	session,
  pwr_tObjid	*objid,
  char		*name
);

pwr_tStatus
ldh_ObjidToName (
  ldh_tSession	session,
  pwr_tObjid	objid,
  ldh_eName	nameType,
  char		*nameBuffer,
  int		maxSize,
  int		*size
);

void
ldh_AddThisSessionCallback (
  ldh_tSession	session,
  void		*editorContext,
  pwr_tStatus	(*receiveThisSession)(
		  void *editorContext,
		  ldh_sEvent *event
		)
);

void
ldh_AddOtherSessionCallback (
  ldh_tSession	session,
  void		*editorContext,
  pwr_tStatus	(*receiveOtherSession)(
		  void *editorContext,
		  ldh_sEvent *event
		)
);

pwr_tStatus
ldh_OpenSession (
  ldh_tSession		*Session,
  ldh_tVolContext	Volume,
  ldh_eAccess		Access,
  ldh_eUtility		Utility
);

pwr_tStatus
ldh_OpenWB (
  ldh_tWorkbench	*WorkBench
);

pwr_tStatus ldh_Paste (
  ldh_tSession   	Session,
  pwr_tObjid	    	dstoid,
  ldh_eDest	    	dest
);

pwr_tStatus
ldh_ReadAttribute (
  ldh_tSession	Session,
  pwr_sAttrRef	*AttrRef,
  void		*PointerToAttributeValue,
  int		Size
);

pwr_tStatus ldh_ReadObjectBody (
  ldh_tSession 	Session,
  pwr_tObjid 	object,
  char 		*bodyname,
  void 		*body,
  int 		size
);
pwr_tStatus ldh_RebuildWB (
  ldh_tSession  Session
);
pwr_tStatus ldh_RevertSession (
  ldh_tSession Session
);
pwr_tStatus ldh_SaveSession (
  ldh_tSession Session
);

ldh_tVolContext
ldh_SessionToVol (
  ldh_tSession	Session
);

ldh_tWorkbench ldh_SessionToWB (
  ldh_tSession Session
);
pwr_tStatus ldh_SetObjectBody (
  ldh_tSession Session,
  pwr_tObjid object,
  char *bodyname,
  char *body,
  int size
);
pwr_tStatus ldh_SetObjectBuffer (
  ldh_tSession Session,
  pwr_tObjid object,
  char *bodyname,
  char *buffername,
  char *buff
);
pwr_tStatus ldh_SetObjectName (
  ldh_tSession		session,
  pwr_tObjid		object,
  char 			*newname
);
pwr_tStatus ldh_SetObjectNode (
  ldh_tSession		session,
  pwr_tObjid		object,
  pwr_tObjid		wnode,
  pwr_tObjid		anode
);
pwr_tStatus ldh_SetObjectPar (
  ldh_tSession		session,
  pwr_tObjid		object,
  char			*bodyname,
  char			*parname,
  char			*buff,
  int 			size
);

pwr_tStatus
ldh_SetSession (
  ldh_tSession		session,
  ldh_eAccess		access
);

pwr_tStatus
ldh_StringGetAttribute (
  ldh_tSession	Session,
  pwr_sAttrRef		*attributeReference,
  pwr_tUInt32		maxSize,
  char			*string
);

pwr_tStatus
ldh_StringSetAttribute (
  ldh_tSession	Session,
  pwr_sAttrRef		*attributeReference,
  char			*string,
  pwr_tBoolean		write
);

pwr_tStatus
ldh_TOpenWB (
  ldh_tWorkbench	*workbench,
  int			flag
);

pwr_tStatus
ldh_WriteAttribute (
  ldh_tSession	Session,
  pwr_sAttrRef		*attributeReference,
  void			*attributeValuePointer,
  int			size
);

pwr_tStatus
ldh_GetVolumeInfo (
  ldh_tVolContext	volContext,
  ldh_sVolumeInfo	*info
);

pwr_tStatus
ldh_TypeIdToName (
  ldh_tSession		session,
  pwr_tTypeId		type,
  char			*namebuf,
  int 			maxsize,
  int 			*size
);

pwr_tBoolean
ldh_LocalObject (
  ldh_tSession		session,
  pwr_tObjid		object
);


pwr_tStatus
ldh_GetVolumeRootList (
  ldh_tSession 		session,
  pwr_tVolumeId		volume,
  pwr_tObjid		*object
);

pwr_tStatus
ldh_SyntaxCheck (
  ldh_tSession		session,
  int			*errorCount,
  int			*warningCount
);

pwr_tStatus
ldh_CreateLoadFile(
  ldh_tSession     session
);

#ifdef __cplusplus
}
#endif

#endif
