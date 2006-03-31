/* 
 * Proview   $Id: wb_c_webhandler.cpp,v 1.1 2006-03-31 14:24:34 claes Exp $
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

/* wb_c_webhandler.cpp -- work bench methods of the WebHandler class. */

#include <X11/Intrinsic.h>
#undef Status
#include "wb_pwrs.h"
#include "pwr_baseclasses.h"
#include "wb_ldh_msg.h"
#include "wb_ldh.h"
#include "ge.h"
#include "wb_pwrb_msg.h"
#include "wb_wsx.h"
#include "wb_wsx_msg.h"
#include "co_cdh.h"
#include "co_dcli.h"

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget.h"
#include "wb_wnav.h"
#include "wb_build.h"
#include "co_msgwindow.h"

static pwr_tStatus Build (
  ldh_sMenuCall *ip
)
{
  WNav *wnav = (WNav *)ip->EditorContext;
  wb_build build( *(wb_session *)ip->PointedSession);

  build.opt = wnav->gbl.build;
  build.webhandler( ip->Pointed.Objid);

  if ( build.sts() == PWRB__NOBUILT)
    wnav->message( 'I', "Nothing to build");
  return build.sts();
}

static pwr_tStatus PostCreate (
  ldh_tSesContext   Session,
  pwr_tOid	    Object,
  pwr_tOid	    Father,
  pwr_tCid	    Class
) {
  pwr_tOid oid;
  pwr_tCid cid;
  pwr_tStatus sts;
  pwr_tInt32 *number;
  pwr_tInt32 max_number = 0;
  int size;

  sts = ldh_ClassNameToId(Session, &cid, "OpPlace");
  if ( EVEN(sts)) return sts;

  // Get next OpNumber
  sts = ldh_GetClassList(Session, cid, &oid);
  while ( ODD(sts)) {

    sts = ldh_GetObjectPar(Session, oid, "RtBody", "OpNumber", (char **)&number,
			   &size);
    if ( EVEN(sts)) return sts;
    if ( *number > max_number)
      max_number = *number;
    free( (char *) number);
    sts = ldh_GetNextObject(Session, oid, &oid);
  }

  sts = ldh_ClassNameToId(Session, &cid, "User");
  if ( EVEN(sts)) return sts;
  sts = ldh_GetClassList(Session, cid, &oid);
  while ( ODD(sts)) {

    sts = ldh_GetObjectPar(Session, oid, "RtBody", "OpNumber", (char **)&number,
			   &size);
    if ( EVEN(sts)) return sts;
    if ( *number > max_number)
      max_number = *number;
    free( (char *) number);
    sts = ldh_GetNextObject(Session, oid, &oid);
  }
  max_number++;

  sts = ldh_CreateObject(Session, &oid, "User", cid, Object, ldh_eDest_IntoLast); 
  if ( EVEN(sts)) return sts;

  sts = ldh_SetObjectPar(Session, oid, "RtBody", "OpNumber", (char *)&max_number,
			 sizeof(max_number));
  if ( EVEN(sts)) return sts;

  sts = ldh_SetObjectPar(Session, Object, "RtBody", "UserObject", (char *)&oid,
			 sizeof(oid));
  if ( EVEN(sts)) return sts;

  return PWRB__SUCCESS;
}

pwr_dExport pwr_BindMethods(WebHandler) = {
  pwr_BindMethod(Build),
  pwr_BindMethod(PostCreate),
  pwr_NullMethod
};

