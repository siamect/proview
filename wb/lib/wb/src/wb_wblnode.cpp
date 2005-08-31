#include "pwr_class.h"
#include "co_cdh.h"
#include "co_time.h"
#include "wb_wblnode.h"
#include "wb_vrepwbl.h"
#include "wb_orepwbl.h"
#include "wb_merep.h"
#include "wb_cdrep.h"
#include "wb_wblvocabTokenTypes.hpp"
#include "wb_dbs.h"
#include "wb_name.h"
#include "wb_treeimport.h"

#define wblAlign(size) ((size + 3) & ~3)

struct wb_wblvocabTokenTypes tokens;

/* Datatypes */
static wbl_sSym datatypes[] =
{
  { "pwr_eType_Boolean", pwr_eType_Boolean }
  ,{ "pwr_eTix_Boolean", pwr_eTix_Boolean }
  ,{ "pwr_eType_Float32", pwr_eType_Float32 }
  ,{ "pwr_eTix_Float32", pwr_eTix_Float32 }
  ,{ "pwr_eType_Float64", pwr_eType_Float64 }
  ,{ "pwr_eTix_Float64", pwr_eTix_Float64 }
  ,{ "pwr_eType_Char", pwr_eType_Char }
  ,{ "pwr_eTix_Char", pwr_eTix_Char }
  ,{ "pwr_eType_Int8", pwr_eType_Int8 }
  ,{ "pwr_eTix_Int8", pwr_eTix_Int8 }
  ,{ "pwr_eType_Int16", pwr_eType_Int16 }
  ,{ "pwr_eTix_Int16", pwr_eTix_Int16 }
  ,{ "pwr_eType_Int32", pwr_eType_Int32 }
  ,{ "pwr_eTix_Int32", pwr_eTix_Int32 }
  ,{ "pwr_eType_UInt8", pwr_eType_UInt8 }
  ,{ "pwr_eTix_UInt8", pwr_eTix_UInt8 }
  ,{ "pwr_eType_UInt16", pwr_eType_UInt16 }
  ,{ "pwr_eTix_UInt16", pwr_eTix_UInt16 }
  ,{ "pwr_eType_UInt32", pwr_eType_UInt32 }
  ,{ "pwr_eTix_UInt32", pwr_eTix_UInt32 }
/*  ,{ "pwr_eType_ObjDId", pwr_eType_ObjDId } */
/*  ,{ "pwr_eTix_ObjDId", pwr_eTix_ObjDId } */
  ,{ "pwr_eType_Buffer", pwr_eType_Buffer }
  ,{ "pwr_eTix_Buffer", pwr_eTix_Buffer }
  ,{ "pwr_eType_String", pwr_eType_String }
  ,{ "pwr_eTix_String", pwr_eTix_String }
  ,{ "pwr_eType_Enum", pwr_eType_Enum }
  ,{ "pwr_eTix_Enum", pwr_eTix_Enum }
  ,{ "pwr_eType_Struct", pwr_eType_Struct }
  ,{ "pwr_eTix_Struct", pwr_eTix_Struct }
  ,{ "pwr_eType_Mask", pwr_eType_Mask }
  ,{ "pwr_eTix_Mask", pwr_eTix_Mask }
  ,{ "pwr_eType_Array", pwr_eType_Array }
  ,{ "pwr_eTix_Array", pwr_eTix_Array }
  ,{ "pwr_eType_Time", pwr_eType_Time }
  ,{ "pwr_eTix_Time", pwr_eTix_Time }
  ,{ "pwr_eType_Text", pwr_eType_Text }
  ,{ "pwr_eTix_Text", pwr_eTix_Text }
  ,{ "pwr_eType_Objid", pwr_eType_Objid }
  ,{ "pwr_eTix_Objid", pwr_eTix_Objid }
  ,{ "pwr_eType_AttrRef", pwr_eType_AttrRef }
  ,{ "pwr_eTix_AttrRef", pwr_eTix_AttrRef }
  ,{ "pwr_eType_ClassId", pwr_eType_ClassId }
  ,{ "pwr_eTix_ClassId", pwr_eTix_ClassId }
  ,{ "pwr_eType_TypeId", pwr_eType_TypeId }
  ,{ "pwr_eTix_TypeId", pwr_eTix_TypeId }
  ,{ "pwr_eType_VolumeId", pwr_eType_VolumeId }
  ,{ "pwr_eTix_VolumeId", pwr_eTix_VolumeId }
  ,{ "pwr_eType_ObjectIx", pwr_eType_ObjectIx }
  ,{ "pwr_eTix_ObjectIx", pwr_eTix_ObjectIx }
  ,{ "pwr_eType_RefId", pwr_eType_RefId }
  ,{ "pwr_eTix_RefId", pwr_eTix_RefId }
  ,{ "pwr_eType_DeltaTime", pwr_eType_DeltaTime }
  ,{ "pwr_eTix_DeltaTime", pwr_eTix_DeltaTime }
  ,{ "pwr_eType_Status", pwr_eType_Status }
  ,{ "pwr_eTix_Status", pwr_eTix_Status }
  ,{ "pwr_eType_NetStatus", pwr_eType_NetStatus }
  ,{ "pwr_eTix_NetStatus", pwr_eTix_NetStatus }
  ,{ "pwr_eType_CastId", pwr_eType_CastId }
  ,{ "pwr_eTix_CastId", pwr_eTix_CastId }
  ,{ "pwr_eTypeDef_AdefFlags", pwr_eTypeDef_AdefFlags }
  ,{ "pwr_eTdix_AdefFlags", pwr_eTdix_AdefFlags }
  ,{ "pwr_eTypeDef_ClassDefFlags", pwr_eTypeDef_ClassDefFlags }
  ,{ "pwr_eTdix_ClassDefFlags", pwr_eTdix_ClassDefFlags }
  ,{ "pwr_eTypeDef_ObjBodyDefFlags", pwr_eTypeDef_ObjBodyDefFlags }
  ,{ "pwr_eTdix_ObjBodyDefFlags", pwr_eTdix_ObjBodyDefFlags }
  ,{ 0, 0 }
};

