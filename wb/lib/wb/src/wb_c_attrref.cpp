/* 
 * Proview   $Id: wb_c_attrref.cpp,v 1.3 2005-09-06 10:43:30 claes Exp $
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

/* wb_c_attrref.cpp -- work bench methods of the AttrRef type. */

#include <stdio.h>
#include <string.h>
#include "wb_pwrs.h"
#include "wb_ldh_msg.h"
#include "wb_ldh.h"
#include "wb_dutl.h"
#include "wb_pwrs_msg.h"
#include "co_dcli.h"
#include "co_cdh.h"
#include "co_msg.h"
#include "wb_session.h"


/*----------------------------------------------------------------------------*\
  Is it OK to connect an object ?
\*----------------------------------------------------------------------------*/

static int IsOkConnect (
  ldh_sMenuCall *ip,
  pwr_sMenuButton *mbp
) {
  pwr_tStatus	sts;

  if ( strcmp( mbp->FilterArguments[1], "") != 0) {
    char arg_cid_str[20][32];
    int arg_cid_cnt;
    int i;
    int cid_ok = 0;
    pwr_tCid	cid;
    pwr_tCid	arg_cid;

    // arg 1 holds the allowed class or classes to connect to
    arg_cid_cnt = dcli_parse( mbp->FilterArguments[1], ",", "", (char *) arg_cid_str, 
		sizeof( arg_cid_str) / sizeof( arg_cid_str[0]),
		sizeof( arg_cid_str[0]), 0);


    sts = ldh_GetAttrRefTid( ip->PointedSession, &ip->Selected[0], &cid);
    if ( EVEN(sts)) return 0;

    for ( i = 0; i < arg_cid_cnt; i++) {
      sts = ldh_ClassNameToId( ip->PointedSession, &arg_cid, arg_cid_str[i]);
      if ( EVEN(sts)) return 0;

      if ( cid == arg_cid) {
	cid_ok = 1;
	break;
      }
    }
    if ( !cid_ok)
      return 0;
  }

  return 1;
}

/*----------------------------------------------------------------------------*\
  To connect an object to AttrRef
\*----------------------------------------------------------------------------*/

