/* xtt_c_object.c -- common xtt methods for objects.

   PROVIEW/R
   Copyright (C) 1994 by Comator Process AB.  */

#include "pwr_baseclasses.h"
#include "pwr_nmpsclasses.h"
#include "flow_std.h"

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "xtt_menu.h"
#include "xtt_xnav.h"
#include "xtt_url.h"
#include "rt_xnav_msg.h"
extern "C" {
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_wow.h"
}


/////////////////////////////////////////////////////////////////////
//
//  Object methods
//
/////////////////////////////////////////////////////////////////////

//
// Open object
//
static pwr_tStatus OpenObject( xmenu_sMenuCall *ip)
{
  ((XNav *)ip->EditorContext)->open_object( ip->Pointed.Objid);
  return 1;
}

// Open Object filter
static pwr_tStatus OpenObjectFilter( xmenu_sMenuCall *ip)
{
  int sts;
  pwr_tClassId classid;

  if ( ip->Caller == xmenu_mUtility_Ge) {
    sts = gdh_GetObjectClass( ip->Pointed.Objid, &classid);
    if ( EVEN(sts)) return sts;

    if ( classid == pwr_eClass_PlantHier)
      return XNAV__INVISIBLE;
  }
  return XNAV__SUCCESS;
}

//
// Open crossref
//
static pwr_tStatus OpenCrossref( xmenu_sMenuCall *ip)
{
  ((XNav *)ip->EditorContext)->open_crossref( ip->Pointed.Objid);
  return 1;
}

//
// Open crossref filter
//
static pwr_tStatus OpenCrossrefFilter( xmenu_sMenuCall *ip)
{
  return 1;
}

