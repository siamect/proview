/* wb_c_pb_dp_slave.c -- work bench methods of the Pb_DP_Slave class.

   PROVIEW/R
   Copyright (C) 1994 by Comator Process AB.  */

#include <X11/Intrinsic.h>
#undef Status
#include "wb_pwrs.h"
#include "pwr_baseclasses.h"
#include "wb_ldh_msg.h"
#include "wb_ldh.h"
#include "wb_pwrb_msg.h"
#include "wb_api.h"


/*----------------------------------------------------------------------------*\
  Configure the slave from gsd file.
\*----------------------------------------------------------------------------*/

static pwr_tStatus Configure (
  ldh_sMenuCall *ip
)
{
  char name[120];
  char *gsd;
  int size;
  int sts;
  char cmd[200];

  sts = ldh_ObjidToName(ip->PointedSession, ip->Pointed.Objid, 
			ldh_eName_Hierarchy, name, sizeof(name), &size);
  if ( EVEN(sts)) return sts;

  sts = ldh_GetObjectPar( ip->PointedSession, ip->Pointed.Objid, "RtBody",
			  "GSDfile", &gsd, &size);
  if ( EVEN(sts)) return sts;
  if ( strcmp( gsd, "") == 0) {
    free( gsd);  
    return 1;
  }
  free( gsd);  

  sprintf( cmd, "@$pwr_exe/pb_slave_config %s", name);

  wtt_command( ip->EditorContext, cmd);

  return 1;
}

static pwr_tStatus ConfigureFilter (
  ldh_sMenuCall *ip
)
{
  char *gsd;
  int size;
  int sts;

  sts = ldh_GetObjectPar( ip->PointedSession, ip->Pointed.Objid, "RtBody",
			  "GSDfile", &gsd, &size);
  if ( EVEN(sts)) return sts;
  if ( strcmp( gsd, "") == 0) {
    free( gsd);  
    return 0;
  }
  free( gsd);  
  return 1;
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindMethods(Pb_DP_Slave) = {
  pwr_BindMethod(Configure),
  pwr_BindMethod(ConfigureFilter),
  pwr_NullMethod
};