/* System Classes */
static wbl_sSym classes[] =
{
  { "pwr_eClass_ClassDef", pwr_eClass_ClassDef }
  ,{ "pwr_eCix_ClassDef", pwr_eCix_ClassDef }
  ,{ "pwr_eClass_Type", pwr_eClass_Type }
  ,{ "pwr_eCix_Type", pwr_eCix_Type }
  ,{ "pwr_eClass_TypeDef", pwr_eClass_TypeDef }
  ,{ "pwr_eCix_TypeDef", pwr_eCix_TypeDef }
  ,{ "pwr_eClass_ObjBodyDef", pwr_eClass_ObjBodyDef }
  ,{ "pwr_eCix_ObjBodyDef", pwr_eCix_ObjBodyDef }
  ,{ "pwr_eClass_Param", pwr_eClass_Param }
  ,{ "pwr_eCix_Param", pwr_eCix_Param }
  ,{ "pwr_eClass_Attribute", pwr_eClass_Param }
  ,{ "pwr_eCix_Attribute", pwr_eCix_Param }
  ,{ "pwr_eClass_Input", pwr_eClass_Input }
  ,{ "pwr_eCix_Input", pwr_eCix_Input }
  ,{ "pwr_eClass_Output", pwr_eClass_Output }
  ,{ "pwr_eCix_Output", pwr_eCix_Output }
  ,{ "pwr_eClass_Intern", pwr_eClass_Intern }
  ,{ "pwr_eCix_Intern", pwr_eCix_Intern }
  ,{ "pwr_eClass_Buffer", pwr_eClass_Buffer }
  ,{ "pwr_eCix_Buffer", pwr_eCix_Buffer }
  ,{ "pwr_eClass_ObjXRef", pwr_eClass_ObjXRef }
  ,{ "pwr_eCix_ObjXRef", pwr_eCix_ObjXRef }
  ,{ "pwr_eClass_Layout", pwr_eClass_Layout }
  ,{ "pwr_eCix_Layout", pwr_eCix_Layout }
  ,{ "pwr_eClass_Group", pwr_eClass_Group }
  ,{ "pwr_eCix_Group", pwr_eCix_Group }
  ,{ "pwr_eClass_GroupRef", pwr_eClass_GroupRef }
  ,{ "pwr_eCix_GroupRef", pwr_eCix_GroupRef }
  ,{ "pwr_eClass_TypeHier", pwr_eClass_TypeHier }
  ,{ "pwr_eCix_TypeHier", pwr_eCix_TypeHier }
  ,{ "pwr_eClass_ClassHier", pwr_eClass_ClassHier }
  ,{ "pwr_eCix_ClassHier", pwr_eCix_ClassHier }
  ,{ "pwr_eClass_ModHier", pwr_eClass_ModHier }
  ,{ "pwr_eCix_ModHier", pwr_eCix_ModHier }
  ,{ "pwr_eClass_PlantHier", pwr_eClass_PlantHier }
  ,{ "pwr_eCix_PlantHier", pwr_eCix_PlantHier }
  ,{ "pwr_eClass_PlcProgram", pwr_eClass_PlcProgram }
  ,{ "pwr_eCix_PlcProgram", pwr_eCix_PlcProgram }
  ,{ "pwr_eClass_PlcWindow", pwr_eClass_PlcWindow }
  ,{ "pwr_eCix_PlcWindow", pwr_eCix_PlcWindow }
  ,{ "pwr_eClass_PlcNode", pwr_eClass_PlcNode }
  ,{ "pwr_eCix_PlcNode", pwr_eCix_PlcNode }
  ,{ "pwr_eClass_PlcConnection", pwr_eClass_PlcConnection }
  ,{ "pwr_eCix_PlcConnection", pwr_eCix_PlcConnection }
  ,{ "pwr_eClass_Point", pwr_eClass_Point }
  ,{ "pwr_eCix_Point", pwr_eCix_Point }
  ,{ "pwr_eClass_GraphPlcProgram", pwr_eClass_GraphPlcProgram }
  ,{ "pwr_eCix_GraphPlcProgram", pwr_eCix_GraphPlcProgram }
  ,{ "pwr_eClass_GraphPlcWindow", pwr_eClass_GraphPlcWindow }
  ,{ "pwr_eCix_GraphPlcWindow", pwr_eCix_GraphPlcWindow }
  ,{ "pwr_eClass_GraphPlcNode", pwr_eClass_GraphPlcNode }
  ,{ "pwr_eCix_GraphPlcNode", pwr_eCix_GraphPlcNode }
  ,{ "pwr_eClass_GraphPlcConnection", pwr_eClass_GraphPlcConnection }
  ,{ "pwr_eCix_GraphPlcConnection", pwr_eCix_GraphPlcConnection }
  ,{ "pwr_eClass_PlcPgm", pwr_eClass_PlcPgm }
  ,{ "pwr_eCix_PlcPgm", pwr_eCix_PlcPgm }
/*  ,{ "pwr_eClass_Hierarchy", pwr_eClass_Hierarchy }*/
/*  ,{ "pwr_eCix_Hierarchy", pwr_eCix_Hierarchy }*/
  ,{ "pwr_eClass_NodeHier", pwr_eClass_NodeHier }
  ,{ "pwr_eCix_NodeHier", pwr_eCix_NodeHier }
  ,{ "pwr_eClass_PgmDef", pwr_eClass_PgmDef }
  ,{ "pwr_eCix_PgmDef", pwr_eCix_PgmDef }
  ,{ "pwr_eClass_Node", pwr_eClass_Node }
  ,{ "pwr_eCix_Node", pwr_eCix_Node }
  ,{ "pwr_eClass_Appl", pwr_eClass_Appl }
  ,{ "pwr_eCix_Appl", pwr_eCix_Appl }
  ,{ "pwr_eClass_System", pwr_eClass_System }
  ,{ "pwr_eCix_System", pwr_eCix_System }
  ,{ "pwr_eClass_LibHier", pwr_eClass_LibHier }
  ,{ "pwr_eCix_LibHier", pwr_eCix_LibHier }
  ,{ "pwr_eClass_DocHier", pwr_eClass_DocHier }
  ,{ "pwr_eCix_DocHier", pwr_eCix_DocHier }
  ,{ "pwr_eClass_AttrXRef", pwr_eClass_AttrXRef }
  ,{ "pwr_eCix_AttrXRef", pwr_eCix_AttrXRef }
  ,{ "pwr_eClass_Menu", pwr_eClass_Menu }
  ,{ "pwr_eCix_Menu", pwr_eCix_Menu }
  ,{ "pwr_eClass_RtMenu", pwr_eClass_RtMenu }
  ,{ "pwr_eCix_RtMenu", pwr_eCix_RtMenu }
  ,{ "pwr_eClass_MenuSeparator", pwr_eClass_MenuSeparator }
  ,{ "pwr_eCix_MenuSeparator", pwr_eCix_MenuSeparator }
  ,{ "pwr_eClass_MenuCascade", pwr_eClass_MenuCascade }
  ,{ "pwr_eCix_MenuCascade", pwr_eCix_MenuCascade }
  ,{ "pwr_eClass_MenuButton", pwr_eClass_MenuButton }
  ,{ "pwr_eCix_MenuButton", pwr_eCix_MenuButton }
  ,{ "pwr_eClass_MenuRef", pwr_eClass_MenuRef }
  ,{ "pwr_eCix_MenuRef", pwr_eCix_MenuRef }
  ,{ "pwr_eClass_Object", pwr_eClass_Object }
  ,{ "pwr_eCix_Object", pwr_eCix_Object }
  ,{ "pwr_eClass_DbCallBack", pwr_eClass_DbCallBack }
  ,{ "pwr_eCix_DbCallBack", pwr_eCix_DbCallBack }
  ,{ "pwr_eClass_Alias", pwr_eClass_Alias }
  ,{ "pwr_eCix_Alias", pwr_eCix_Alias }
  ,{ "pwr_eClass_RootVolume", pwr_eClass_RootVolume }
  ,{ "pwr_eCix_RootVolume", pwr_eCix_RootVolume }
  ,{ "pwr_eClass_SubVolume", pwr_eClass_SubVolume }
  ,{ "pwr_eCix_SubVolume", pwr_eCix_SubVolume }
  ,{ "pwr_eClass_SharedVolume", pwr_eClass_SharedVolume }
  ,{ "pwr_eCix_SharedVolume", pwr_eCix_SharedVolume }
  ,{ "pwr_eClass_DynamicVolume", pwr_eClass_DynamicVolume }
  ,{ "pwr_eCix_DynamicVolume", pwr_eCix_DynamicVolume }
  ,{ "pwr_eClass_SystemVolume", pwr_eClass_SystemVolume }
  ,{ "pwr_eCix_SystemVolume", pwr_eCix_SystemVolume }
  ,{ "pwr_eClass_ClassVolume", pwr_eClass_ClassVolume }
  ,{ "pwr_eCix_ClassVolume", pwr_eCix_ClassVolume }
  ,{ "pwr_eClass_WorkBenchVolume", pwr_eClass_WorkBenchVolume }
  ,{ "pwr_eCix_WorkBenchVolume", pwr_eCix_WorkBenchVolume }
  ,{ "pwr_eClass_DirectoryVolume", pwr_eClass_DirectoryVolume }
  ,{ "pwr_eCix_DirectoryVolume", pwr_eCix_DirectoryVolume }
  ,{ "pwr_eClass_VolatileVolume", pwr_eClass_VolatileVolume }
  ,{ "pwr_eCix_VolatileVolume", pwr_eCix_VolatileVolume }
  ,{ "pwr_eClass_ExternVolume", pwr_eClass_ExternVolume }
  ,{ "pwr_eCix_ExternVolume", pwr_eCix_ExternVolume }
  ,{ "pwr_eClass_CreateVolume", pwr_eClass_CreateVolume }
  ,{ "pwr_eCix_CreateVolume", pwr_eCix_CreateVolume }
  ,{ "pwr_eClass_MountVolume", pwr_eClass_MountVolume }
  ,{ "pwr_eCix_MountVolume", pwr_eCix_MountVolume }
  ,{ "pwr_eClass_MountObject", pwr_eClass_MountObject }
  ,{ "pwr_eCix_MountObject", pwr_eCix_MountObject }
  ,{ "pwr_eClass_Bit", pwr_eClass_Bit }
  ,{ "pwr_eCix_Bit", pwr_eCix_Bit }
  ,{ "pwr_eClass_Value", pwr_eClass_Value }
  ,{ "pwr_eCix_Value", pwr_eCix_Value }
  ,{ "pwr_eClass_Method", pwr_eClass_Method }
  ,{ "pwr_eCix_Method", pwr_eCix_Method }
  ,{ "pwr_eClass_RtMethod", pwr_eClass_RtMethod }
  ,{ "pwr_eCix_RtMethod", pwr_eCix_RtMethod }
  ,{ 0, 0 }
};


/* Datatypes */
static wbl_sSym editor[] =
{
  { "pwr_eEditor_HiEd", pwr_eEditor_HiEd }
  ,{ "pwr_eEditor_PlcEd", pwr_eEditor_PlcEd }
  ,{ "pwr_eEditor_AttrEd", pwr_eEditor_AttrEd }
  ,{ "pwr_eEditor_ClassEd", pwr_eEditor_ClassEd }

  ,{ "pwr_eMethod__", pwr_eMethod__ }
  ,{ "pwr_eMethod_Standard", pwr_eMethod_Standard }
  ,{ "pwr_eMethod_Connections", pwr_eMethod_Connections }
  ,{ "pwr_eMethod_DrawingInfo", pwr_eMethod_DrawingInfo }
  ,{ "pwr_eMethod_DevBodyOnly", pwr_eMethod_DevBodyOnly }
  ,{ "pwr_eMethod_RtAndDevBodies", pwr_eMethod_RtAndDevBodies }
  ,{ "pwr_eMethod_RtConnectionsAndDevBodies", pwr_eMethod_RtConnectionsAndDevBodies }
  ,{ "pwr_eMethod_DevBodyWithChkb", pwr_eMethod_DevBodyWithChkb }
  ,{ "pwr_eMethod_SysBody", pwr_eMethod_SysBody }
  ,{ "pwr_eMethod_DevBodyConnections", pwr_eMethod_DevBodyConnections }
  ,{ "pwr_ePopEditor__", pwr_ePopEditor__ }
  ,{ "pwr_ePopEditor_GMS", pwr_ePopEditor_GMS }
  ,{ "pwr_ePopEditor_Opcom", pwr_ePopEditor_Opcom }
  ,{ 0, 0 }
};

static wbl_sSym attr_flags[] =
{
/* Bitmask for flags */

  { "PWR_MASK_POINTER",	PWR_MASK_POINTER } 
  ,{ "PWR_MASK_ARRAY",		PWR_MASK_ARRAY } 
  ,{ "PWR_MASK_BACKUP",		PWR_MASK_BACKUP } 
  ,{ "PWR_MASK_CASTATTR",	PWR_MASK_CASTATTR } 
  ,{ "PWR_MASK_STATE",		PWR_MASK_STATE } 
  ,{ "PWR_MASK_CONST",		PWR_MASK_CONST } 
  ,{ "PWR_MASK_RTVIRTUAL",	PWR_MASK_RTVIRTUAL } 
  ,{ "PWR_MASK_DEVBODYREF",	PWR_MASK_DEVBODYREF } 
  ,{ "PWR_MASK_DYNAMIC",	PWR_MASK_DYNAMIC } 
  ,{ "PWR_MASK_OBJDIDSELF",	PWR_MASK_OBJDIDSELF } 
  ,{ "PWR_MASK_OBJIDSELF",	PWR_MASK_OBJIDSELF } 
  ,{ "PWR_MASK_NOEDIT",		PWR_MASK_NOEDIT } 
  ,{ "PWR_MASK_INVISIBLE",	PWR_MASK_INVISIBLE } 
  ,{ "PWR_MASK_REFDIRECT",	PWR_MASK_REFDIRECT }
  ,{ "PWR_MASK_NOINVERT",	PWR_MASK_NOINVERT }
  ,{ "PWR_MASK_NOREMOVE",	PWR_MASK_NOREMOVE }
  ,{ "PWR_MASK_RTDBREF",	PWR_MASK_RTDBREF }
  ,{ "PWR_MASK_PRIVATE",	PWR_MASK_PRIVATE }
  ,{ "PWR_MASK_CLASS",		PWR_MASK_CLASS }
  ,{ "PWR_MASK_SUPERCLASS",	PWR_MASK_SUPERCLASS }
  ,{ "PWR_MASK_BUFFER",		PWR_MASK_BUFFER }
  ,{ "PWR_MASK_NOWBL",		PWR_MASK_NOWBL }
  ,{ "PWR_MASK_ALWAYSWBL",     	PWR_MASK_ALWAYSWBL }

  ,{ "pwr_mClassDef_DevOnly",	pwr_mClassDef_DevOnly }
  ,{ "pwr_mClassDef_System",	pwr_mClassDef_System }
  ,{ "pwr_mClassDef_Multinode",	pwr_mClassDef_Multinode }
  ,{ "pwr_mClassDef_ObjXRef",	pwr_mClassDef_ObjXRef }
  ,{ "pwr_mClassDef_RtBody",	pwr_mClassDef_RtBody }
  ,{ "pwr_mClassDef_AttrXRef",	pwr_mClassDef_AttrXRef }
  ,{ "pwr_mClassDef_ObjRef",	pwr_mClassDef_ObjRef }
  ,{ "pwr_mClassDef_AttrRef",	pwr_mClassDef_AttrRef }
  ,{ "pwr_mClassDef_TopObject",	pwr_mClassDef_TopObject }
  ,{ "pwr_mClassDef_NoAdopt",	pwr_mClassDef_NoAdopt }
  ,{ "pwr_mClassDef_Template",	pwr_mClassDef_Template }
  ,{ "pwr_mClassDef_IO",	pwr_mClassDef_IO }
  ,{ "pwr_mClassDef_IOAgent",	pwr_mClassDef_IOAgent }
  ,{ "pwr_mClassDef_IORack",	pwr_mClassDef_IORack }
  ,{ "pwr_mClassDef_IOCard",	pwr_mClassDef_IOCard }
  ,{ "pwr_mClassDef_HasCallBack", pwr_mClassDef_HasCallBack }
  ,{ 0, 0 }
};

