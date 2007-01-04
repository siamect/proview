/* 
 * Proview   $Id: wb_c_dbconfig.cpp,v 1.1 2007-01-04 07:29:03 claes Exp $
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

/* wb_c_plcpgm.c -- work bench methods of the PlcPgm class. */

#include <stdio.h>
#include <stdlib.h>

#ifdef OS_VMS
#include <descrip.h>
#include <lib$routines.h>
#include <clidef.h>
#endif

#include "wb_pwrs.h"
#include "wb_ldh_msg.h"
#include "wb_ldh.h"
#include "wb_utl_api.h"
#include "wb_login.h"
#include "co_cdh.h"
#include "co_dcli.h"


/*----------------------------------------------------------------------------*\
  Open a workbench from a DbConfig object.
\*----------------------------------------------------------------------------*/

static pwr_tStatus OpenDb (
  ldh_sMenuCall *ip
)
{
#ifdef OS_VMS
  int size;
  char *db_id;
  int	sts;
  char name[80];
  static char	cmd[100];
  $DESCRIPTOR(cmd_desc,cmd);
  unsigned long cli_flag = CLI$M_NOWAIT;

  sts = ldh_GetObjectPar( ip->PointedSession,
			ip->Pointed.Objid, "RtBody",
			"Id", (char **) &db_id, &size);
  if (EVEN(sts)) return sts;

  sts = ldh_ObjidToName ( ip->PointedSession, ip->Pointed.Objid,
		ldh_eName_Object, name, sizeof(name), &size);
  if ( EVEN(sts)) return sts;

  sprintf( cmd,
	"@pwr_exe:wb_open_db \"%s\" \"%s\" \"%s\" \"\" \"%s\"",
	db_id, login_prv.username, login_prv.password, name);
  free( db_id);

  sts = lib$spawn (&cmd_desc , 0 , 0 , &cli_flag );
  if (EVEN(sts)) {
    printf("-- Error when creating subprocess.\n");
    return sts;
  }
#else
  int size;
  char *db_id_p;
  char db_id[80];
  int	sts;
  char name[80];
  char filename[80];
  char	cmd[100];

  sts = ldh_GetObjectPar( ip->PointedSession,
			ip->Pointed.Objid, "RtBody",
			"Id", (char **) &db_id_p, &size);
  if (EVEN(sts)) return sts;

  sts = ldh_ObjidToName ( ip->PointedSession, ip->Pointed.Objid,
		ldh_eName_Object, name, sizeof(name), &size);
  if ( EVEN(sts)) return sts;

  dcli_translate_filename( filename, "$pwr_exe/wb_open_db.sh");
  cdh_ToLower( db_id, db_id_p);
  sprintf( cmd,
	"%s \"%s\" \"%s\" \"%s\" \"\" \"%s\" &",
	filename, db_id, login_prv.username, login_prv.password, name);
  free( db_id_p);

  sts = system( cmd);
  if ( sts == -1 || sts == 127) {
    printf("-- Error when creating process.\n");
    return sts;
  }
#endif
  return 1;
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindMethods(DbConfig) = {
  pwr_BindMethod(OpenDb),
  pwr_NullMethod
};
