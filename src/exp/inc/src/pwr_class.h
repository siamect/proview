#ifndef pwr_class_h
#define pwr_class_h

/* pwr_class.h -- system classes

   PROVIEW/R
   Copyright (C) 1989-1996 by Comator Process AB.

   <Description>.  */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef pwr_h
#include "pwr.h"
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
typedef struct pwr_s_BootVersion	pwr_sBootVersion;
typedef struct pwr_s_Node		pwr_sNode;
typedef struct pwr_s_NodeVersion	pwr_sNodeVersion;
typedef struct pwr_s_Appl		pwr_sAppl;
typedef struct pwr_s_System		pwr_sSystem;
typedef struct pwr_s_SystemVersion	pwr_sSystemVersion;
typedef struct pwr_s_MenuCascade	pwr_sMenuCascade;
typedef struct pwr_s_MenuButton		pwr_sMenuButton;
typedef struct pwr_s_Object		pwr_sObject;
typedef struct pwr_s_DbCallBack		pwr_sDbCallBack;
typedef struct pwr_s_RootVolume		pwr_sRootVolume;
typedef struct pwr_s_SubVolume		pwr_sSubVolume;
typedef struct pwr_s_SharedVolume	pwr_sSharedVolume;
typedef struct pwr_s_DynamicVolume	pwr_sDynamicVolume;
typedef struct pwr_s_SystemVolume	pwr_sSystemVolume;
typedef struct pwr_s_ClassVolume	pwr_sClassVolume;
typedef struct pwr_s_WorkBenchVolume	pwr_sWorkBenchVolume;
typedef struct pwr_s_DirectoryVolume	pwr_sDirectoryVolume;
typedef struct pwr_s_CreateVolume	pwr_sCreateVolume;
typedef struct pwr_s_MountVolume	pwr_sMountVolume;
typedef struct pwr_s_MountObject	pwr_sMountObject;

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
  pwr_eType_		= pwr_TypeId(pwr_eTix_)
} pwr_eType;

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
  pwr_eCix_SystemVersion	=  36,
  pwr_eCix_NodeVersion		=  37,
  pwr_eCix_BootVersion		=  38,
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
  pwr_eClass_SystemVersion	= pwr_ClassId(pwr_eCix_SystemVersion),
  pwr_eClass_NodeVersion	= pwr_ClassId(pwr_eCix_NodeVersion),
  pwr_eClass_BootVersion	= pwr_ClassId(pwr_eCix_BootVersion),
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

/* Operating system.  */
#if 1

typedef enum {
  pwr_mOpSys__ = 0,
  pwr_mOpSys_VAX_ELN =  1 << 0,
  pwr_mOpSys_VAX_VMS =  1 << 1,
  pwr_mOpSys_AXP_VMS =  1 << 2,
  pwr_mOpSys_PPC_LYNX = 1 << 3,
  pwr_mOpSys_X86_LYNX = 1 << 4,
  pwr_mOpSys_PPC_LINUX = 1 << 5,
  pwr_mOpSys_X86_LINUX = 1 << 6,
  pwr_mOpSys_AXP_LINUX = 1 << 7,
  pwr_mOpSys_ = 1 << 8
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

struct pwr_s_Type
    {
    pwr_eType		Type;
    pwr_tInt32		Size;			/* Number of bytes. */
    };

struct pwr_s_TypeDef
    {
    pwr_eType		Type;
    pwr_tInt32		Size;			/* Number of bytes. */
    pwr_tTypeId		TypeRef;
    pwr_tInt32		Elements;
    };

/* Class defining classes. 

   This section defines the classes to use when defining classes.  */

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
    pwr_Bits( HasCallBack , 1),	/* object has DbCallBack  */
    pwr_Bits( fill_0      , 3),,,
    pwr_Bits( fill_1      , 8),,,,,,,,
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
#define pwr_mClassDef_HasCallBack pwr_Bit(12)

#define pwr_mClassDef_HasRef	  (pwr_mClassDef_ObjXRef|pwr_mClassDef_AttrXRef|\
				   pwr_mClassDef_ObjRef|pwr_mClassDef_AttrRef)
};

