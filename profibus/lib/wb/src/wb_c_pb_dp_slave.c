/* 
 * Proview   $Id: wb_c_pb_dp_slave.c,v 1.1 2006-01-16 13:56:52 claes Exp $
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

/* wb_c_pb_dp_slave.c -- work bench methods of the Pb_DP_Slave class. */

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
  pwr_tOName name;
  char *gsd;
  int size;
  int sts;
  pwr_tCmd cmd;

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