//
// Hist event
//
static pwr_tStatus HistEvent( xmenu_sMenuCall *ip)
{
  char cmd[200];
  char name[120];
  int sts;

  sts = gdh_ObjidToName( ip->Pointed.Objid, name, sizeof(name),
			cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;

  strcpy( cmd, "show hist ");
  strcat( cmd, name);
  ((XNav *)ip->EditorContext)->command( cmd);
  return 1;
}

//
// Open crossref filter
//
static pwr_tStatus HistEventFilter( xmenu_sMenuCall *ip)
{
  return 1;
}

//
// Open trace
//
static pwr_tStatus OpenTrace( xmenu_sMenuCall *ip)
{
  char name[120];
  char cmd[200];
  int sts;
  pwr_tObjid parent;
  char parent_name[120];

  sts = gdh_GetParent( ip->Pointed.Objid, &parent);
  sts = gdh_ObjidToName( parent, parent_name, sizeof(parent_name),
			cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;
  sts = gdh_ObjidToName( ip->Pointed.Objid, name, sizeof(name),
			cdh_mName_object);
  if ( EVEN(sts)) return sts;

  sprintf( cmd, "open trace/name=%s/center=%s", parent_name, name);
  ((XNav *)ip->EditorContext)->command( cmd);
  return 1;
}

// Open trace
static pwr_tStatus OpenTraceFilter( xmenu_sMenuCall *ip)
{
  int sts;
  pwr_tObjid parent;
  pwr_tClassId classid;

  if ( ip->Caller == xmenu_mUtility_Trace ||
       ip->Caller == xmenu_mUtility_Simulate)
    return XNAV__INVISIBLE;

  sts = gdh_GetParent( ip->Pointed.Objid, &parent);
  while ( ODD(sts)) {
    gdh_GetObjectClass( parent, &classid);
    if ( classid == pwr_cClass_plc)
      return XNAV__SUCCESS;
    sts = gdh_GetParent( parent, &parent);
  }
  return XNAV__INVISIBLE;
}

//
// Open trace
//
static pwr_tStatus OpenTrend( xmenu_sMenuCall *ip)
{
  char name[120];
  char cmd[200];
  int sts;
  pwr_tObjid child;
  pwr_tClassId classid;
  int found;
  pwr_tObjid deftrend;

  sts = gdh_GetObjectClass( ip->Pointed.Objid, &classid);
  if ( EVEN(sts)) return sts;

  if ( classid == pwr_cClass_DsTrend || classid == pwr_cClass_PlotGroup) {
    sts = gdh_ObjidToName( ip->Pointed.Objid, name, sizeof(name),
			cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    // Open trend
    sprintf( cmd, "open trend /name=%s /title=\"%s\"", name, name);
    ((XNav *)ip->EditorContext)->command( cmd);
    return 1;
  }

  // Look for attribute DefTrend
  sts = gdh_ObjidToName( ip->Pointed.Objid, name, sizeof(name),
			cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;

  strcat( name, ".DefTrend");
  sts = gdh_GetObjectInfo( name, (void *)&deftrend, sizeof(deftrend));
  if ( ODD(sts) && cdh_ObjidIsNotNull( deftrend)) {
    // Default XttGraph found
    sts = gdh_GetObjectClass( deftrend, &classid);
    if ( ODD(sts) &&
         (classid == pwr_cClass_DsTrend || classid == pwr_cClass_PlotGroup)) {

      sts = gdh_ObjidToName( deftrend, name, sizeof(name),
			cdh_mName_volumeStrict);
      if ( EVEN(sts)) return sts;

      // Open trend
      sprintf( cmd, "open trend /name=%s /title=\"%s\"", name, name);
      ((XNav *)ip->EditorContext)->command( cmd);
      return 1;
    }
  }

  // Look for DsTrend as child
  found = 0;
  sts = gdh_GetChild( ip->Pointed.Objid, &child);
  while ( ODD(sts)) {
    sts = gdh_GetObjectClass( child, &classid);
    if ( EVEN(sts)) return sts;

    if ( classid == pwr_cClass_DsTrend) {
      found = 1;
      break;
    }      
    sts = gdh_GetNextSibling( child, &child);
  }
  if ( !found)
    return 1;

  sts = gdh_ObjidToName( child, name, sizeof(name),
			cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;

  // Open trend
  sprintf( cmd, "open trend /name=%s /title=\"%s\"", name, name);
  ((XNav *)ip->EditorContext)->command( cmd);
  return 1;
}

// Open trend filter
static pwr_tStatus OpenTrendFilter( xmenu_sMenuCall *ip)
{
  int sts;
  pwr_tObjid child;
  pwr_tClassId classid;
  pwr_tObjid deftrend;
  char name[120];

  sts = gdh_GetObjectClass( ip->Pointed.Objid, &classid);
  if ( EVEN(sts)) return sts;

  if ( classid == pwr_cClass_DsTrend || classid == pwr_cClass_PlotGroup) {
    return XNAV__SUCCESS;
  }

  // Check if attribute DefTrend exist
  sts = gdh_ObjidToName( ip->Pointed.Objid, name, sizeof(name),
			cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;

  strcat( name, ".DefTrend");
  sts = gdh_GetObjectInfo( name, (void *)&deftrend, sizeof(deftrend));
  if ( ODD(sts) && cdh_ObjidIsNotNull( deftrend)) {
    // Default XttGraph found
    sts = gdh_GetObjectClass( deftrend, &classid);
    if ( ODD(sts) &&
         (classid == pwr_cClass_DsTrend || classid == pwr_cClass_PlotGroup))
      return XNAV__SUCCESS;
  }

  // Check if object has a DsTrend as child
  sts = gdh_GetChild( ip->Pointed.Objid, &child);
  while ( ODD(sts)) {
    sts = gdh_GetObjectClass( child, &classid);
    if ( EVEN(sts)) return sts;

    if ( classid == pwr_cClass_DsTrend)
      return XNAV__SUCCESS;

    sts = gdh_GetNextSibling( child, &child);
  }
  return XNAV__INVISIBLE;
}

// Open runtime navigator object
static pwr_tStatus RtNavigator( xmenu_sMenuCall *ip)
{
  ((XNav *)ip->EditorContext)->display_object( ip->Pointed.Objid, 0);
  ((XNav *)ip->EditorContext)->pop();

  return 1;
}

// Open runtime navigator filter
static pwr_tStatus RtNavigatorFilter( xmenu_sMenuCall *ip)
{
  if ( ip->Caller == xmenu_mUtility_XNav)
    return XNAV__INVISIBLE;
  return XNAV__SUCCESS;
}

// Open class graph
static pwr_tStatus OpenClassGraph( xmenu_sMenuCall *ip)
{
  int		sts;
  pwr_tClassId	classid;
  char		name[120];
  char		classname[80];
  char		filename[120];
  char		fname[120];
  char		found_file[120];
  char		cmd[200];
  pwr_tObjid    objid;

  sts = gdh_ObjidToName( ip->Pointed.Objid,
		  name, sizeof(name), cdh_mNName);
  if ( EVEN(sts)) return sts;

  // Check if object is mounted with other name
  sts = gdh_NameToObjid( name, &objid);
  if ( EVEN(sts)) {
    sts = gdh_ObjidToName( ip->Pointed.Objid,
		  name, sizeof(name), cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;
  }

  sts = gdh_GetObjectClass( ip->Pointed.Objid, &classid);
  if ( EVEN(sts)) return sts;

  sts = gdh_ObjidToName( cdh_ClassIdToObjid( classid),
		  classname, sizeof(classname), cdh_mName_object);
  if ( EVEN(sts)) return sts;
  cdh_ToLower( classname, classname);

  sprintf( filename, "pwr_exe:pwr_c_%s.pwg", classname);
  dcli_translate_filename( fname, filename);
  sts = dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_INIT);
  dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_END);
  if ( EVEN(sts))
  {
    sprintf( filename, "pwrp_exe:%s.pwg", classname);
    dcli_translate_filename( fname, filename);
    sts = dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_INIT);
    dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_END);
    if ( EVEN(sts)) return sts;
  }
  sprintf( cmd, "open graph %s/inst=%s/name=\"%s\"", 
           filename, name, name);

  // Add scrollbars for some classes
  switch ( classid) {
    case pwr_cClass_NMpsCell:
    case pwr_cClass_NMpsStoreCell:
      strcat( cmd, "/sc");
      break;
    default:
      ;
  }
  ((XNav *)ip->EditorContext)->command( cmd);

  return XNAV__SUCCESS;
}

// Open object graph filter
static pwr_tStatus OpenClassGraphFilter( xmenu_sMenuCall *ip)
{
  int		sts;
  pwr_tClassId	classid;
  char		classname[80];
  char		filename[120];
  char		fname[120];
  char		found_file[120];

  sts = gdh_GetObjectClass( ip->Pointed.Objid, &classid);
  if ( EVEN(sts)) return sts;

  sts = gdh_ObjidToName( cdh_ClassIdToObjid( classid),
		  classname, sizeof(classname), cdh_mName_object);
  if ( EVEN(sts)) return sts;
  cdh_ToLower( classname, classname);

  sprintf( filename, "pwr_exe:pwr_c_%s.pwg", classname);
  dcli_translate_filename( fname, filename);
  sts = dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_INIT);
  dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_END);
  if ( EVEN(sts))
  {
    sprintf( filename, "pwrp_exe:%s.pwg", classname);
    dcli_translate_filename( fname, filename);
    sts = dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_INIT);
    dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_END);
    if ( EVEN(sts))
      return XNAV__INVISIBLE;
  }
  return XNAV__SUCCESS;
}