static pwr_tStatus Connect (
  ldh_sMenuCall *ip
) {
  pwr_tStatus	    	sts;
  pwr_sMenuButton   	mb;
  pwr_sAttrRef	    	PattrRef;
  pwr_sAttrRef 		aref = ip->Selected[0];


  sts = ldh_ReadObjectBody(ip->PointedSession,
    ip->ItemList[ip->ChosenItem].MenuObject,
    "SysBody", &mb, sizeof(pwr_sMenuButton));
  if (EVEN(sts)) return sts;

  if ( strcmp( mb.MethodArguments[1], "") != 0) {
    char arg_cid_str[20][32];
    int arg_cid_cnt;
    int i;
    int cid_ok = 0;
    pwr_tCid		cid;
    pwr_tCid		arg_cid;

    // arg 1 holds the allowed class or classes to connect to
    arg_cid_cnt = dcli_parse( mb.MethodArguments[1], ",", "", (char *) arg_cid_str, 
		sizeof( arg_cid_str) / sizeof( arg_cid_str[0]),
		sizeof( arg_cid_str[0]), 0);


    sts = ldh_GetAttrRefTid( ip->PointedSession, &ip->Selected[0], &cid);
    if ( EVEN(sts)) return 0;

    for ( i = 0; i < arg_cid_cnt; i++) {
      sts = ldh_ClassNameToId( ip->PointedSession, &arg_cid, arg_cid_str[i]);
      if ( EVEN(sts)) return 0;

      if ( cid == arg_cid) {
	cid_ok = 1;
	break;
      }
    }
    if ( !cid_ok)
      return 0;
  }

  if ( strcmp( mb.MethodArguments[2], "") != 0)
    // Body in MethodArguments 2, probably DevBody
    sts = ldh_SetObjectPar( ip->PointedSession, ip->Pointed.Objid, mb.MethodArguments[2],
		mb.MethodArguments[0], (char *) &ip->Selected[0], sizeof(ip->Selected[0]));
  else {
    char *name_p;
    char name[200];
    int size;
    pwr_tCid cid;

    // If Selected is a Template object in a classvolume, replace with 
    // symbol $IoConnect
    if ( cdh_IsClassVolume( aref.Objid.vid)) {
      sts = ldh_ObjidToName( ip->PointedSession, aref.Objid, ldh_eName_Object, 
			     name, sizeof(name), &size);
      if ( ODD(sts) &&  strcmp( name, "Template") == 0) {
	sts = ldh_GetObjectClass( ip->PointedSession, aref.Objid, &cid);
	if ( EVEN(sts)) return sts;

	aref.Objid.vid = ldh_cIoConnectVolume;
	aref.Objid.oix = cid;
      }
    }

    // Assume RtBody or SysBody
    sts = ldh_AttrRefToName(ip->PointedSession, &ip->Pointed, ldh_eName_VolPath,
			    &name_p, &size);
    if ( ODD(sts)) {
      strcpy( name, name_p);
      strcat( name, ".");
      strcat( name, mb.MethodArguments[0]);

      sts = ldh_NameToAttrRef( ip->PointedSession, name, &PattrRef);
      if (ODD(sts))
	sts = ldh_WriteAttribute(ip->PointedSession, &PattrRef, &aref,
			     sizeof(aref));
    }
  }
  if ( ip->message_cb) {
    char msg[200];
    
    if ( ODD(sts)) {
      char name[120];
      char *name_p;
      int len;

      sts = ldh_AttrRefToName( ip->PointedSession, &aref, ldh_eName_Hierarchy, 
			     &name_p, &len);
      if ( EVEN(sts))
	cdh_ObjidToString( name, aref.Objid, 1);
      else
	strcpy( name, name_p);
      sprintf( msg, "%s connected to:   %s", mb.MethodArguments[0], name);
      (ip->message_cb)( ip->EditorContext, 'I', msg);
    }
    else {
      msg_GetMsg( sts, msg, sizeof(msg));
      (ip->message_cb)( ip->EditorContext, 'E', msg);
    }
  }
  return PWRS__SUCCESS;
}

static int IoConnectFilter (
  ldh_sMenuCall *ip,
  pwr_sMenuButton *mbp
) {
  pwr_tStatus	sts;

  if ( strcmp( mbp->FilterArguments[0], "") != 0) {
    char arg_cid_str[20][32];
    int arg_cid_cnt;
    int i;
    int cid_ok = 0;
    pwr_tCid	cid;
    pwr_tCid	arg_cid;

    // arg 0 holds the allowed class or classes to connect to
    arg_cid_cnt = dcli_parse( mbp->FilterArguments[0], ",", "", (char *) arg_cid_str, 
		sizeof( arg_cid_str) / sizeof( arg_cid_str[0]),
		sizeof( arg_cid_str[0]), 0);


    sts = ldh_GetAttrRefTid( ip->PointedSession, &ip->Selected[0], &cid);
    if ( EVEN(sts)) return 0;

    for ( i = 0; i < arg_cid_cnt; i++) {
      sts = ldh_ClassNameToId( ip->PointedSession, &arg_cid, arg_cid_str[i]);
      if ( EVEN(sts)) return 0;

      if ( cid == arg_cid) {
	cid_ok = 1;
	break;
      }
    }
    if ( !cid_ok)
      return 0;
  }

  return 1;
}

/*----------------------------------------------------------------------------*\
  To connect an object to AttrRef
\*----------------------------------------------------------------------------*/

#if 0
static pwr_tStatus replaceAttr( wb_session *sp, wb_attribute& a,
				pwr_sAttrRef *target)
{
  printf( "%s\n", a.attrName());

  switch( a.type()) {
  case pwr_eType_AttrRef: {
    pwr_sAttrRef *value = (pwr_sAttrRef *)a.value();
    pwr_sAttrRef aref = *value;

    if ( aref.Objid.vid == ldh_cIoConnectVolume) {
      aref.Objid = target->Objid;

      wb_attribute va = sp->attribute( &aref);
      printf( "Replacing %s\n", va.attrName());

      sp->writeAttribute( a, &aref, sizeof(aref));
    }
    break;
  }
  default: ;
  }
  return LDH__SUCCESS;
}

