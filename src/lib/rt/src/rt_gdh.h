#ifndef rt_gdh_h
#define rt_gdh_h

/* rt_gdh.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   This include file defines the global datastructures and
   data entities in GDH. Also, the GDH entry points are defined.  */
/**
 *@example gdh_examples.c
 */


/** @defgroup GDH Global Data Handler  
 * This module contains the access routines to the Global Data
 * Handler. Those routines are callable from application level.   
 * @{
 */

#ifndef pwr_h
#include "pwr.h" 
#endif

typedef pwr_tSubid   gdh_tSubid;
typedef pwr_tSubid   SUBID;
typedef	pwr_tDlid    gdh_tDlid;
typedef	pwr_tDlid    DLID;

/** @defgroup DS Data Structures
 *  @ingroup GDH
 *  @{
 */

/** 
 * gdh_RefObjectInfoList parameter structure.  
 */
typedef struct {
  char		        fullname[160]; /**< Name of object or parameter */
  void			*adrs;    /**< Address of requested info */
  unsigned int		bufsize;  /**< Requested buffer size */
} gdh_sObjRef;

typedef gdh_sObjRef OBJREF_STRUCT;

/**
 * Link states
 */
typedef enum {
  gdh_eLinkState_Down,          /**< Down. No connection */
  gdh_eLinkState_Istrt,         /**< Start state I */
  gdh_eLinkState_Astrt,         /**< Start state A */
  gdh_eLinkState_Up             /**< Running. Now we're talking... */
} gdh_eLinkState;

/**
 * Node info
 */
typedef struct {

  pwr_tNodeId nodidx;           /**< Node index (!= qcom qix #) */
  pwr_tUInt32 nix;            	/**< Network address */
  char nodename [32];           /**< Ascii name of node (nul-terminated) */
                                /**< zero-length means empty slot */
  pwr_tObjid objid;             /**< Node object for this node */
  gdh_eLinkState linkstate;     /**< Nethandler down/istrt/astrt/up */

  pwr_tUInt32 upcnt;            /**< # of times up */
  pwr_tTime timeup;             /**< Most recent time link came up */
  pwr_tTime timedown;           /**< Most recent time link went down */

} gdh_sNodeInfo;

/* GDH entrypoints (as function prototypes).  */

/** @} */
/** 
 * @defgroup FC Function Calls
 * @ingroup GDH
 * @{
 */
/** 
 * @defgroup OMR Object manupilation routines
 * @ingroup FC
 * @{
 */

pwr_tStatus
gdh_CreateObject (
  char			*objectName,
  pwr_tClassId		classid,
  unsigned int		sizeOfObjectData,
  pwr_tObjid		*objectId,
  pwr_tObjid		internal1,
  pwr_tBitMask		internal2,
  pwr_tObjid		internal3
);

pwr_tStatus
gdh_DeleteObject (
  pwr_tObjid		objectId
);

pwr_tStatus
gdh_RenameObject (
  pwr_tObjid		objectId,
  char			*newObjectName
);

pwr_tStatus
gdh_MoveObject (
  pwr_tObjid		objectId,
  pwr_tObjid		newParentId
);
/** @} */

/** 
 * @defgroup RWO Reading and writing object data
 * @ingroup FC
 * @{
 */

pwr_tStatus
gdh_GetObjectInfo (   
  char			*name,		
  pwr_tAddress		bufp,					    
  pwr_tUInt32		bufsize	
);

pwr_tStatus
gdh_GetObjectInfoAttrref (
  pwr_sAttrRef		*attributeReference,
  void			*buffer,
  unsigned int		sizeOfBuffer
);

pwr_tStatus
gdh_SetObjectInfo (
  char			*attributeName,
  void			*buffer,
  unsigned int		sizeOfBuffer
);

pwr_tStatus
gdh_SetObjectInfoAttrref (
  pwr_sAttrRef		*attributeReference,
  void			*buffer,
  unsigned int		sizeOfBuffer
);
/** @} */
/** 
 * @defgroup DL Direct linking
 * @ingroup FC
 * @{
 */

pwr_tStatus
gdh_DLRefObjectInfoAttrref (
  pwr_sAttrRef		*addressOfAttributeReference,
  void			**objectData,
  pwr_tDlid		*directLinkId
);

pwr_tStatus
gdh_DLUnrefObjectInfo (
  pwr_tDlid		directLinkId
);

void
gdh_DLUnrefObjectInfoAll ();

/** @} */
/** 
 * @defgroup DLS Direct link/subscription
 * @ingroup FC
 * @{
 */