struct pwr_s_ClassDef
    {
    pwr_eEditor		Editor;		/* Editor to call on a "Edit"
                                           command.  */
    pwr_eMethod		Method;		/* Method to use by the called
                                           editor.  */
    pwr_mClassDef	Flags;
    pwr_tUInt32		NumOfObjBodies;
    pwr_ePopEditor	PopEditor;	/* What kind of object picture
                                           dispatcher to use.  */
    };


/* Body defining classes.

  This section defines the classes to use when defining bodies.  */

struct pwr_s_ObjBodyDef
    {
    pwr_tStructName	StructName;	/* Name of the C-struct to be
                                           generated.  */
    pwr_tUInt32		NumOfParams;	/* Compiled number of parameters.  */
    pwr_tUInt32		Size;		/* Compiled size.  */
    pwr_tObjectIx	NextAix;	/* Next free attribute index.  */ 
    pwr_tMask		Flags;		/* Not used */
    };

/* Parameter defining classes.

   This section defines the classes to be used when defining parameters
   in classes.
  
   At the end of this section is a union: pwr_uPar, wich must be updated for
   each new parameter class defined.  */

/* This class is used to define graphic appearence of parameters.  */

struct pwr_s_ParGraph
    {
    pwr_tGraphName	GraphName;
    pwr_tUInt32		InputType;
    pwr_tUInt32		NiNaAnnot;
    pwr_tBoolean	NiNaCond;
    pwr_tBoolean	NiNaSegments;
    pwr_tUInt32		DebAnnot;
    pwr_tUInt32		ConPointNr;
    };

/* This class is used to define information about parameters.  */


struct pwr_s_ParInfo
    {
    pwr_tPgmName	PgmName;	/* Name to be used in program. */
    pwr_eType		Type;		/* Compiled system type. */
    pwr_tUInt32		Offset;		/* Compiled offset in body. */
    pwr_tUInt32		Size;		/* Compiled size, number of bytes. */
    pwr_tUInt32		Flags;		/* Parameter flags */
    pwr_tUInt32		Elements;	/* If array; number of rows. */
    pwr_tUInt32		ParamIndex;	/* Index of param within a body. */
    };

/* Bitmask for flags  */

union pwr_m_Adef {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( pointer	, 1),
    pwr_Bits( array	, 1),
    pwr_Bits( backup	, 1),
    pwr_Bits( changelog	, 1),
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
    pwr_Bits( subclass  , 1),  
    pwr_Bits( buffer    , 1),  
    pwr_Bits( fill_0	, 4),,,,

    pwr_Bits( fill_1	, 8),,,,,,,
  ) b;

#define pwr_mAdef_pointer	pwr_Bit(0)		/*  1	  */
#define pwr_mAdef_array		pwr_Bit(1)		/*  2	  */
#define pwr_mAdef_backup	pwr_Bit(2)		/*  4	  */
#define pwr_mAdef_changelog	pwr_Bit(3)		/*  8	  */
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
#define pwr_mAdef_subclass	pwr_Bit(18)		
#define pwr_mAdef_buffer	pwr_Bit(19)		
};

#define PWR_MASK_POINTER	pwr_mAdef_pointer
#define PWR_MASK_ARRAY		pwr_mAdef_array
#define PWR_MASK_BACKUP		pwr_mAdef_backup
#define PWR_MASK_CHANGELOG	pwr_mAdef_changelog
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
#define PWR_MASK_SUBCLASS	pwr_mAdef_subclass
#define PWR_MASK_BUFFER	        pwr_mAdef_buffer

struct pwr_s_Param
    {
    pwr_sParInfo	Info;
    pwr_tTypeId		TypeRef;	/* Reference to the object defining
                                           the type.  */
    };

