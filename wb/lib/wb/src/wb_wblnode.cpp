#include "pwr_class.h"
#include "co_cdh.h"
#include "co_time.h"
#include "wb_wblnode.h"
#include "wb_vrepwbl.h"
#include "wb_merep.h"
#include "wb_cdrep.h"
#include "wb_wblvocabTokenTypes.hpp"
#include "wb_dbs.h"
#include "wb_name.h"

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
  ,{ "pwr_eClass_SystemVersion", pwr_eClass_SystemVersion }
  ,{ "pwr_eCix_SystemVersion", pwr_eCix_SystemVersion }
  ,{ "pwr_eClass_NodeVersion", pwr_eClass_NodeVersion }
  ,{ "pwr_eCix_NodeVersion", pwr_eCix_NodeVersion }
  ,{ "pwr_eClass_BootVersion", pwr_eClass_BootVersion }
  ,{ "pwr_eCix_BootVersion", pwr_eCix_BootVersion }
  ,{ "pwr_eClass_AttrXRef", pwr_eClass_AttrXRef }
  ,{ "pwr_eCix_AttrXRef", pwr_eCix_AttrXRef }
  ,{ "pwr_eClass_Menu", pwr_eClass_Menu }
  ,{ "pwr_eCix_Menu", pwr_eCix_Menu }
  ,{ "pwr_eClass_MenuSeparator", pwr_eClass_MenuSeparator }
  ,{ "pwr_eCix_MenuSeparator", pwr_eCix_MenuSeparator }
  ,{ "pwr_eClass_MenuCascade", pwr_eClass_MenuCascade }
  ,{ "pwr_eCix_MenuCascade", pwr_eCix_MenuCascade }
  ,{ "pwr_eClass_MenuButton", pwr_eClass_MenuButton }
  ,{ "pwr_eCix_MenuButton", pwr_eCix_MenuButton }
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
  ,{ "pwr_eClass_CreateVolume", pwr_eClass_CreateVolume }
  ,{ "pwr_eCix_CreateVolume", pwr_eCix_CreateVolume }
  ,{ "pwr_eClass_MountVolume", pwr_eClass_MountVolume }
  ,{ "pwr_eCix_MountVolume", pwr_eCix_MountVolume }
  ,{ "pwr_eClass_MountObject", pwr_eClass_MountObject }
  ,{ "pwr_eCix_MountObject", pwr_eCix_MountObject }
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
  ,{ "PWR_MASK_CHANGELOG",	PWR_MASK_CHANGELOG } 
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
  ,{ 0, 0 }
};

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
      else if ( o_fch)
        return o_fch->find( oname, level+1);
      else
        return 0;
    }
    else if ( o_fws)
      return o_fws->find( oname, level);
    else
      return 0;
  case tokens.VOLUME:
    if ( oname->volumeIsEqual( name()) && !oname->hasSegment(0))
      return this;
    else if ( o_fch)
      return o_fch->find( oname, 0);
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
  cout << name() << "  " << cname << "  " << m_oid.oix << " " << m_oid.vid << endl; 
  if ( o_fch)
    o_fch->info_link( level + 1);
  if ( o_fws)
    o_fws->info_link( level);
}

ref_wblnode wb_wblnode::get_o_lch()
{
  ref_wblnode child = o_fch;

  if ( !child)
    return 0;

  while ( child->o_fws)
    child = child->o_fws;

  return child;
}

