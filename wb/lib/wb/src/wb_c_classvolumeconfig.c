/* wb_c_classvolumeconfig.c -- work bench methods of the ClassVolumeConfig class.

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
#include "wb_api.h"


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

  sts = ldh_ObjidToName ( ip->PointedSession, ip->Pointed.Objid,
		ldh_eName_Object, name, sizeof(name), &size);
  if ( EVEN(sts)) return sts;

  cdh_ToLower( name, name);
  sprintf( fname, "$pwrp_db/%s.wb_load", name);
  dcli_translate_filename( fname, fname);
  sprintf( cmd, "open classeditor /file=\"%s\"", fname);

  wtt_command( ip->EditorContext, cmd);
  return 1;
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindMethods(ClassVolumeConfig) = {
  pwr_BindMethod(EditClassVolume),
  pwr_NullMethod
};