struct pwr_s_Intern
    {
    pwr_sParInfo	Info;
    pwr_tTypeId		TypeRef;	/* Reference to the object defining
                                           the type.  */
    pwr_sParGraph	Graph;		/* Used by graphic editor.  */
    };

struct pwr_s_Input
    {
    pwr_sParInfo	Info;
    pwr_tTypeId		TypeRef;	/* Reference to the object defining
                                           the type.  */
    pwr_sParGraph	Graph;		/* Used by graphic editor.  */
    };

struct pwr_s_Output
    {
    pwr_sParInfo	Info;
    pwr_tTypeId		TypeRef;	/* Reference to the object defining
                                           the type.  */
    pwr_sParGraph	Graph;		/* Used by graphic editor.  */
    };

struct pwr_s_AttrXRef
    {
    pwr_sParInfo	Info;
    pwr_tXRef		Identity;	/* A name to identify a cross
                                           reference.  */
    pwr_tXRef		Source;
    pwr_tXRef		Target;
    pwr_eXRefType	XRefType;
    };

struct pwr_s_ObjXRef
    {
    pwr_sParInfo	Info;
    pwr_tXRef		Identity;	/* A name to identify a cross
                                           reference.  */
    pwr_tXRef		Source;
    pwr_tXRef		Target;
    pwr_eXRefType	XRefType;
    pwr_tObjName	SourceAttribute;
    pwr_tObjName	TargetAttribute;
    };

struct pwr_s_Buffer
    {
    pwr_sParInfo	Info;
    pwr_eClass		Class;		/* The system class defining the
                                           data stored here.  */
    };
    
/* The union pwr_uParDef contains all classes defining parameters.  */

union pwr_u_ParDef
    {
    pwr_sInput		Input;
    pwr_sOutput		Output;
    pwr_sIntern		Intern;
    pwr_sObjXRef	ObjXRef;
    pwr_sAttrXRef	AttrXRef;
    pwr_sBuffer		Buffer;
    pwr_sParam		Param;
    };

/* Method defining classes.

   This section defines the classes to use when defining methods.  */

struct pwr_s_DbCallBack
    {
    pwr_tString80	MethodName;
    pwr_tString40	MethodArguments[5];
    pwr_tStatus		(*Method)();	/* Address to method. */
    pwr_tUInt32		Flags;
    };


/* Menu defining classes.

   This section defines the classes to use when defining menus.  */

struct pwr_s_MenuCascade
    {
    pwr_tString40	ButtonName;
    pwr_tString80	FilterName;
    pwr_tString40	FilterArguments[5];
    pwr_tBoolean	(*Filter)();		/* Address to method
                                                   visibility function.  */
    };

struct pwr_s_MenuButton
    {
    pwr_tString40	ButtonName;
    pwr_tString80	MethodName;
    pwr_tString40	MethodArguments[5];
    pwr_tString80	FilterName;
    pwr_tString40	FilterArguments[5];
    pwr_tStatus		(*Method)();		/* Address to method.  */
    pwr_tBoolean	(*Filter)();		/* Address to method
                                                   visibility function.  */
    pwr_tUInt32		Flags;
    };


/* Classes for PLC editors.

   This section defines the classes which are used by the graphic PLC
   editors.  */

struct pwr_s_Point
    {
    pwr_tFloat32	x;
    pwr_tFloat32	y;
    };

struct pwr_s_PlcProgram
    {
    pwr_tObjid		objdid;
    pwr_tUInt32		object_type;
    pwr_tClassId	classid;
    pwr_tUInt32		defnamecount[PWR_OBJTYPES_MAX];
    pwr_tObjid		reset_objdid;
    pwr_tUInt32		connamecount;
    pwr_tObjid		window_did;
    };