void wb_wblnode::build( bool recursive)
{
  if ( !is_built) {
    m_oid.vid = m_vrep->vid();

    if ( isClassDef()) {
      m_vrep->getTemplateBody( m_cid, pwr_eBix_sys, &rbody_size, &rbody);
    }
    else if ( isType() || isTypeDef()) {
      m_vrep->getTemplateBody( m_cid, pwr_eBix_sys, &rbody_size, &rbody);
    }
    else if ( isTemplate()) {
      // Build later by classdef
      if ( recursive && o_fws)
        o_fws->build( recursive);
      return;
    } 
    else {
      static int tot_size = 0;
      tot_size += rbody_size + dbody_size;

      // cout << name() << ": " << tot_size << " " << rbody_size << " " << dbody_size << endl;
      m_vrep->getTemplateBody( m_cid, pwr_eBix_rt, &rbody_size, &rbody);
      m_vrep->getTemplateBody( m_cid, pwr_eBix_dev, &dbody_size, &dbody);
    }

    ref_wblnode first_child;
    first_child = getFirstChild();

    if ( first_child) {
      if ( node_type == wbl_eNodeType_No)  // Avoid infinite loop
        is_built = 1;
      first_child->buildBody( this);
      if ( node_type == wbl_eNodeType_No)
        is_built = 0;
    }

    if ( o_fch && ( recursive || isClassDef())) {
      if ( node_type == wbl_eNodeType_No)
        is_built = 1;
      o_fch->build( 1);
      if ( node_type == wbl_eNodeType_No)
        is_built = 0;
    }
    if ( isClassDef()) {

      m_oid.oix = cdh_cixToOix( m_oid.oix, 0, 0);
      if ( !m_vrep->registerObject( m_oid.oix, this))
        m_vrep->error( "Duplicate class index", getFileName(), line_number);

      if ( m_vrep->vid() == 1)
        ((pwr_sClassDef *)rbody)->Flags.b.System = 1;

      // Calculate offset for attributes
      wb_wblnode *child = o_fch;
      while ( child) {
        if ( child->isObjBodyDef()) {
          child->buildObjBodyDef( this);

          ((pwr_sClassDef *)rbody)->NumOfObjBodies++;
          if ( cdh_oixToBix( child->m_oid.oix) == pwr_eBix_rt)
            ((pwr_sClassDef *)rbody)->Flags.b.RtBody = 1;

          wb_wblnode *attr = child->o_fch;
          while( attr) {
            if ( attr->isAttribute()) {
              switch ( attr->m_cid) {
              case pwr_eClass_ObjXRef:
                ((pwr_sClassDef *)rbody)->Flags.b.ObjXRef = 1;
                break;
              case pwr_eClass_AttrXRef:
                ((pwr_sClassDef *)rbody)->Flags.b.AttrXRef = 1;
                break;
              case pwr_eClass_Input:
              case pwr_eClass_Output:
              case pwr_eClass_Intern:
              case pwr_eClass_Param:
                if ( attr->a_type == pwr_eType_AttrRef)
                  ((pwr_sClassDef *)rbody)->Flags.b.AttrRef = 1;
                else if ( attr->a_type == pwr_eType_AttrRef)
                  ((pwr_sClassDef *)rbody)->Flags.b.ObjRef = 1;
                break;
              default:
                ;
              }
            }


            attr = attr->o_fws;
          }
        }
	else if ( cdh_NoCaseStrcmp( child->cname, "$DbCallBack") == 0 &&
		  !child->isTemplate() ) {
          ((pwr_sClassDef *)rbody)->Flags.b.HasCallBack = 1;
	}

        child = child->o_fws;
        m_flags = ((pwr_sClassDef *)rbody)->Flags;

      }
      is_built = 1;

      // Build template
      c_template->buildTemplate( this);
    }
    else if ( isType()) {
      m_oid.oix = cdh_tixToOix( 0, m_oid.oix);
      if ( !m_vrep->registerObject( m_oid.oix, this))
        m_vrep->error( "Duplicate type index", getFileName(), line_number);

      ty_tid = ty_type = ((pwr_sType *)rbody)->Type;
      ty_size = ((pwr_sType *)rbody)->Size;
      ty_elements = 1;

    }
    else if ( isTypeDef()) {
      m_oid.oix = cdh_tixToOix( 1, m_oid.oix);
      if ( !m_vrep->registerObject( m_oid.oix, this))
        m_vrep->error( "Duplicate type index", getFileName(), line_number);

      ty_tid = ((pwr_sTypeDef *)rbody)->TypeRef;
      ty_type = ((pwr_sTypeDef *)rbody)->Type;
      ty_size = ((pwr_sTypeDef *)rbody)->Size;
      ty_elements = ((pwr_sTypeDef *)rbody)->Elements;
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
      if( !m_vrep->registerObject( m_oid.oix, this)) {
        // Print error message
        char name[120];
        char msg[180];
        pwr_tStatus sts;

        wb_orep *o = m_vrep->object( &sts, m_oid);
        m_vrep->objectName( o, name);
        sprintf( msg, "Duplicate object index %d (%s)", m_oid.oix, name);
        m_vrep->error( msg, getFileName(), line_number);
      }
    }
    is_built = 1;
  }

  if ( recursive && o_fch)
    o_fch->build( recursive);
  if ( recursive && o_fws)
    o_fws->build( recursive);
}