void wb_wblnode::initialize( antlr::RefToken t )
{
  CommonAST::initialize(t);
  line_number = t->getLine();
  
  // Test 
  static int last = 0;
  if ( (line_number % 1000) == 0 && line_number != last) {
    printf( "-- Processing line: %d\r", line_number); 
    fflush(stdout);
    last = line_number;
  }
}

int wb_wblnode::classNameToCid( char *class_name, pwr_tCid *cid)
{
  pwr_tStatus sts;

  wb_cdrep *cd = m_vrep->merep()->cdrep( &sts, class_name);
  if ( ODD(sts)) {
    *cid = cd->cid();
    delete cd;
  }
  else {
    char buf[80];

    if ( !m_vrep->classNameToCid( class_name, cid)) {
      if (sscanf( class_name, "%d", cid) != 1) {
        if (class_name[0] == '$') {
          sprintf( buf, "pwr_eClass_%s", &class_name[1]);
          if (!lookup( (int *) cid, buf, classes)) {
            if (!lookup( (int *) cid, buf, datatypes))
              return 0;
          }
        }
        else {
          if (!lookup( (int *) cid, class_name, classes)) {
            if (!lookup( (int *) cid, class_name, datatypes))
              return 0;
          }
        }
      }
    }
  }
  return 1;
}

int wb_wblnode::stringToOix( const char *buf, pwr_tOix *oix) const
{
  *oix = 0;
  if (sscanf( buf, "%d", oix) != 1) {
    if (!lookup( (int *) oix, buf, classes)) {
      if (!lookup( (int *) oix, buf, datatypes))
        return 0;
    }
  }
  return 1;
}

int wb_wblnode::lookup( int *type, const char *keyword, wbl_sSym *table) 
{
  int cond;
  wbl_sSym *p = table;

  while ( p->sym != NULL && (cond = strcmp( keyword, p->sym)) )
    ++p;

  if ( (cond = (cond == 0))) *type = p->value;

  return( cond );
}

int wb_wblnode::convconst( int *val, char *str) 
{
  if (wb_wblnode::lookup( val, str, classes))	return( TRUE);
  if (wb_wblnode::lookup( val, str, editor))	return( TRUE);
  if (wb_wblnode::lookup( val, str, datatypes))	return( TRUE);
  if (wb_wblnode::lookup( val, str, attr_flags))   return( TRUE);
  return( FALSE);
}

ref_wblnode wb_wblnode::find( wb_name *oname, int level)
{
  switch ( getType()) {
  case tokens.OBJECT:
    if ( oname->segmentIsEqual( name(), level)) {
      if ( !oname->hasSegment(level+1))
        return this;
      else if ( o->fch)
        return o->fch->find( oname, level+1);
      else
        return 0;
    }
    else if ( o->fws)
      return o->fws->find( oname, level);
    else
      return 0;
  case tokens.VOLUME:
    if ( oname->volumeIsEqual( name()) && !oname->hasSegment(0))
      return this;
    else if ( o->fch)
      return o->fch->find( oname, 0);
    else
      return 0;
  default:
    ;
  }
  return 0;
}

void wb_wblnode::info_link( int level)
{
  for ( int i = 0; i < level; i++)
    cout << "  ";
  cout << name() << "  " << o->cname << "  " << o->m_oid.oix << " " << o->m_oid.vid << endl; 
  if ( o->fch)
    o->fch->info_link( level + 1);
  if ( o->fws)
    o->fws->info_link( level);
}

ref_wblnode wb_wblnode::get_o_lch()
{
  ref_wblnode child = o->fch;

  if ( !child)
    return 0;

  while ( child->o->fws)
    child = child->o->fws;

  return child;
}

void wb_wblnode::build( bool recursive)
{
  if ( !o->is_built) {
    // Check name
    wb_name n = wb_name(name());
    if (!n)  
      m_vrep->error( "Object name syntax error", getFileName(), line_number);

    o->m_oid.vid = m_vrep->vid();

    if ( isClassDef()) {
      m_vrep->getTemplateBody( o->m_cid, pwr_eBix_sys, &o->rbody_size, &o->rbody);
    }
    else if ( isType() || isTypeDef()) {
      m_vrep->getTemplateBody( o->m_cid, pwr_eBix_sys, &o->rbody_size, &o->rbody);
    }
    else if ( isTemplate()) {
      // Build later by classdef
      return;
    } 
    else {
      static int tot_size = 0;
      tot_size += o->rbody_size + o->dbody_size;

      // cout << name() << ": " << tot_size << " " << o->rbody_size << " " << o->dbody_size << endl;
      m_vrep->getTemplateBody( o->m_cid, pwr_eBix_rt, &o->rbody_size, &o->rbody);
      m_vrep->getTemplateBody( o->m_cid, pwr_eBix_dev, &o->dbody_size, &o->dbody);
    }

    ref_wblnode first_child;
    first_child = getFirstChild();

    if ( first_child) {
      if ( node_type == wbl_eNodeType_No || node_type == wbl_eNodeType_Code)  // Avoid infinite loop
        o->is_built = 1;
      first_child->buildBody( this);
      if ( node_type == wbl_eNodeType_No || node_type == wbl_eNodeType_Code)
        o->is_built = 0;
    }

    if ( o->fch && ( recursive || isClassDef())) {
      if ( node_type == wbl_eNodeType_No)
        o->is_built = 1;
      // o->fch->build( 1);
      wb_wblnode *ch = o->fch;
      while ( ch) {
	if ( !(isClassDef() && ch->isCode()))
	  ch->build( 1);
        ch = ch->o->fws;
      }
      if ( node_type == wbl_eNodeType_No)
        o->is_built = 0;
    }
    if ( isClassDef()) {

      o->m_oid.oix = cdh_cixToOix( o->m_oid.oix, 0, 0);
      if ( !m_vrep->registerObject( o->m_oid.oix, this))
        m_vrep->error( "Duplicate class index", getFileName(), line_number);

      if ( m_vrep->vid() == 1)
        ((pwr_sClassDef *)o->rbody)->Flags.b.System = 1;

      // Calculate offset for attributes
      wb_wblnode *child = o->fch;
      while ( child) {
        if ( child->isObjBodyDef()) {
          child->buildObjBodyDef( this);

          ((pwr_sClassDef *)o->rbody)->NumOfObjBodies++;
          if ( cdh_oixToBix( child->o->m_oid.oix) == pwr_eBix_rt)
            ((pwr_sClassDef *)o->rbody)->Flags.b.RtBody = 1;

          wb_wblnode *attr = child->o->fch;
          while( attr) {
            if ( attr->isAttribute()) {
              switch ( attr->o->m_cid) {
              case pwr_eClass_ObjXRef:
                ((pwr_sClassDef *)o->rbody)->Flags.b.ObjXRef = 1;
                break;
              case pwr_eClass_AttrXRef:
                ((pwr_sClassDef *)o->rbody)->Flags.b.AttrXRef = 1;
                break;
              case pwr_eClass_Input:
              case pwr_eClass_Output:
              case pwr_eClass_Intern:
              case pwr_eClass_Param:
                if ( attr->o->a.type == pwr_eType_AttrRef)
                  ((pwr_sClassDef *)o->rbody)->Flags.b.AttrRef = 1;
                else if ( attr->o->a.type == pwr_eType_Objid)
                  ((pwr_sClassDef *)o->rbody)->Flags.b.ObjRef = 1;
                break;
              default:
                ;
              }
            }


            attr = attr->o->fws;
          }
        }
	else if ( cdh_NoCaseStrcmp( child->o->cname, "$DbCallBack") == 0 &&
		  !child->isTemplate() ) {
          ((pwr_sClassDef *)o->rbody)->Flags.b.HasCallBack = 1;
	}

        child = child->o->fws;
        o->m_flags = ((pwr_sClassDef *)o->rbody)->Flags;

      }
      o->is_built = 1;

      // Build template and code
      o->c.templ->buildTemplate( this);
      if ( o->c.code)
	o->c.code->build(1);
    }
    else if ( isType()) {
      o->m_oid.oix = cdh_tixToOix( 0, o->m_oid.oix);
      if ( !m_vrep->registerObject( o->m_oid.oix, this))
        m_vrep->error( "Duplicate type index", getFileName(), line_number);

      o->ty.tid = o->ty.type = ((pwr_sType *)o->rbody)->Type;
      o->ty.size = ((pwr_sType *)o->rbody)->Size;
      o->ty.elements = 1;

    }
    else if ( isTypeDef()) {
      o->m_oid.oix = cdh_tixToOix( 1, o->m_oid.oix);
      if ( !m_vrep->registerObject( o->m_oid.oix, this))
        m_vrep->error( "Duplicate type index", getFileName(), line_number);

      o->ty.tid = ((pwr_sTypeDef *)o->rbody)->TypeRef;
      o->ty.type = ((pwr_sTypeDef *)o->rbody)->Type;
      o->ty.size = ((pwr_sTypeDef *)o->rbody)->Size;
      if ( ((pwr_sTypeDef *)o->rbody)->Elements == 0)
	((pwr_sTypeDef *)o->rbody)->Elements = 1;
      o->ty.elements = ((pwr_sTypeDef *)o->rbody)->Elements;
      if ( o->ty.type == 0) {
	pwr_eType type;
	size_t size;
	int elements;

	if ( !m_vrep->getTypeInfo( o->ty.tid, &type, &size, &elements)) {
	  m_vrep->error( "Can't find TypeDef type", getFileName(), line_number);
	  return;
	}
	o->ty.type = ((pwr_sTypeDef *)o->rbody)->Type = type;
	o->ty.size = ((pwr_sTypeDef *)o->rbody)->Size = o->ty.elements * size;
      }
    }
    else if ( isObjBodyDef()) {
    }
    else if ( isAttribute()) {
    }
    else if ( isBuffer()) {
    }
    else if ( isVolume()) {
    }
    else {
      if( !m_vrep->registerObject( o->m_oid.oix, this)) {
        // Print error message
	pwr_tStatus sts;
        char name[120];
        char msg[180];

        wb_orep *orep = m_vrep->object( &sts, o->m_oid);
        m_vrep->objectName( orep, name);
        sprintf( msg, "Duplicate object index %d (%s)", o->m_oid.oix, name);
        m_vrep->error( msg, getFileName(), line_number);
      }
    }
    o->is_built = 1;
  }

  if ( recursive) {
    wb_wblnode *ch = o->fch;
    while ( ch) {
      ch->build( recursive);
      ch = ch->o->fws;
    }
  }

  //if ( recursive && o->fch)
  //  o->fch->build( recursive);
  //if ( recursive && o->fws)
  //  o->fws->build( recursive);
}


