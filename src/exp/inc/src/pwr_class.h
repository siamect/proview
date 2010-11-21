/* 
 * Proview   $Id: pwr_class.h,v 1.26 2008-10-16 11:11:31 claes Exp $
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

#ifndef pwr_class_h
#define pwr_class_h

/* pwr_class.h -- system classes
*/

#ifndef pwr_h
#include "pwr.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif


#define PWR_BREAKPOINTS_MAX 10
#define PWR_OBJTYPES_MAX 300
#define PRW_DEFOBJ_MAX 5
#define pwr_cMaxTyg 15

typedef struct pwr_s_ParInfo	pwr_sParInfo;
typedef struct pwr_s_ParGraph	pwr_sParGraph;
typedef union pwr_m_Adef	pwr_mAdef;
typedef union pwr_m_ClassDef	pwr_mClassDef;

    	
/* System classes
 
   This section defines the typedef's for structs corresponding
   to system classes.  */

typedef struct pwr_s_Alias		pwr_sAlias;
typedef struct pwr_s_AttrXRef		pwr_sAttrXRef;
typedef struct pwr_s_ClassDef		pwr_sClassDef;
typedef struct pwr_s_Hier		pwr_sHier;
typedef struct pwr_s_DocHier		pwr_sDocHier;
typedef struct pwr_s_LibHier		pwr_sLibHier;
typedef struct pwr_s_NodeHier		pwr_sNodeHier;
typedef struct pwr_s_Type		pwr_sType;
typedef struct pwr_s_TypeDef		pwr_sTypeDef;
typedef struct pwr_s_ObjBodyDef		pwr_sObjBodyDef;
typedef struct pwr_s_Input		pwr_sInput;
typedef struct pwr_s_Output		pwr_sOutput;
typedef struct pwr_s_Intern		pwr_sIntern;
typedef struct pwr_s_ObjXRef		pwr_sObjXRef;
typedef struct pwr_s_Point		pwr_sPoint;
typedef struct pwr_s_PlcProgram		pwr_sPlcProgram;
typedef struct pwr_s_PlcWindow		pwr_sPlcWindow;
typedef struct pwr_s_PlcNode		pwr_sPlcNode;
typedef struct pwr_s_PlcConnection	pwr_sPlcConnection;
typedef struct pwr_s_GraphPlcProgram	pwr_sGraphPlcProgram;
typedef struct pwr_s_GraphPlcWindow	pwr_sGraphPlcWindow;
typedef struct pwr_s_GraphPlcNode	pwr_sGraphPlcNode;
typedef struct pwr_s_GraphPlcConnection	pwr_sGraphPlcConnection;
typedef struct pwr_s_Buffer		pwr_sBuffer;
typedef struct pwr_s_Param		pwr_sParam;
typedef struct pwr_s_PlantHier		pwr_sPlantHier;
typedef struct pwr_s_Node		pwr_sNode;
typedef struct pwr_s_Appl		pwr_sAppl;
typedef struct pwr_s_System		pwr_sSystem;
typedef struct pwr_s_MenuCascade	pwr_sMenuCascade;
typedef struct pwr_s_MenuButton		pwr_sMenuButton;
typedef struct pwr_s_MenuRef		pwr_sMenuRef;
typedef struct pwr_s_Object		pwr_sObject;
typedef struct pwr_s_DbCallBack		pwr_sDbCallBack;
typedef struct pwr_s_RootVolume		pwr_sRootVolume;
typedef struct pwr_s_SubVolume		pwr_sSubVolume;
typedef struct pwr_s_SharedVolume	pwr_sSharedVolume;
typedef struct pwr_s_DynamicVolume	pwr_sDynamicVolume;
typedef struct pwr_s_SystemVolume	pwr_sSystemVolume;
typedef struct pwr_s_ClassVolume	pwr_sClassVolume;
typedef struct pwr_s_ClassVolume        pwr_sDetachedClassVolume;
typedef struct pwr_s_WorkBenchVolume	pwr_sWorkBenchVolume;
typedef struct pwr_s_DirectoryVolume	pwr_sDirectoryVolume;
typedef struct pwr_s_VolatileVolume	pwr_sVolatileVolume;
typedef struct pwr_s_ExternVolume	pwr_sExternVolume;
typedef struct pwr_s_CreateVolume	pwr_sCreateVolume;
typedef struct pwr_s_MountVolume	pwr_sMountVolume;
typedef struct pwr_s_MountObject	pwr_sMountObject;
typedef struct pwr_s_Bit		pwr_sBit;
typedef struct pwr_s_Value		pwr_sValue;
typedef struct pwr_s_Method		pwr_sMethod;
typedef struct pwr_s_Security		pwr_sSecurity;

typedef union pwr_u_ParDef		pwr_uParDef;
typedef union pwr_u_Volume		pwr_uVolume;

/** 
 * @brief System types
 *
 * All other typedef's, structdef's and parameter def's
 * refer to these types.
 * The numbers defined for each type in this enumeration
 * corresponds to the objid of the Type object defining the
 * system typ.  
 */
typedef enum {
  pwr_eTix__		= 0,
  pwr_eTix_Boolean	= 1,	/* Basic type */
  pwr_eTix_Float32	= 2,	/* Basic type */
  pwr_eTix_Float64	= 3,	/* Basic type */
  pwr_eTix_Char		= 4,	/* Basic type */
  pwr_eTix_Int8		= 5,	/* Basic type */
  pwr_eTix_Int16	= 6,	/* Basic type */
  pwr_eTix_Int32	= 7,	/* Basic type */
  pwr_eTix_UInt8	= 8,	/* Basic type */
  pwr_eTix_UInt16	= 9,	/* Basic type */
  pwr_eTix_UInt32	= 10,	/* Basic type */
  pwr_eTix_Objid	= 11,	/* Basic type */
  pwr_eTix_Buffer	= 12,	/* Virtual type */
  pwr_eTix_String	= 13,	/* Virtual type */
  pwr_eTix_Enum		= 14,	/* Virtual type */
  pwr_eTix_Struct	= 15,	/* Virtual type */
  pwr_eTix_Mask		= 16,	/* Virtual type */
  pwr_eTix_Array	= 17,	/* Virtual type */
  pwr_eTix_Time		= 18,	/* Basic type */
  pwr_eTix_Text		= 19,	/* Virtual type */
  pwr_eTix_AttrRef	= 20,	/* Basic type */
  pwr_eTix_UInt64	= 21,	/* Basic type */
  pwr_eTix_Int64	= 22,	/* Basic type */
  pwr_eTix_ClassId	= 23,	/* Basic type */
  pwr_eTix_TypeId	= 24,	/* Basic type */
  pwr_eTix_VolumeId	= 25,	/* Basic type */
  pwr_eTix_ObjectIx	= 26,	/* Basic type */
  pwr_eTix_RefId	= 27,	/* Basic type */
  pwr_eTix_DeltaTime	= 28,	/* Basic type */
  pwr_eTix_Status	= 29,	/* Basic type */
  pwr_eTix_NetStatus	= 30,	/* Basic type */
  pwr_eTix_CastId	= 31,	/* Basic type */
  pwr_eTix_ProString	= 32,	/* Basic type */
  pwr_eTix_DisableAttr	= 33,	/* Basic type */
  pwr_eTix_DataRef	= 34,	/* Basic type */
  pwr_eTix_
} pwr_eTix;

#define pwr_cVolumeId 1
#define pwr_TypeId(Tix) (0 + (pwr_cVolumeId << 16) + (1 << 15)  +  Tix)
#define pwr_Tix(Tid)	(Tid & 0x7ff)