void wb_wblnode::buildObjBodyDef( ref_wblnode classdef)
{
  m_oid.oix = cdh_cixToOix( classdef->c_cix, m_oid.oix, 0);
  if ( !m_vrep->registerObject( m_oid.oix, this))
    m_vrep->error( "Duplicate objbodydef index", getFileName(), line_number);

  int index = 0;
  b_size = 0;
  wb_wblnode *child = o_fch;
  while ( child) {
    if ( child->isAttribute())
      child->buildAttribute( classdef, this, &index, &b_size);
    if ( child->isBuffer())
      child->buildBuffer( classdef, this, &index, &b_size);
    child = child->o_fws;
  }
  ((pwr_sObjBodyDef *)rbody)->Size = b_size;
  ((pwr_sObjBodyDef *)rbody)->NumOfParams = index;
}

void wb_wblnode::buildAttribute( ref_wblnode classdef, ref_wblnode objbodydef,
                                 int *bindex, size_t *boffset)
{
  pwr_eType type;
  size_t size;
  int elements;

  m_oid.oix = cdh_cixToOix( classdef->c_cix, objbodydef->b_bix, m_oid.oix);
  if ( !m_vrep->registerObject( m_oid.oix, this))
    m_vrep->error( "Duplicate attribute index", getFileName(), line_number);

  if ( ((pwr_sParam *)rbody)->Info.Elements == 0)
    ((pwr_sParam *)rbody)->Info.Elements = 1;
  // if ( ((pwr_sParam *)rbody)->Info.Type == 0)
  //  ((pwr_sParam *)rbody)->Info.Type = (pwr_eType)((pwr_sParam *)rbody)->TypeRef;

  if ( cdh_NoCaseStrcmp( cname, "$ObjXRef") == 0 )
    a_tid = pwr_eType_Objid;
  else if ( cdh_NoCaseStrcmp( cname, "$AttrXRef") == 0)
    a_tid = pwr_eType_AttrRef;
  else
    a_tid = ((pwr_sParam *)rbody)->TypeRef;

  a_type = ((pwr_sParam *)rbody)->Info.Type;
  a_size = ((pwr_sParam *)rbody)->Info.Size;
  a_offset = ((pwr_sParam *)rbody)->Info.Offset;
  a_elements = ((pwr_sParam *)rbody)->Info.Elements;
  a_flags = ((pwr_sParam *)rbody)->Info.Flags;

  if ( a_elements == 0)
    a_elements = 1;

  if ( !a_tid) {
    m_vrep->error( "Unknown attribute type", getFileName(), line_number);
    return;
  }
  if ( !m_vrep->getTypeInfo( a_tid, &type, &size, &elements)) {
    m_vrep->error( "Can't find attribute type", getFileName(), line_number);
    return;
  }
  if ( a_type == 0)
    a_type = ((pwr_sParam *)rbody)->Info.Type = type;

  if ( a_flags & pwr_mAdef_pointer) {
    size = sizeof( void *);
  }
  if ( a_flags & pwr_mAdef_array) {
    size *= a_elements;
  }
  if ( ((pwr_sParam *)rbody)->Info.PgmName[0] == 0) {
    strncpy( ((pwr_sParam *)rbody)->Info.PgmName, wb_name::unatName( name()),
	     sizeof( ((pwr_sParam *)rbody)->Info.PgmName));
  }
  if ( cdh_NoCaseStrcmp( cname, "$Input") == 0) {
    a_size = ((pwr_sParam *)rbody)->Info.Size = size;
    a_offset = ((pwr_sParam *)rbody)->Info.Offset = *boffset + sizeof( void *);
    *boffset += sizeof( void *) + wblAlign( a_size);
  }
  else {
    a_size = ((pwr_sParam *)rbody)->Info.Size = size;
    a_offset = ((pwr_sParam *)rbody)->Info.Offset = *boffset;
    *boffset += wblAlign( a_size);
  }
  ((pwr_sParam *)rbody)->Info.ParamIndex = *bindex;
  (*bindex)++;
}

