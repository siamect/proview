/* 
 * Proview   $Id: wb_c_objid.cpp,v 1.1 2007-01-04 07:29:03 claes Exp $
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

/* wb_c_objid.c -- work bench methods of the Objid class. */

#include <stdio.h>
#include <string.h>
#include "wb_pwrs.h"
#include "wb_ldh_msg.h"
#include "wb_ldh.h"
#include "wb_wtt.h"
#include "wb_pwrs_msg.h"
#include "co_dcli.h"
#include "co_cdh.h"
#include "co_msg.h"


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


    sts = ldh_GetObjectClass( ip->PointedSession, ip->Selected[0].Objid, &cid);
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
  To connect an object to Objid
\*----------------------------------------------------------------------------*/

static pwr_tStatus Connect (
  ldh_sMenuCall *ip
) {
  pwr_tStatus	    	sts;
  pwr_sMenuButton   	mb;
  pwr_sAttrRef	    	PattrRef;


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


    sts = ldh_GetObjectClass( ip->PointedSession, ip->Selected[0].Objid, &cid);
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
		mb.MethodArguments[0], (char *) &ip->Selected[0].Objid, sizeof(ip->Selected[0].Objid));
  else {
    // Assume RtBody or SysBody
    char *aname_p;
    pwr_tAName aname;
    int size;

    sts = ldh_AttrRefToName( ip->PointedSession, &ip->Pointed, ldh_eName_ArefVol, &aname_p, &size);
    if ( EVEN(sts)) return 0;
 
    strncpy( aname, aname_p, sizeof(aname));
    strcat( aname, ".");
    strcat( aname, mb.MethodArguments[0]);

    sts = ldh_NameToAttrRef( ip->PointedSession, aname, &PattrRef);
    //sts = ldh_GetAttrRef(ip->PointedSession, ip->Pointed.Objid,
    //		 mb.MethodArguments[0], &PattrRef);
    if (ODD(sts))
      sts = ldh_WriteAttribute(ip->PointedSession, &PattrRef, &ip->Selected[0].Objid,
			     sizeof(pwr_tObjid));
  }
  if ( ip->message_cb) {
    char msg[200];
    
    if ( ODD(sts)) {
      pwr_tOName name;
      int len;

      sts = ldh_ObjidToName( ip->PointedSession, ip->Selected[0].Objid, ldh_eName_Hierarchy, 
			     name, sizeof(name), &len);
      if ( EVEN(sts))
	cdh_ObjidToString( name, ip->Selected[0].Objid, 1);
      sprintf( msg, "%s connected to:   %s", mb.MethodArguments[0], name);
      ip->wtt->message( 'I', msg);
    }
    else {
      msg_GetMsg( sts, msg, sizeof(msg));
      ip->wtt->message( 'E', msg);
    }
  }
  return PWRS__SUCCESS;
}


static int IsOkDisconnect (
  ldh_sMenuCall *ip,
  pwr_sMenuButton *mbp
) {
  pwr_tStatus	    	sts;
  pwr_sAttrRef	    	PattrRef;
  pwr_tOid		oid;

  sts = ldh_GetAttrRef(ip->PointedSession, ip->Pointed.Objid,
		       mbp->FilterArguments[0], &PattrRef);
  if (EVEN(sts)) return sts;

  sts = ldh_ReadAttribute( ip->PointedSession, &PattrRef, &oid, sizeof(pwr_tOid));

  if ( cdh_ObjidIsNull( oid))
    return 0;

  return 1;
}
/*----------------------------------------------------------------------------*\
  To disconnect an objid attribute
\*----------------------------------------------------------------------------*/

static pwr_tStatus Disconnect (
  ldh_sMenuCall *ip
) {
  pwr_tStatus	    	sts;
  pwr_sMenuButton   	mb;
  pwr_sAttrRef	    	PattrRef;
  pwr_tOid		oid;

  sts = ldh_ReadObjectBody(ip->PointedSession,
    ip->ItemList[ip->ChosenItem].MenuObject,
    "SysBody", &mb, sizeof(pwr_sMenuButton));
  if (EVEN(sts)) return sts;

  sts = ldh_GetAttrRef(ip->PointedSession, ip->Pointed.Objid,
		       mb.MethodArguments[0], &PattrRef);
  if (EVEN(sts)) return sts;

  oid = pwr_cNObjid;
  sts = ldh_WriteAttribute( ip->PointedSession, &PattrRef, &oid, sizeof(pwr_tOid));

  return PWRS__SUCCESS;
}

pwr_dExport pwr_BindMethods($Objid) = {
  pwr_BindMethod(Connect),
  pwr_BindMethod(IsOkConnect),
  pwr_BindMethod(Disconnect),
  pwr_BindMethod(IsOkDisconnect),
  pwr_NullMethod
};