typedef enum {
  pwr_eType__		= pwr_TypeId(pwr_eTix__),
  pwr_eType_Boolean	= pwr_TypeId(pwr_eTix_Boolean),
  pwr_eType_Float32	= pwr_TypeId(pwr_eTix_Float32),
  pwr_eType_Float64	= pwr_TypeId(pwr_eTix_Float64),
  pwr_eType_Char	= pwr_TypeId(pwr_eTix_Char),
  pwr_eType_Int8	= pwr_TypeId(pwr_eTix_Int8),
  pwr_eType_Int16	= pwr_TypeId(pwr_eTix_Int16),
  pwr_eType_Int32	= pwr_TypeId(pwr_eTix_Int32),
  pwr_eType_UInt8	= pwr_TypeId(pwr_eTix_UInt8),
  pwr_eType_UInt16	= pwr_TypeId(pwr_eTix_UInt16),
  pwr_eType_UInt32	= pwr_TypeId(pwr_eTix_UInt32),
  pwr_eType_Objid	= pwr_TypeId(pwr_eTix_Objid),
  pwr_eType_Buffer	= pwr_TypeId(pwr_eTix_Buffer),
  pwr_eType_String	= pwr_TypeId(pwr_eTix_String),
  pwr_eType_Enum	= pwr_TypeId(pwr_eTix_Enum),
  pwr_eType_Struct	= pwr_TypeId(pwr_eTix_Struct),
  pwr_eType_Mask	= pwr_TypeId(pwr_eTix_Mask),
  pwr_eType_Array	= pwr_TypeId(pwr_eTix_Array),
  pwr_eType_Time	= pwr_TypeId(pwr_eTix_Time),
  pwr_eType_Text	= pwr_TypeId(pwr_eTix_Text),
  pwr_eType_AttrRef	= pwr_TypeId(pwr_eTix_AttrRef),
  pwr_eType_UInt64	= pwr_TypeId(pwr_eTix_UInt64),
  pwr_eType_Int64	= pwr_TypeId(pwr_eTix_Int64),
  pwr_eType_ClassId	= pwr_TypeId(pwr_eTix_ClassId),
  pwr_eType_TypeId	= pwr_TypeId(pwr_eTix_TypeId),
  pwr_eType_VolumeId	= pwr_TypeId(pwr_eTix_VolumeId),
  pwr_eType_ObjectIx	= pwr_TypeId(pwr_eTix_ObjectIx),
  pwr_eType_RefId	= pwr_TypeId(pwr_eTix_RefId),
  pwr_eType_DeltaTime	= pwr_TypeId(pwr_eTix_DeltaTime),
  pwr_eType_Status	= pwr_TypeId(pwr_eTix_Status),
  pwr_eType_NetStatus	= pwr_TypeId(pwr_eTix_NetStatus),
  pwr_eType_CastId	= pwr_TypeId(pwr_eTix_CastId),
  pwr_eType_ProString	= pwr_TypeId(pwr_eTix_ProString),
  pwr_eType_DisableAttr	= pwr_TypeId(pwr_eTix_DisableAttr),
  pwr_eType_DataRef	= pwr_TypeId(pwr_eTix_DataRef),
  pwr_eType_		= pwr_TypeId(pwr_eTix_)
} pwr_eType;

typedef enum {
  pwr_eTdix__			= 0,
  pwr_eTdix_AdefFlags		= 15,	/* Derived type */
  pwr_eTdix_ClassDefFlags	= 16,	/* Derived type */
  pwr_eTdix_ObjBodyDefFlags	= 18,	/* Derived type */
  pwr_eTdix_
} pwr_eTdix;

typedef enum {
  pwr_eTypeDef__		= pwr_TypeId(pwr_eTix__) | 1 << 11,
  pwr_eTypeDef_AdefFlags	= pwr_TypeId(pwr_eTdix_AdefFlags) | 1 << 11,
  pwr_eTypeDef_ClassDefFlags	= pwr_TypeId(pwr_eTdix_ClassDefFlags) | 1 << 11,
  pwr_eTypeDef_ObjBodyDefFlags	= pwr_TypeId(pwr_eTdix_ObjBodyDefFlags) | 1 << 11,
  pwr_eTypeDef_			= pwr_TypeId(pwr_eTix_) | 1 << 11
} pwr_eTypeDef;

/** 
 * Due to compiler warnings in some switch statements, pwr_eType_ObjDId has been
 * removed from pwr_eType.
 *
 * @todo Change all ObjDId to Objid and remove this define
 */ 
#define pwr_eType_ObjDId pwr_eType_Objid

/**
 * The numbers defined for each class in this enumeration
 * corresponds to the objid of the ClassDef object defining the
 * system class.  
 */

typedef enum {
  pwr_eCix__			=   0,
  pwr_eCix_ClassDef		=   1,
  pwr_eCix_Type			=   2,
  pwr_eCix_TypeDef		=   3,
  pwr_eCix_ObjBodyDef		=   4,
  pwr_eCix_Param		=   5,
  pwr_eCix_Input		=   6,
  pwr_eCix_Output		=   7,
  pwr_eCix_Intern		=   8,
  pwr_eCix_Buffer		=   9,
  pwr_eCix_ObjXRef		=  10,
  pwr_eCix_Layout		=  11,
  pwr_eCix_Group		=  12,
  pwr_eCix_GroupRef		=  13,
  pwr_eCix_TypeHier		=  14,
  pwr_eCix_ClassHier		=  15,
  pwr_eCix_ModHier		=  16,
  pwr_eCix_PlantHier		=  17,
  pwr_eCix_PlcProgram		=  18,
  pwr_eCix_PlcWindow		=  19,
  pwr_eCix_PlcNode		=  20,
  pwr_eCix_PlcConnection	=  21,
  pwr_eCix_Point		=  22,
  pwr_eCix_GraphPlcProgram	=  23,
  pwr_eCix_GraphPlcWindow	=  24,
  pwr_eCix_GraphPlcNode		=  25,
  pwr_eCix_GraphPlcConnection	=  26,
  pwr_eCix_PlcPgm		=  27,
  pwr_eCix_Hierarchy		=  28,
  pwr_eCix_NodeHier		=  29,
  pwr_eCix_PgmDef		=  30,
  pwr_eCix_Node			=  31,
  pwr_eCix_Appl			=  32,
  pwr_eCix_System		=  33,
  pwr_eCix_LibHier		=  34,
  pwr_eCix_DocHier		=  35,
  pwr_eCix_AttrXRef		=  39,
  pwr_eCix_Menu			=  40,
  pwr_eCix_MenuSeparator	=  41,
  pwr_eCix_MenuCascade		=  42,
  pwr_eCix_MenuButton		=  43,
  pwr_eCix_Object		=  44,
  pwr_eCix_DbCallBack		=  45,
  pwr_eCix_Alias		=  46,
  pwr_eCix_RootVolume		=  47,
  pwr_eCix_SubVolume		=  48,
  pwr_eCix_SharedVolume		=  49,
  pwr_eCix_DynamicVolume	=  50,
  pwr_eCix_SystemVolume		=  51,
  pwr_eCix_ClassVolume		=  52,
  pwr_eCix_WorkBenchVolume	=  53,
  pwr_eCix_DirectoryVolume	=  54,
  pwr_eCix_CreateVolume		=  55,
  pwr_eCix_MountVolume		=  56,
  pwr_eCix_MountObject		=  57,
  pwr_eCix_RtMenu		=  58,
  pwr_eCix_VolatileVolume	=  59,
  pwr_eCix_MenuRef		=  60,
  pwr_eCix_Bit			=  61,
  pwr_eCix_Value		=  62,
  pwr_eCix_Method		=  63,
  pwr_eCix_RtMethod		=  64,
  pwr_eCix_ExternVolume		=  65,
  pwr_eCix_Hier			=  66,
  pwr_eCix_ClassLost   		=  67,
  pwr_eCix_Security  		=  68,
  pwr_eCix_DetachedClassVolume 	=  69,
  pwr_eCix_
} pwr_eCix;
    
#define pwr_ClassId(Cix) (0 + (pwr_cVolumeId << 16) +  (Cix << 3))