void wb_wblnode::buildBuffer( ref_wblnode classdef, ref_wblnode objbodydef, 
                              int *bindex, size_t *boffset)
{
  size_t rsize, dsize;

  m_oid.oix = cdh_cixToOix( classdef->c_cix, objbodydef->b_bix, m_oid.oix);
  if ( !m_vrep->registerObject( m_oid.oix, this))
    m_vrep->error( "Duplicate buffer index", getFileName(), line_number);

  if ( ((pwr_sBuffer *)rbody)->Info.Elements == 0)
    ((pwr_sBuffer *)rbody)->Info.Elements = 1;
  if ( ((pwr_sBuffer *)rbody)->Info.Type == 0)
    ((pwr_sParam *)rbody)->Info.Type = (pwr_eType)((pwr_sBuffer *)rbody)->Class;

  a_tid = ((pwr_sBuffer *)rbody)->Class;
  a_type = ((pwr_sParam *)rbody)->Info.Type;
  a_size = ((pwr_sBuffer *)rbody)->Info.Size;
  a_offset = ((pwr_sBuffer *)rbody)->Info.Offset;
  a_elements = ((pwr_sBuffer *)rbody)->Info.Elements;
  a_flags = ((pwr_sBuffer *)rbody)->Info.Flags;

  if ( !a_tid) {
    m_vrep->error( "Unknown buffer type", getFileName(), line_number);
    return;
  }
  if ( !m_vrep->getClassInfo( a_tid, &rsize, &dsize)) {
    m_vrep->error( "Can't find buffer class", getFileName(), line_number);
    return;
  }

  a_size = ((pwr_sBuffer *)rbody)->Info.Size = a_elements * rsize;
  a_offset = ((pwr_sBuffer *)rbody)->Info.Offset = *boffset;
  ((pwr_sBuffer *)rbody)->Info.ParamIndex = *bindex;
  *boffset += a_size;
  (*bindex)++;
}

