/* 
 * Proview   $Id: wb_c_classvolumeconfig.cpp,v 1.3 2008-06-24 07:52:21 claes Exp $
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

/* wb_c_classvolumeconfig.c -- work bench methods of the ClassVolumeConfig class. */

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
#include "co_login.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "wb_wnav.h"
#include "pwr_baseclasses.h"


/*----------------------------------------------------------------------------*\
  Open a workbench from a RootVolumeConfig object.
\*----------------------------------------------------------------------------*/

static pwr_tStatus EditClassVolume (
  ldh_sMenuCall *ip
)
{
  int size;
  int	sts;
  char name[120];
  char fname[200];
  char	cmd[200];
  pwr_eClassVolumeDatabaseEnum  *dbenum;

  sts = ldh_ObjidToName ( ip->PointedSession, ip->Pointed.Objid,
		ldh_eName_Object, name, sizeof(name), &size);
  if ( EVEN(sts)) return sts;

  sts = ldh_GetObjectPar( ip->PointedSession, ip->Pointed.Objid, "RtBody",
			  "Database", (char **) &dbenum, &size);
  if ( EVEN(sts)) return sts;


  switch ( *dbenum) {
  case pwr_eClassVolumeDatabaseEnum_WbLoad:
    cdh_ToLower( name, name);
    sprintf( fname, "$pwrp_db/%s.wb_load", name);
    dcli_translate_filename( fname, fname);

    sprintf( cmd, "open classeditor /file=\"%s\"/database=wbload", fname);
    ip->wnav->command( cmd);
    break;
  case pwr_eClassVolumeDatabaseEnum_BerkeleyDb:
  case pwr_eClassVolumeDatabaseEnum_MySql: {
    pwr_tFileName filename;

    cdh_ToLower( name, name);
    dcli_translate_filename( filename, "$pwr_exe/wb_open_db.sh");
    sprintf( cmd,
	     "%s \"%s\" \"%s\" \"%s\" \"%s\" &",
	     filename, CoLogin::username(), CoLogin::ucpassword(), name, name);

    sts = system( cmd);
    if ( sts == -1 || sts == 127) {
      printf("-- Error when creating process.\n");
      return sts;
    }
    break;
  }
  default:
    return 1;
  }


  free( dbenum);
  return 1;
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindMethods(ClassVolumeConfig) = {
  pwr_BindMethod(EditClassVolume),
  pwr_NullMethod
};