void wb_wblnode::postBuild()
{
  pwr_tStatus sts;
  pwr_mClassDef flags;
  m_vrep->getClassFlags( &sts, o->m_cid, &flags);
  o->m_flags.m |= flags.m;
  if ( EVEN(sts))
    m_vrep->error( "Can't find class flags", getFileName(), line_number);
  
  wb_wblnode *ch = o->fch;
  while ( ch) {
    ch->postBuild();
    ch = ch->o->fws;
  }

  if ( isVolume()) {
    size_t size, offset;
    int elements;
    pwr_tTypeId tid;
    pwr_eType type;
    int flags;
    pwr_tObjectIx no;

    // Transfer next_oix to volumes rtbody

    if ( m_vrep->getAttrInfo( "NextOix", pwr_eBix_sys, o->m_cid, &size, &offset,
                               &tid, &elements, &type, &flags)) {
     
      no = *(pwr_tObjectIx *)((char *)o->rbody + offset);
      if ( no < (pwr_tObjectIx)m_vrep->next_oix)
	*(pwr_tObjectIx *)((char *)o->rbody + offset) = m_vrep->next_oix;
    }
  }
  else if ( o->m_cid == pwr_eClass_GraphPlcNode && m_vrep->vid() > cdh_cSystemClassVolMax) {
    // Count number of $Input, $Output and $Intern and put in parameters
    pwr_tStatus sts;
    int interns = 0;
    int inputs = 0;
    int outputs = 0;

    wb_orep *orep = m_vrep->object( &sts, o->m_oid);
    wb_orep *oclassdef = orep->parent( &sts);
    if ( EVEN(sts)) return;

    wb_orep *obody = 0;
    wb_name nrb = wb_name("RtBody");
    obody = oclassdef->child( &sts, nrb);
    if ( ODD(sts)) {
      wb_orep *oattr = obody->first( &sts);
      wb_orep *new_oattr;

      while( ODD(sts)) {
	if ( oattr->cid() == pwr_eClass_Intern)
	  interns++;
	if ( oattr->cid() == pwr_eClass_Input && !outputs)
	  inputs++;
	if ( oattr->cid() == pwr_eClass_Output)
	  outputs++;
	
	new_oattr = oattr->next( &sts);
	delete oattr;
	oattr = new_oattr;
      }
    }
    delete oclassdef;
    if ( obody)
      delete obody;
    delete orep;

    ((pwr_sGraphPlcNode *)o->rbody)->parameters[0] = inputs;
    ((pwr_sGraphPlcNode *)o->rbody)->parameters[1] = interns;
    ((pwr_sGraphPlcNode *)o->rbody)->parameters[2] = outputs;

    if ( ((pwr_sGraphPlcNode *)o->rbody)->default_mask[0] == 0 &&
	 ((pwr_sGraphPlcNode *)o->rbody)->default_mask[1] == 0) {
      unsigned int mask = 0;
      unsigned int m = 0;
      for ( int i = 0; i < inputs; i++) {
	m = i ? 2 * m : 1;
	mask += m;
      }
      ((pwr_sGraphPlcNode *)o->rbody)->default_mask[0] = mask;

      mask = 0;
      m = 0;
      for ( int i = 0; i < outputs; i++) {
	m = i ? 2 * m : 1;
	mask += m;
      }
      ((pwr_sGraphPlcNode *)o->rbody)->default_mask[1] = mask;
    }
  }
  else if ( isTemplate() && o->templ.created) {
    // New template object, insert template values of attribute objects into body
    size_t size;
    void *body;

    ch = o->fth->o->fch;
    while ( ch) {
      if ( ch->isObjBodyDef() && ch->o->b.bix == pwr_eBix_rt) {
	wb_wblnode *attr = ch->o->fch;
	while ( attr) {
	  if ( attr->isAttribute()) {
	    if ( cdh_tidIsCid( attr->o->a.tid)) {
	      // Copy template for this object to offset of the attribute
	      if ( m_vrep->getTemplateBody( attr->o->a.tid, pwr_eBix_sys, &size, &body)) {
		if ( size != attr->o->a.size ||
		     attr->o->a.offset + size > o->rbody_size) 
		  m_vrep->error( "AttrObject size mismatch", getFileName(), line_number);
		else
		  memcpy( (char *)o->rbody + attr->o->a.offset, body, size);
		free( body);
	      }
	    }
	  }
	  attr = attr->o->fws;
	}
	
	break;
      }
      ch = ch->o->fws;
    }
    
  }
}

void wb_wblnode::buildObjBodyDef( ref_wblnode classdef)
{
  o->m_oid.oix = cdh_cixToOix( classdef->o->c.cix, o->m_oid.oix, 0);
  if ( !m_vrep->registerObject( o->m_oid.oix, this))
    m_vrep->error( "Duplicate objbodydef index", getFileName(), line_number);

  int index = 0;
  o->b.size = 0;
  wb_wblnode *child = o->fch;
  while ( child) {
    if ( child->isAttribute())
      child->buildAttribute( classdef, this, &index, &o->b.size);
    if ( child->isBuffer())
      child->buildBuffer( classdef, this, &index, &o->b.size);
    child = child->o->fws;
  }
  ((pwr_sObjBodyDef *)o->rbody)->Size = o->b.size;
  ((pwr_sObjBodyDef *)o->rbody)->NumOfParams = index;
}

void wb_wblnode::buildAttribute( ref_wblnode classdef, ref_wblnode objbodydef,
                                 int *bindex, size_t *boffset)
{
  pwr_eType type;
  size_t size;
  int elements;

  o->m_oid.oix = cdh_cixToOix( classdef->o->c.cix, objbodydef->o->b.bix, o->m_oid.oix);
  if ( !m_vrep->registerObject( o->m_oid.oix, this))
    m_vrep->error( "Duplicate attribute index", getFileName(), line_number);

  if ( ((pwr_sParam *)o->rbody)->Info.Elements == 0)
    ((pwr_sParam *)o->rbody)->Info.Elements = 1;
  // if ( ((pwr_sParam *)o->rbody)->Info.Type == 0)
  //  ((pwr_sParam *)o->rbody)->Info->Type = (pwr_eType)((pwr_sParam *)o->rbody)->TypeRef;

  if ( cdh_NoCaseStrcmp( o->cname, "$ObjXRef") == 0 )
    o->a.tid = pwr_eType_Objid;
  else if ( cdh_NoCaseStrcmp( o->cname, "$AttrXRef") == 0)
    o->a.tid = pwr_eType_AttrRef;
  else
    o->a.tid = ((pwr_sParam *)o->rbody)->TypeRef;

  o->a.type = ((pwr_sParam *)o->rbody)->Info.Type;
  o->a.size = ((pwr_sParam *)o->rbody)->Info.Size;
  o->a.offset = ((pwr_sParam *)o->rbody)->Info.Offset;
  o->a.elements = ((pwr_sParam *)o->rbody)->Info.Elements;
  o->a.flags = ((pwr_sParam *)o->rbody)->Info.Flags;

  if ( o->a.elements == 0)
    o->a.elements = 1;

  if ( !o->a.tid) {
    m_vrep->error( "Unknown attribute type", getFileName(), line_number);
    ((pwr_sParam *)o->rbody)->Info.ParamIndex = *bindex;
    (*bindex)++;
    return;
  }
  if ( cdh_tidIsCid( o->a.tid)) {
    size_t dsize;

    if ( !m_vrep->getClassInfo( o->a.tid, &size, &dsize)) {
      m_vrep->error( "Can't find attribute type", getFileName(), line_number);
      ((pwr_sParam *)o->rbody)->Info.ParamIndex = *bindex;
      (*bindex)++;
      return;
    }
    elements = 1;
    type = (pwr_eType) o->a.tid;
  }
  else {
    if ( !m_vrep->getTypeInfo( o->a.tid, &type, &size, &elements)) {
      m_vrep->error( "Can't find attribute type", getFileName(), line_number);
      ((pwr_sParam *)o->rbody)->Info.ParamIndex = *bindex;
      (*bindex)++;
      return;
    }
  }
  if ( o->a.type == 0)
    o->a.type = ((pwr_sParam *)o->rbody)->Info.Type = type;

  if ( o->a.flags & pwr_mAdef_pointer) {
    size = sizeof( void *);
  }
  if ( o->a.flags & pwr_mAdef_array) {
    size *= o->a.elements;
  }
  if ( ((pwr_sParam *)o->rbody)->Info.PgmName[0] == 0) {
    strncpy( ((pwr_sParam *)o->rbody)->Info.PgmName, wb_name::unatName( name()),
	     sizeof( ((pwr_sParam *)o->rbody)->Info.PgmName));
  }
  if ( cdh_NoCaseStrcmp( o->cname, "$Input") == 0) {
    o->a.size = ((pwr_sParam *)o->rbody)->Info.Size = size;
    o->a.offset = ((pwr_sParam *)o->rbody)->Info.Offset = *boffset + sizeof( void *);
    *boffset += sizeof( void *) + wblAlign( o->a.size);
  }
  else {
    o->a.size = ((pwr_sParam *)o->rbody)->Info.Size = size;
    o->a.offset = ((pwr_sParam *)o->rbody)->Info.Offset = *boffset;
    *boffset += wblAlign( o->a.size);
  }
  ((pwr_sParam *)o->rbody)->Info.ParamIndex = *bindex;
  (*bindex)++;


  // Do some syntax check of flags and typeref
  if ( ((pwr_sParam *)o->rbody)->Info.Flags & PWR_MASK_SUPERCLASS && 
       strcmp( name(), "Super") != 0)
    m_vrep->error( "Super class attribute not named Super", getFileName(), line_number);
  if ( strcmp( name(), "Super") == 0) {
    if ( *bindex != 1)
      m_vrep->error( "Super has to be first attribute", getFileName(), line_number);
    if ( !(((pwr_sParam *)o->rbody)->Info.Flags & PWR_MASK_SUPERCLASS))
      ((pwr_sParam *)o->rbody)->Info.Flags |= PWR_MASK_SUPERCLASS;
    if ( !(((pwr_sParam *)o->rbody)->Info.Flags & PWR_MASK_CLASS))
      ((pwr_sParam *)o->rbody)->Info.Flags |= PWR_MASK_CLASS;
    if ( !cdh_tidIsCid( o->a.tid))
      m_vrep->error( "TypeRef is not a class", getFileName(), line_number);
  }
  if ( ((pwr_sParam *)o->rbody)->Info.Flags & PWR_MASK_CLASS && 
       !cdh_tidIsCid( o->a.tid))
    m_vrep->error( "TypeRef is not a class", getFileName(), line_number);
  if ( cdh_tidIsCid( o->a.tid)) {
    if ( !(((pwr_sParam *)o->rbody)->Info.Flags & PWR_MASK_CLASS))
      ((pwr_sParam *)o->rbody)->Info.Flags |= PWR_MASK_CLASS;
  }
  if ( ((pwr_sParam *)o->rbody)->Info.Flags & PWR_MASK_CASTATTR) {
    if ( !o->bws || o->bws->o->a.type !=  pwr_eType_CastId)
      m_vrep->error( "Cast attribute not found", getFileName(), line_number);
  }
}