void wb_wblnode::buildTemplate( ref_wblnode classdef)
{
  wb_wblnode *objbodydef = classdef->o_fch;
  m_oid.oix = cdh_cixToOix( classdef->c_cix, pwr_eBix_template, 0);
  if ( !m_vrep->registerObject( m_oid.oix, this))
    m_vrep->error( "Duplicate template oix", getFileName(), line_number);

  while ( objbodydef) {
    if ( objbodydef->isObjBodyDef()) {
      if ( objbodydef->b_bix == pwr_eBix_sys || 
           objbodydef->b_bix == pwr_eBix_rt) {  
        rbody_size = objbodydef->b_size;
        if ( rbody_size) {
          rbody = calloc( 1, rbody_size);
        }
      }
      if ( objbodydef->b_bix == pwr_eBix_dev) {
        dbody_size = objbodydef->b_size;
        if ( dbody_size)
          dbody = calloc( 1, dbody_size);
      }
    }
    objbodydef = objbodydef->o_fws;
  }
  m_flags.b.Template = 1;

  ref_wblnode first_child;
  first_child = getFirstChild();
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

    first_child = getFirstChild();
    if ( first_child)
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
  char buf[2048];
  int int_val, current_int_val;
  bool add_newline = false;

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


    if ( !m_vrep->getAttrInfo( name(), (pwr_eBix) bix, object->m_cid, &size, &offset,
                               &tid, &elements, &type)) {
      // This might be string special syntax
      wb_attrname n = wb_attrname( name());
      if ( n.hasAttrIndex() && 
           m_vrep->getAttrInfo( n.attribute(), (pwr_eBix) bix, object->m_cid, &size, 
                                &offset, &tid, &elements, &type) &&
           elements == 1 && 
           (type == pwr_eType_String || type == pwr_eType_Text)) {
        // Index in string attribute marks a new row
        int index = n.attrIndex();
        if ( index > 0) {
          add_newline = true;
          index--;
        }
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
          object->rbody_size == 0) ||
         (bix == pwr_eBix_dev && object->dbody_size == 0)) {
      m_vrep->error( "Attribute body", getFileName(), line_number);
      return;
    }

    if ( ((bix == pwr_eBix_rt || bix == pwr_eBix_sys) &&
          offset + size > object->rbody_size) ||
         ( bix == pwr_eBix_rt &&
           offset + size > object->rbody_size)) {
      m_vrep->error( "Mismatch in attribute offset", getFileName(), line_number);
      return;
    }

    if ( !add_newline)
      strncpy( value, second_child->name(), sizeof(value));
    else {
      strncpy( &value[1], second_child->name(), sizeof(value)-1);
      value[0] = '\n';
    }

    // printf( "Attr %s %s %d %d %s\n", object->name, name, size, offset, value);
    if ( size == sizeof(int_val) && convconst( &int_val, value)) {
      if ( oper == tokens.EQ) {
        if ( bix == pwr_eBix_rt || bix == pwr_eBix_sys) 
          memcpy( (char *)((unsigned int) object->rbody + offset), 
                  &int_val, size);
        else if ( bix == pwr_eBix_dev)
          memcpy( (char *)((unsigned int) object->dbody + offset), 
                  &int_val, size);
      }
      else if ( oper == tokens.OREQ) {
        if ( bix == pwr_eBix_rt || bix == pwr_eBix_sys) {
          current_int_val = *(int *) ((unsigned int) object->rbody + offset);
          int_val |= current_int_val;
          memcpy( (char *)((unsigned int) object->rbody + offset),
                  &int_val, size);
        }
        else if ( bix == pwr_eBix_dev) {
          current_int_val = *(int *) ((unsigned int) object->dbody + offset);
          int_val |= current_int_val;
          memcpy( (char *)((unsigned int) object->dbody + offset),            
                  &int_val, size);
        }
      }
    }
    else if ( attrStringToValue( type, value, buf, sizeof( buf), size)) {
      if ( bix == pwr_eBix_rt || bix == pwr_eBix_sys)
        memcpy( (char *)((unsigned int) object->rbody + offset), 
                buf, size);
      else if ( bix == pwr_eBix_dev)
        memcpy( (char *)((unsigned int) object->dbody + offset), 
                buf, size);
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
  next_sibling = getNextSibling();
  if ( next_sibling)
    next_sibling->buildAttr( object, bix);
}

void wb_wblnode::buildBuff( ref_wblnode object, pwr_eBix bix, pwr_tCid buffer_cid,
                            int buffer_offset, int buffer_size)
{
  ref_wblnode first_child;
  int size, offset, elements;
  pwr_tStatus sts;
  pwr_tCid host_cid;

  if ( buffer_cid != 0) {
    // Buffer in buffer... Fix
    host_cid = buffer_cid;
  }
  else
    host_cid = object->m_cid;

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

  pwr_tCid cid = adrep->subClass();
  size = adrep->size();
  offset = adrep->offset();
  elements = adrep->nElement();
  delete cdrep;
  delete adrep;

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

    // Backward compability with V3.4 : classid was named class
    // This section can be removed in later versions
    if ( strcmp( name(), "class") == 0 &&  
         (strcmp( cdrep->name(), "$PlcProgram") == 0 ||
          strcmp( cdrep->name(), "$PlcWindow") == 0 ||
          strcmp( cdrep->name(), "$PlcNode") == 0 ||
          strcmp( cdrep->name(), "$PlcConnection") == 0))
      adrep = cdrep->adrep( &sts, "classid");
    else
      // end of compability section
      adrep = cdrep->adrep( &sts, aname.attribute());
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
          object->rbody_size == 0) ||
         (bix == pwr_eBix_dev && object->dbody_size == 0)) {
      m_vrep->error( "Attribute body", getFileName(), line_number);
      return;
    }

    if ( ((bix == pwr_eBix_rt || bix == pwr_eBix_sys) &&
          offset + size/elements > object->rbody_size) ||
         ( bix == pwr_eBix_rt &&
           offset + size/elements > object->rbody_size)) {
      m_vrep->error( "Mismatch in attribute offset", getFileName(), line_number);
      return;
    }

    strcpy( value, second_child->name());

    // printf( "Attr %s %s %d %d %s\n", object->name, name, size, offset, value);
    if ( size/elements == sizeof(int_val) && convconst( &int_val, value)) {
      if ( oper == tokens.EQ) {
        if ( bix == pwr_eBix_rt || bix == pwr_eBix_sys) 
          memcpy( (char *)((unsigned int) object->rbody + offset), 
                  &int_val, size/elements);
        else if ( bix == pwr_eBix_dev)
          memcpy( (char *)((unsigned int) object->dbody + offset), 
                  &int_val, size/elements);
      }
      else if ( oper == tokens.OREQ) {
        if ( bix == pwr_eBix_rt || bix == pwr_eBix_sys) {
          current_int_val = *(int *) ((unsigned int) object->rbody + offset);
          int_val |= current_int_val;
          memcpy( (char *)((unsigned int) object->rbody + offset),
                  &int_val, size/elements);
        }
        else if ( bix == pwr_eBix_dev) {
          current_int_val = *(int *) ((unsigned int) object->dbody + offset);
          int_val |= current_int_val;
          memcpy( (char *)((unsigned int) object->dbody + offset),            
                  &int_val, size/elements);
        }
      }
    }
    else if ( attrStringToValue( type, value, buf, sizeof( buf), size)) {
      if ( bix == pwr_eBix_rt || bix == pwr_eBix_sys)
        memcpy( (char *)((unsigned int) object->rbody + offset), 
                buf, size/elements);
      else if ( bix == pwr_eBix_dev)
        memcpy( (char *)((unsigned int) object->dbody + offset), 
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

void wb_wblnode::link( wb_vrepwbl *vol, ref_wblnode father)
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
    o_fth = father;
    if ( o_fth) {
      o_bws = o_fth->get_o_lch();
      if ( o_bws)
        o_bws->o_fws = this;
      else
        o_fth->o_fch = this;
    }
    first_child = getFirstChild();
    if ( first_child)
      first_child->link( vol, this);

    next_sibling = getNextSibling();
    if ( next_sibling)
      next_sibling->link( vol, father);

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
      first_child->link( vol, snode);

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
      next_sibling->link( vol, father);
  }
}