struct pwr_s_PlcWindow
    {
    pwr_tObjid		objdid;
    pwr_tUInt32		object_type;
    pwr_tClassId	classid;
    pwr_tFloat32	x;
    pwr_tFloat32	y;	
    pwr_tFloat32	width;
    pwr_tFloat32	height;
    pwr_tFloat32	zoom;
    pwr_tInt32		x_root;
    pwr_tInt32		y_root;
    pwr_tObjid		parent_node_did;
    pwr_tUInt32		compobjcount;
    pwr_tUInt32		refconcount;
    pwr_tUInt32		subwindowindex;
    };

struct pwr_s_PlcNode
    {
    pwr_tUInt32		object_type;
    pwr_tClassId	classid;
    pwr_tObjid		object_did;
    pwr_tFloat32	x;
    pwr_tFloat32	y;
    pwr_tFloat32	width;
    pwr_tFloat32	height;
    pwr_tObjid		window_did;
    pwr_tObjid		subwind_objdid[2];
    pwr_tUInt32		subwindow;
    pwr_tUInt32		graphtype;
    pwr_tUInt32		mask[3];
    pwr_tUInt32		compdirection;
    pwr_tUInt32		nodewidth;
    };

struct pwr_s_PlcConnection
    {
    pwr_tObjid		objdid;
    pwr_tUInt32		object_type;
    pwr_tClassId	classid;
    pwr_tUInt32		curvature;
    pwr_tUInt32		drawtype;
    pwr_tUInt32		attributes;
    pwr_tUInt32		refnr;
    pwr_tUInt32		source_point;
    pwr_tObjid		source_node_did;
    pwr_tUInt32		dest_point;
    pwr_tObjid		dest_node_did;
    pwr_tUInt32		point_count;
    pwr_sPoint		point[PWR_BREAKPOINTS_MAX];
    pwr_tObjid		window_did;
    };

struct pwr_s_GraphPlcProgram
    {
    pwr_tUInt32		plc_type;
    pwr_tClassId	subwindow_class;
    };

struct pwr_s_GraphPlcWindow
    {
    pwr_tUInt32		window_type;
    pwr_tUInt32		defaultobjects;
    pwr_tUInt32		width;
    pwr_tUInt32		height;
    pwr_tUInt32		x;
    pwr_tUInt32		y;
    pwr_tFloat32	zoom;
    pwr_tObjid		node_palettelayout;
    pwr_tObjid		con_palettelayout;
    pwr_tObjid		trace_palettelayout;
    pwr_tObjid		sim_palettelayout;
    pwr_tClassId	defobj_class[PRW_DEFOBJ_MAX];
    pwr_tFloat32	defobj_x[PRW_DEFOBJ_MAX];
    pwr_tFloat32	defobj_y[PRW_DEFOBJ_MAX];
    pwr_tUInt32		compmethod;
    pwr_tUInt32		compindex;
    pwr_tUInt32		tracemethod;
    pwr_tUInt32		traceindex;
    pwr_tUInt32		executeordermethod;
    pwr_tString16	objname;
    pwr_tString16	graphname;
    };

struct pwr_s_GraphPlcNode
    {
    pwr_tUInt32		object_type;
    pwr_tUInt32		parameters[4];
    pwr_tUInt32		subwindows;
    pwr_tClassId	subwindow_class[2];
    pwr_tFloat32	sw_page_x[2];
    pwr_tFloat32	sw_page_y[2];
    pwr_tUInt32		graphmethod;
    pwr_tUInt32		graphindex;
    pwr_tUInt32		default_mask[2];
    pwr_tUInt32		segname_annotation;
    pwr_tBoolean	rtbody_annotation;
    pwr_tBoolean	devbody_annotation;
    pwr_tUInt32		compmethod;
    pwr_tUInt32		compindex;
    pwr_tUInt32		tracemethod;
    pwr_tUInt32		traceindex;
    pwr_tUInt32		connectmethod;
    pwr_tUInt32		executeordermethod;
    pwr_tString16	objname;
    pwr_tString16	graphname;
    pwr_tString32	debugpar;
    };