void wb_wblnode::buildBuffer( ref_wblnode classdef, ref_wblnode objbodydef, 
                              int *bindex, size_t *boffset)
{
  size_t rsize, dsize;

  o->m_oid.oix = cdh_cixToOix( classdef->o->c.cix, objbodydef->o->b.bix, o->m_oid.oix);
  if ( !m_vrep->registerObject( o->m_oid.oix, this))
    m_vrep->error( "Duplicate buffer index", getFileName(), line_number);

  if ( ((pwr_sBuffer *)o->rbody)->Info.Elements == 0)
    ((pwr_sBuffer *)o->rbody)->Info.Elements = 1;
  if ( ((pwr_sBuffer *)o->rbody)->Info.Type == 0)
    ((pwr_sParam *)o->rbody)->Info.Type = (pwr_eType)((pwr_sBuffer *)o->rbody)->Class;

  o->a.tid = ((pwr_sBuffer *)o->rbody)->Class;
  o->a.type = ((pwr_sParam *)o->rbody)->Info.Type;
  o->a.size = ((pwr_sBuffer *)o->rbody)->Info.Size;
  o->a.offset = ((pwr_sBuffer *)o->rbody)->Info.Offset;
  o->a.elements = ((pwr_sBuffer *)o->rbody)->Info.Elements;
  o->a.flags = ((pwr_sBuffer *)o->rbody)->Info.Flags;

  if ( !o->a.tid) {
    m_vrep->error( "Unknown buffer type", getFileName(), line_number);
    return;
  }
  if ( !m_vrep->getClassInfo( o->a.tid, &rsize, &dsize)) {
    m_vrep->error( "Can't find buffer class", getFileName(), line_number);
    return;
  }

  o->a.size = ((pwr_sBuffer *)o->rbody)->Info.Size = o->a.elements * rsize;
  o->a.offset = ((pwr_sBuffer *)o->rbody)->Info.Offset = *boffset;
  ((pwr_sBuffer *)o->rbody)->Info.ParamIndex = *bindex;
  *boffset += o->a.size;
  (*bindex)++;
}

void wb_wblnode::buildTemplate( ref_wblnode classdef)
{
  wb_wblnode *objbodydef = classdef->o->fch;
  o->m_oid.oix = cdh_cixToOix( classdef->o->c.cix, pwr_eBix_template, 0);
  if ( !m_vrep->registerObject( o->m_oid.oix, this)) {
    ref_wblnode n = m_vrep->findObject( o->m_oid.oix);
    printf( "Duplicate: %s\n", n->name());
    m_vrep->error( "Duplicate template oix", getFileName(), line_number);
  }
  while ( objbodydef) {
    if ( objbodydef->isObjBodyDef()) {
      if ( objbodydef->o->b.bix == pwr_eBix_sys || 
           objbodydef->o->b.bix == pwr_eBix_rt) {  
        o->rbody_size = objbodydef->o->b.size;
        if ( o->rbody_size) {
          o->rbody = calloc( 1, o->rbody_size);
        }
      }
      if ( objbodydef->o->b.bix == pwr_eBix_dev) {
        o->dbody_size = objbodydef->o->b.size;
        if ( o->dbody_size)
          o->dbody = calloc( 1, o->dbody_size);
      }
    }
    objbodydef = objbodydef->o->fws;
  }
  o->m_flags.b.Template = 1;

  ref_wblnode first_child = getFirstChild();
  if ( first_child)
    first_child->buildBody( this);
}

void wb_wblnode::buildBody( ref_wblnode object)
{
  pwr_eBix bix;
  ref_wblnode first_child;
  ref_wblnode next_sibling;

  switch ( getType()) {
  case tokens.BODY:
    if ( cdh_NoCaseStrcmp( name(), "SysBody") == 0)
      bix = pwr_eBix_sys;
    else if ( cdh_NoCaseStrcmp( name(), "RtBody") == 0)
      bix = pwr_eBix_rt;
    else if ( cdh_NoCaseStrcmp( name(), "DevBody") == 0) 
      bix = pwr_eBix_dev;
    else {
      // Body exception
      m_vrep->error( "Bad body name", getFileName(), line_number);
    }

    for ( first_child = getFirstChild();
	  first_child;
	  first_child = first_child->getNextSibling()) 
      first_child->buildAttr( object, bix);

    next_sibling = getNextSibling();
    if ( next_sibling)
      next_sibling->buildBody( object);
    break;
  default:
    next_sibling = getNextSibling();
    if ( next_sibling)
      next_sibling->buildBody( object);
  }
}

void wb_wblnode::buildAttr( ref_wblnode object, pwr_eBix bix)
{
  ref_wblnode first_child;
  ref_wblnode second_child;
  ref_wblnode next_sibling;
  int oper;
  char value[2048];
  size_t size, offset;
  int elements;
  pwr_tTypeId tid;
  pwr_eType type;
  int flags;
  char buf[2048];
  int int_val, current_int_val;
  bool string_continue = false;

  switch ( getType()) {
  case tokens.ATTRIBUTE:
  {
    first_child = getFirstChild();
    if ( !first_child) {
      // Attr exception
      m_vrep->error( "Attribute syntax", getFileName(), line_number);
      goto error_continue;
    }

    switch ( oper = first_child->getType()) {
    case tokens.OREQ:
    case tokens.EQ:
      break;
    default:
      // Attr exception
      m_vrep->error( "Attribute value required", getFileName(), line_number);
      goto error_continue;
    }


    if ( !m_vrep->getAttrInfo( name(), (pwr_eBix) bix, object->o->m_cid, &size, &offset,
                               &tid, &elements, &type, &flags)) {
      // This might be string special syntax
      wb_attrname n = wb_attrname( name());
      if ( n.hasAttrIndex() && 
           m_vrep->getAttrInfo( n.attribute(), (pwr_eBix) bix, object->o->m_cid, &size, 
                                &offset, &tid, &elements, &type, &flags) &&
           elements == 1 && 
           (type == pwr_eType_String || type == pwr_eType_Text)) {
        // Index in string attribute marks a new row
        int index = n.attrIndex();
        if ( index > 0)
          string_continue = true;
        offset += index;
        size = size - index;
      }
      else {
        // Attr exception
        m_vrep->error( "Unknown attribute", getFileName(), line_number);
        goto error_continue;
      }
    }

    second_child = first_child->getNextSibling();
    if ( !second_child) {
      // Attr exception
      m_vrep->error( "Attribute value required", getFileName(), line_number);
      goto error_continue;
    }

    if ( ((bix == pwr_eBix_rt || bix == pwr_eBix_sys) && 
          object->o->rbody_size == 0) ||
         (bix == pwr_eBix_dev && object->o->dbody_size == 0)) {
      m_vrep->error( "Attribute body", getFileName(), line_number);
      return;
    }

    if ( ((bix == pwr_eBix_rt || bix == pwr_eBix_sys) &&
          offset + size > object->o->rbody_size) ||
         ( bix == pwr_eBix_rt &&
           offset + size > object->o->rbody_size)) {
      m_vrep->error( "Mismatch in attribute offset", getFileName(), line_number);
      return;
    }

    strncpy( value, second_child->name(), sizeof(value));

    // printf( "Attr %s %s %d %d %s\n", object->name, name, size, offset, value);
    if ( size == sizeof(int_val) && convconst( &int_val, value)) {
      if ( oper == tokens.EQ) {
        if ( bix == pwr_eBix_rt || bix == pwr_eBix_sys)
          memcpy( (char *)((unsigned int) object->o->rbody + offset), 
                  &int_val, size);
        else if ( bix == pwr_eBix_dev)
          memcpy( (char *)((unsigned int) object->o->dbody + offset), 
                  &int_val, size);
      }
      else if ( oper == tokens.OREQ) {
        if ( bix == pwr_eBix_rt || bix == pwr_eBix_sys) {
          current_int_val = *(int *) ((unsigned int) object->o->rbody + offset);
          int_val |= current_int_val;
          memcpy( (char *)((unsigned int) object->o->rbody + offset),
                  &int_val, size);
        }
        else if ( bix == pwr_eBix_dev) {
          current_int_val = *(int *) ((unsigned int) object->o->dbody + offset);
          int_val |= current_int_val;
          memcpy( (char *)((unsigned int) object->o->dbody + offset),            
                  &int_val, size);
        }
      }
    }
    else if ( attrStringToValue( type, value, buf, sizeof( buf), size)) {
      if ( bix == pwr_eBix_rt || bix == pwr_eBix_sys) {
	if ( string_continue && ! *(char *)((unsigned int) object->o->rbody + offset - 1))
	  // If previous char is null, this was originally linefeed
	  *(char *)((unsigned int) object->o->rbody + offset - 1) = '\n';
        memcpy( (char *)((unsigned int) object->o->rbody + offset), 
                buf, size);
      }
      else if ( bix == pwr_eBix_dev) {
	if ( string_continue && ! *(char *)((unsigned int) object->o->dbody + offset - 1))
	  // If previous char is null, this was originally linefeed
	  *(char *)((unsigned int) object->o->dbody + offset - 1) = '\n';
        memcpy( (char *)((unsigned int) object->o->dbody + offset), 
                buf, size);
      }
    }
    else {
      // Attr conversion exception
      m_vrep->error( "Unable to convert string to value", getFileName(), line_number);
    }
    error_continue:
    break;
  }
  case tokens.BUFFER:
  {
    buildBuff( object, bix, 0, 0, 0);
    break;
  }
  default:
    ;
  }
}