typedef enum {
  pwr_eClass__			= pwr_ClassId(pwr_eCix__),
  pwr_eClass_ClassDef		= pwr_ClassId(pwr_eCix_ClassDef),
  pwr_eClass_Type		= pwr_ClassId(pwr_eCix_Type),
  pwr_eClass_TypeDef		= pwr_ClassId(pwr_eCix_TypeDef),
  pwr_eClass_ObjBodyDef		= pwr_ClassId(pwr_eCix_ObjBodyDef),
  pwr_eClass_Param		= pwr_ClassId(pwr_eCix_Param),
  pwr_eClass_Input		= pwr_ClassId(pwr_eCix_Input),
  pwr_eClass_Output		= pwr_ClassId(pwr_eCix_Output),
  pwr_eClass_Intern		= pwr_ClassId(pwr_eCix_Intern),
  pwr_eClass_Buffer		= pwr_ClassId(pwr_eCix_Buffer),
  pwr_eClass_ObjXRef		= pwr_ClassId(pwr_eCix_ObjXRef),
  pwr_eClass_Layout		= pwr_ClassId(pwr_eCix_Layout),
  pwr_eClass_Group		= pwr_ClassId(pwr_eCix_Group),
  pwr_eClass_GroupRef		= pwr_ClassId(pwr_eCix_GroupRef),
  pwr_eClass_TypeHier		= pwr_ClassId(pwr_eCix_TypeHier),
  pwr_eClass_ClassHier		= pwr_ClassId(pwr_eCix_ClassHier),
  pwr_eClass_ModHier		= pwr_ClassId(pwr_eCix_ModHier),
  pwr_eClass_PlantHier		= pwr_ClassId(pwr_eCix_PlantHier),
  pwr_eClass_PlcProgram		= pwr_ClassId(pwr_eCix_PlcProgram),
  pwr_eClass_PlcWindow		= pwr_ClassId(pwr_eCix_PlcWindow),
  pwr_eClass_PlcNode		= pwr_ClassId(pwr_eCix_PlcNode),
  pwr_eClass_PlcConnection	= pwr_ClassId(pwr_eCix_PlcConnection),
  pwr_eClass_Point		= pwr_ClassId(pwr_eCix_Point),
  pwr_eClass_GraphPlcProgram	= pwr_ClassId(pwr_eCix_GraphPlcProgram),
  pwr_eClass_GraphPlcWindow	= pwr_ClassId(pwr_eCix_GraphPlcWindow),
  pwr_eClass_GraphPlcNode	= pwr_ClassId(pwr_eCix_GraphPlcNode),
  pwr_eClass_GraphPlcConnection	= pwr_ClassId(pwr_eCix_GraphPlcConnection),
  pwr_eClass_PlcPgm		= pwr_ClassId(pwr_eCix_PlcPgm),
  pwr_eClass_Hierarchy		= pwr_ClassId(pwr_eCix_Hierarchy),
  pwr_eClass_NodeHier		= pwr_ClassId(pwr_eCix_NodeHier),
  pwr_eClass_PgmDef		= pwr_ClassId(pwr_eCix_PgmDef),
  pwr_eClass_Node		= pwr_ClassId(pwr_eCix_Node),
  pwr_eClass_Appl		= pwr_ClassId(pwr_eCix_Appl),
  pwr_eClass_System		= pwr_ClassId(pwr_eCix_System),
  pwr_eClass_LibHier		= pwr_ClassId(pwr_eCix_LibHier),
  pwr_eClass_DocHier		= pwr_ClassId(pwr_eCix_DocHier),
  pwr_eClass_AttrXRef		= pwr_ClassId(pwr_eCix_AttrXRef),
  pwr_eClass_Menu		= pwr_ClassId(pwr_eCix_Menu),
  pwr_eClass_MenuSeparator	= pwr_ClassId(pwr_eCix_MenuSeparator),
  pwr_eClass_MenuCascade	= pwr_ClassId(pwr_eCix_MenuCascade),
  pwr_eClass_MenuButton		= pwr_ClassId(pwr_eCix_MenuButton),
  pwr_eClass_Object		= pwr_ClassId(pwr_eCix_Object),
  pwr_eClass_DbCallBack		= pwr_ClassId(pwr_eCix_DbCallBack),
  pwr_eClass_Alias		= pwr_ClassId(pwr_eCix_Alias),
  pwr_eClass_RootVolume		= pwr_ClassId(pwr_eCix_RootVolume),
  pwr_eClass_SubVolume		= pwr_ClassId(pwr_eCix_SubVolume),
  pwr_eClass_SharedVolume	= pwr_ClassId(pwr_eCix_SharedVolume),
  pwr_eClass_DynamicVolume	= pwr_ClassId(pwr_eCix_DynamicVolume),
  pwr_eClass_SystemVolume	= pwr_ClassId(pwr_eCix_SystemVolume),
  pwr_eClass_ClassVolume	= pwr_ClassId(pwr_eCix_ClassVolume),
  pwr_eClass_WorkBenchVolume	= pwr_ClassId(pwr_eCix_WorkBenchVolume),
  pwr_eClass_DirectoryVolume	= pwr_ClassId(pwr_eCix_DirectoryVolume),
  pwr_eClass_CreateVolume	= pwr_ClassId(pwr_eCix_CreateVolume),
  pwr_eClass_MountVolume	= pwr_ClassId(pwr_eCix_MountVolume),
  pwr_eClass_MountObject	= pwr_ClassId(pwr_eCix_MountObject),
  pwr_eClass_RtMenu		= pwr_ClassId(pwr_eCix_RtMenu),
  pwr_eClass_VolatileVolume	= pwr_ClassId(pwr_eCix_VolatileVolume),
  pwr_eClass_MenuRef		= pwr_ClassId(pwr_eCix_MenuRef),
  pwr_eClass_Bit		= pwr_ClassId(pwr_eCix_Bit),
  pwr_eClass_Value		= pwr_ClassId(pwr_eCix_Value),
  pwr_eClass_Method		= pwr_ClassId(pwr_eCix_Method),
  pwr_eClass_RtMethod		= pwr_ClassId(pwr_eCix_RtMethod),
  pwr_eClass_ExternVolume	= pwr_ClassId(pwr_eCix_ExternVolume),
  pwr_eClass_Hier		= pwr_ClassId(pwr_eCix_Hier),
  pwr_eClass_ClassLost		= pwr_ClassId(pwr_eCix_ClassLost),
  pwr_eClass_Security		= pwr_ClassId(pwr_eCix_Security),
  pwr_eClass_DetachedClassVolume = pwr_ClassId(pwr_eCix_DetachedClassVolume),
  pwr_eClass_			
} pwr_eClass;
    
/**
 * When the user wants to edit an object, the Editor defined in ClassDef
 * is started with the object.  
 */

typedef enum {
  pwr_eEditor__ = 0,
  pwr_eEditor_HiEd,
  pwr_eEditor_PlcEd,
  pwr_eEditor_AttrEd,
  pwr_eEditor_ClassEd,
  pwr_eEditor_
} pwr_eEditor;

/** 
 * When the user wants to edit an object, the Editor defined in ClassDef
 * is started with the object and with a method defined here. 
 */
typedef enum {
  pwr_eMethod__ = 0,
  pwr_eMethod_Standard,
  pwr_eMethod_Connections,
  pwr_eMethod_DrawingInfo,
  pwr_eMethod_DevBodyOnly,
  pwr_eMethod_RtAndDevBodies,
  pwr_eMethod_RtConnectionsAndDevBodies,
  pwr_eMethod_DevBodyWithChkb,
  pwr_eMethod_SysBody,
  pwr_eMethod_DevBodyConnections,
  pwr_eMethod_
} pwr_eMethod;

/**
 * Used in cross references.  
 */
typedef enum {
  pwr_eXRefType__ = 0,
  pwr_eXRefType_
} pwr_eXRefType;

/**
 * Class to hold methods common to all classes.
 * Class is also used in classdefinitions to give default name to 
 * an instance of that class.  
 */

struct pwr_s_Object {
  pwr_tObjName	Name;
};

/** 
 * When the user wants to edit an object, the PopEditor defined in ClassDef
 * is started with the object.  
*/

