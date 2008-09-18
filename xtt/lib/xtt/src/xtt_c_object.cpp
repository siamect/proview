/* 
 * Proview   $Id: xtt_c_object.cpp,v 1.20 2008-09-18 14:58:00 claes Exp $
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

/* xtt_c_object.c -- common xtt methods for objects. */

#include "pwr_baseclasses.h"
#include "flow_std.h"

#include "xtt_menu.h"
#include "xtt_xnav.h"
#include "xtt_url.h"
#include "rt_xnav_msg.h"
#include "rt_gdh.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_wow.h"
#include "co_lng.h"


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
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid)) 
    objar = &ip->Pointed;
  else 
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  ((XNav *)ip->EditorContext)->open_object( objar);
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
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  ((XNav *)ip->EditorContext)->open_crossref( objar);
  return 1;
}

//
// Open crossref filter
//
static pwr_tStatus OpenCrossrefFilter( xmenu_sMenuCall *ip)
{
  pwr_tClassId classid;
  pwr_tStatus sts;
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  sts = gdh_GetObjectClass( objar->Objid, &classid);
  if ( EVEN(sts)) return sts;

  if ( classid == pwr_eClass_PlantHier)
    return XNAV__INVISIBLE;

  return 1;
}

//
// Hist event
//
static pwr_tStatus HistEvent( xmenu_sMenuCall *ip)
{
  char cmd[420];
  pwr_tAName name;
  int sts;
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  sts = gdh_AttrrefToName( objar, name, sizeof(name),
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
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  if ( objar->Flags.b.Object)
    return 1;
  return 0;
}

//
// Open trace
//
static pwr_tStatus OpenTrace( xmenu_sMenuCall *ip)
{
  pwr_tAName name;
  pwr_tCmd cmd;
  int sts;
  pwr_tOid parent;
  pwr_tOid oid;
  pwr_tOName parent_name;
  bool in_plc = false;
  pwr_tCid classid;
  pwr_sAttrRef plcconnect;
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  // Check if object reside in plc
  for ( sts = gdh_GetParent( objar->Objid, &parent);
	ODD(sts);
	sts = gdh_GetParent( parent, &parent)) {
    gdh_GetObjectClass( parent, &classid);
    if ( classid == pwr_cClass_plc) {
      in_plc = true;
      oid = objar->Objid;
      gdh_GetParent( oid, &parent);
      break;
    }
  }
  if ( !in_plc) {
    sts = gdh_AttrrefToName( objar, name, sizeof(name),
			   cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;
    strcat( name, ".PlcConnect");
    sts = gdh_GetObjectInfo( name, (void *)&plcconnect, sizeof(plcconnect));
    if ( EVEN(sts)) return sts;
	 
    oid = plcconnect.Objid;
    sts = gdh_GetParent( oid, &parent);
    if ( EVEN(sts)) return sts;
  }

  sts = gdh_ObjidToName( parent, parent_name, sizeof(parent_name),
			cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;
  sts = gdh_ObjidToName( oid, name, sizeof(name),
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
  pwr_sAttrRef plcconnect;
  pwr_tAName name;
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  if ( ip->Caller == xmenu_mUtility_Trace ||
       ip->Caller == xmenu_mUtility_Simulate)
    return XNAV__INVISIBLE;

  for ( sts = gdh_GetParent( objar->Objid, &parent);
	ODD(sts);
	sts = gdh_GetParent( parent, &parent)) {
    gdh_GetObjectClass( parent, &classid);
    if ( classid == pwr_cClass_plc)
      return XNAV__SUCCESS;
  }

  // Try PlcConnect attribute
  sts = gdh_AttrrefToName( objar, name, sizeof(name),
			   cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;
  strcat( name, ".PlcConnect");
  sts = gdh_GetObjectInfo( name, (void *)&plcconnect, sizeof(plcconnect));
  if ( ODD(sts) && cdh_ObjidIsNotNull( plcconnect.Objid)) {
    for ( sts = gdh_GetParent( plcconnect.Objid, &parent);
	  ODD(sts);
	  sts = gdh_GetParent( parent, &parent)) {
      gdh_GetObjectClass( parent, &classid);
      if ( classid == pwr_cClass_plc)
	return XNAV__SUCCESS;
    }
  }

  return XNAV__INVISIBLE;
}

//
// Open trend
//
static pwr_tStatus OpenTrend( xmenu_sMenuCall *ip)
{
  pwr_tAName name;
  char cmd[800];
  int sts;
  pwr_tObjid child;
  pwr_tClassId classid;
  int found;
  pwr_sAttrRef deftrend;
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  sts = gdh_GetAttrRefTid( objar, &classid);
  if ( EVEN(sts)) return sts;

  if ( classid == pwr_cClass_DsTrend || classid == pwr_cClass_PlotGroup) {
    sts = gdh_AttrrefToName( &ip->Pointed, name, sizeof(name),
			cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    // Open trend
    sprintf( cmd, "open trend /name=%s /title=\"%s\"", name, name);
    ((XNav *)ip->EditorContext)->command( cmd);
    return 1;
  }

  // Look for attribute DefTrend
  sts = gdh_AttrrefToName( objar, name, sizeof(name),
			cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;

  strcat( name, ".DefTrend");
  sts = gdh_GetObjectInfo( name, (void *)&deftrend, sizeof(deftrend));
  if ( ODD(sts) && cdh_ObjidIsNotNull( deftrend.Objid)) {
    // Default XttGraph found
    sts = gdh_GetAttrRefTid( &deftrend, &classid);
    if ( ODD(sts) &&
         (classid == pwr_cClass_DsTrend || classid == pwr_cClass_PlotGroup)) {

      sts = gdh_AttrrefToName( &deftrend, name, sizeof(name),
			cdh_mName_volumeStrict);
      if ( EVEN(sts)) return sts;

      // Open trend
      sprintf( cmd, "open trend /name=%s /title=\"%s\"", name, name);
      ((XNav *)ip->EditorContext)->command( cmd);
      return 1;
    }
  }

  // Look for DsTrend as child
  if ( !ip->Pointed.Flags.b.Object)
    return 0;

  found = 0;
  sts = gdh_GetChild( objar->Objid, &child);
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
  pwr_sAttrRef deftrend;
  pwr_tAName name;
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else 
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  sts = gdh_GetAttrRefTid( objar, &classid);
  if ( EVEN(sts)) return sts;

  if ( classid == pwr_cClass_DsTrend || classid == pwr_cClass_PlotGroup) {
    return XNAV__SUCCESS;
  }

  // Check if attribute DefTrend exist
  sts = gdh_AttrrefToName( objar, name, sizeof(name),
			cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;

  strcat( name, ".DefTrend");
  sts = gdh_GetObjectInfo( name, (void *)&deftrend, sizeof(deftrend));
  if ( ODD(sts) && cdh_ObjidIsNotNull( deftrend.Objid)) {
    // Default XttGraph found
    sts = gdh_GetAttrRefTid( &deftrend, &classid);
    if ( ODD(sts) &&
         (classid == pwr_cClass_DsTrend || classid == pwr_cClass_PlotGroup))
      return XNAV__SUCCESS;
  }

  // Check if object has a DsTrend as child
  if ( !objar->Flags.b.Object)
    return XNAV__INVISIBLE;

  sts = gdh_GetChild( objar->Objid, &child);
  while ( ODD(sts)) {
    sts = gdh_GetObjectClass( child, &classid);
    if ( EVEN(sts)) return sts;

    if ( classid == pwr_cClass_DsTrend)
      return XNAV__SUCCESS;

    sts = gdh_GetNextSibling( child, &child);
  }
  return XNAV__INVISIBLE;
}

//
// Open History
//
static pwr_tStatus OpenHistory( xmenu_sMenuCall *ip)
{
  pwr_tAName name;
  char cmd[800];
  int sts;
  pwr_tOid oid, child;
  pwr_tClassId classid;
  int found;
  pwr_sAttrRef defhist;
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  sts = gdh_GetAttrRefTid( objar, &classid);
  if ( EVEN(sts)) return sts;

  if ( classid == pwr_cClass_SevHist || classid == pwr_cClass_PlotGroup) {
    sts = gdh_AttrrefToName( &ip->Pointed, name, sizeof(name),
			cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    // Open hist
    sprintf( cmd, "open history /name=%s /title=\"%s\"", name, name);
    ((XNav *)ip->EditorContext)->command( cmd);
    return 1;
  }

  // Look for attribute DefHistory
  sts = gdh_AttrrefToName( objar, name, sizeof(name),
			cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;

  strcat( name, ".DefHistory");
  sts = gdh_GetObjectInfo( name, (void *)&defhist, sizeof(defhist));
  if ( ODD(sts) && cdh_ObjidIsNotNull( defhist.Objid)) {
    // Default XttGraph found
    sts = gdh_GetAttrRefTid( &defhist, &classid);
    if ( ODD(sts) &&
         (classid == pwr_cClass_SevHist || classid == pwr_cClass_PlotGroup)) {

      sts = gdh_AttrrefToName( &defhist, name, sizeof(name),
			cdh_mName_volumeStrict);
      if ( EVEN(sts)) return sts;

      // Open hist
      sprintf( cmd, "open history /name=%s /title=\"%s\"", name, name);
      ((XNav *)ip->EditorContext)->command( cmd);
      return 1;
    }
  }

  // Look for SevHist as child
  if ( !ip->Pointed.Flags.b.Object)
    return 0;

  found = 0;
  sts = gdh_GetChild( objar->Objid, &oid);
  while ( ODD(sts)) {
    sts = gdh_GetObjectClass( oid, &classid);
    if ( EVEN(sts)) return sts;
    
    if ( classid == pwr_cClass_SevHist) {
      found++;
      if ( found == 1)
	child = oid;
    }      
    sts = gdh_GetNextSibling( oid, &oid);
  }
  if ( !found)
    return 1;

  if ( found == 1) {
    sts = gdh_ObjidToName( child, name, sizeof(name),
			   cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    // Open history
    sprintf( cmd, "open history /name=%s /title=\"%s\"", name, name);
    ((XNav *)ip->EditorContext)->command( cmd);
  }
  else {
    // Open history selection
    sts = gdh_AttrrefToName( &ip->Pointed, name, sizeof(name),
			cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    sprintf( cmd, "open shist /name=%s", name);
    ((XNav *)ip->EditorContext)->command( cmd);
  }
  return 1;
}

// Open History filter
static pwr_tStatus OpenHistoryFilter( xmenu_sMenuCall *ip)
{
  int sts;
  pwr_tObjid child;
  pwr_tClassId classid;
  pwr_sAttrRef defhist;
  pwr_tAName name;
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else 
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  sts = gdh_GetAttrRefTid( objar, &classid);
  if ( EVEN(sts)) return sts;

  if ( classid == pwr_cClass_SevHist || classid == pwr_cClass_PlotGroup) {
    return XNAV__SUCCESS;
  }

  // Check if attribute DefHist exist
  sts = gdh_AttrrefToName( objar, name, sizeof(name),
			cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;

  strcat( name, ".DefHist");
  sts = gdh_GetObjectInfo( name, (void *)&defhist, sizeof(defhist));
  if ( ODD(sts) && cdh_ObjidIsNotNull( defhist.Objid)) {
    // Default XttGraph found
    sts = gdh_GetAttrRefTid( &defhist, &classid);
    if ( ODD(sts) &&
         (classid == pwr_cClass_SevHist || classid == pwr_cClass_PlotGroup))
      return XNAV__SUCCESS;
  }

  // Check if object has a SevHist as child
  if ( !objar->Flags.b.Object)
    return XNAV__INVISIBLE;

  sts = gdh_GetChild( objar->Objid, &child);
  while ( ODD(sts)) {
    sts = gdh_GetObjectClass( child, &classid);
    if ( EVEN(sts)) return sts;

    if ( classid == pwr_cClass_SevHist)
      return XNAV__SUCCESS;

    sts = gdh_GetNextSibling( child, &child);
  }
  return XNAV__INVISIBLE;
}

//
// Open Fast
//
static pwr_tStatus OpenFast( xmenu_sMenuCall *ip)
{
  pwr_tAName name;
  char cmd[800];
  int sts;
  pwr_tObjid child;
  pwr_tClassId classid;
  int found;
  pwr_sAttrRef deffast;
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  sts = gdh_GetAttrRefTid( objar, &classid);
  if ( EVEN(sts)) return sts;

  if ( classid == pwr_cClass_DsFastCurve) {
    sts = gdh_AttrrefToName( objar, name, sizeof(name),
			cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    // Open fast
    sprintf( cmd, "open fast /name=%s /title=\"%s\"", name, name);
    ((XNav *)ip->EditorContext)->command( cmd);
    return 1;
  }

  // Look for attribute DefFast
  sts = gdh_AttrrefToName( objar, name, sizeof(name),
			cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;

  strcat( name, ".DefFast");
  sts = gdh_GetObjectInfo( name, (void *)&deffast, sizeof(deffast));
  if ( ODD(sts) && cdh_ObjidIsNotNull( deffast.Objid)) {
    // Default XttGraph found
    sts = gdh_GetAttrRefTid( &deffast, &classid);
    if ( ODD(sts) &&
         classid == pwr_cClass_DsFastCurve) {

      sts = gdh_AttrrefToName( &deffast, name, sizeof(name),
			cdh_mName_volumeStrict);
      if ( EVEN(sts)) return sts;

      // Open fast
      sprintf( cmd, "open fast /name=%s /title=\"%s\"", name, name);
      ((XNav *)ip->EditorContext)->command( cmd);
      return 1;
    }
  }

  // Look for DsFastCurve as child
  if ( !ip->Pointed.Flags.b.Object)
    return 0;

  found = 0;
  sts = gdh_GetChild( objar->Objid, &child);
  while ( ODD(sts)) {
    sts = gdh_GetObjectClass( child, &classid);
    if ( EVEN(sts)) return sts;
    
    if ( classid == pwr_cClass_DsFastCurve) {
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

  // Open fast
  sprintf( cmd, "open fast /name=%s /title=\"%s\"", name, name);
  ((XNav *)ip->EditorContext)->command( cmd);
 
  return 1;
}

// Open fast filter
static pwr_tStatus OpenFastFilter( xmenu_sMenuCall *ip)
{
  int sts;
  pwr_tObjid child;
  pwr_tClassId classid;
  pwr_sAttrRef deffast;
  pwr_tAName name;
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  sts = gdh_GetAttrRefTid( objar, &classid);
  if ( EVEN(sts)) return sts;

  if ( classid == pwr_cClass_DsFastCurve)
    return XNAV__SUCCESS;

  // Check if attribute DefFast exist
  sts = gdh_AttrrefToName( objar, name, sizeof(name),
			cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;

  strcat( name, ".DefFast");
  sts = gdh_GetObjectInfo( name, (void *)&deffast, sizeof(deffast));
  if ( ODD(sts) && cdh_ObjidIsNotNull( deffast.Objid)) {
    // Default XttGraph found
    sts = gdh_GetAttrRefTid( &deffast, &classid);
    if ( ODD(sts) &&
         classid == pwr_cClass_DsFastCurve)
      return XNAV__SUCCESS;
  }

  // Check if object has a DsFastCurve as child
  if ( !objar->Flags.b.Object)
    return XNAV__INVISIBLE;

  sts = gdh_GetChild( objar->Objid, &child);
  while ( ODD(sts)) {
    sts = gdh_GetObjectClass( child, &classid);
    if ( EVEN(sts)) return sts;

    if ( classid == pwr_cClass_DsFastCurve)
      return XNAV__SUCCESS;

    sts = gdh_GetNextSibling( child, &child);
  }
  return XNAV__INVISIBLE;
}

// Open runtime navigator object
static pwr_tStatus RtNavigator( xmenu_sMenuCall *ip)
{
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  ((XNav *)ip->EditorContext)->display_object( objar, 0);
  ((XNav *)ip->EditorContext)->pop();

  return 1;
}

// Open runtime navigator filter
static pwr_tStatus RtNavigatorFilter( xmenu_sMenuCall *ip)
{

  if ( ip->Caller == xmenu_mUtility_XNav &&
       cdh_ObjidIsEqual( ip->Pointed.Objid,
			 ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    return XNAV__INVISIBLE;
  return XNAV__SUCCESS;
}

// Open class graph
static pwr_tStatus OpenObjectGraph( xmenu_sMenuCall *ip)
{
  int		sts;
  pwr_tAName   	name;
  char     	cmd[800];
  pwr_sAttrRef  aref;
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  sts = gdh_AttrrefToName( objar,
		  name, sizeof(name), cdh_mNName);
  if ( EVEN(sts)) return sts;

  // Check if object is mounted with other name
  sts = gdh_NameToAttrref( pwr_cNObjid, name, &aref);
  if ( EVEN(sts)) {
    sts = gdh_AttrrefToName( objar,
		  name, sizeof(name), cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;
  }
  sprintf( cmd, "open graph/class/inst=%s/name=\"%s\"", name, name);

  ((XNav *)ip->EditorContext)->command( cmd);

  return XNAV__SUCCESS;
}

// Open object graph filter
static pwr_tStatus OpenObjectGraphFilter( xmenu_sMenuCall *ip)
{
  int		sts;
  pwr_tClassId	classid;
  pwr_tObjName 	classname;
  pwr_tFileName	fname;
  pwr_tFileName	found_file;
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  for ( sts = gdh_GetAttrRefTid( objar, &classid);
	ODD(sts);
	sts = gdh_GetSuperClass( classid, &classid, pwr_cNObjid)) {

    sts = gdh_ObjidToName( cdh_ClassIdToObjid( classid),
			   classname, sizeof(classname), cdh_mName_object);
    if ( EVEN(sts)) return sts;
    cdh_ToLower( classname, classname);

    if ( classname[0] == '$')
      sprintf( fname, "$pwr_exe/pwr_c_%s.pwg", &classname[1]);
    else
      sprintf( fname, "$pwr_exe/pwr_c_%s.pwg", classname);
    sts = dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_INIT);
    dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_END);
    if ( EVEN(sts)) {
      sprintf( fname, "$pwrp_exe/%s.pwg", classname);
      sts = dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_INIT);
      dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_END);
    }
    if ( ODD(sts))
      return XNAV__SUCCESS;
  }
  return XNAV__INVISIBLE;
}

// Open Graph
static pwr_tStatus OpenGraph( xmenu_sMenuCall *ip)
{
  int sts;
  pwr_tAName name;
  pwr_sAttrRef defgraph;
  pwr_sAttrRef aref;
  pwr_tObjid objid;
  pwr_tClassId classid;
  char cmd[400];
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  aref = *objar;
  sts = XNAV__SUCCESS;
  while( ODD(sts)) {
    sts = gdh_AttrrefToName( &aref, name, sizeof(name),
			cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    strcat( name, ".DefGraph");
    sts = gdh_GetObjectInfo( name, (void *)&defgraph, sizeof(defgraph));
    if ( ODD(sts) && cdh_ObjidIsNotNull( defgraph.Objid)) {
      // Default XttGraph found
      sts = gdh_GetAttrRefTid( &defgraph, &classid);
      if ( EVEN(sts)) return sts;

      if ( classid == pwr_cClass_XttGraph) {
        printf( "DefGraph found\n");
 
        sts = gdh_AttrrefToName( &defgraph, name, sizeof(name), cdh_mNName);
        strcpy( cmd, "ope gra/obj=");
        strcat( cmd, name);
        sts = ((XNav *)ip->EditorContext)->command( cmd);
        break;
      }
    }

    if ( !objar->Flags.b.Object)
      break;

    sts = gdh_GetParent( aref.Objid, &objid);
    aref = cdh_ObjidToAref( objid);
  }

  return XNAV__SUCCESS;
}

// Open Graph filter
static pwr_tStatus OpenGraphFilter( xmenu_sMenuCall *ip)
{
  int sts;
  pwr_tObjid objid;
  pwr_tAName name;
  pwr_sAttrRef defgraph;
  pwr_sAttrRef aref;
  pwr_tClassId classid;
  char action[80];
  char *s;
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  aref = *objar;
  sts = XNAV__SUCCESS;
  while( ODD(sts)) {
    sts = gdh_AttrrefToName( &aref, name, sizeof(name),
			     cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    strcat( name, ".DefGraph");
    sts = gdh_GetObjectInfo( name, (void *)&defgraph, sizeof(defgraph));
    if ( ODD(sts) && cdh_ObjidIsNotNull( defgraph.Objid)) {
      // Default XttGraph found
      sts = gdh_GetAttrRefTid( &defgraph, &classid);
      if ( EVEN(sts)) return sts;

      if ( classid == pwr_cClass_XttGraph) {
        if ( ip->Caller == xmenu_mUtility_Ge && ip->Arg) {
          // Check that graph is not the same as caller
          sts = gdh_AttrrefToName( &defgraph, name, sizeof(name),
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
    if ( !objar->Flags.b.Object)
      break;

    sts = gdh_GetParent( aref.Objid, &objid);
    aref = cdh_ObjidToAref( objid);
  }

  return XNAV__INVISIBLE;
}

// Collect
static pwr_tStatus Collect( xmenu_sMenuCall *ip)
{
  int sts;
  pwr_sAttrRef aref;
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  aref = *objar;
  sts = ((XNav *)ip->EditorContext)->collect_insert( &aref);
  if ( EVEN(sts)) {
    ((XNav *)ip->EditorContext)->wow->DisplayError( "Collect error", 
		      XNav::get_message(sts));
  }

  return XNAV__SUCCESS;
}

// Collect filter
static pwr_tStatus CollectFilter( xmenu_sMenuCall *ip)
{
  char attr[80];
  int sts;
  pwr_sAttrRef aref;
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  if ( ip->ItemType == xmenu_eItemType_Object ||
       ip->ItemType == xmenu_eItemType_AttrObject) {
    aref = *objar;
    // Note, get_trace_attr replaces the value in aref for channels
    sts = XNav::get_trace_attr( &aref, attr);
    if ( EVEN(sts))
      return XNAV__INVISIBLE;
  }
  return XNAV__SUCCESS;
}

// Help
static pwr_tStatus Help( xmenu_sMenuCall *ip)
{
  int sts;
  pwr_tAName name;
  pwr_tString40 helptopic;
  pwr_tCmd cmd;
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  sts = gdh_AttrrefToName( objar, name, sizeof(name),
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
  pwr_tAName name;
  pwr_tString40 helptopic;
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  sts = gdh_AttrrefToName( objar, name, sizeof(name),
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
  pwr_tAName name;
  pwr_tURL datasheet;
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  sts = gdh_AttrrefToName( objar, name, sizeof(name),
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
  pwr_tAName name;
  pwr_tURL datasheet;
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  sts = gdh_AttrrefToName( objar, name, sizeof(name),
			cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;

  strcat( name, ".DataSheet");
  sts = gdh_GetObjectInfo( name, (void *)datasheet, sizeof(datasheet));
  if ( EVEN(sts)) return XNAV__INVISIBLE;

  if ( strcmp( datasheet, "") == 0)
    return XNAV__INVISIBLE;

  return XNAV__SUCCESS;
}

// Photo
static pwr_tStatus Photo( xmenu_sMenuCall *ip)
{
  int sts;
  pwr_tAName name;
  pwr_tURL photo;
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  sts = gdh_AttrrefToName( objar, name, sizeof(name),
			cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;

  strcat( name, ".Photo");
  sts = gdh_GetObjectInfo( name, (void *)photo, sizeof(photo));
  if ( EVEN(sts)) return sts;

  if ( strcmp( (char *)photo, "") == 0)
    return 0;

  if ( strchr( photo, '.'))
    xnav_open_URL( photo);
  else {
    pwr_tCmd cmd;

    sprintf( cmd, "help %s", photo);
    sts = ((XNav *)ip->EditorContext)->command( cmd);
  }
  return XNAV__SUCCESS;
}

// Photo filter
static pwr_tStatus PhotoFilter( xmenu_sMenuCall *ip)
{
  int sts;
  pwr_tAName name;
  pwr_tURL photo;
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  sts = gdh_AttrrefToName( objar, name, sizeof(name),
			cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;

  strcat( name, ".Photo");
  sts = gdh_GetObjectInfo( name, (void *)photo, sizeof(photo));
  if ( EVEN(sts)) return XNAV__INVISIBLE;

  if ( strcmp( photo, "") == 0)
    return XNAV__INVISIBLE;

  return XNAV__SUCCESS;
}

// Open CircuitDiagram
static pwr_tStatus CircuitDiagram( xmenu_sMenuCall *ip)
{
  int sts;
  pwr_tURL circuitdiagram;
  pwr_tAName name;
  pwr_tObjid objid;
  bool is_parent = false;
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  objid = objar->Objid;
  sts = XNAV__SUCCESS;
  while( ODD(sts)) {
    sts = gdh_ObjidToName( objid, name, sizeof(name),
			cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    strcat( name, ".CircuitDiagram");
    sts = gdh_GetObjectInfo( name, (void *)circuitdiagram, sizeof(circuitdiagram));
    if ( ODD(sts) && cdh_NoCaseStrcmp( circuitdiagram, "disabled") == 0)
      break;
    if ( ODD(sts) && strcmp( circuitdiagram, "") != 0) {
      // CircuitDiagram found
      xnav_open_URL( circuitdiagram);
      break;
    }
    if ( !is_parent && EVEN(sts))
      break;

    is_parent = true;
    sts = gdh_GetParent( objid, &objid);
  }

  return XNAV__SUCCESS;
}

// Open CircuitDiagram filter
static pwr_tStatus CircuitDiagramFilter( xmenu_sMenuCall *ip)
{
  int sts;
  pwr_tURL circuitdiagram;
  pwr_tAName name;
  pwr_tObjid objid;
  bool is_parent = false;
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  objid = objar->Objid;
  sts = XNAV__SUCCESS;
  while( ODD(sts)) {
    sts = gdh_ObjidToName( objid, name, sizeof(name),
			cdh_mName_volumeStrict);
    if ( EVEN(sts)) return sts;

    strcat( name, ".CircuitDiagram");
    sts = gdh_GetObjectInfo( name, (void *)circuitdiagram, sizeof(circuitdiagram));
    if ( ODD(sts) && cdh_NoCaseStrcmp( circuitdiagram, "disabled") == 0)
      break;
    if ( ODD(sts) && strcmp( circuitdiagram, "") != 0) {
      // CircuitDiagram found
      return XNAV__SUCCESS;
    }
    if ( !is_parent && EVEN(sts))
      break;

    is_parent = true;
    sts = gdh_GetParent( objid, &objid);
  }

  return XNAV__INVISIBLE;
}

// Note
static pwr_tStatus Note( xmenu_sMenuCall *ip)
{
  int sts;
  pwr_tAName name;
  char cmd[420];
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  sts = gdh_AttrrefToName( objar, name, sizeof(name),
			cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;

  sprintf( cmd, "open attr/bypass/name=%s.Note", name);
    sts = ((XNav *)ip->EditorContext)->command( cmd);
  // xnav_edit_attribute( name, value);
  return XNAV__SUCCESS;
}

// Note filter
static pwr_tStatus NoteFilter( xmenu_sMenuCall *ip)
{
  int sts;
  pwr_tAName name;
  pwr_tURL datasheet;
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  sts = gdh_AttrrefToName( objar, name, sizeof(name),
			cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;

  strcat( name, ".Note");
  sts = gdh_GetObjectInfo( name, (void *)datasheet, sizeof(datasheet));
  if ( EVEN(sts)) return XNAV__INVISIBLE;

  return XNAV__SUCCESS;
}

// Block Events
static pwr_tStatus BlockEvents( xmenu_sMenuCall *ip)
{
  char cmd[420];
  pwr_tOName name;
  pwr_tStatus sts;

  sts = gdh_ObjidToName( ip->Pointed.Objid, name, sizeof(name),
			 cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;

  sprintf( cmd, "event block/name=%s", name);
  sts = ((XNav *)ip->EditorContext)->command( cmd);

  return XNAV__SUCCESS;
}

// Block Events Filter
static pwr_tStatus BlockEventsFilter( xmenu_sMenuCall *ip)
{
  return XNAV__SUCCESS;
}

// Help Class
static pwr_tStatus HelpClass( xmenu_sMenuCall *ip)
{
  int sts;
  pwr_tClassId classid;
  pwr_tVid vid;
  pwr_tObjName vname;
  pwr_tObjName classname;
  pwr_tCmd cmd;
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  sts = gdh_GetAttrRefTid( objar, &classid);
  if ( EVEN(sts)) return sts;

  sts = gdh_ObjidToName( cdh_ClassIdToObjid( classid),
		  classname, sizeof(classname), cdh_mName_object);
  if ( EVEN(sts)) return sts;

  vid = cdh_CidToVid(classid);
  if ( cdh_cManufactClassVolMin <= vid && vid <= cdh_cManufactClassVolMax) {
    /* Get help file for this volume */
    sts = gdh_VolumeIdToName( vid, vname, sizeof(vname));
    if ( EVEN(sts)) return sts;

    cdh_ToLower( vname, vname);
    sprintf( cmd, "help %s /helpfile=\"$pwr_exe/%s/%s_xtthelp.dat\"", classname, 
	     Lng::get_language_str(), vname);

    sts = ((XNav *)ip->EditorContext)->command( cmd);
    return XNAV__SUCCESS;
  }

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
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  if ( ip->Caller == xmenu_mUtility_Ge) {
    sts = gdh_GetAttrRefTid( objar, &classid);
    if ( EVEN(sts)) return sts;

    if ( classid == pwr_eClass_PlantHier)
      return XNAV__INVISIBLE;
  }
  return XNAV__SUCCESS;
}

// Simulate
static pwr_tStatus Simulate( xmenu_sMenuCall *ip)
{
  int sts;
  pwr_tAName name;
  pwr_sAttrRef simconnect;
  pwr_sAttrRef aref;
  char cmd[800];
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  sts = gdh_AttrrefToName( objar, name, sizeof(name),
			     cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;

  strcat( name, ".SimConnect");
  sts = gdh_GetObjectInfo( name, (void *)&simconnect, sizeof(simconnect));
  if ( EVEN(sts)) {
    // Look for sim graph to main object
    pwr_tClassId classid;
    pwr_tObjName       	classname;
    pwr_tFileName      	fname;
    pwr_tFileName      	found_file;
    char		*s;

    for ( sts = gdh_GetAttrRefTid( objar, &classid);
	  ODD(sts);
	  sts = gdh_GetSuperClass( classid, &classid, pwr_cNObjid)) {
      sts = gdh_ObjidToName( cdh_ClassIdToObjid( classid),
			     classname, sizeof(classname), cdh_mName_object);
      if ( EVEN(sts)) return sts;
      cdh_ToLower( classname, classname);

      if ( classname[0] == '$')
	sprintf( fname, "$pwr_exe/pwr_c_%ssim.pwg", &classname[1]);
      else
	sprintf( fname, "$pwr_exe/pwr_c_%ssim.pwg", classname);
      sts = dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_INIT);
      dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_END);
      if ( EVEN(sts)) {
	sprintf( fname, "$pwrp_exe/%ssim.pwg", classname);
	sts = dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_INIT);
	dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_END);
      }
      if ( ODD(sts))
	break;
    }
    if ( EVEN(sts)) return sts;

    s = strrchr( fname, '.');
    *s = 0;
    s = strrchr( fname, '/');

    sts = gdh_AttrrefToName( objar,
			     name, sizeof(name), cdh_mNName);
    if ( EVEN(sts)) return sts;

    // Check if object is mounted with other name
    sts = gdh_NameToAttrref( pwr_cNObjid, name, &aref);
    if ( EVEN(sts)) {
      sts = gdh_AttrrefToName( objar,
			       name, sizeof(name), cdh_mName_volumeStrict);
      if ( EVEN(sts)) return sts;
    }

    sprintf( cmd, "open graph %s/inst=%s/name=\"%s\"", s+1, name, name);
  }
  else {
    sts = gdh_AttrrefToName( &simconnect,
			     name, sizeof(name), cdh_mNName);
    if ( EVEN(sts)) return sts;

    // Check if object is mounted with other name
    sts = gdh_NameToAttrref( pwr_cNObjid, name, &aref);
    if ( EVEN(sts)) {
      sts = gdh_AttrrefToName( &simconnect,
			       name, sizeof(name), cdh_mName_volumeStrict);
      if ( EVEN(sts)) return sts;
    }
    sprintf( cmd, "open graph/class/inst=%s/name=\"%s\"", name, name);
  }
  ((XNav *)ip->EditorContext)->command( cmd);
  return XNAV__SUCCESS;
}

// Simulate filter
static pwr_tStatus SimulateFilter( xmenu_sMenuCall *ip)
{
  int sts;
  pwr_tAName name;
  pwr_sAttrRef simconnect;
  pwr_tClassId classid;
  pwr_tObjName 	classname;
  pwr_tFileName	fname;
  pwr_tFileName found_file;
  pwr_tOid	iohandler;
  pwr_sClass_IOHandler *iohandler_p;
  pwr_sAttrRef *objar;

  if (!ip->ItemList || cdh_ObjidIsNull( ip->ItemList[ip->ChosenItem].CurrentObject.Objid))
    objar = &ip->Pointed;
  else
    objar = &ip->ItemList[ip->ChosenItem].CurrentObject;

  sts = gdh_GetClassList( pwr_cClass_IOHandler, &iohandler);
  if ( EVEN(sts)) return XNAV__INVISIBLE;

  sts = gdh_ObjidToPointer( iohandler, (void **)&iohandler_p);
  if ( EVEN(sts)) return XNAV__INVISIBLE;

  if ( !iohandler_p->IOSimulFlag)
    return XNAV__INVISIBLE;

  sts = gdh_AttrrefToName( objar, name, sizeof(name),
			     cdh_mName_volumeStrict);
  if ( EVEN(sts)) return sts;

  strcat( name, ".SimConnect");
  sts = gdh_GetObjectInfo( name, (void *)&simconnect, sizeof(simconnect));
  if ( EVEN(sts)) {
    // Look for sim graph to main object
    for ( sts = gdh_GetAttrRefTid( objar, &classid);
	  ODD(sts);
	  sts = gdh_GetSuperClass( classid, &classid, pwr_cNObjid)) {

      sts = gdh_ObjidToName( cdh_ClassIdToObjid( classid),
			     classname, sizeof(classname), cdh_mName_object);
      if ( EVEN(sts)) return sts;
      cdh_ToLower( classname, classname);

      if ( classname[0] == '$')
	sprintf( fname, "$pwr_exe/pwr_c_%ssim.pwg", &classname[1]);
      else
	sprintf( fname, "$pwr_exe/pwr_c_%ssim.pwg", classname);
      sts = dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_INIT);
      dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_END);
      if ( EVEN(sts)) {
	sprintf( fname, "$pwrp_exe/%ssim.pwg", classname);
	sts = dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_INIT);
	dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_END);
      }
      if ( ODD(sts))
	return XNAV__SUCCESS;
    }
    return XNAV__INVISIBLE;
  }
  else if ( cdh_ObjidIsNull( simconnect.Objid))
    return XNAV__INVISIBLE;

  // Simconnect found
  sts = gdh_GetAttrRefTid( &simconnect, &classid);
  if ( EVEN(sts)) return XNAV__INVISIBLE;

  sts = gdh_ObjidToName( cdh_ClassIdToObjid( classid),
			 classname, sizeof(classname), cdh_mName_object);
  if ( EVEN(sts)) return sts;
  cdh_ToLower( classname, classname);

  if ( classname[0] == '$')
    sprintf( fname, "$pwr_exe/pwr_c_%s.pwg", &classname[1]);
  else
    sprintf( fname, "$pwr_exe/pwr_c_%s.pwg", classname);
  sts = dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_INIT);
  dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_END);
  if ( EVEN(sts)) {
    sprintf( fname, "$pwrp_exe/%s.pwg", classname);
    sts = dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_INIT);
    dcli_search_file( fname, found_file, DCLI_DIR_SEARCH_END);
  }
  if ( ODD(sts))
    return XNAV__SUCCESS;

  return XNAV__INVISIBLE;
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
  pwr_tAName   	name;
  char     	cmd[800];
  pwr_tTypeId   attr_type;
  pwr_tUInt32   attr_size;
  pwr_tUInt32   attr_offset;
  pwr_tUInt32   attr_dimension;
  pwr_tFileName filename;

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
    case pwr_eType_Int32:
      sprintf( filename, "pwr_exe:pwr_t_int32.pwg");
      break;
    case pwr_eType_UInt32:
      sprintf( filename, "pwr_exe:pwr_t_uint32.pwg");
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
  pwr_tAName   	name;
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
    case pwr_eType_Int32:
    case pwr_eType_UInt32:
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
  pwr_tAName   	name;
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
      pwr_sAttrRef aref = pwr_cNAttrRef;

      sts = gdh_GetObjectInfoAttrref( &ip->Pointed, (void *)&aref.Objid, 
				      sizeof(aref.Objid));
      if ( EVEN(sts)) return sts;
      if ( cdh_ObjidIsNull( aref.Objid))
        return 1;

      ((XNav *)ip->EditorContext)->open_object( &aref);
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

      ((XNav *)ip->EditorContext)->open_object( &attrref);
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
  pwr_tAName   	name;
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

      ((XNav *)ip->EditorContext)->display_object( &attrref, 0);
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
  pwr_tAName   	name;
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
  pwr_tAName   	name;
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
  pwr_tAName   	name;
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


// Open trace filter
static pwr_tStatus CrrOpenTraceFilter( xmenu_sMenuCall *ip)
{
  pwr_tStatus sts;
  pwr_tCid cid;

  sts = gdh_GetObjectClass( ip->Pointed.Objid, &cid);
  if ( ODD(sts) && cid == pwr_cClass_XttGraph)
    return XNAV__INVISIBLE;

  return XNAV__SUCCESS;
}

// Open graph
static pwr_tStatus CrrOpenGraph( xmenu_sMenuCall *ip)
{
  ((XNav *)ip->EditorContext)->exec_xttgraph( ip->Pointed.Objid, 0, 0, 0);
  return XNAV__SUCCESS;
}


// Open graph filter
static pwr_tStatus CrrOpenGraphFilter( xmenu_sMenuCall *ip)
{
  pwr_tStatus sts;
  pwr_tCid cid;

  sts = gdh_GetObjectClass( ip->Pointed.Objid, &cid);
  if ( ODD(sts) && cid == pwr_cClass_XttGraph)
    return XNAV__SUCCESS;

  return XNAV__INVISIBLE;
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
  pwr_BindXttMethod(OpenHistory),
  pwr_BindXttMethod(OpenHistoryFilter),
  pwr_BindXttMethod(OpenFast),
  pwr_BindXttMethod(OpenFastFilter),
  pwr_BindXttMethod(RtNavigator),
  pwr_BindXttMethod(RtNavigatorFilter),
  pwr_BindXttMethod(OpenObjectGraph),
  pwr_BindXttMethod(OpenObjectGraphFilter),
  pwr_BindXttMethod(OpenGraph),
  pwr_BindXttMethod(OpenGraphFilter),
  pwr_BindXttMethod(Collect),
  pwr_BindXttMethod(CollectFilter),
  pwr_BindXttMethod(Help),
  pwr_BindXttMethod(HelpFilter),
  pwr_BindXttMethod(DataSheet),
  pwr_BindXttMethod(DataSheetFilter),
  pwr_BindXttMethod(Photo),
  pwr_BindXttMethod(PhotoFilter),
  pwr_BindXttMethod(CircuitDiagram),
  pwr_BindXttMethod(CircuitDiagramFilter),
  pwr_BindXttMethod(Note),
  pwr_BindXttMethod(NoteFilter),
  pwr_BindXttMethod(BlockEvents),
  pwr_BindXttMethod(BlockEventsFilter),
  pwr_BindXttMethod(HelpClass),
  pwr_BindXttMethod(HelpClassFilter),
  pwr_BindXttMethod(Simulate),
  pwr_BindXttMethod(SimulateFilter),
  pwr_BindXttMethod(OpenTypeGraph),
  pwr_BindXttMethod(OpenTypeGraphFilter),
  pwr_BindXttMethod(RefOpenObject),
  pwr_BindXttMethod(RefRtNavigator),
  pwr_BindXttMethod(OpenURL),
  pwr_BindXttMethod(IsRefAttribute),
  pwr_BindXttMethod(IsURLAttribute),
  pwr_BindXttMethod(CrrOpenTrace),
  pwr_BindXttMethod(CrrOpenTraceFilter),
  pwr_BindXttMethod(CrrOpenGraph),
  pwr_BindXttMethod(CrrOpenGraphFilter),
  pwr_NullMethod
};