pwr_tStatus
gdh_GetSubscriptionOldness (
  pwr_tSubid		subid,
  pwr_tBoolean		*isOld,
  pwr_tTime		*lastUpdate,
  pwr_tStatus		*lastStatus
);

pwr_tStatus
gdh_RefObjectInfo (
  char			*attributeName,
  void			**objectData,
  pwr_tSubid		*subid,
  unsigned int		sizeOfObjectData
);

pwr_tStatus
gdh_UnrefObjectInfo (
  pwr_tSubid		subid
);

pwr_tStatus
gdh_UnrefObjectInfoAll ();

pwr_tStatus
gdh_GetRootList (
  pwr_tObjid		*object
);

pwr_tStatus
gdh_GetParent (
  pwr_tObjid		object,
  pwr_tObjid		*parent
);

pwr_tStatus
gdh_GetLocalParent (
  pwr_tObjid		object,
  pwr_tObjid		*parent
);

pwr_tStatus
gdh_GetChild (
  pwr_tObjid		object,
  pwr_tObjid		*firstChild
);

pwr_tStatus
gdh_GetNextSibling (
  pwr_tObjid		object,
  pwr_tObjid		*nextSibling
);

pwr_tStatus
gdh_GetPreviousSibling (
  pwr_tObjid		object,
  pwr_tObjid		*previousSibling
);

pwr_tStatus
gdh_RefObjectInfoList (
  unsigned int		nEntry,
  gdh_sObjRef		*objectReference,
  pwr_tSubid		*subid
);

/** @} */
/** 
 * @defgroup SUB Subscription 
 * @ingroup FC
 * @{
 */

pwr_tStatus
gdh_SetSubscriptionDefaults (
  pwr_tInt32		defaultUpdateTime,
  pwr_tInt32		defaultTimeOut
);


pwr_tStatus
gdh_SubRefObjectInfoAttrref (
  pwr_sAttrRef		*attributeReference,
  pwr_tSubid		*subid
);

pwr_tStatus
gdh_SubRefObjectInfoName (
  char			*attributeName,
  pwr_tSubid		*subid
);

pwr_tStatus
gdh_SubUnrefObjectInfoList (
  unsigned int		nEntry,
  pwr_tSubid		*subid
);

pwr_tStatus
gdh_SubUnrefObjectInfo (
  pwr_tSubid		subid
);

void
gdh_SubUnrefObjectInfoAll ();

pwr_tStatus
gdh_SubData (
  pwr_tSubid		subid,
  void			*buffer,
  unsigned int		sizeOfBuffer
);

pwr_tStatus
gdh_SubSize (
  pwr_tSubid		subid,
  unsigned int		*size
);

pwr_tStatus
gdh_SubAssociateBuffer (
  pwr_tSubid		subid,
  void			**buffer,
  unsigned int		sizeOfBuffer
);

pwr_tStatus
gdh_SubRefObjectInfoList (
  unsigned int		entryCount,
  void			*object[],
  pwr_tBoolean		isAttrref[],
  pwr_tSubid		subid[]
);

/** @} */

/** 
 * @defgroup CR Class routines
 * @ingroup FC
 * @{
 */

pwr_tStatus
gdh_GetClassList (
  pwr_tClassId		classid,
  pwr_tObjid		*object
);

pwr_tStatus
gdh_GetNextObject (
  pwr_tObjid		object,
  pwr_tObjid		*nextObject
);

pwr_tStatus
gdh_GetPreviousObject (
  pwr_tObjid		object,
  pwr_tObjid		*previousObject
);


#define gdh_ClassNameToNumber gdh_ClassNameToId

pwr_tStatus
gdh_ClassNameToId (
  char			*className,
  pwr_tClassId		*classid
);

/** @} */ 
/** 
 * @defgroup TBDOR Translating between different object representation
 * @ingroup FC
 * @{
 */

pwr_tStatus
gdh_ObjidToName (
  pwr_tObjid		oid,		
  char			*namebuf,
  pwr_tUInt32		size,		
  pwr_tBitMask		nametype      
);

pwr_tStatus
gdh_ObjidToPointer (
  pwr_tObjid		object,
  void			**objectData
);

pwr_tStatus
gdh_NameToObjid (
  char			*objectName,
  pwr_tObjid		*objid
);

pwr_tStatus
gdh_NameToPointer (
  char			*objectName,
  void			**objectData
);
/** @} */

/** 
 * @defgroup OP Object properties
 * @ingroup FC
 * @{
 */

pwr_tStatus
gdh_GetObjectSize (
  pwr_tObjid		oid,
  pwr_tUInt32		*size 
);

pwr_tStatus
gdh_GetObjectClass (
  pwr_tObjid		object,
  pwr_tClassId		*classid
);

