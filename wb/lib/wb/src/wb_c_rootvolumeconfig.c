/* wb_c_rootvolumeconfig.c -- work bench methods of the RootVolumeConfig class.

   PROVIEW/R
   Copyright (C) 1994 by Comator Process AB.  */

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
#include "wb_login.h"
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
	filename, login_prv.username, login_prv.password, volume_name, volume_name);

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


