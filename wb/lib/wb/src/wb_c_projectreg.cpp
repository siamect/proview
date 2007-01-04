/* 
 * Proview   $Id: wb_c_projectreg.cpp,v 1.1 2007-01-04 07:29:03 claes Exp $
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

/* wb_c_projectref.c -- work bench methods of the ProjectReg class. */

#include <X11/Intrinsic.h>
#undef Status
#include "co_dcli.h"
#include "wb_pwrs.h"
#include "pwr_baseclasses.h"
#include "wb_ldh_msg.h"
#include "wb_ldh.h"
#include "wb_pwrb_msg.h"
#include "wb_login.h"


/*----------------------------------------------------------------------------*\
  To open a PLC program with the PLC editor.
\*----------------------------------------------------------------------------*/

static pwr_tStatus OpenProject (
  ldh_sMenuCall *ip
)
{
  char *project;
  pwr_tStatus sts;
  int size;
  pwr_tFileName fname;
  pwr_tCmd cmd;

  sts = ldh_GetObjectPar( ip->PointedSession, ip->Pointed.Objid, "RtBody",
			  "Project", &project, &size);
  if ( EVEN(sts)) return sts;

  printf( "Open %s\n", project);

  dcli_translate_filename( fname, "$pwr_exe/wb_pvd_pl.sh");
  sprintf( cmd,
	"%s opendb \"%s\" \"%s\" \"%s\" &",
	fname, project, login_prv.username, login_prv.password);

  free( project);

  sts = system( cmd);
  if ( sts == -1 || sts == 127) {
    printf("-- Error when creating process.\n");
    return sts;
  }


  return 1;
}

static pwr_tStatus CopyProject (
  ldh_sMenuCall *ip
)
{
  pwr_tStatus sts;
  int size;
  pwr_tObjName oname;
  pwr_tCid cid;
  pwr_tOid oid;
  pwr_sClass_ProjectReg *body;
  char description[120];

  sts = ldh_ObjidToName( ip->PointedSession, ip->Pointed.Objid, ldh_eName_Object,
			  oname, sizeof(oname), &size);
  if ( EVEN(sts)) return sts;

  sts = ldh_GetObjectClass( ip->PointedSession, ip->Pointed.Objid, &cid);
  if ( EVEN(sts)) return sts;

  sts = ldh_GetObjectBody( ip->PointedSession, ip->Pointed.Objid, "RtBody",
			   (void **)&body, &size);
  if ( EVEN(sts)) return sts;

  strcat( oname, "2");
  strcat( body->Path, "2");
  sprintf( description, "%s (Copy of %s)", body->Description, body->Project);
  description[79] = 0;
  strcpy( body->Description, description);
  strcpy( body->CopyFrom, body->Project);
  strcat( body->Project, "2");

  sts = ldh_CreateObject( ip->PointedSession, &oid, oname, cid,
			  ip->Pointed.Objid, ldh_eDest_After);
  if ( EVEN(sts)) return sts;

  sts = ldh_SetObjectBody( ip->PointedSession, oid, "RtBody",
			   (char *)body, sizeof( *body));
  if ( EVEN(sts)) return sts;

  free(body);

  return 1;
}



/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindMethods(ProjectReg) = {
  pwr_BindMethod(OpenProject),
  pwr_BindMethod(CopyProject),
  pwr_NullMethod
};