// Open Graph
static pwr_tStatus OpenGraph( xmenu_sMenuCall *ip)
{
  int sts;
  pwr_tObjid objid;
  char name[140];
  pwr_tObjid defgraph;
  pwr_tClassId classid;
  char cmd[200];

  objid = ip->Pointed.Objid;
  sts = XNAV__SUCCESS;
  while( ODD(sts)) {
    sts = gdh_ObjidToName( objid, name, sizeof(name),
			cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    strcat( name, ".DefGraph");
    sts = gdh_GetObjectInfo( name, (void *)&defgraph, sizeof(defgraph));
    if ( ODD(sts) && cdh_ObjidIsNotNull( defgraph)) {
      // Default XttGraph found
      sts = gdh_GetObjectClass( defgraph, &classid);
      if ( EVEN(sts)) return sts;

      if ( classid == pwr_cClass_XttGraph) {
        printf( "DefGraph found\n");
 
        sts = gdh_ObjidToName( defgraph, name, sizeof(name), cdh_mNName);
        strcpy( cmd, "ope gra/obj=");
        strcat( cmd, name);
        sts = ((XNav *)ip->EditorContext)->command( cmd);
        break;
      }
    }

    gdh_GetParent( objid, &objid);
  }

  return XNAV__SUCCESS;
}

// Open Graph filter
static pwr_tStatus OpenGraphFilter( xmenu_sMenuCall *ip)
{
  int sts;
  pwr_tObjid objid;
  char name[140];
  pwr_tObjid defgraph;
  pwr_tClassId classid;
  char action[80];
  char *s;

  objid = ip->Pointed.Objid;
  sts = XNAV__SUCCESS;
  while( ODD(sts)) {
    sts = gdh_ObjidToName( objid, name, sizeof(name),
			cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    strcat( name, ".DefGraph");
    sts = gdh_GetObjectInfo( name, (void *)&defgraph, sizeof(defgraph));
    if ( ODD(sts) && cdh_ObjidIsNotNull( defgraph)) {
      // Default XttGraph found
      sts = gdh_GetObjectClass( defgraph, &classid);
      if ( EVEN(sts)) return sts;

      if ( classid == pwr_cClass_XttGraph) {
        if ( ip->Caller == xmenu_mUtility_Ge && ip->Arg) {
          // Check that graph is not the same as caller
          sts = gdh_ObjidToName( defgraph, name, sizeof(name),
			cdh_mName_volumeStrict);
          if ( EVEN(sts)) return sts;
          
          strcat( name, ".Action");
          sts = gdh_GetObjectInfo( name, (void *)action, sizeof(action));
          if ( EVEN(sts)) return sts;

          if ( (s = strrchr(action, '.')))
            *s = 0;
          if ( cdh_NoCaseStrcmp( action, ip->Arg) == 0)
            return XNAV__INVISIBLE;
        }
        return XNAV__SUCCESS;
      }
    }

    gdh_GetParent( objid, &objid);
  }

  return XNAV__INVISIBLE;
}

// Collect
static pwr_tStatus Collect( xmenu_sMenuCall *ip)
{
  int sts;

  sts = ((XNav *)ip->EditorContext)->collect_insert( &ip->Pointed);
  if ( EVEN(sts)) {
    wow_DisplayError( (Widget) ip->WindowContext, "Collect error", 
		      XNav::get_message(sts));
  }

  return XNAV__SUCCESS;
}

// Collect filter
static pwr_tStatus CollectFilter( xmenu_sMenuCall *ip)
{
  char attr[80];
  int sts;

  if ( ip->ItemType == xmenu_eItemType_Object) {
    sts = xnav_get_trace_attr( ip->Pointed.Objid, attr);
    if ( EVEN(sts))
      return XNAV__INVISIBLE;
  }
  return XNAV__SUCCESS;
}

// Help
static pwr_tStatus Help( xmenu_sMenuCall *ip)
{
  int sts;
  pwr_tObjid objid;
  char name[140];
  pwr_tString40 helptopic;
  char cmd[200];


  objid = ip->Pointed.Objid;

  sts = gdh_ObjidToName( objid, name, sizeof(name),
			cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;

  strcat( name, ".HelpTopic");
  sts = gdh_GetObjectInfo( name, (void *)helptopic, sizeof(helptopic));
  if ( EVEN(sts)) return sts;

  if ( strcmp( helptopic, "") == 0)
    return 0;

  sprintf( cmd, "help %s", helptopic);

  sts = ((XNav *)ip->EditorContext)->command( cmd);

  return XNAV__SUCCESS;
}

// Help filter
static pwr_tStatus HelpFilter( xmenu_sMenuCall *ip)
{
  int sts;
  pwr_tObjid objid;
  char name[140];
  pwr_tString40 helptopic;

  objid = ip->Pointed.Objid;

  sts = gdh_ObjidToName( objid, name, sizeof(name),
			cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;

  strcat( name, ".HelpTopic");
  sts = gdh_GetObjectInfo( name, (void *)helptopic, sizeof(helptopic));
  if ( EVEN(sts)) return XNAV__INVISIBLE;

  if ( strcmp( helptopic, "") == 0)
    return XNAV__INVISIBLE;

  return XNAV__SUCCESS;
}

// DataSheet
static pwr_tStatus DataSheet( xmenu_sMenuCall *ip)
{
  int sts;
  pwr_tObjid objid;
  char name[140];
  pwr_tURL datasheet;

  objid = ip->Pointed.Objid;

  sts = gdh_ObjidToName( objid, name, sizeof(name),
			cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;

  strcat( name, ".DataSheet");
  sts = gdh_GetObjectInfo( name, (void *)datasheet, sizeof(datasheet));
  if ( EVEN(sts)) return sts;

  if ( strcmp( (char *)datasheet, "") == 0)
    return 0;

  xnav_open_URL( datasheet);
  return XNAV__SUCCESS;
}

// DataSheet filter
static pwr_tStatus DataSheetFilter( xmenu_sMenuCall *ip)
{
  int sts;
  pwr_tObjid objid;
  char name[140];
  pwr_tURL datasheet;


  objid = ip->Pointed.Objid;

  sts = gdh_ObjidToName( objid, name, sizeof(name),
			cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;

  strcat( name, ".DataSheet");
  sts = gdh_GetObjectInfo( name, (void *)datasheet, sizeof(datasheet));
  if ( EVEN(sts)) return XNAV__INVISIBLE;

  if ( strcmp( datasheet, "") == 0)
    return XNAV__INVISIBLE;

  return XNAV__SUCCESS;
}

// Help Class
static pwr_tStatus HelpClass( xmenu_sMenuCall *ip)
{
  int sts;
  pwr_tClassId classid;
  char classname[80];
  char cmd[200];

  sts = gdh_GetObjectClass( ip->Pointed.Objid, &classid);
  if ( EVEN(sts)) return sts;

  sts = gdh_ObjidToName( cdh_ClassIdToObjid( classid),
		  classname, sizeof(classname), cdh_mName_object);
  if ( EVEN(sts)) return sts;

  if ( classname[0] == '$')
    sprintf( cmd, "help %s", &classname[1]);
  else
    sprintf( cmd, "help %s", classname);

  sts = ((XNav *)ip->EditorContext)->command( cmd);

  return XNAV__SUCCESS;
}

// Help Class filter
static pwr_tStatus HelpClassFilter( xmenu_sMenuCall *ip)
{
  int sts;
  pwr_tClassId classid;

  if ( ip->Caller == xmenu_mUtility_Ge) {
    sts = gdh_GetObjectClass( ip->Pointed.Objid, &classid);
    if ( EVEN(sts)) return sts;

    if ( classid == pwr_eClass_PlantHier)
      return XNAV__INVISIBLE;
  }
  return XNAV__SUCCESS;
}

/////////////////////////////////////////////////////////////////////
//
//  Attribute methods
//
/////////////////////////////////////////////////////////////////////

// Open type graph
static pwr_tStatus OpenTypeGraph( xmenu_sMenuCall *ip)
{
  int		sts;
  char		name[120];
  char		cmd[200];
  pwr_tTypeId   attr_type;
  pwr_tUInt32   attr_size;
  pwr_tUInt32   attr_offset;
  pwr_tUInt32   attr_dimension;
  char          filename[200];

  sts = gdh_AttrrefToName( &ip->Pointed, name, sizeof(name), cdh_mNName);
  if ( EVEN(sts)) return sts;

  sts = gdh_GetAttributeCharacteristics( name, &attr_type, &attr_size,
		&attr_offset, &attr_dimension);
  if ( EVEN(sts)) {
    // Try volume strict name
    sts = gdh_AttrrefToName( &ip->Pointed, name, sizeof(name), 
          cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    sts = gdh_GetAttributeCharacteristics( name, &attr_type, &attr_size,
		&attr_offset, &attr_dimension);
    if ( EVEN(sts)) return sts;
  }

  switch( attr_type)
  {
    case pwr_eType_Float32:
      sprintf( filename, "pwr_exe:pwr_t_float32.pwg");
      break;
    case pwr_eType_Boolean:
      sprintf( filename, "pwr_exe:pwr_t_boolean.pwg");
      break;
    default:
      return XNAV__SUCCESS;
  }
  sprintf( cmd, "open graph %s/inst=%s/name=\"%s\"", filename, name, name);
  ((XNav *)ip->EditorContext)->command( cmd);
  return XNAV__SUCCESS;
}


// Open type graph filter
static pwr_tStatus OpenTypeGraphFilter( xmenu_sMenuCall *ip)
{
  int		sts;
  char		name[120];
  pwr_tTypeId   attr_type;
  pwr_tUInt32   attr_size;
  pwr_tUInt32   attr_offset;
  pwr_tUInt32   attr_dimension;

  sts = gdh_AttrrefToName( &ip->Pointed, name, sizeof(name), cdh_mNName);
  if ( EVEN(sts)) return sts;

  sts = gdh_GetAttributeCharacteristics( name, &attr_type, &attr_size,
		&attr_offset, &attr_dimension);
  if ( EVEN(sts)) {
    // Try volume strict name
    sts = gdh_AttrrefToName( &ip->Pointed, name, sizeof(name),
          cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    sts = gdh_GetAttributeCharacteristics( name, &attr_type, &attr_size,
		&attr_offset, &attr_dimension);
    if ( EVEN(sts)) return sts;
  }

  switch ( attr_type) {
    case pwr_eType_Float32:
    case pwr_eType_Boolean:
      return XNAV__SUCCESS;
      break;
    default:
      return XNAV__INVISIBLE;
  }
  return XNAV__SUCCESS;
}

// Open referenced object for pwr_tObjid and pwr_sAttrRef attributes
static pwr_tStatus RefOpenObject( xmenu_sMenuCall *ip)
{
  int		sts;
  char		name[120];
  pwr_tTypeId   attr_type;
  pwr_tUInt32   attr_size;
  pwr_tUInt32   attr_offset;
  pwr_tUInt32   attr_dimension;

  sts = gdh_AttrrefToName( &ip->Pointed, name, sizeof(name), cdh_mNName);
  if ( EVEN(sts)) return sts;

  sts = gdh_GetAttributeCharacteristics( name, &attr_type, &attr_size,
		&attr_offset, &attr_dimension);
  if ( EVEN(sts)) {
    // Try volume strict name
    sts = gdh_AttrrefToName( &ip->Pointed, name, sizeof(name), 
          cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    sts = gdh_GetAttributeCharacteristics( name, &attr_type, &attr_size,
		&attr_offset, &attr_dimension);
    if ( EVEN(sts)) return sts;
  }

  switch( attr_type)
  {
    case pwr_eType_Objid:
    {
      pwr_tObjid objid;

      sts = gdh_GetObjectInfoAttrref( &ip->Pointed, (void *)&objid, 
				      sizeof(objid));
      if ( EVEN(sts)) return sts;
      if ( cdh_ObjidIsNull( objid))
        return 1;

      ((XNav *)ip->EditorContext)->open_object( objid);
      break;
    }
    case pwr_eType_AttrRef:
    {
      pwr_sAttrRef attrref;

      sts = gdh_GetObjectInfoAttrref( &ip->Pointed, (void *)&attrref, 
				      sizeof(attrref));
      if ( EVEN(sts)) return sts;
      if ( cdh_ObjidIsNull( attrref.Objid))
        return 1;

      ((XNav *)ip->EditorContext)->open_object( attrref.Objid);
      break;
    }
    default:
      ;
  }
  return XNAV__SUCCESS;
}

// Find referenced object for pwr_tObjid and pwr_sAttrRef attributes in xtt
static pwr_tStatus RefRtNavigator( xmenu_sMenuCall *ip)
{
  int		sts;
  char		name[120];
  pwr_tTypeId   attr_type;
  pwr_tUInt32   attr_size;
  pwr_tUInt32   attr_offset;
  pwr_tUInt32   attr_dimension;

  sts = gdh_AttrrefToName( &ip->Pointed, name, sizeof(name), cdh_mNName);
  if ( EVEN(sts)) return sts;

  sts = gdh_GetAttributeCharacteristics( name, &attr_type, &attr_size,
		&attr_offset, &attr_dimension);
  if ( EVEN(sts)) {
    // Try volume strict name
    sts = gdh_AttrrefToName( &ip->Pointed, name, sizeof(name), 
          cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    sts = gdh_GetAttributeCharacteristics( name, &attr_type, &attr_size,
		&attr_offset, &attr_dimension);
    if ( EVEN(sts)) return sts;
  }

  switch( attr_type)
  {
    case pwr_eType_Objid:
    {
      pwr_tObjid objid;

      sts = gdh_GetObjectInfoAttrref( &ip->Pointed, (void *)&objid, 
				      sizeof(objid));
      if ( EVEN(sts)) return sts;
      if ( cdh_ObjidIsNull( objid))
        return 1;

      ((XNav *)ip->EditorContext)->display_object( objid, 0);
      if ( ip->Caller != xmenu_mUtility_XNav)
        ((XNav *)ip->EditorContext)->pop();
      break;
    }
    case pwr_eType_AttrRef:
    {
      pwr_sAttrRef attrref;

      sts = gdh_GetObjectInfoAttrref( &ip->Pointed, (void *)&attrref, 
				      sizeof(attrref));
      if ( EVEN(sts)) return sts;
      if ( cdh_ObjidIsNull( attrref.Objid))
        return 1;

      ((XNav *)ip->EditorContext)->display_object( attrref.Objid, 0);
      if ( ip->Caller != xmenu_mUtility_XNav)
        ((XNav *)ip->EditorContext)->pop();
      break;
    }
    default:
      ;
  }
  return XNAV__SUCCESS;
}

// Open an URL attribute
static pwr_tStatus OpenURL( xmenu_sMenuCall *ip)
{
  int		sts;
  char		name[120];
  pwr_tTypeId   attr_type;
  pwr_tUInt32   attr_size;
  pwr_tUInt32   attr_offset;
  pwr_tUInt32   attr_dimension;
  pwr_tURL      url;

  sts = gdh_AttrrefToName( &ip->Pointed, name, sizeof(name), cdh_mNName);
  if ( EVEN(sts)) return sts;

  sts = gdh_GetAttributeCharacteristics( name, &attr_type, &attr_size,
		&attr_offset, &attr_dimension);
  if ( EVEN(sts)) {
    // Try volume strict name
    sts = gdh_AttrrefToName( &ip->Pointed, name, sizeof(name), 
          cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    sts = gdh_GetAttributeCharacteristics( name, &attr_type, &attr_size,
		&attr_offset, &attr_dimension);
    if ( EVEN(sts)) return sts;
  }

  switch( attr_type)
  {
    case pwr_eType_String:
    {
      if ( attr_size == sizeof( pwr_tURL)) {
        sts = gdh_GetObjectInfoAttrref( &ip->Pointed, (void *)url, 
				      sizeof(url));
        if ( EVEN(sts)) return sts;
        if ( strcmp( url, "") == 0)
          return 1;

        xnav_open_URL( url);
      }
      break;
    }
    default:
      ;
  }
  return XNAV__SUCCESS;
}

// Check if attribute is of type pwr_tObjid and pwr_sAttrRef attributes
static pwr_tStatus IsRefAttribute( xmenu_sMenuCall *ip)
{
  int		sts;
  char		name[120];
  pwr_tTypeId   attr_type;
  pwr_tUInt32   attr_size;
  pwr_tUInt32   attr_offset;
  pwr_tUInt32   attr_dimension;

  sts = gdh_AttrrefToName( &ip->Pointed, name, sizeof(name), cdh_mNName);
  if ( EVEN(sts)) return sts;

  sts = gdh_GetAttributeCharacteristics( name, &attr_type, &attr_size,
		&attr_offset, &attr_dimension);
  if ( EVEN(sts)) {
    // Try volume strict name
    sts = gdh_AttrrefToName( &ip->Pointed, name, sizeof(name), 
          cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    sts = gdh_GetAttributeCharacteristics( name, &attr_type, &attr_size,
		&attr_offset, &attr_dimension);
    if ( EVEN(sts)) return sts;
  }

  switch( attr_type)
  {
    case pwr_eType_Objid:
    case pwr_eType_AttrRef:
      return XNAV__SUCCESS;
    default:
      ;
  }
  return XNAV__INVISIBLE;
}

// Check if attribute is of type pwr_tObjid and pwr_sAttrRef attributes
static pwr_tStatus IsURLAttribute( xmenu_sMenuCall *ip)
{
  int		sts;
  char		name[120];
  pwr_tTypeId   attr_type;
  pwr_tUInt32   attr_size;
  pwr_tUInt32   attr_offset;
  pwr_tUInt32   attr_dimension;

  sts = gdh_AttrrefToName( &ip->Pointed, name, sizeof(name), cdh_mNName);
  if ( EVEN(sts)) return sts;

  sts = gdh_GetAttributeCharacteristics( name, &attr_type, &attr_size,
		&attr_offset, &attr_dimension);
  if ( EVEN(sts)) {
    // Try volume strict name
    sts = gdh_AttrrefToName( &ip->Pointed, name, sizeof(name), 
          cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    sts = gdh_GetAttributeCharacteristics( name, &attr_type, &attr_size,
		&attr_offset, &attr_dimension);
    if ( EVEN(sts)) return sts;
  }

  switch( attr_type)
  {
    case pwr_eType_String:
      if ( attr_size == sizeof( pwr_tURL))
        return XNAV__SUCCESS;
      break;
    default:
      ;
  }
  return XNAV__INVISIBLE;
}

/////////////////////////////////////////////////////////////////////
//
//  Crossreference methods
//
/////////////////////////////////////////////////////////////////////

// Open trace
static pwr_tStatus CrrOpenTrace( xmenu_sMenuCall *ip)
{
  ((XNav *)ip->EditorContext)->start_trace( ip->Pointed.Objid, ip->Arg);
  return XNAV__SUCCESS;
}


// Open type graph filter
static pwr_tStatus CrrOpenTraceFilter( xmenu_sMenuCall *ip)
{
  // Check if there is any cross references...

  return XNAV__SUCCESS;
}



/*----------------------------------------------------------------------------*\
  Every method to be exported to xtt should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindXttMethods($Object) = {
  pwr_BindXttMethod(OpenObject),
  pwr_BindXttMethod(OpenObjectFilter),
  pwr_BindXttMethod(OpenCrossref),
  pwr_BindXttMethod(OpenCrossrefFilter),
  pwr_BindXttMethod(HistEvent),
  pwr_BindXttMethod(HistEventFilter),
  pwr_BindXttMethod(OpenTrace),
  pwr_BindXttMethod(OpenTraceFilter),
  pwr_BindXttMethod(OpenTrend),
  pwr_BindXttMethod(OpenTrendFilter),
  pwr_BindXttMethod(RtNavigator),
  pwr_BindXttMethod(RtNavigatorFilter),
  pwr_BindXttMethod(OpenClassGraph),
  pwr_BindXttMethod(OpenClassGraphFilter),
  pwr_BindXttMethod(OpenGraph),
  pwr_BindXttMethod(OpenGraphFilter),
  pwr_BindXttMethod(Collect),
  pwr_BindXttMethod(CollectFilter),
  pwr_BindXttMethod(Help),
  pwr_BindXttMethod(HelpFilter),
  pwr_BindXttMethod(DataSheet),
  pwr_BindXttMethod(DataSheetFilter),
  pwr_BindXttMethod(HelpClass),
  pwr_BindXttMethod(HelpClassFilter),
  pwr_BindXttMethod(OpenTypeGraph),
  pwr_BindXttMethod(OpenTypeGraphFilter),
  pwr_BindXttMethod(RefOpenObject),
  pwr_BindXttMethod(RefRtNavigator),
  pwr_BindXttMethod(OpenURL),
  pwr_BindXttMethod(IsRefAttribute),
  pwr_BindXttMethod(IsURLAttribute),
  pwr_BindXttMethod(CrrOpenTrace),
  pwr_BindXttMethod(CrrOpenTraceFilter),
  pwr_NullMethod
};