void wb_wblnode::buildBuff( ref_wblnode object, pwr_eBix bix, pwr_tCid buffer_cid,
                            int buffer_offset, int buffer_size)
{
  ref_wblnode first_child;
  int size, offset, elements;
  pwr_tStatus sts;
  pwr_tCid host_cid;
  pwr_tCid cid;

  if ( buffer_cid != 0) {
    // Buffer in buffer... Fix
    host_cid = buffer_cid;
  }
  else
    host_cid = object->o->m_cid;

  if ( cdh_CidToVid(host_cid) == m_vrep->vid()) {
    pwr_tTypeId tid;
    pwr_eType type;
    int flags;
    sts = m_vrep->getAttrInfo( name(), (pwr_eBix) bix, host_cid, (size_t *) &size, 
			       (size_t *) &offset,
			       &tid, &elements, &type, &flags);
    cid = (pwr_tCid) tid;
  }
  else {
    wb_cdrep *cdrep = m_vrep->merep()->cdrep( &sts, host_cid);
    if ( EVEN(sts)) {
      m_vrep->error( "Unknown class of buffer owner", getFileName(), line_number);
      return;
    }
    wb_attrname aname = wb_attrname( name());
    wb_adrep *adrep = cdrep->adrep( &sts, aname.attribute());
    if ( EVEN(sts)) {
      m_vrep->error( "Unknown Buffer", getFileName(), line_number);
      delete cdrep;
      return;
    }

    cid = adrep->subClass();
    size = adrep->size();
    offset = adrep->offset();
    elements = adrep->nElement();
    delete cdrep;
    delete adrep;
  }
  first_child = getFirstChild();
  if ( first_child && first_child->getType() == tokens.INDEX) {
    int index;
    int nr = sscanf( first_child->name(), "%d", &index);
    if ( nr != 1) {
      m_vrep->error( "Buffer index syntax error", getFileName(), line_number);
      return;
    }
    if ( index >= elements) {
      m_vrep->error( "Buffer index exceeded", getFileName(), line_number);
      return;
    }
    size = size / elements;
    offset += index * size;
  }

  if ( buffer_cid != 0)
    offset += buffer_offset;

  if ( first_child)
    first_child->buildBuffAttr( object, bix, cid, offset, size);
}

void wb_wblnode::buildBuffAttr( ref_wblnode object, pwr_eBix bix, pwr_tCid buffer_cid,
                                size_t buffer_offset, size_t buffer_size)
{
  ref_wblnode first_child;
  ref_wblnode second_child;
  ref_wblnode next_sibling;
  int oper;
  char value[2048];
  size_t size, offset;
  int elements;
  pwr_tTypeId tid;
  pwr_eType type;
  char buf[2048];
  int int_val, current_int_val;
  wb_cdrep *cdrep;
  wb_adrep *adrep;
  wb_attrname aname;

  switch ( getType()) {
  case tokens.ATTRIBUTE:
  {
    first_child = getFirstChild();
    if ( !first_child) {
      // Attr exception
      m_vrep->error( "Attribute syntax", getFileName(), line_number);
      goto error_continue;
    }

    switch ( oper = first_child->getType()) {
    case tokens.OREQ:
    case tokens.EQ:
      break;
    default:
      // Attr exception
      m_vrep->error( "Attribute value required", getFileName(), line_number);
      goto error_continue;
    }

    pwr_tStatus sts;
    cdrep = m_vrep->merep()->cdrep( &sts, buffer_cid);
    if ( EVEN(sts)) {
      m_vrep->error( "Error in buffer class", getFileName(), line_number);
      goto error_continue;	
    }

    aname = wb_attrname( name());

    // Backward compability with V4.0 : classid was named class
    // This section can be removed in later versions
    switch ( cdrep->cid()) {
    case pwr_eClass_PlcProgram:
      if ( strcmp( name(), "objdid") == 0)
	adrep = cdrep->adrep( &sts, "oid");
      else if ( strcmp( name(), "classid") == 0)
	adrep = cdrep->adrep( &sts, "cid");
      else if ( strcmp( name(), "window_did") == 0)
	adrep = cdrep->adrep( &sts, "woid");
      else
	adrep = cdrep->adrep( &sts, aname.attribute());
      break;
    case pwr_eClass_PlcWindow:
      if ( strcmp( name(), "objdid") == 0)
	adrep = cdrep->adrep( &sts, "oid");
      else if ( strcmp( name(), "classid") == 0)
	adrep = cdrep->adrep( &sts, "cid");
      else if ( strcmp( name(), "parent_node_did") == 0)
	adrep = cdrep->adrep( &sts, "poid");
      else
	adrep = cdrep->adrep( &sts, aname.attribute());
      break;
    case pwr_eClass_PlcNode:
      if ( strcmp( name(), "classid") == 0)
	adrep = cdrep->adrep( &sts, "cid");
      else if ( strcmp( name(), "object_did") == 0)
	adrep = cdrep->adrep( &sts, "oid");
      else if ( strcmp( name(), "window_did") == 0)
	adrep = cdrep->adrep( &sts, "woid");
      else if ( strcmp( name(), "subwindow_objdid[0]") == 0)
	adrep = cdrep->adrep( &sts, "subwind_oid[0]");
      else if ( strcmp( name(), "subwindow_objdid[1]") == 0)
	adrep = cdrep->adrep( &sts, "subwind_oid[1]");
      else
	adrep = cdrep->adrep( &sts, aname.attribute());
      break;
    case pwr_eClass_PlcConnection:
      if ( strcmp( name(), "objdid") == 0)
	adrep = cdrep->adrep( &sts, "oid");
      else if ( strcmp( name(), "classid") == 0)
	adrep = cdrep->adrep( &sts, "cid");
      else if ( strcmp( name(), "source_node_did") == 0)
	adrep = cdrep->adrep( &sts, "source_oid");
      else if ( strcmp( name(), "dest_node_did") == 0)
	adrep = cdrep->adrep( &sts, "dest_oid");
      else if ( strcmp( name(), "window_did") == 0)
	adrep = cdrep->adrep( &sts, "woid");
      else
	adrep = cdrep->adrep( &sts, aname.attribute());
      break;
    default:
      // end of compability section
      adrep = cdrep->adrep( &sts, aname.attribute());
    }
    if ( EVEN(sts)) {
      m_vrep->error( "Unknown buffer attribute", getFileName(), line_number);
      delete cdrep;
      goto error_continue;	
    }

    size = adrep->size();
    offset = adrep->offset() + buffer_offset;
    tid = adrep->tid();
    elements = adrep->nElement();
    type = adrep->type();

    if ( aname.hasAttrIndex()) {
      int index = aname.attrIndex();
      if ( index >= elements) {
        m_vrep->error( "Max attribute index exceeded", getFileName(), line_number);
        goto error_continue;
      }
      offset += index * size / elements;
    }
    delete cdrep;
    delete adrep;

    second_child = first_child->getNextSibling();
    if ( !second_child) {
      // Attr exception
      m_vrep->error( "Attribute value required", getFileName(), line_number);
      goto error_continue;
    }

    if ( ((bix == pwr_eBix_rt || bix == pwr_eBix_sys) && 
          object->o->rbody_size == 0) ||
         (bix == pwr_eBix_dev && object->o->dbody_size == 0)) {
      m_vrep->error( "Attribute body", getFileName(), line_number);
      return;
    }

    if ( ((bix == pwr_eBix_rt || bix == pwr_eBix_sys) &&
          offset + size/elements > object->o->rbody_size) ||
         ( bix == pwr_eBix_rt &&
           offset + size/elements > object->o->rbody_size)) {
      m_vrep->error( "Mismatch in attribute offset", getFileName(), line_number);
      return;
    }

    strcpy( value, second_child->name());

    // printf( "Attr %s %s %d %d %s\n", object->name, name, size, offset, value);
    if ( size/elements == sizeof(int_val) && convconst( &int_val, value)) {
      if ( oper == tokens.EQ) {
        if ( bix == pwr_eBix_rt || bix == pwr_eBix_sys) 
          memcpy( (char *)((unsigned int) object->o->rbody + offset), 
                  &int_val, size/elements);
        else if ( bix == pwr_eBix_dev)
          memcpy( (char *)((unsigned int) object->o->dbody + offset), 
                  &int_val, size/elements);
      }
      else if ( oper == tokens.OREQ) {
        if ( bix == pwr_eBix_rt || bix == pwr_eBix_sys) {
          current_int_val = *(int *) ((unsigned int) object->o->rbody + offset);
          int_val |= current_int_val;
          memcpy( (char *)((unsigned int) object->o->rbody + offset),
                  &int_val, size/elements);
        }
        else if ( bix == pwr_eBix_dev) {
          current_int_val = *(int *) ((unsigned int) object->o->dbody + offset);
          int_val |= current_int_val;
          memcpy( (char *)((unsigned int) object->o->dbody + offset),            
                  &int_val, size/elements);
        }
      }
    }
    else if ( attrStringToValue( type, value, buf, sizeof( buf), size)) {
      if ( bix == pwr_eBix_rt || bix == pwr_eBix_sys)
        memcpy( (char *)((unsigned int) object->o->rbody + offset), 
                buf, size/elements);
      else if ( bix == pwr_eBix_dev)
        memcpy( (char *)((unsigned int) object->o->dbody + offset), 
                buf, size/elements);
    }
    else {
      // Attr conversion exception
      m_vrep->error( "Unable to convert string to value", getFileName(), line_number);
    }
    error_continue:
    break;
  }
  case tokens.BUFFER:
  {
    buildBuff( object, bix, buffer_cid, buffer_offset, buffer_size);
    break;
  }
  default:
    ;
  }
  next_sibling = getNextSibling();
  if ( next_sibling)
    next_sibling->buildBuffAttr( object, bix, buffer_cid, buffer_offset, buffer_size);
}

void wb_wblnode::link( wb_vrepwbl *vol, ref_wblnode father, ref_wblnode parent_ast)
{
  ref_wblnode first_child;
  ref_wblnode next_sibling;

  switch ( getType()) {
  case tokens.OBJECT:
  case tokens.VOLUME:
    if ( !father) {
      // Volume root
      vol->root_object = this;
    }
    o->fth = father;
    if ( o->fth) {
      o->bws = o->fth->get_o_lch();
      if ( o->bws)
        o->bws->o->fws = this;
      else
        o->fth->o->fch = this;
    }
    first_child = getFirstChild();
    if ( first_child)
      first_child->link( vol, this);

    next_sibling = getNextSibling();
    if ( next_sibling)
      next_sibling->link( vol, father, parent_ast);


    // Link docblock, previous child to parent ast
    if ( !parent_ast)
      parent_ast = father;
    if ( parent_ast) {
      ref_wblnode prev = 0;
      ref_wblnode child = parent_ast->getFirstChild();
      while ( child && child != this) {
	prev = child;
	child = child->getNextSibling();
      }
      if ( prev && prev->getType() == tokens.DOCBLOCK)
	o->docblock = prev;
    }
    // cout << "Linking " << name << endl;
    break;
  case tokens.SOBJECT:
  {
    ref_wblnode snode = m_vrep->find( name());
    if ( !snode) {
      // SObject exception
      m_vrep->error( "SObject syntax", getFileName(), line_number);
    }

    first_child = getFirstChild();
    if ( first_child)
      first_child->link( vol, snode, this);

    next_sibling = getNextSibling();
    if ( next_sibling)
      next_sibling->link( vol, father);
    break;
  }
  default:
    first_child = getFirstChild();
    if ( first_child)
      first_child->link( vol, father);

    next_sibling = getNextSibling();
    if ( next_sibling)
      next_sibling->link( vol, father, parent_ast);
  }
}