typedef enum {
  pwr_ePopEditor__ = 0,
  pwr_ePopEditor_GMS,
  pwr_ePopEditor_Opcom,
  pwr_ePopEditor_
} pwr_ePopEditor;


typedef enum {
  pwr_eVolumeAccess__	      = 0,
  pwr_eVolumeAccess_ReadOnly  = 1,
  pwr_eVolumeAccess_ReadWrite = 2,
  pwr_eVolumeAccess_
} pwr_eVolumeAccess;

typedef enum {
  pwr_eMountType__	      = 0,
  pwr_eMountType_Default      = 1,
  pwr_eMountType_Cached	      = 2,
  pwr_eMountType_Structure    = 3,
  pwr_eMountType_TryStructure = 4,
  pwr_eMountType_
} pwr_eMountType;

typedef enum {
  pwr_eMountedType__	      = 0,
  pwr_eMountedType_Full	      = 1,
  pwr_eMountedType_Cached     = 2,
  pwr_eMountedType_Structure  = 3,
  pwr_eMountedType_Shared     = 4,
  pwr_eMountedType_Owned      = 5,
  pwr_eMountedType_Class      = 6,
  pwr_eMountedType_System     = 7,
  pwr_eMountedType_NotMounted = 8,
  pwr_eMountedType_
} pwr_eMountedType;

/* This section defines the classes to use when defining types.  */

struct pwr_s_Type {
  pwr_eType    	Type;
  pwr_tInt32   	Size 		pwr_dAlignW;	/* Number of bytes. */
};

struct pwr_s_TypeDef {
  pwr_eType    	Type;
  pwr_tInt32   	Size 		pwr_dAlignW; 	/* Number of bytes. */
  pwr_tTypeId  	TypeRef 	pwr_dAlignW;
  pwr_tInt32   	Elements 	pwr_dAlignW;
  pwr_tPgmName	PgmName 	pwr_dAlignW;
};

/* Class defining classes. 

   This section defines the classes to use when defining classes.  */
/*_*
  @aref classdefflags ClassDef
*/
typedef pwr_tMask pwr_tClassDefFlags;

union pwr_m_ClassDef {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( DevOnly	  , 1), /* object exists only in development
                                   environment  */
    pwr_Bits( System	  , 1), /* object is a system object  */
    pwr_Bits( Multinode	  , 1), /* object is a multinode object  */
    pwr_Bits( ObjXRef	  , 1), /* object contains ObjXRefs  */
    pwr_Bits( RtBody	  , 1),	/* object has a body in runtime  */
    pwr_Bits( AttrXRef	  , 1),	/* object contains AttrXRefs  */
    pwr_Bits( ObjRef	  , 1),	/* object contains ObjRefs  */
    pwr_Bits( AttrRef	  , 1),	/* object contains AttrRefs  */

    pwr_Bits( TopObject	  , 1), /* object can be top object  */
    pwr_Bits( NoAdopt	  , 1), /* object can not have children  */
    pwr_Bits( Template	  , 1),	/* object is a template object  */
    pwr_Bits( IO	  , 1),	/* object is an IO object  */
    pwr_Bits( IOAgent     , 1),	/* object is IO agent  */
    pwr_Bits( IORack      , 1),	/* object is IO rack  */
    pwr_Bits( IOCard      , 1),	/* object is IO card  */
    pwr_Bits( HasCallBack , 1),	/* object has DbCallBack  */

    pwr_Bits( RtReadOnly , 1),	/* object is readonly in runtime  */
    pwr_Bits( fill_1      , 7),,,,,,,
    pwr_Bits( fill_2      , 8),,,,,,,
  ) b;

#define pwr_mClassDef_DevOnly	  pwr_Bit(0)
#define pwr_mClassDef_System	  pwr_Bit(1)
#define pwr_mClassDef_Multinode	  pwr_Bit(2)
#define pwr_mClassDef_ObjXRef	  pwr_Bit(3)
#define pwr_mClassDef_RtBody	  pwr_Bit(4)
#define pwr_mClassDef_AttrXRef	  pwr_Bit(5)
#define pwr_mClassDef_ObjRef	  pwr_Bit(6)
#define pwr_mClassDef_AttrRef	  pwr_Bit(7)
#define pwr_mClassDef_TopObject	  pwr_Bit(8)
#define pwr_mClassDef_NoAdopt	  pwr_Bit(9)
#define pwr_mClassDef_Template    pwr_Bit(10)
#define pwr_mClassDef_IO	  pwr_Bit(11)
#define pwr_mClassDef_IOAgent     pwr_Bit(12)
#define pwr_mClassDef_IORack      pwr_Bit(13)
#define pwr_mClassDef_IOCard      pwr_Bit(14)
#define pwr_mClassDef_HasCallBack pwr_Bit(15)
#define pwr_mClassDef_RtReadOnly  pwr_Bit(16)

#define pwr_mClassDef_HasRef	  (pwr_mClassDef_ObjXRef|pwr_mClassDef_AttrXRef|\
				   pwr_mClassDef_ObjRef|pwr_mClassDef_AttrRef)
};

struct pwr_s_ClassDef {
  pwr_eEditor	Editor;  			/* Editor to call on a "Edit"
                                               	   command.  */
  pwr_eMethod	Method 		pwr_dAlignW;   /* Method to use by the called
                                               	   editor.  */
  pwr_mClassDef	Flags	 	pwr_dAlignW;
  pwr_tUInt32	NumOfObjBodies 	pwr_dAlignW;
  pwr_ePopEditor PopEditor 	pwr_dAlignW;   /* What kind of object picture
                                               	   dispatcher to use.  */
  pwr_tInt32    Filler		pwr_dAlignW;   /* LongWord size alignment */ 
};


/* Body defining classes.

  This section defines the classes to use when defining bodies.  */

struct pwr_s_ObjBodyDef {
  pwr_tStructName	StructName; 			/* Name of the C-struct to be
                                           		   generated.  */
  pwr_tUInt32		NumOfParams 	pwr_dAlignW; 	/* Compiled number of parameters.  */
  pwr_tUInt32		Size 		pwr_dAlignW;	/* Compiled size.  */
  pwr_tObjectIx		NextAix 	pwr_dAlignW;	/* Next free attribute index.  */ 
  pwr_tMask		Flags 		pwr_dAlignW;   /* Not used */
};

/* Parameter defining classes.

   This section defines the classes to be used when defining parameters
   in classes.
  
   At the end of this section is a union: pwr_uPar, wich must be updated for
   each new parameter class defined.  */

/* This class is used to define graphic appearence of parameters.  */

struct pwr_s_ParGraph {
  pwr_tGraphName	GraphName;
  pwr_tUInt32		InputType	pwr_dAlignW;
  pwr_tUInt32		NiNaAnnot	pwr_dAlignW;
  pwr_tBoolean		NiNaCond	pwr_dAlignW;
  pwr_tBoolean		NiNaSegments	pwr_dAlignW;
  pwr_tUInt32		DebAnnot	pwr_dAlignW;
  pwr_tUInt32		ConPointNr	pwr_dAlignW;
};

/* This class is used to define information about parameters.  */


struct pwr_s_ParInfo {
  pwr_tPgmName		PgmName		pwr_dAlignW;   	/* Name to be used in program. */
  pwr_eType		Type		pwr_dAlignW;  	/* Compiled system type. */
  pwr_tUInt32		Offset		pwr_dAlignW;  	/* Compiled offset in body. */
  pwr_tUInt32		Size		pwr_dAlignW;  	/* Compiled size, number of bytes. */
  pwr_tUInt32		Flags		pwr_dAlignW;  	/* Parameter flags */
  pwr_tUInt32		Elements 	pwr_dAlignW;	/* If array; number of rows. */
  pwr_tUInt32		ParamIndex 	pwr_dAlignW; 	/* Index of param within a body. */
};

/*_*
  @aref privmask PrivMask
*/
typedef pwr_tMask pwr_tPrivMask;

/*_*
  @aref configstatusenum ConfigStatusEnum
*/
typedef pwr_tEnum pwr_tConfigStatusEnum;