void wb_wblnode::registerNode( wb_vrepwbl *vol)
{
  ref_wblnode first_child = getFirstChild();
  m_vrep = vol;

  switch ( getType()) {
  case tokens.OBJECT:
  {
    if ( !wb_name::checkObjectName( name())) {
      m_vrep->error( "Bad object name", getFileName(), line_number);
    }

    // Get class
    if ( first_child) {
      string class_name = first_child->getText();
      strcpy( cname, class_name.c_str());
 
      if ( !classNameToCid( cname, &m_cid)) {
        m_vrep->error( "Unknown class", getFileName(), line_number);
      }

      // If $ClassDef, register class in classlist
      if ( !isTemplate()) {
        if ( first_child->getType() == tokens.VALUE &&
             (strcmp( cname, "$ClassDef") == 0 ||
              strcmp( cname, "pwr_eClass_ClassDef") == 0)) {
          node_type = wbl_eNodeType_ClassDef;
        }
        else if ( first_child->getType() == tokens.VALUE &&
                  ( strcmp( cname, "$Type") == 0 ||
                    strcmp( cname, "pwr_eClass_Type") == 0)) {
          node_type = wbl_eNodeType_Type;
        }
        else if ( first_child->getType() == tokens.VALUE &&
                  ( strcmp( cname, "$TypeDef") == 0 ||
                    strcmp( cname, "pwr_eClass_TypeDef") == 0)) {
          node_type = wbl_eNodeType_TypeDef;
        }
        else if ( first_child->getType() == tokens.VALUE &&
                  ( strcmp( cname, "$ObjBodyDef") == 0 ||
                    strcmp( cname, "pwr_eClass_ObjBodyDef") == 0)) {
          node_type = wbl_eNodeType_ObjBodyDef;
        }
        else if ( first_child->getType() == tokens.VALUE &&
                  ( strcmp( cname, "$Attribute") == 0 ||
                    strcmp( cname, "$Input") == 0 ||
                    strcmp( cname, "$Output") == 0 ||
                    strcmp( cname, "$Intern") == 0 ||
                    strcmp( cname, "$ObjXRef") == 0 ||
                    strcmp( cname, "$AttrXRef") == 0 ||
                    strcmp( cname, "pwr_eClass_Param") == 0)) {
          node_type = wbl_eNodeType_Attribute;
        }
        else if ( first_child->getType() == tokens.VALUE &&
                  ( strcmp( cname, "$Buffer") == 0)) {
          node_type = wbl_eNodeType_Buffer;
        }
        else if ( first_child->getType() == tokens.VALUE &&
                  ( strcmp( cname, "$Param") == 0)) {
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
          if ( !stringToOix( oixstr.c_str(), &m_oid.oix)) {
            m_oid.oix = m_vrep->nextOix();
          }
          break;
        }
        case tokens.ENDOBJECT:
        case tokens.OBJECT:
        case tokens.BODY:
          m_oid.oix = m_vrep->nextOix();
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
            m_oid.oix = m_vrep->nextOix();
        }
      }
    }
    else if ( isTemplate() && strcmp( cname, "") != 0) {
      // Created template object
    }
    else {
      // Syntax exception -- No class
      m_vrep->error( "Missing class", getFileName(), line_number);
    }

    if ( isClassDef()) {
      c_cid = cdh_cixToCid( m_vrep->vid(), m_oid.oix);
      c_cix = m_oid.oix;
      m_vrep->registerClass( name(), c_cid, this);

      // Find Template object
      ref_wblnode child = first_child;
      ref_wblnode last_child = child;
      string childname;
      while ( child) {
        if ( child->getType() == tokens.OBJECT) {
          childname = child->getText();
          if ( strcmp( childname.c_str(), "Template") == 0) {
            c_template = child;
            c_template->node_type = wbl_eNodeType_Template;
            break;
          }
        }
        last_child = child;
        child = child->getNextSibling();
      }
      if ( !c_template) {
        // Create a template node
        c_template = new wb_wblnode();
        c_template->setType( tokens.OBJECT);
        string tname("Template");
        c_template->setText( tname);
        if ( last_child)
          last_child->setNextSibling( (RefAST)c_template);
        else
          setFirstChild( (RefAST)c_template);
        strcpy( c_template->cname, name());
        // c_template->m_oid.oix = m_vrep->nextOix();
        c_template->m_cid = c_cid;
        c_template->node_type = wbl_eNodeType_Template;
      }
    }
    else if ( isType()) {
      m_tid = cdh_tixToTid( m_vrep->vid(), 0, m_oid.oix);
      m_vrep->registerType( name(), m_tid, this);
    }
    else if ( isTypeDef()) {
      m_tid = cdh_tixToTid( m_vrep->vid(), 1, m_oid.oix);
      m_vrep->registerType( name(), m_tid, this);
    }
    else if ( isObjBodyDef()) {
      b_bix = (pwr_eBix)m_oid.oix;
      if ( !(b_bix == pwr_eBix_rt || b_bix == pwr_eBix_sys || b_bix == pwr_eBix_dev))
        m_vrep->error( "Bad body index", getFileName(), line_number);
    }
    break;
  }
  case tokens.VOLUME:
  {
    pwr_tVid vid;
    int sts;

    // Get class
    if ( first_child) {
      string class_name = first_child->getText();
      strcpy( cname, class_name.c_str());

      if ( !classNameToCid( cname, &m_cid)) {
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
    m_vrep->registerVolume( name(), m_cid, vid, this);

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

  if ( first_child)
    first_child->registerNode( vol);

  ref_wblnode next_sibling = getNextSibling();
  if ( next_sibling)
    next_sibling->registerNode( vol);

}

#if 0
void wb_wblnode::iterObject( wb_dbs *dbs)
{
  ref_wblnode o_lch = get_o_lch();
  pwr_tOid fthoid = o_fth ? o_fth->m_oid : pwr_cNOid;
  pwr_tOid fwsoid = o_fws ? o_fws->m_oid : pwr_cNOid;
  pwr_tOid bwsoid = o_bws ? o_bws->m_oid : pwr_cNOid;
  pwr_tOid fchoid = o_fch ? o_fch->m_oid : pwr_cNOid;
  pwr_tOid lchoid = o_lch ? o_lch->m_oid : pwr_cNOid;
  wb_name n = wb_name(name());

  dbs->installObject( m_oid, m_cid, fthoid, fwsoid, bwsoid, fchoid, lchoid, 
                      (char *)name(), n.normName(cdh_mName_object),
                      getFileTime(), getFileTime(), getFileTime(), rbody_size, dbody_size);
  
  if ( o_fch)
    o_fch->iterObject( dbs);

  if ( o_fws)
    o_fws->iterObject( dbs);
}

void wb_wblnode::iterDbody( wb_dbs *dbs)
{
  dbs->installDbody( m_oid, dbody);
  
  if ( o_fch)
    o_fch->iterDbody( dbs);

  if ( o_fws)
    o_fws->iterDbody( dbs);
}

void wb_wblnode::iterRbody( wb_dbs *dbs)
{
  dbs->installRbody( m_oid, rbody);
  
  if ( o_fch)
    o_fch->iterRbody( dbs);

  if ( o_fws)
    o_fws->iterRbody( dbs);
}
#endif

bool wb_wblnode::exportHead(wb_import &i)
{
  pwr_tStatus sts;
  pwr_mClassDef flags;

  m_vrep->getClassFlags( &sts, m_cid, &flags);
  if ( EVEN(sts)) throw wb_error( sts);

  m_flags.m |= flags.m;
  ref_wblnode o_lch = get_o_lch();
  pwr_tOid fthoid = o_fth ? o_fth->m_oid : pwr_cNOid;
  pwr_tOid fwsoid = o_fws ? o_fws->m_oid : pwr_cNOid;
  pwr_tOid bwsoid = o_bws ? o_bws->m_oid : pwr_cNOid;
  pwr_tOid fchoid = o_fch ? o_fch->m_oid : pwr_cNOid;
  pwr_tOid lchoid = o_lch ? o_lch->m_oid : pwr_cNOid;
  wb_name n = wb_name(name());

  i.importHead( m_oid, m_cid, fthoid, bwsoid, fwsoid, fchoid, lchoid, name(), n.normName(cdh_mName_object),
                getFileTime(), getFileTime(), getFileTime(), rbody_size, dbody_size);
  
  if ( o_fch)
    o_fch->exportHead( i);

  if ( o_fws)
    o_fws->exportHead( i);

  return true;
}

bool wb_wblnode::exportDbody( wb_import &i)
{
  i.importDbody( m_oid, dbody_size, dbody);
  
  if ( o_fch)
    o_fch->exportDbody( i);

  if ( o_fws)
    o_fws->exportDbody( i);

  return true;
}

bool wb_wblnode::exportRbody( wb_import &i)
{
  i.importRbody( m_oid, rbody_size, rbody);
  
  if ( o_fch)
    o_fch->exportRbody( i);

  if ( o_fws)
    o_fws->exportRbody( i);

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
    {
      pwr_tTypeId	val_typeid;
      pwr_eType type;
      size_t size;
      int elements;

      // sts = m_vrep->nameToOid( value_str, &objid);
      if ( m_vrep->getTypeInfo( value_str, &val_typeid, &type,
                                &size, &elements))
        memcpy( buffer_ptr, (char *) &val_typeid, sizeof(val_typeid));
      else
        return 0;
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