void wb_wblnode::registerNode( wb_vrepwbl *vol)
{
  ref_wblnode first_child = getFirstChild();
  m_vrep = vol;

  switch ( getType()) {
  case tokens.DOCBLOCK: {
    string txt = getText();
    break;
  }  
  case tokens.OBJECT:
  {

    if ( !o)
      o = new wbl_object();

    if ( !wb_name::checkObjectName( name())) {
      m_vrep->error( "Bad object name", getFileName(), line_number);
    }

    // Get class
    if ( first_child) {
      string class_name = first_child->getText();
      strcpy( o->cname, class_name.c_str());
 
      if ( !classNameToCid( o->cname, &o->m_cid)) {
        m_vrep->error( "Unknown class", getFileName(), line_number);
      }

      // If $ClassDef, register class in classlist
      if ( !isTemplate()) {
        if ( first_child->getType() == tokens.VALUE &&
             (strcmp( o->cname, "$ClassDef") == 0 ||
              strcmp( o->cname, "pwr_eClass_ClassDef") == 0)) {
          node_type = wbl_eNodeType_ClassDef;
        }
        else if ( first_child->getType() == tokens.VALUE &&
                  ( strcmp( o->cname, "$Type") == 0 ||
                    strcmp( o->cname, "pwr_eClass_Type") == 0)) {
          node_type = wbl_eNodeType_Type;
        }
        else if ( first_child->getType() == tokens.VALUE &&
                  ( strcmp( o->cname, "$TypeDef") == 0 ||
                    strcmp( o->cname, "pwr_eClass_TypeDef") == 0)) {
          node_type = wbl_eNodeType_TypeDef;
        }
        else if ( first_child->getType() == tokens.VALUE &&
                  ( strcmp( o->cname, "$ObjBodyDef") == 0 ||
                    strcmp( o->cname, "pwr_eClass_ObjBodyDef") == 0)) {
          node_type = wbl_eNodeType_ObjBodyDef;
        }
        else if ( first_child->getType() == tokens.VALUE &&
                  ( strcmp( o->cname, "$Attribute") == 0 ||
                    strcmp( o->cname, "$Input") == 0 ||
                    strcmp( o->cname, "$Output") == 0 ||
                    strcmp( o->cname, "$Intern") == 0 ||
                    strcmp( o->cname, "$ObjXRef") == 0 ||
                    strcmp( o->cname, "$AttrXRef") == 0 ||
                    strcmp( o->cname, "pwr_eClass_Param") == 0)) {
          node_type = wbl_eNodeType_Attribute;
        }
        else if ( first_child->getType() == tokens.VALUE &&
                  ( strcmp( o->cname, "$Buffer") == 0)) {
          node_type = wbl_eNodeType_Buffer;
        }
        else if ( first_child->getType() == tokens.VALUE &&
                  ( strcmp( o->cname, "$Param") == 0)) {
          m_vrep->error( "Obsolete attribute class, use $Attribute instead",
                         getFileName(), line_number);
        }
      }

      // Get oix
      ref_wblnode second_child = first_child->getNextSibling();
      if ( second_child) {
        switch ( second_child->getType()) {
        case tokens.VALUE:
        case tokens.INT:
        {
          string oixstr = second_child->getText();
          if ( !stringToOix( oixstr.c_str(), &o->m_oid.oix)) {
            o->m_oid.oix = m_vrep->nextOix();
          }
          break;
        }
        case tokens.ENDOBJECT:
        case tokens.OBJECT:
        case tokens.BODY:
          o->m_oid.oix = m_vrep->nextOix();
          break;
        default:
          ; // Syntax exception -- oix
          m_vrep->error( "Syntax", getFileName(), line_number);
        }
      }
      else {
        if ( isClassDef() || isType() || isTypeDef() || isObjBodyDef()) {
          // Syntax exception -- no cix or tix
          m_vrep->error( "Missing index", getFileName(), line_number);
        }
        else {
          if ( !isTemplate())
            o->m_oid.oix = m_vrep->nextOix();
        }
      }
    }
    else if ( isTemplate() && strcmp( o->cname, "") != 0) {
      // Created template object
    }
    else {
      // Syntax exception -- No class
      m_vrep->error( "Missing class", getFileName(), line_number);
    }

    if ( isClassDef()) {
      o->c.cid = cdh_cixToCid( m_vrep->vid(), o->m_oid.oix);
      o->c.cix = o->m_oid.oix;
      m_vrep->registerClass( name(), o->c.cid, this);

      // Find Template object
      ref_wblnode child = first_child;
      ref_wblnode last_child = child;
      string childname;
      while ( child) {
        if ( child->getType() == tokens.OBJECT) {
          childname = child->getText();
          if ( strcmp( childname.c_str(), "Template") == 0) {
            o->c.templ = child;
            o->c.templ->node_type = wbl_eNodeType_Template;
          }
          else if ( strcmp( childname.c_str(), "Code") == 0) {
            o->c.code = child;
            o->c.code->node_type = wbl_eNodeType_Code;
          }
        }
        last_child = child;
        child = child->getNextSibling();
      }
      if ( !o->c.templ) {
        // Create a template node

	wb_wblnode *templ = new wb_wblnode();
	// ref_wblnode reftempl(templ);
        o->c.templ = templ;
        o->c.templ->setType( tokens.OBJECT);
        string tname("Template");
        o->c.templ->setText( tname);
        if ( last_child)
          last_child->setNextSibling( (RefAST)o->c.templ);
        else
          setFirstChild( (RefAST)o->c.templ);
        o->c.templ->o = new wbl_object();
        strcpy( o->c.templ->o->cname, name());
        // o->c.templ->o->m_oid.oix = m_vrep->nextOix();
        o->c.templ->o->m_cid = o->c.cid;
	o->c.templ->o->templ.created = true;
        o->c.templ->node_type = wbl_eNodeType_Template;
      }
    }
    else if ( isType()) {
      o->m_tid = cdh_tixToTid( m_vrep->vid(), 0, o->m_oid.oix);
      m_vrep->registerType( name(), o->m_tid, this);
    }
    else if ( isTypeDef()) {
      o->m_tid = cdh_tixToTid( m_vrep->vid(), 1, o->m_oid.oix);
      m_vrep->registerType( name(), o->m_tid, this);
    }
    else if ( isObjBodyDef()) {
      o->b.bix = (pwr_eBix)o->m_oid.oix;
      if ( !(o->b.bix == pwr_eBix_rt || o->b.bix == pwr_eBix_sys || o->b.bix == pwr_eBix_dev))
        m_vrep->error( "Bad body index", getFileName(), line_number);
    }
    break;
  }
  case tokens.VOLUME:
  {
    pwr_tVid vid;
    int sts;

    o = new wbl_object();

    // Get class
    if ( first_child) {
      string class_name = first_child->getText();
      strcpy( o->cname, class_name.c_str());

      if ( !classNameToCid( o->cname, &o->m_cid)) {
        // Syntax exception -- vid
        m_vrep->error( "Unkowon class", getFileName(), line_number);
      }

      // Get oid
      ref_wblnode second_child = first_child->getNextSibling();
      if ( second_child) {
        switch ( second_child->getType()) {
        case tokens.OID:
        {
          string vidstring = second_child->getText();
          sts = cdh_StringToVolumeId( (char *)vidstring.c_str(), &vid);
          if ( EVEN(sts)) {
            // Syntax exception -- vid
            m_vrep->error( "Volume id syntax", getFileName(), line_number);
          }
          break;
        }
        default:
          // Syntax exception -- vid
          m_vrep->error( "Volume syntax", getFileName(), line_number);
        }
      }
      else {
        // Syntax exception -- no cix
        m_vrep->error( "Volume id is missing", getFileName(), line_number);
      }
      node_type = wbl_eNodeType_Volume;
    }
    else {
      // Syntax exception -- No class
      m_vrep->error( "Volume class is missing", getFileName(), line_number);
    } 
    // Register volume
    m_vrep->registerVolume( name(), o->m_cid, vid, this);

    break;
  }
  case tokens.ATTRIBUTE:
  case tokens.SOBJECT:
  case tokens.BODY:
  case tokens.VALUE:
  case tokens.INT:
  case tokens.NUM_FLOAT:
  {
    break;
  }
  case tokens.CHAR_LITERAL:
  {
    // Remove quotes
    char str[10];
    string text = getText();
    const char *text_p = text.c_str();
    strncpy( str, &text_p[1], sizeof(str));
    str[strlen(str)-1] = 0;
    string new_text(str);
    setText(new_text);
    break;
  }
  case tokens.STRING_LITERAL:
  {
    // Remove quotes and replace \" with "
    char str[2048];
    const char *s;
    char *t;
    bool first = true;
    t = str;
    for ( s = getText().c_str(); *s; s++) {
      if ( first) {
        first = false;
        continue;
      }
      if ( *s == '"' && *(s-1) == '\\')
        t--;
      *t = *s;
      t++;
    }
    t--;
    *t = 0;
    string new_text(str);
    setText(new_text);
    break;
  }
  default:
    ;
  }

  ref_wblnode child = first_child;
  while ( child) {
    child->registerNode( vol);
    child = child->getNextSibling();
  }

}

#if 0
void wb_wblnode::iterObject( wb_dbs *dbs)
{
  ref_wblnode o_lch = get_o_lch();
  pwr_tOid fthoid = o->fth ? o->fth->o->m_oid : pwr_cNOid;
  pwr_tOid fwsoid = o->fws ? o->fws->o->m_oid : pwr_cNOid;
  pwr_tOid bwsoid = o->bws ? o->bws->o->m_oid : pwr_cNOid;
  pwr_tOid fchoid = o->fch ? o->fch->o->m_oid : pwr_cNOid;
  pwr_tOid lchoid = o_lch ? o_lch->o->m_oid : pwr_cNOid;
  wb_name n = wb_name(name());

  dbs->installObject( o->m_oid, o->m_cid, fthoid, fwsoid, bwsoid, fchoid, lchoid, 
                      (char *)name(), n.normName(cdh_mName_object),
                      getFileTime(), getFileTime(), getFileTime(), o->rbody_size, o->dbody_size);
  
  if ( o->fch)
    o->fch->iterObject( dbs);

  if ( o->fws)
    o->fws->iterObject( dbs);
}

void wb_wblnode::iterO->Dbody( wb_dbs *dbs)
{
  dbs->installO->Dbody( o->m_oid, o->dbody);
  
  if ( o->fch)
    o->fch->iterO->Dbody( dbs);

  if ( o->fws)
    o->fws->iterO->Dbody( dbs);
}

