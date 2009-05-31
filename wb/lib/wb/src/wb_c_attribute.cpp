/* 
 * Proview   $Id: wb_c_attribute.cpp,v 1.1 2007-01-04 07:29:02 claes Exp $
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

/* wb_c_attribute.c -- work bench methods of the Attribute class. */

#include <string.h>
#include "pwr.h"
#include "co_api.h"
#include "wb_pwrs.h"
#include "wb_pwrs_msg.h"
#include "wb_ldh.h"
#include "wb_wtt.h"
#include "wb_session.h"
#include "wb_attribute.h"

static pwr_tStatus AnteCreate (
  ldh_tSesContext   Session,
  pwr_tObjid	    Father,
  pwr_tClassId	    Class
) {
  pwr_tCid cid;
  pwr_tStatus sts;

  if ( cdh_ObjidIsNull( Father))
    return PWRS__POSATTRIBUTE;

  // Check that the father is of class ClassDef
  sts = ldh_GetObjectClass( Session, Father, &cid);
  if ( EVEN(sts) || cid != pwr_eClass_ObjBodyDef)
    return PWRS__POSATTRIBUTE;
  return PWRS__SUCCESS;
}

static pwr_tStatus AnteMove (
  ldh_tSesContext   Session,
  pwr_tObjid	    Object,
  pwr_tObjid	    Father,
  pwr_tObjid	    OldFather
) {

  // Check that that the father is the same
  if ( !(Father.oix == OldFather.oix && Father.vid == OldFather.vid))
    return PWRS__MOVEATTRIBUTE;
  return PWRS__SUCCESS;
}

//
// Help for class of an object in wnav
//
static pwr_tStatus HelpClass( ldh_sMenuCall *ip) 
{
  pwr_tStatus sts;
  int size;
  pwr_tObjName cname;
  pwr_tObjName vname;
  pwr_tCmd cmd;
  pwr_tTid tid;
  pwr_tVid vid;
  pwr_tObjName attr;
  pwr_tAName oname;
  char *namep;
  char *s;
  pwr_tAttrRef aref;
  
  sts = ldh_GetAttrRefTid( ip->PointedSession, &ip->Pointed, &tid);
  if ( EVEN(sts)) return sts;

  if ( cdh_tidIsCid( tid)) {
    sts = ldh_ClassIdToName( ip->PointedSession, tid, cname, sizeof(cname), &size);    
    if ( EVEN(sts)) return sts;

    strcpy( attr, "");
  }
  else {
    sts = ldh_AttrRefToName( ip->PointedSession, &ip->Pointed, ldh_eName_VolPath, &namep, &size);
    if ( EVEN(sts)) return sts;

    strcpy( oname, namep);
    if ( (s = strrchr( oname, '.'))) {
      strcpy( attr, s+1);
      *s = 0;
    }
    else
      return 0;

    sts = ldh_NameToAttrRef( ip->PointedSession, oname, &aref);
    if ( EVEN(sts)) return sts;

    sts = ldh_GetAttrRefTid( ip->PointedSession, &aref, &tid);
    if ( EVEN(sts)) return sts;

    if ( !cdh_tidIsCid( tid))
      return 0;

    sts = ldh_ClassIdToName( ip->PointedSession, tid, cname, sizeof(cname), &size);    
    if ( EVEN(sts)) return sts;
  }
    
  if ( (s = strrchr( attr, '[')))
    *s = 0;

  vid = cdh_CidToVid(tid);
  if ( cdh_cManufactClassVolMin <= vid && vid <= cdh_cManufactClassVolMax) {
    /* Get help file for this volume */
    sts = ldh_VolumeIdToName( ldh_SessionToWB(ip->PointedSession), vid, vname, sizeof(vname), &size);
    if ( EVEN(sts)) return sts;

    cdh_ToLower( vname, vname);
    if ( strcmp( attr, "") == 0)
      sprintf( cmd, "help %s /helpfile=\"$pwr_exe/%s/%s_xtthelp.dat\"/strict", cname, 
	       lng_get_language_str(), vname);
    else
      sprintf( cmd, "help %s /helpfile=\"$pwr_exe/%s/%s_xtthelp.dat\"/bookmark=%s/strict", cname, 
	       lng_get_language_str(), vname, attr);

    ip->wnav->command( cmd);
    return 1;
  }

  if ( cname[0] == '$') {
    if ( strcmp( attr, "") == 0)
      sprintf( cmd, "help %s /strict", &cname[1]);
    else
      sprintf( cmd, "help %s /bookmark=%s /strict", &cname[1], attr);
  }
  else {
    if ( strcmp( attr, "") == 0)
      sprintf( cmd, "help %s /strict", cname);
    else
      sprintf( cmd, "help %s /bookmark=%s/strict", cname, attr);
  }

  ip->wnav->command( cmd);
  return 1;
}

static pwr_tStatus HelpClassFilter( ldh_sMenuCall *ip) 
{
  return 1;
}

//
// Set the selected objid or attrref as value
//
static pwr_tStatus InsertSelectedObject( ldh_sMenuCall *ip) 
{
  wb_session *sp = (wb_session *)ip->PointedSession;

  wb_attribute a_pointed = sp->attribute( &ip->Pointed);
  if ( !a_pointed) return a_pointed.sts();

  switch ( a_pointed.tid()) {
  case pwr_eType_Objid:
    sp->writeAttribute( a_pointed, &ip->Selected->Objid);
    if ( EVEN(sp->sts())) return sp->sts();
    break;
  case pwr_eType_AttrRef:
    sp->writeAttribute( a_pointed, ip->Selected);
    if ( EVEN(sp->sts())) return sp->sts();
    break;
  default:
    return 0;
  }

  return 1;
}

static pwr_tStatus InsertSelectedObjectFilter( ldh_sMenuCall *ip) 
{
  wb_session *sp = (wb_session *)ip->PointedSession;

  wb_attribute a_pointed = sp->attribute( &ip->Pointed);
  if ( !a_pointed) return a_pointed.sts();

  switch ( a_pointed.tid()) {
  case pwr_eType_Objid:
  case pwr_eType_AttrRef:
    if ( ip->Pointed.Flags.b.Array)
      return 0;
    break;
  default:
    return 0;
  }

  return 1;
}

//
// Change value of pointed and selected attribute
//
static pwr_tStatus ChangeValue( ldh_sMenuCall *ip) 
{
  ip->wtt->keep_input_open = 1;
  ip->wtt->open_change_value();
  ip->wtt->disable_focus();

  return 1;
}

static pwr_tStatus ChangeValueFilter( ldh_sMenuCall *ip) 
{
  return 1;
}

pwr_dExport pwr_BindMethods($Attribute) = {
  pwr_BindMethod(AnteCreate),
  pwr_BindMethod(AnteMove),
  pwr_BindMethod(HelpClass),
  pwr_BindMethod(HelpClassFilter),
  pwr_BindMethod(InsertSelectedObject),
  pwr_BindMethod(InsertSelectedObjectFilter),
  pwr_BindMethod(ChangeValue),
  pwr_BindMethod(ChangeValueFilter),
  pwr_NullMethod
};