pwr_tStatus
gdh_GetObjectLocation (
  pwr_tObjid		object,
  pwr_tBoolean		*isLocal
);

pwr_tStatus
gdh_GetObjectDLCount (
  pwr_tObjid		object,
  pwr_tUInt32		*count
);

/** @} */

/** 
 * @defgroup Node Node information
 * @ingroup FC
 * @{
 */

pwr_tStatus
gdh_GetObjectNodeIndex (
  pwr_tObjid		object,
  pwr_tNodeId		*nodeIndex
);

pwr_tStatus
gdh_GetNodeIndex (
  pwr_tNodeId		*nodeIndex
);

pwr_tStatus
gdh_GetNodeObject (
  pwr_tNodeId		nodeIndex,
  pwr_tObjid		*object
);

pwr_tStatus
gdh_GetNodeInfo (
  pwr_tNodeId nodidx,
  gdh_sNodeInfo *ip
);


/** @} */

/** 
 * @defgroup AR Attribute routines
 * @ingroup FC
 * @{
 */

pwr_tStatus
gdh_GetAttributeCharacteristics (
  char			*name,					  
  pwr_tTypeId		*tid,	
  pwr_tUInt32		*size,		
  pwr_tUInt32		*offs,		
  pwr_tUInt32		*elem	
);

pwr_tStatus
gdh_NameToAttrref (
  pwr_tObjid		parent,
  char			*attributeName,
  pwr_sAttrRef		*attributeReference
);

pwr_tStatus
gdh_AttrrefToName (
  pwr_sAttrRef		*attributeReference,
  char			*nameBuffer,
  unsigned int		sizeOfNameBuffer,
  pwr_tBitMask		nameType
);

pwr_tStatus
gdh_ClassAttrToAttrref (
  pwr_tClassId		classid,
  char			*attributeName,
  pwr_sAttrRef		*attributeReference
);

pwr_tStatus
gdh_GetAttributeCharAttrref (
  pwr_sAttrRef		*attributeReference,
  pwr_tTypeId		*attributeType,
  unsigned int		*attributeSize,
  unsigned int		*attributeOffset,
  unsigned int		*attributeDimension
);

/** @} */

/** 
 * @defgroup OAS Object alarm status
 * @ingroup FC
 * @{
 */

pwr_tStatus
gdh_GetAlarmInfo (
  pwr_tObjid		object,
  pwr_tUInt32		*alarmLevel,
  pwr_tUInt32		*maxAlarmLevel,
  pwr_tUInt32		*alarmBlockLevel,
  pwr_tUInt32		*maxAlarmBlockLevel,
  pwr_tUInt32		*alarmVisibility
);

pwr_tStatus
gdh_SetAlarmLevel (
  pwr_tObjid		object,
  pwr_tUInt32		alarmLevel
);

pwr_tStatus
gdh_SetAlarmBlockLevel (
  pwr_tObjid		object,
  pwr_tUInt32		alarmBlockLevel
);

/** @} */

/** 
 * @defgroup INIT Initializing GDH
 * @ingroup FC
 * @{
 */

pwr_tStatus gdh_Init (
  char		*name
);

/** @} */

/** 
 * @defgroup MI Miscellaneous
 * @ingroup FC
 * @{
 */
pwr_tStatus
gdh_NethandlerRunning ();

/** @} */


void
gdh_InitialLoadDone (
  pwr_tObjid		systemobject,
  pwr_tObjid		nodeobject
);



pwr_tStatus
gdh_IsAlias (
  pwr_tObjid		object,
  pwr_tBoolean		*isAlias
);

pwr_tStatus
gdh_IsMountClean (
  pwr_tObjid		object,
  pwr_tBoolean		*isMountClean
);

/*
 * Exclusive mode
 */

void
gdh_ExclusiveModeOn ();

void
gdh_ExclusiveModeOff ();

/*
 * Translation between Rtdb reference and job context virtual address formats.
 */

void gdh_StoreRtdbPointer (
  pwr_tUInt32		*rtdbReference,
  void			*virtualAddress
);

void *
gdh_TranslateRtdbPointer (
  pwr_tUInt32		rtdbReference
);



/* Undocumented routines. For internal use only.  */

pwr_tStatus
gdh_MDAttribute (
  pwr_tClassId		classid,
  pwr_sAttrRef		*attributeReference,
  char			*attributeName,
  pwr_tTypeId		*attributeType,
  unsigned int		*attributeSize,
  unsigned int		*attributeOffset,
  unsigned int		*attributeDimension
);

pwr_tStatus
gdh_SetCache (
  pwr_tUInt32           activation_level,
  pwr_tUInt32           trimmed_level
);

/** @} */
/** @} */

#endif