void wb_wblnode::iterO->Rbody( wb_dbs *dbs)
{
  dbs->installO->Rbody( o->m_oid, o->rbody);
  
  if ( o->fch)
    o->fch->iterO->Rbody( dbs);

  if ( o->fws)
    o->fws->iterO->Rbody( dbs);
}
#endif

bool wb_wblnode::exportHead(wb_import &i)
{

  ref_wblnode o_lch = get_o_lch();
  pwr_tOid fthoid = o->fth ? o->fth->o->m_oid : pwr_cNOid;
  pwr_tOid fwsoid = o->fws ? o->fws->o->m_oid : pwr_cNOid;
  pwr_tOid bwsoid = o->bws ? o->bws->o->m_oid : pwr_cNOid;
  pwr_tOid fchoid = o->fch ? o->fch->o->m_oid : pwr_cNOid;
  pwr_tOid lchoid = o_lch ? o_lch->o->m_oid : pwr_cNOid;
  wb_name n = wb_name(name());

  i.importHead( o->m_oid, o->m_cid, fthoid, bwsoid, fwsoid, fchoid, lchoid, name(), 
		n.normName(cdh_mName_object), o->m_flags,
                getFileTime(), getFileTime(), getFileTime(), o->rbody_size, o->dbody_size);
  
  if ( o->fch)
    o->fch->exportHead( i);

  if ( o->fws)
    o->fws->exportHead( i);

  return true;
}

bool wb_wblnode::exportDbody( wb_import &i)
{
  i.importDbody( o->m_oid, o->dbody_size, o->dbody);
  
  if ( o->fch)
    o->fch->exportDbody( i);

  if ( o->fws)
    o->fws->exportDbody( i);

  return true;
}

bool wb_wblnode::exportRbody( wb_import &i)
{
  i.importRbody( o->m_oid, o->rbody_size, o->rbody);
  
  if ( o->fch)
    o->fch->exportRbody( i);

  if ( o->fws)
    o->fws->exportRbody( i);

  return true;
}

bool wb_wblnode::exportDocBlock( wb_import &i)
{
  char *block;
  int size;

  if ( o->docblock && docBlock( &block, &size)) {
    i.importDocBlock( o->m_oid, size, block);
    free( block); 
  }
  
  if ( o->fch)
    o->fch->exportDocBlock( i);

  if ( o->fws)
    o->fws->exportDocBlock( i);

  return true;
}

bool wb_wblnode::exportTree( wb_treeimport &i, bool isRoot)
{
  pwr_tOid fthoid = (o->fth && !isRoot) ? o->fth->o->m_oid : pwr_cNOid;
  pwr_tOid bwsoid = (o->bws && !isRoot)  ? o->bws->o->m_oid : pwr_cNOid;

  i.importTreeObject( m_vrep->merep(), o->m_oid, o->m_cid, fthoid, bwsoid, name(), o->m_flags,
		      o->rbody_size, o->dbody_size, o->rbody, o->dbody);
  
  if ( o->fch)
    o->fch->exportTree( i, false);

  if ( !isRoot && o->fws)
    o->fws->exportTree( i, false);

  return true;
}

void wb_wblnode::setFile( wb_wblfile *f)
{
  file = f;

  ref_wblnode first_child = getFirstChild();
  if ( first_child)
    first_child->setFile( f);

  ref_wblnode next_sibling = getNextSibling();
  if ( next_sibling)
    next_sibling->setFile( f);
}


int wb_wblnode::attrStringToValue( int type_id, char *value_str, 
                                   void *buffer_ptr, size_t buff_size, size_t attr_size)
{
  int		sts;

  switch ( type_id )
    {
    case pwr_eType_Boolean:
    {
      if ( sscanf( value_str, "%d", (pwr_tBoolean *)buffer_ptr) != 1)
        return 0;
      if ( *(pwr_tBoolean *)buffer_ptr > 1)
        return 0;
      break;
    }
    case pwr_eType_Float32:
    {
      if ( sscanf( value_str, "%f", (float *)buffer_ptr) != 1)
        return 0;
      break;
    }
    case pwr_eType_Float64:
    {
      pwr_tFloat32 f;
      pwr_tFloat64 d;
      if ( sscanf( value_str, "%f", &f) != 1)
        return 0;
      d = f;
      memcpy( buffer_ptr, (char *) &d, sizeof(d));

      break;
    }
    case pwr_eType_Char:
    {
      if ( sscanf( value_str, "%c", (char *)buffer_ptr) != 1)
        return 0;
      break;
    }
    case pwr_eType_Int8:
    {
      pwr_tInt8 	i8;
      pwr_tInt16	i16;
      if ( sscanf( value_str, "%hd", &i16) != 1)
        return 0;
      i8 = i16;
      memcpy( buffer_ptr, (char *)&i8, sizeof(i8));
      break;
    }
    case pwr_eType_Int16:
    {
      if ( sscanf( value_str, "%hd", (short *)buffer_ptr) != 1)
        return 0;
      break;
    }
    case pwr_eType_Int32:
    case pwr_eType_Status:
    case pwr_eType_NetStatus:
    {
      if ( sscanf( value_str, "%d", (int *)buffer_ptr) != 1)
        return 0;
      break;
    }
    case pwr_eType_UInt8:
    {
      pwr_tUInt8 	i8;
      pwr_tUInt16	i16;
      if ( sscanf( value_str, "%hu", &i16) != 1)
        return 0;
      i8 = i16;
      memcpy( buffer_ptr, (char *)&i8, sizeof(i8));
      break;
    }
    case pwr_eType_UInt16:
    {
      if ( sscanf( value_str, "%hu", (unsigned short *)buffer_ptr) != 1)
        return 0;
      break;
    }
    case pwr_eType_UInt32:
    case pwr_eType_Mask:
    case pwr_eType_Enum:
    {
      if ( sscanf( value_str, "%lu", (unsigned long *)buffer_ptr) != 1)
        return 0;
      break;
    }
    case pwr_eType_Text:
    case pwr_eType_String:
    {
      if ( strlen( value_str) >= attr_size)
        return 0;
      strncpy( (char *)buffer_ptr, value_str, min(attr_size, buff_size));
      break;
    }
    case pwr_eType_Objid:
    {
      pwr_tObjid	objid;

      sts = m_vrep->nameToOid( value_str, &objid);
      if (EVEN(sts)) return 0;
      memcpy( buffer_ptr, &objid, sizeof(objid));
      break;
    }
    case pwr_eType_ClassId:
    {
      pwr_tClassId	classid;
      pwr_tObjid	objid;

      sts = m_vrep->nameToOid( value_str, &objid);
      if (EVEN(sts)) return 0;
      classid = cdh_ClassObjidToId( objid);
      memcpy( buffer_ptr, (char *) &classid, sizeof(classid));
      break;
    }
    case pwr_eType_TypeId:
    case pwr_eType_CastId:
    {
      pwr_tTypeId	val_typeid;
      pwr_eType type;
      size_t size;
      int elements;

      // sts = m_vrep->nameToOid( value_str, &objid);
      if ( m_vrep->getTypeInfo( value_str, &val_typeid, &type,
                                &size, &elements))
        memcpy( buffer_ptr, (char *) &val_typeid, sizeof(val_typeid));
      else {
	pwr_tClassId	classid;
	pwr_tObjid	objid;

	// Try classid
	sts = m_vrep->nameToOid( value_str, &objid);
	if (EVEN(sts)) return 0;
	classid = cdh_ClassObjidToId( objid);
	memcpy( buffer_ptr, (char *) &classid, sizeof(classid));
      }
      break;
    }
    case pwr_eType_ObjectIx:
    {
      pwr_tObjectIx	objectix;

      sts = cdh_StringToObjectIx( value_str, &objectix); 
      if (EVEN(sts)) return 0;
      memcpy( buffer_ptr, (char *) &objectix, sizeof(objectix));
      break;
    }
    case pwr_eType_VolumeId:
    {
      pwr_tVolumeId	volumeid;

      sts = cdh_StringToVolumeId( value_str, &volumeid); 
      if (EVEN(sts)) return 0;
      memcpy( buffer_ptr, (char *) &volumeid, sizeof(volumeid));
      break;
    }
    case pwr_eType_RefId:
    {
      pwr_tRefId	subid;

      sts = cdh_StringToSubid( value_str, &subid);
      if (EVEN(sts)) return 0;
      memcpy( buffer_ptr, (char *) &subid, sizeof(subid));
      break;
    }
    case pwr_eType_AttrRef:
    {
      pwr_sAttrRef	attrref;

      sts = m_vrep->nameToAttrRef( value_str, &attrref);
      if (EVEN(sts)) return 0;
      memcpy( buffer_ptr, &attrref, sizeof(attrref));

      break;
    }
    case pwr_eType_Time:
    {
      pwr_tTime	time;

      sts = time_AsciiToA( value_str, &time);
      if (EVEN(sts)) return 0;
      memcpy( buffer_ptr, (char *) &time, sizeof(time));
      break;
    }
    case pwr_eType_DeltaTime:
    {
      pwr_tDeltaTime deltatime;

      sts = time_AsciiToD( value_str, &deltatime);
      if (EVEN(sts)) return 0;
      memcpy( buffer_ptr, (char *) &deltatime, sizeof(deltatime));
      break;
    }
    default:
      return 0;
    }
  return 1;
}

bool wb_wblnode::docBlock( char **block, int *size) const
{
  switch ( o->m_cid) {
  case pwr_eClass_ClassDef:
  case pwr_eClass_Param:
  case pwr_eClass_Intern:
  case pwr_eClass_Input:
  case pwr_eClass_Output:
  case pwr_eClass_ObjXRef:
  case pwr_eClass_AttrXRef:
    break;
  default:
    return false;
  }
  if ( !o->docblock) {
    // Return nullstring
    *block = (char *) calloc( 1, 1);
    *size = 1;
    return true;
  }

  *size = strlen( o->docblock->getText().c_str()) + 1;
  *block = (char *) calloc( 1, *size);
  // strncpy( *block, o->docblock->getText().c_str(), *size);

  // Remove first and last row, and the beginning ! on each row
  
  char *s = (char *)o->docblock->getText().c_str();
  char *t = *block;
  char *start_t = t;
  // Skip first line
  while ( *s && *s != '\n')
    s++;
  s++;
  // Skip leading '!'
  bool skip = true;
  while( *s) {
    if ( t - start_t >= *size - 1)
      break;
    if ( *s == '!') {
      skip = false;
      s++;
      // Skip first space also
      if ( *s == ' ')
	s++;
    }
    if ( !skip)
      *t++ = *s;
    if ( *s == '\n')
      skip = true;
    s++;
  }
  // Skip last line
  while ( *t != '\n') {
    if ( t <= start_t)
      break;
    t--;
  }
  *t = 0;
  *size = t - start_t + 1;

  return true;
}