/* Operating system.  */
/*_*
  @aref opsys OpSys
*/
typedef pwr_tMask pwr_tOpSys;

#if 1
typedef enum {
  pwr_mOpSys__ = 0,
  pwr_mOpSys_CustomBuild =  1 << 0,
  pwr_mOpSys_VAX_VMS =  1 << 1,
  pwr_mOpSys_AXP_VMS =  1 << 2,
  pwr_mOpSys_PPC_LYNX = 1 << 3,
  pwr_mOpSys_X86_LYNX = 1 << 4,
  pwr_mOpSys_PPC_LINUX = 1 << 5,
  pwr_mOpSys_X86_LINUX = 1 << 6,
  pwr_mOpSys_X86_64_LINUX = 1 << 7,
  pwr_mOpSys_X86_64_MACOS = 1 << 8,
  pwr_mOpSys_ARM_LINUX = 1 << 9,
  pwr_mOpSys_X86_64_FREEBSD = 1 << 10,
  pwr_mOpSys_ = 1 << 11,
  pwr_mOpSys_VAX_ELN =  1 << 30    /* TODO Remove! */
} pwr_mOpSys;

#else
typedef union {
  pwr_tBitMask m;
  struct {
    pwr_tBit  VAX_ELN	: 1;
    pwr_tBit  VAX_VMS	: 1;
    pwr_tBit  AXP_VMS	: 1;
    pwr_tBit  PPC_LYNX	: 1;
    pwr_tBit  X86_LYNX	: 1;

    pwr_tBit  fill	: 27;
  } b;

#define pwr_mOpSys__	      0
#define pwr_mOpSys_VAX_ELN    pwr_Bit(0)
#define pwr_mOpSys_VAX_VMS    pwr_Bit(1)
#define pwr_mOpSys_AXP_VMS    pwr_Bit(2)
#define pwr_mOpSys_PPC_LYNX   pwr_Bit(3)
#define pwr_mOpSys_X86_LYNX   pwr_Bit(4)
#define pwr_mOpSys_	      (~pwr_mOpSys__)
} pwr_mOpSys;
#endif

/* Bitmask for body flags  */
/*_*
  @aref objbodydefflags ObjBodyDef
*/
typedef pwr_tMask pwr_tObjBodyDefFlags;

#define pwr_mObjBodyDef_rtvirtual	pwr_Bit(6)		/*  64	  */

/* Bitmask for attribute flags  */
/*_*
  @aref adef Adef
*/
typedef pwr_tMask pwr_tAdefFlags;

union pwr_m_Adef {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( pointer	, 1),
    pwr_Bits( array	, 1),
    pwr_Bits( backup	, 1),
    pwr_Bits( castattr	, 1),
    pwr_Bits( state	, 1),
    pwr_Bits( constant	, 1),
    pwr_Bits( rtvirtual	, 1),
    pwr_Bits( devbodyref, 1),

    pwr_Bits( dynamic	, 1),
    pwr_Bits( objidself	, 1),
    pwr_Bits( noedit	, 1),
    pwr_Bits( invisible	, 1),
    pwr_Bits( refdirect	, 1),
    pwr_Bits( noinvert	, 1),
    pwr_Bits( noremove	, 1),
    pwr_Bits( rtdbref	, 1),

    pwr_Bits( privatepointer , 1),  
    pwr_Bits( isclass   , 1),  /* class is a reserved word */
    pwr_Bits( superclass, 1),  
    pwr_Bits( buffer    , 1),  
    pwr_Bits( nowbl     , 1),  
    pwr_Bits( alwayswbl , 1), 
    pwr_Bits( newattribute , 1), 
    pwr_Bits( fill_0	, 1),

    pwr_Bits( fill_1	, 8),,,,,,,
  ) b;

#define pwr_mAdef_pointer	pwr_Bit(0)		/*  1	  */
#define pwr_mAdef_array		pwr_Bit(1)		/*  2	  */
#define pwr_mAdef_backup	pwr_Bit(2)		/*  4	  */
#define pwr_mAdef_castattr	pwr_Bit(3)		/*  8	  */
#define pwr_mAdef_state		pwr_Bit(4)		/*  16	  */
#define pwr_mAdef_const		pwr_Bit(5)		/*  32	  */
#define pwr_mAdef_rtvirtual	pwr_Bit(6)		/*  64	  */
#define pwr_mAdef_devbodyref	pwr_Bit(7)		/*  128	  */
#define pwr_mAdef_dynamic	pwr_Bit(8)		/*  256	  */
#define pwr_mAdef_objidself	pwr_Bit(9)		/*  512	  */
#define pwr_mAdef_noedit	pwr_Bit(10)		/*  1024  */
#define pwr_mAdef_invisible	pwr_Bit(11)		/*  2048  */
#define pwr_mAdef_refdirect	pwr_Bit(12)		/*  4096  If an AttrRef should not give alue pointed to  */
#define pwr_mAdef_noinvert	pwr_Bit(13)		/*  8192  */
#define pwr_mAdef_noremove	pwr_Bit(14)		/*  16384 Cannot be removed, no used chkbx */
#define pwr_mAdef_rtdbref	pwr_Bit(15)		/*  32768 */
#define pwr_mAdef_private	pwr_Bit(16)		/*  65536 Private pointer, not to be displayed */
#define pwr_mAdef_class	        pwr_Bit(17)		
#define pwr_mAdef_superclass	pwr_Bit(18)		
#define pwr_mAdef_buffer	pwr_Bit(19)		
#define pwr_mAdef_nowbl		pwr_Bit(20)		/* Don't print to wbl file */		
#define pwr_mAdef_alwayswbl	pwr_Bit(21)		/* Always print to wbl file */		
#define pwr_mAdef_disableattr	pwr_Bit(22)		/* Can be disabled */
#define pwr_mAdef_rthide	pwr_Bit(23)		/* Hide in runtime */
#define pwr_mAdef_newattribute 	pwr_Bit(24)		/* New attribute */
};

#define PWR_MASK_POINTER	pwr_mAdef_pointer
#define PWR_MASK_ARRAY		pwr_mAdef_array
#define PWR_MASK_BACKUP		pwr_mAdef_backup
#define PWR_MASK_CASTATTR	pwr_mAdef_castattr
#define PWR_MASK_STATE		pwr_mAdef_state
#define PWR_MASK_CONST		pwr_mAdef_const
#define PWR_MASK_RTVIRTUAL	pwr_mAdef_rtvirtual
#define PWR_MASK_DEVBODYREF	pwr_mAdef_devbodyref
#define PWR_MASK_DYNAMIC	pwr_mAdef_dynamic
#define PWR_MASK_OBJDIDSELF	pwr_mAdef_objidself
#define PWR_MASK_OBJIDSELF	pwr_mAdef_objidself
#define PWR_MASK_NOEDIT		pwr_mAdef_noedit
#define PWR_MASK_INVISIBLE	pwr_mAdef_invisible
#define PWR_MASK_REFDIRECT	pwr_mAdef_refdirect
#define PWR_MASK_NOINVERT	pwr_mAdef_noinvert
#define PWR_MASK_NOREMOVE	pwr_mAdef_noremove
#define PWR_MASK_RTDBREF	pwr_mAdef_rtdbref
#define PWR_MASK_PRIVATE	pwr_mAdef_private
#define PWR_MASK_CLASS	        pwr_mAdef_class
#define PWR_MASK_SUPERCLASS	pwr_mAdef_superclass
#define PWR_MASK_BUFFER	        pwr_mAdef_buffer
#define PWR_MASK_NOWBL        	pwr_mAdef_nowbl
#define PWR_MASK_ALWAYSWBL      pwr_mAdef_alwayswbl
#define PWR_MASK_DISABLEATTR    pwr_mAdef_disableattr
#define PWR_MASK_RTHIDE    	pwr_mAdef_rthide
#define PWR_MASK_NEWATTRIBUTE  	pwr_mAdef_newattribute