struct pwr_s_GraphPlcConnection
    {
    pwr_tUInt32		con_type;
    pwr_tInt32		arrows;
    pwr_tFloat32	linewidth;
    pwr_tFloat32	dashes;
    pwr_tInt32 		fillpattern;
    pwr_tInt32		color;
    pwr_tInt32		curvature;
    pwr_tFloat32	corners;
    pwr_tUInt32		attributes;
    };

/* Plant defining classes.  */

struct pwr_s_PlantHier
    {
    pwr_tString80	Description;
    pwr_tObjid          DefGraph;
    pwr_tObjid          DefTrend;
    pwr_tString40       HelpTopic;
    pwr_tURL            DataSheet;
    };

/* Node defining classes.  */

struct pwr_s_System
    {
    pwr_tString80	Description;
    char		SystemName[80];
    char		SystemGroup[80];
    };

struct pwr_s_SystemVersion
    {
    pwr_tProjVersion	RsmoVersion;
    pwr_tTime		RsmoCreTime;
    pwr_tUInt32		RsmoCardinality;
    pwr_tUInt32		RsmoBodySize;
    pwr_tProjVersion	RpmoVersion;
    pwr_tTime		RpmoCreTime;
    pwr_tUInt32		RpmoCardinality;
    pwr_tUInt32		RpmoBodySize;
    pwr_tProjVersion	RcoVersion;
    pwr_tTime		RcoCreTime;
    pwr_tUInt32		RcoCardinality;
    pwr_tUInt32		RcoBodySize;
    };

struct pwr_s_BootVersion
    {
    pwr_tProjVersion	BootVersion;
    pwr_tTime		BootCreTime;
    pwr_tObjid		Node[256];
    pwr_tProjVersion	DnoVersion[256];
    };

struct pwr_s_NodeVersion
    {
    pwr_tObjid		Node;
    pwr_tUInt32		Cardinality;
    pwr_tUInt32		BodySize;
    pwr_tProjVersion	DnoVersion;
    pwr_tTime		DnoCreTime;
    pwr_tUInt32		DnoCardinality;
    pwr_tUInt32		DnoBodySize;
    pwr_tProjVersion	RsmoVersion;
    pwr_tTime		RsmoCreTime;
    pwr_tUInt32		RsmoCardinality;
    pwr_tUInt32		RsmoBodySize;
    pwr_tProjVersion	RpmoVersion;
    pwr_tTime		RpmoCreTime;
    pwr_tUInt32		RpmoCardinality;
    pwr_tUInt32		RpmoBodySize;
    pwr_tProjVersion	RcoVersion;
    pwr_tTime		RcoCreTime;
    pwr_tUInt32		RcoCardinality;
    pwr_tUInt32		RcoBodySize;
    };

struct pwr_s_Node
    {
    pwr_tString80	Description;
    char		ErrLogTerm[132];
    char		ErrLogFile[132];
    pwr_tProjVersion	BootVersion;
    pwr_tTime		BootCreTime;
    pwr_tProjVersion	CurVersion;
    pwr_tTime		CurCreTime;
    pwr_tBoolean	EmergBreakTrue;
    pwr_tUInt32		EmergBreakSelect;
    };

#define pwr_mAppl_PLC	1	/* This is the PLC program */

struct pwr_s_Appl
    {
    pwr_tString80	Description;
    char		FileName[256];
    char		ProgramName[40];
    pwr_tBoolean	StartWithDebug;
    pwr_tBoolean	Load;
    pwr_tBoolean	Run;
    pwr_tBoolean	KernelMode;
    pwr_tInt32		KernelStackSize;
    pwr_tInt32		JobPriority;
    pwr_tInt32		ProcessPriority;
    char		Arg[256];
    };

struct pwr_s_Alias
    {
    pwr_tString80	Description;
    pwr_tObjid		Object;
    };

struct pwr_s_DocHier
    {
    pwr_tString80	Description;
    };

