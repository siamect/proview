/* 
 * Proview   $Id: wb_c_rootvolumeconfig.cpp,v 1.2 2008-06-24 07:52:21 claes Exp $
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

/* wb_c_rootvolumeconfig.c -- work bench methods of the RootVolumeConfig class. */

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


/*----------------------------------------------------------------------------*\
  Open a workbench from a RootVolumeConfig object.
\*----------------------------------------------------------------------------*/

static pwr_tStatus OpenDb (
  ldh_sMenuCall *ip
)
{
  int size;
  int	sts;
  char volume_name[80];
  char filename[80];
  char	cmd[100];

  sts = ldh_ObjidToName ( ip->PointedSession, ip->Pointed.Objid,
		ldh_eName_Object, volume_name, sizeof(volume_name), &size);
  if ( EVEN(sts)) return sts;

  dcli_translate_filename( filename, "$pwr_exe/wb_open_db.sh");
  sprintf( cmd,
	"%s \"%s\" \"%s\" \"%s\" \"%s\" &",
	filename, CoLogin::username(), CoLogin::ucpassword(), volume_name, volume_name);

  sts = system( cmd);
  if ( sts == -1 || sts == 127) {
    printf("-- Error when creating process.\n");
    return sts;
  }
  return 1;
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindMethods(RootVolumeConfig) = {
  pwr_BindMethod(OpenDb),
  pwr_NullMethod
};
pwr_dExport pwr_BindMethods(SubVolumeConfig) = {
  pwr_BindMethod(OpenDb),
  pwr_NullMethod
};
pwr_dExport pwr_BindMethods(SharedVolumeConfig) = {
  pwr_BindMethod(OpenDb),
  pwr_NullMethod
};