struct pwr_s_Param {
  pwr_sParInfo	Info 	pwr_dAlignLW;
  pwr_tTypeId  	TypeRef	pwr_dAlignW;	/* Reference to the object defining
                                           the type.  */
  pwr_tInt32    Filler   pwr_dAlignW;    /* LongWord size alignment */
};

struct pwr_s_Intern {
  pwr_sParInfo	Info 	pwr_dAlignLW;
  pwr_tTypeId  	TypeRef	pwr_dAlignW;	/* Reference to the object defining
                                           the type.  */
  pwr_sParGraph	Graph	pwr_dAlignW;  	/* Used by graphic editor.  */
};

struct pwr_s_Input {
  pwr_sParInfo	Info 	pwr_dAlignLW;
  pwr_tTypeId  	TypeRef	pwr_dAlignW;	/* Reference to the object defining
                                           the type.  */
  pwr_sParGraph	Graph	pwr_dAlignW;  	/* Used by graphic editor.  */
};

struct pwr_s_Output {
  pwr_sParInfo	Info 	pwr_dAlignLW;
  pwr_tTypeId  	TypeRef	pwr_dAlignW;	/* Reference to the object defining
                                           the type.  */
  pwr_sParGraph	Graph	pwr_dAlignW;  	/* Used by graphic editor.  */
};

struct pwr_s_AttrXRef {
  pwr_sParInfo	Info 		pwr_dAlignLW;
  pwr_tXRef    	Identity	pwr_dAlignW;	/* A name to identify a cross
					           reference.  */
  pwr_tXRef    	Source		pwr_dAlignW;
  pwr_tXRef    	Target		pwr_dAlignW;
  pwr_eXRefType	XRefType	pwr_dAlignW;
};

struct pwr_s_ObjXRef {
  pwr_sParInfo	Info 		pwr_dAlignLW;
  pwr_tXRef    	Identity	pwr_dAlignW;	/* A name to identify a cross
                                	           reference.  */
  pwr_tXRef    	Source		pwr_dAlignW;
  pwr_tXRef    	Target		pwr_dAlignW;
  pwr_eXRefType	XRefType	pwr_dAlignW;
  pwr_tObjName	SourceAttribute	pwr_dAlignW;
  pwr_tObjName	TargetAttribute	pwr_dAlignW;
};

struct pwr_s_Buffer {
  pwr_sParInfo	Info 	pwr_dAlignLW;
  pwr_eClass   	Class	pwr_dAlignW;		/* The system class defining the
						   data stored here.  */
};
    
/* The union pwr_uParDef contains all classes defining parameters.  */

union pwr_u_ParDef {
  pwr_sInput		Input;
  pwr_sOutput		Output;
  pwr_sIntern		Intern;
  pwr_sObjXRef		ObjXRef;
  pwr_sAttrXRef		AttrXRef;
  pwr_sBuffer		Buffer;
  pwr_sParam		Param;
};

struct pwr_s_Bit {
  pwr_tString80 Text 	pwr_dAlignLW;
  pwr_tString32 PgmName	pwr_dAlignW;
  pwr_tUInt32	Value	pwr_dAlignW;
};

struct pwr_s_Value {
  pwr_tString80 Text 	pwr_dAlignLW;
  pwr_tString32 PgmName	pwr_dAlignW;
  pwr_tInt32	Value	pwr_dAlignW;
};

/* Method defining classes.

   This section defines the classes to use when defining methods.  */

struct pwr_s_DbCallBack {
  pwr_tString80	MethodName 		pwr_dAlignLW;
  pwr_tString40	MethodArguments[5]	pwr_dAlignW;
  pwr_tStatus  	(*Method)()		pwr_dAlignLW;	/* Address to method. */
  pwr_tUInt32  	Flags			pwr_dAlignW;
};
  
struct pwr_s_Method {
  pwr_tString80	MethodName 		pwr_dAlignLW;
  pwr_tString40	MethodArguments[5]	pwr_dAlignW;
};


/* Menu defining classes.

   This section defines the classes to use when defining menus.  */

struct pwr_s_MenuCascade {
  pwr_tString40	ButtonName 		pwr_dAlignLW;
  pwr_tString80	FilterName		pwr_dAlignW;
  pwr_tString40	FilterArguments[5]	pwr_dAlignW;
  pwr_tBoolean	(*Filter)()		pwr_dAlignLW; /* Address to method
                                         	          visibility function.  */
};

struct pwr_s_MenuButton {
  pwr_tString40	ButtonName	 	pwr_dAlignLW;
  pwr_tString80	MethodName		pwr_dAlignW;
  pwr_tString80	MethodArguments[5]	pwr_dAlignW;
  pwr_tString80	FilterName		pwr_dAlignW;
  pwr_tString80	FilterArguments[5]	pwr_dAlignW;
  pwr_tStatus	(*Method)()		pwr_dAlignLW;		/* Address to method.  */
  pwr_tBoolean	(*Filter)()		pwr_dAlignLW;		/* Address to method
								   visibility function.  */
  pwr_tUInt32		Flags		pwr_dAlignW;
};

struct pwr_s_MenuRef {
  pwr_tString40	ButtonName	 	pwr_dAlignLW;
  pwr_tString40	RefAttribute		pwr_dAlignW;
  pwr_tString80	FilterName		pwr_dAlignW;
  pwr_tString40	FilterArguments[5]	pwr_dAlignW;
  pwr_tBoolean	(*Filter)()		pwr_dAlignLW;		/* Address to method
                                        		           visibility function.  */
};

/* Classes for PLC editors.

   This section defines the classes which are used by the graphic PLC
   editors.  */

struct pwr_s_Point {
  pwr_tFloat32	x 	pwr_dAlignW;
  pwr_tFloat32	y	pwr_dAlignW;
};

struct pwr_s_PlcProgram {
  pwr_tObjid		oid 		pwr_dAlignLW;
  pwr_tUInt32		object_type	pwr_dAlignW;
  pwr_tClassId		cid		pwr_dAlignW;
  pwr_tUInt32		defnamecount[PWR_OBJTYPES_MAX]	pwr_dAlignW;
  pwr_tObjid		reset_objdid	pwr_dAlignW;
  pwr_tUInt32		connamecount	pwr_dAlignW;
  pwr_tObjid		woid		pwr_dAlignW;
};

struct pwr_s_PlcWindow {
  pwr_tObjid		oid	 	pwr_dAlignLW;
  pwr_tUInt32		object_type	pwr_dAlignW;
  pwr_tClassId		cid		pwr_dAlignW;
  pwr_tFloat32		x		pwr_dAlignW;
  pwr_tFloat32		y		pwr_dAlignW;	
  pwr_tFloat32		width		pwr_dAlignW;
  pwr_tFloat32		height		pwr_dAlignW;
  pwr_tFloat32		zoom		pwr_dAlignW;
  pwr_tInt32		x_root		pwr_dAlignW;
  pwr_tInt32		y_root		pwr_dAlignW;
  pwr_tObjid		poid		pwr_dAlignW;
  pwr_tUInt32		compobjcount	pwr_dAlignW;
  pwr_tUInt32		refconcount	pwr_dAlignW;
  pwr_tUInt32		subwindowindex	pwr_dAlignW;
};

struct pwr_s_PlcNode {
  pwr_tUInt32		object_type 	pwr_dAlignLW;
  pwr_tClassId		cid		pwr_dAlignW;
  pwr_tObjid		oid		pwr_dAlignW;
  pwr_tFloat32		x		pwr_dAlignW;
  pwr_tFloat32		y		pwr_dAlignW;
  pwr_tFloat32		width		pwr_dAlignW;
  pwr_tFloat32		height		pwr_dAlignW;
  pwr_tObjid		woid		pwr_dAlignW;
  pwr_tObjid		subwind_oid[2]	pwr_dAlignW;
  pwr_tUInt32		subwindow	pwr_dAlignW;
  pwr_tUInt32		graphtype	pwr_dAlignW;
  pwr_tUInt32		mask[3]		pwr_dAlignW;
  pwr_tUInt32		compdirection	pwr_dAlignW;
  pwr_tUInt32		nodewidth	pwr_dAlignW;
};