struct pwr_s_LibHier
    {
    pwr_tString80	Description;
    };

struct pwr_s_NodeHier
    {
    pwr_tString80	Description;
    };

struct pwr_s_RootVolume {
  pwr_tString80		Description;
  pwr_tObjectIx		NextOix;
  pwr_tProjVersion	RtVersion;
  pwr_tTime		RtCreTime;
  pwr_tString16		RtCreator;
  pwr_tUInt32		RtCardinality;
  pwr_tUInt32		RtBodySize;
  pwr_mOpSys		OperatingSystem;
};

struct pwr_s_SubVolume {
  pwr_tString80		Description;
  pwr_tObjectIx		NextOix;
  pwr_tProjVersion	RtVersion;
  pwr_tTime		RtCreTime;
  pwr_tString16		RtCreator;
  pwr_tUInt32		RtCardinality;
  pwr_tUInt32		RtBodySize;
  pwr_mOpSys		OperatingSystem;
};

struct pwr_s_SharedVolume {
  pwr_tString80		Description;
  pwr_tObjectIx		NextOix;
  pwr_tProjVersion	RtVersion;
  pwr_tTime		RtCreTime;
  pwr_tString16		RtCreator;
  pwr_tUInt32		RtCardinality;
  pwr_tUInt32		RtBodySize;
};

struct pwr_s_DynamicVolume {
  pwr_tString80		Description;
  pwr_tObjectIx		NextOix;
  pwr_tProjVersion	RtVersion;
  pwr_tTime		RtCreTime;
  pwr_tString16		RtCreator;
  pwr_tUInt32		RtCardinality;
  pwr_tUInt32		RtBodySize;
};

struct pwr_s_SystemVolume {
  pwr_tString80		Description;
  pwr_tObjectIx		NextOix;
  pwr_tProjVersion	RtVersion;
  pwr_tTime		RtCreTime;
  pwr_tString16		RtCreator;
  pwr_tUInt32		RtCardinality;
  pwr_tUInt32		RtBodySize;
};

struct pwr_s_ClassVolume {
  pwr_tString80		Description;
  pwr_tObjectIx		NextOix;
  pwr_tProjVersion	RtVersion;
  pwr_tTime		RtCreTime;
  pwr_tString16		RtCreator;
  pwr_tUInt32		RtCardinality;
  pwr_tUInt32		RtBodySize;
  pwr_tObjectIx		NextCix;			/* Next free class index.  */
  pwr_tObjectIx		NextTix[pwr_cMaxTyg + 1];	/* Next free type index.  */
};

struct pwr_s_WorkBenchVolume {
  pwr_tString80		Description;
  pwr_tObjectIx		NextOix;
  pwr_tProjVersion	RtVersion;
  pwr_tTime		RtCreTime;
  pwr_tString16		RtCreator;
  pwr_tUInt32		RtCardinality;
  pwr_tUInt32		RtBodySize;
};

struct pwr_s_DirectoryVolume {
  pwr_tString80		Description;
  pwr_tObjectIx		NextOix;
  pwr_tProjVersion	RtVersion;
  pwr_tTime		RtCreTime;
  pwr_tString16		RtCreator;
  pwr_tUInt32		RtCardinality;
  pwr_tUInt32		RtBodySize;
};

struct pwr_s_CreateVolume {
  pwr_tString80		Description;
  pwr_tVolumeId		Volume;
};

struct pwr_s_MountVolume {
  pwr_tString80		Description;
  pwr_tVolumeId		Volume;
  pwr_eVolumeAccess	Access;
  pwr_eMountType	MountType;
  pwr_eMountedType	MountedType;
  pwr_tUInt32		CacheMaxCount;
  pwr_tUInt32		CacheMinCount;
  pwr_tUInt32		CacheCurCount;
};

struct pwr_s_MountObject {
  pwr_tString80		Description;
  pwr_tObjid		Object;
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
