/* 
 * Proview   $Id: wb_c_pb_dp_slave_gtk.cpp,v 1.1 2007-01-04 08:44:14 claes Exp $
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

/* wb_c_pb_dp_slave_gtk.c -- work bench methods of the Pb_DP_Slave class. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <map>

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "pwr_profibusclasses.h"

#include <gtk/gtk.h>
#include "wb_env.h"
#include "flow.h"
#include "flow_ctx.h"
#include "flow_api.h"
#include "flow_browctx.h"
#include "flow_browapi.h"

#include "co_msg.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "cow_wow.h"
#include "cow_xhelp_gtk.h"
#include "cow_pb_gsd.h"
#include "cow_pb_gsd_attr_gtk.h"
#include "wb_c_pb_dp_slave.h"


#include "wb_pwrs.h"
#include "wb_ldh_msg.h"
#include "wb_ldh.h"
#include "wb_pwrb_msg.h"
#include "rt_pb_msg.h"
#include "wb_wnav.h"
#include "wb_wsx.h"

using namespace std;

static pwr_tStatus Configure (
  ldh_sMenuCall *ip
)
{
  pwr_tStatus sts;
  slave_sCtx *ctx;

  sts = pb_dp_slave_create_ctx( ip->PointedSession, ip->Pointed, ip->wnav, &ctx);
  if ( sts != PB__USERPRMDATALEN && EVEN(sts)) return sts;

  ctx->attr = new GsdAttrGtk( CoXHelpGtk::get_widget(), ctx, 0, ctx->gsd, 
			      ctx->edit_mode);
  ctx->attr->close_cb = pb_dp_slave_close_cb;
  ctx->attr->save_cb = pb_dp_slave_save_cb;
  ctx->attr->help_cb = pb_dp_slave_help_cb;

  if ( EVEN(sts)) {
    ctx->attr->wow->DisplayError( "Configuration load error", 
		      "Configuration load error\nCheck configuration data");
  }

  return 1;
}

static pwr_tStatus ConfigureFilter (
  ldh_sMenuCall *ip
)
{
#if 0
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
#endif
  return 1;
}

//
//  Syntax check.
//

static pwr_tStatus SyntaxCheck (
  ldh_tSesContext Session,
  pwr_tAttrRef Object,	      /* current object */
  int *ErrorCount,	      /* accumulated error count */
  int *WarningCount	      /* accumulated waring count */
) {
  return wsx_CheckIoDevice( Session, Object, ErrorCount, WarningCount, wsx_mCardOption_None);
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindMethods(Pb_DP_Slave) = {
  pwr_BindMethod(Configure),
  pwr_BindMethod(ConfigureFilter),
  pwr_BindMethod(SyntaxCheck),
  pwr_NullMethod
};