static pwr_tStatus replaceClass( wb_session *sp, wb_attribute& oa, 
				 pwr_sAttrRef *target)
{
  for ( int i = 0; i < oa.nElement(); i++) {
    for ( wb_attribute a = oa.first(i); a; a = a.after()) {
      if ( a.isClass())
	replaceClass( sp, a, target);
      else
	replaceAttr( sp, a, target);
    }
  }
  return LDH__SUCCESS;
}
#endif

static pwr_tStatus IoConnect (
  ldh_sMenuCall *ip
) {
  pwr_tStatus	    	sts;
  pwr_sMenuButton   	mb;
  pwr_sAttrRef 		aref = ip->Selected[0];
  pwr_sAttrRef	    	PattrRef;
  char			*name_p;
  char			name[120];
  int			size;

  sts = ldh_ReadObjectBody(ip->PointedSession,
    ip->ItemList[ip->ChosenItem].MenuObject,
    "SysBody", &mb, sizeof(pwr_sMenuButton));
  if (EVEN(sts)) return sts;

  if ( strcmp( mb.MethodArguments[0], "") != 0) {
    char arg_cid_str[20][32];
    int arg_cid_cnt;
    int i;
    int cid_ok = 0;
    pwr_tCid		cid;
    pwr_tCid		arg_cid;

    // arg 0 holds the allowed class or classes to connect to
    arg_cid_cnt = dcli_parse( mb.MethodArguments[0], ",", "", (char *) arg_cid_str, 
		sizeof( arg_cid_str) / sizeof( arg_cid_str[0]),
		sizeof( arg_cid_str[0]), 0);


    sts = ldh_GetAttrRefTid( ip->PointedSession, &ip->Selected[0], &cid);
    if ( EVEN(sts)) return 0;

    for ( i = 0; i < arg_cid_cnt; i++) {
      sts = ldh_ClassNameToId( ip->PointedSession, &arg_cid, arg_cid_str[i]);
      if ( EVEN(sts)) return 0;

      if ( cid == arg_cid) {
	cid_ok = 1;
	break;
      }
    }
    if ( !cid_ok)
      return 0;
  }

#if 0
  printf( "Here in IoConnect\n");

  wb_session *sp = (wb_session *)ip->PointedSession;

  wb_object o = sp->object( ip->Pointed.Objid);
  if ( !o) return o.sts();

  wb_attribute oa = sp->attribute( &ip->Pointed);
  if ( !oa) return oa.sts();

  replaceClass( sp, oa, &ip->Selected[0]);
#endif

  sts = ldh_AttrRefToName(ip->PointedSession, &ip->Pointed, ldh_eName_VolPath,
			  &name_p, &size);
  if ( ODD(sts)) {
    strcpy( name, name_p);
    strcat( name, ".IoConnect");

    sts = ldh_NameToAttrRef( ip->PointedSession, name, &PattrRef);
    if (ODD(sts))
      sts = ldh_WriteAttribute(ip->PointedSession, &PattrRef, &aref,
			       sizeof(aref));
  }

  if ( ip->message_cb) {
    char msg[200];
    
    if ( ODD(sts)) {
      char name[120];
      char *name_p;
      int len;

      sts = ldh_AttrRefToName( ip->PointedSession, &aref, ldh_eName_Hierarchy, 
			     &name_p, &len);
      if ( EVEN(sts))
	cdh_ObjidToString( name, aref.Objid, 1);
      else
	strcpy( name, name_p);
      sprintf( msg, "Io connected to:   %s", name);
      (ip->message_cb)( ip->EditorContext, 'I', msg);
    }
    else {
      msg_GetMsg( sts, msg, sizeof(msg));
      (ip->message_cb)( ip->EditorContext, 'E', msg);
    }
  }
  return PWRS__SUCCESS;
}


pwr_dExport pwr_BindMethods($AttrRef) = {
  pwr_BindMethod(Connect),
  pwr_BindMethod(IsOkConnect),
  pwr_BindMethod(IoConnect),
  pwr_BindMethod(IoConnectFilter),
  pwr_NullMethod
};