struct pwr_s_PlcConnection {
  pwr_tObjid		oid 		pwr_dAlignLW;
  pwr_tUInt32		object_type	pwr_dAlignW;
  pwr_tClassId		cid		pwr_dAlignW;
  pwr_tUInt32		curvature	pwr_dAlignW;
  pwr_tUInt32		drawtype	pwr_dAlignW;
  pwr_tUInt32		attributes	pwr_dAlignW;
  pwr_tUInt32		refnr		pwr_dAlignW;
  pwr_tUInt32		source_point	pwr_dAlignW;
  pwr_tObjid		source_oid	pwr_dAlignW;
  pwr_tUInt32		dest_point	pwr_dAlignW;
  pwr_tObjid		dest_oid	pwr_dAlignW;
  pwr_tUInt32		point_count	pwr_dAlignW;
  pwr_sPoint		point[PWR_BREAKPOINTS_MAX]	pwr_dAlignLW;
  pwr_tObjid		woid		pwr_dAlignLW;
};

struct pwr_s_GraphPlcProgram {
  pwr_tUInt32		plc_type 	pwr_dAlignLW;
  pwr_tClassId		subwindow_class	pwr_dAlignW;
};

struct pwr_s_GraphPlcWindow {
  pwr_tUInt32		window_type	 	pwr_dAlignLW;
  pwr_tUInt32		defaultobjects		pwr_dAlignW;
  pwr_tUInt32		width			pwr_dAlignW;
  pwr_tUInt32		height			pwr_dAlignW;
  pwr_tUInt32		x			pwr_dAlignW;
  pwr_tUInt32		y			pwr_dAlignW;
  pwr_tFloat32		zoom			pwr_dAlignW;
  pwr_tObjid		node_palettelayout	pwr_dAlignW;
  pwr_tObjid		con_palettelayout	pwr_dAlignW;
  pwr_tObjid		trace_palettelayout	pwr_dAlignW;
  pwr_tObjid		sim_palettelayout	pwr_dAlignW;
  pwr_tClassId		defobj_class[PRW_DEFOBJ_MAX] pwr_dAlignW;
  pwr_tFloat32		defobj_x[PRW_DEFOBJ_MAX] pwr_dAlignW;
  pwr_tFloat32		defobj_y[PRW_DEFOBJ_MAX] pwr_dAlignW;
  pwr_tUInt32		compmethod		pwr_dAlignW;
  pwr_tUInt32		compindex		pwr_dAlignW;
  pwr_tUInt32		tracemethod		pwr_dAlignW;
  pwr_tUInt32		traceindex		pwr_dAlignW;
  pwr_tUInt32		executeordermethod	pwr_dAlignW;
  pwr_tString16		objname			pwr_dAlignW;
  pwr_tString16		graphname		pwr_dAlignW;
};

struct pwr_s_GraphPlcNode {
  pwr_tUInt32		object_type    	pwr_dAlignLW;
  pwr_tUInt32		parameters[4]	pwr_dAlignW;
  pwr_tUInt32		subwindows	pwr_dAlignW;
  pwr_tClassId		subwindow_class[2] pwr_dAlignW;
  pwr_tFloat32		sw_page_x[2]	pwr_dAlignW;
  pwr_tFloat32		sw_page_y[2]	pwr_dAlignW;
  pwr_tUInt32		graphmethod	pwr_dAlignW;
  pwr_tUInt32		graphindex	pwr_dAlignW;
  pwr_tUInt32		default_mask[2]	pwr_dAlignW;
  pwr_tUInt32		segname_annotation pwr_dAlignW;
  pwr_tBoolean		rtbody_annotation pwr_dAlignW;
  pwr_tBoolean		devbody_annotation pwr_dAlignW;
  pwr_tUInt32		compmethod	pwr_dAlignW;
  pwr_tUInt32		compindex	pwr_dAlignW;
  pwr_tUInt32		tracemethod	pwr_dAlignW;
  pwr_tUInt32		traceindex	pwr_dAlignW;
  pwr_tUInt32		connectmethod	pwr_dAlignW;
  pwr_tUInt32		executeordermethod pwr_dAlignW;
  pwr_tString16		objname		pwr_dAlignW;
  pwr_tString16		graphname	pwr_dAlignW;
  pwr_tString32		debugpar	pwr_dAlignW;
};

struct pwr_s_GraphPlcConnection {
  pwr_tUInt32		con_type       	pwr_dAlignLW;
  pwr_tInt32		arrows		pwr_dAlignW;
  pwr_tFloat32		linewidth	pwr_dAlignW;
  pwr_tFloat32		dashes		pwr_dAlignW;
  pwr_tInt32 		fillpattern	pwr_dAlignW;
  pwr_tInt32		color		pwr_dAlignW;
  pwr_tInt32		curvature	pwr_dAlignW;
  pwr_tFloat32		corners		pwr_dAlignW;
  pwr_tUInt32		attributes	pwr_dAlignW;
};

/* Plant defining classes.  */

struct pwr_s_PlantHier {
  pwr_tString80		Description 	pwr_dAlignLW;
  pwr_tAttrRef        	DefGraph	pwr_dAlignW;
  pwr_tAttrRef        	DefTrend	pwr_dAlignW;
  pwr_tString40       	HelpTopic	pwr_dAlignW;
  pwr_tURL            	DataSheet	pwr_dAlignW;
  pwr_tURL            	CircuitDiagram	pwr_dAlignW;
  pwr_tURL            	Photo		pwr_dAlignW;
  pwr_tConfigStatusEnum	ConfigurationStatus pwr_dAlignW;
};

/* Node defining classes.  */

struct pwr_s_System {
  pwr_tString80		Description 	pwr_dAlignLW;
  char			SystemName[80]	pwr_dAlignW;
  char			SystemGroup[80]	pwr_dAlignW;
};

struct pwr_s_Node {
  pwr_tString80		Description 	pwr_dAlignLW;
  char			ErrLogTerm[132]	pwr_dAlignW;
  char			ErrLogFile[132]	pwr_dAlignW;
  pwr_tTime		BootTime	pwr_dAlignLW;
  pwr_tTime		BootVersion	pwr_dAlignLW;
  pwr_tProjVersion	BootPlcVersion	pwr_dAlignW;
  pwr_tTime		CurrentVersion	pwr_dAlignLW;
  pwr_tProjVersion	CurrentPlcVersion pwr_dAlignW;
  pwr_tUInt32		Restarts	pwr_dAlignW;
  pwr_tTime		RestartTime	pwr_dAlignLW;
  pwr_tDeltaTime	RestartStallTime pwr_dAlignLW;
  pwr_tTime		SystemTime	pwr_dAlignLW;
  pwr_tNetStatus	SystemStatus	pwr_dAlignW;  
  pwr_tStatus		ProcStatus[40]	pwr_dAlignW;
  pwr_tStatus		ProcMsgSeverity[40] pwr_dAlignW;
  pwr_tString80      	ProcMessage[40]	pwr_dAlignW;
  pwr_tOid		ProcObject[40]	pwr_dAlignW;
  pwr_tTime		ProcTimeStamp[40] pwr_dAlignLW;
  pwr_tBoolean		EmergBreakTrue	pwr_dAlignW;
  pwr_tUInt32		EmergBreakSelect pwr_dAlignW;
};

#define pwr_mAppl_PLC	1	/* This is the PLC program */

struct pwr_s_Appl {
  pwr_tString80		Description 	pwr_dAlignLW;
  char			FileName[256]	pwr_dAlignW;
  char			ProgramName[40]	pwr_dAlignW;
  pwr_tBoolean		StartWithDebug	pwr_dAlignW;
  pwr_tBoolean		Load		pwr_dAlignW;
  pwr_tBoolean		Run		pwr_dAlignW;
  pwr_tBoolean		KernelMode	pwr_dAlignW;
  pwr_tInt32		KernelStackSize	pwr_dAlignW;
  pwr_tInt32		JobPriority	pwr_dAlignW;
  pwr_tInt32		ProcessPriority	pwr_dAlignW;
  char			Arg[256]	pwr_dAlignW;
};

struct pwr_s_Alias {
  pwr_tString80		Description 	pwr_dAlignLW;
  pwr_tObjid		Object		pwr_dAlignW;
};

struct pwr_s_Hier {
  pwr_tString80		Description	pwr_dAlignLW;
};

struct pwr_s_DocHier {
  pwr_tString80		Description	pwr_dAlignLW;
};

struct pwr_s_LibHier {
  pwr_tString80		Description 	pwr_dAlignLW;
};

struct pwr_s_NodeHier {
  pwr_tString80		Description	pwr_dAlignLW;
  pwr_tConfigStatusEnum	ConfigurationStatus pwr_dAlignW;
};

struct pwr_s_RootVolume {
  pwr_tString80		Description 	pwr_dAlignLW;
  pwr_tObjectIx		NextOix		pwr_dAlignW;
  pwr_tProjVersion	RtVersion	pwr_dAlignW;
  pwr_tTime		RtCreTime	pwr_dAlignLW;
  pwr_tString16		RtCreator	pwr_dAlignW;
  pwr_tUInt32		RtCardinality	pwr_dAlignW;
  pwr_tUInt32		RtBodySize	pwr_dAlignW;
  pwr_mOpSys		OperatingSystem	pwr_dAlignW;
  pwr_tTime		Modified	pwr_dAlignLW;
  pwr_tConfigStatusEnum	ConfigurationStatus pwr_dAlignW;
};

struct pwr_s_SubVolume {
  pwr_tString80		Description 	pwr_dAlignLW;
  pwr_tObjectIx		NextOix		pwr_dAlignW;
  pwr_tProjVersion	RtVersion	pwr_dAlignW;
  pwr_tTime		RtCreTime	pwr_dAlignLW;
  pwr_tString16		RtCreator	pwr_dAlignW;
  pwr_tUInt32		RtCardinality	pwr_dAlignW;
  pwr_tUInt32		RtBodySize	pwr_dAlignW;
  pwr_mOpSys		OperatingSystem	pwr_dAlignW;
  pwr_tTime		Modified	pwr_dAlignLW;
  pwr_tConfigStatusEnum	ConfigurationStatus pwr_dAlignW;
};

struct pwr_s_SharedVolume {
  pwr_tString80		Description 	pwr_dAlignLW;
  pwr_tObjectIx		NextOix		pwr_dAlignW;
  pwr_tProjVersion	RtVersion	pwr_dAlignW;
  pwr_tTime		RtCreTime	pwr_dAlignLW;
  pwr_tString16		RtCreator	pwr_dAlignW;
  pwr_tUInt32		RtCardinality	pwr_dAlignW;
  pwr_tUInt32		RtBodySize	pwr_dAlignW;
  pwr_tTime		Modified	pwr_dAlignLW;
  pwr_tConfigStatusEnum	ConfigurationStatus pwr_dAlignW;
};

struct pwr_s_DynamicVolume {
  pwr_tString80		Description;
  pwr_tObjectIx		NextOix		pwr_dAlignW;
  pwr_tProjVersion	RtVersion	pwr_dAlignW;
  pwr_tTime		RtCreTime	pwr_dAlignLW;
  pwr_tString16		RtCreator	pwr_dAlignW;
  pwr_tUInt32		RtCardinality	pwr_dAlignW;
  pwr_tUInt32		RtBodySize	pwr_dAlignW;
};

struct pwr_s_SystemVolume {
  pwr_tString80		Description 	pwr_dAlignLW;
  pwr_tObjectIx		NextOix		pwr_dAlignW;
  pwr_tProjVersion	RtVersion	pwr_dAlignW;
  pwr_tTime		RtCreTime	pwr_dAlignLW;
  pwr_tString16		RtCreator	pwr_dAlignW;
  pwr_tUInt32		RtCardinality	pwr_dAlignW;
  pwr_tUInt32		RtBodySize	pwr_dAlignW;
};

struct pwr_s_ClassVolume {
  pwr_tString80		Description 	pwr_dAlignLW;
  pwr_tObjectIx		NextOix		pwr_dAlignW;
  pwr_tProjVersion	RtVersion	pwr_dAlignW;
  pwr_tTime		RtCreTime	pwr_dAlignLW;
  pwr_tString16		RtCreator	pwr_dAlignW;
  pwr_tUInt32		RtCardinality	pwr_dAlignW;
  pwr_tUInt32		RtBodySize	pwr_dAlignW;
  pwr_tObjectIx		NextCix		pwr_dAlignW;	/* Next free class index.  */
  pwr_tObjectIx		NextTix[pwr_cMaxTyg + 1]  pwr_dAlignW;	/* Next free type index.  */
  pwr_tUInt32		DvVersion	pwr_dAlignW;
  pwr_tConfigStatusEnum	ConfigurationStatus pwr_dAlignW;
};

struct pwr_s_WorkBenchVolume {
  pwr_tString80		Description 	pwr_dAlignLW;
  pwr_tObjectIx		NextOix		pwr_dAlignW;
  pwr_tProjVersion	RtVersion	pwr_dAlignW;
  pwr_tTime		RtCreTime	pwr_dAlignLW;
  pwr_tString16		RtCreator	pwr_dAlignW;
  pwr_tUInt32		RtCardinality	pwr_dAlignW;
  pwr_tUInt32		RtBodySize	pwr_dAlignW;
};

struct pwr_s_DirectoryVolume {
  pwr_tString80		Description 	pwr_dAlignLW;
  pwr_tObjectIx		NextOix		pwr_dAlignW;
  pwr_tProjVersion	RtVersion	pwr_dAlignW;
  pwr_tTime		RtCreTime	pwr_dAlignLW;
  pwr_tString16		RtCreator	pwr_dAlignW;
  pwr_tUInt32		RtCardinality	pwr_dAlignW;
  pwr_tUInt32		RtBodySize	pwr_dAlignW;
};

struct pwr_s_VolatileVolume {
  pwr_tString80		Description 	pwr_dAlignLW;
  pwr_tObjectIx		NextOix		pwr_dAlignW;
};

struct pwr_s_ExternVolume {
  pwr_tString80		Description 	pwr_dAlignLW;
};

struct pwr_s_CreateVolume {
  pwr_tString80		Description 	pwr_dAlignLW;
  pwr_tVolumeId		Volume		pwr_dAlignW;
};

struct pwr_s_MountVolume {
  pwr_tString80		Description 	pwr_dAlignLW;
  pwr_tVolumeId		Volume		pwr_dAlignW;
  pwr_eVolumeAccess	Access	       	pwr_dAlignW;
  pwr_eMountType	MountType	pwr_dAlignW;
  pwr_eMountedType	MountedType	pwr_dAlignW;
  pwr_tUInt32		CacheMaxCount	pwr_dAlignW;
  pwr_tUInt32		CacheMinCount	pwr_dAlignW;
  pwr_tUInt32		CacheCurCount	pwr_dAlignW;
};

struct pwr_s_MountObject {
  pwr_tString80		Description 	pwr_dAlignLW;
  pwr_tObjid		Object		pwr_dAlignW;
};

struct pwr_s_Security {
  pwr_tMask	DefaultWebPriv 		pwr_dAlignLW;
  pwr_tMask	DefaultXttPriv		pwr_dAlignW;
  pwr_tBoolean	XttUseOpsysUser		pwr_dAlignW;
  pwr_tString80 WebSystemGroup		pwr_dAlignW;
};

union pwr_u_Volume {
  pwr_sRootVolume	Root;
  pwr_sSubVolume	Sub;
  pwr_sSharedVolume	Shared;
  pwr_sDynamicVolume	Dynamic;
  pwr_sSystemVolume	System;
  pwr_sClassVolume	Class;
  pwr_sWorkBenchVolume	WorkBench;
  pwr_sDirectoryVolume	Directory;
};

#ifdef __cplusplus
}
#endif

#endif
