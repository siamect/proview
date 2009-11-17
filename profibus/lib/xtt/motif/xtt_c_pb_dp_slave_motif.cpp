/* 
 * Proview   $Id: xtt_c_pb_dp_slave_motif.cpp,v 1.1 2007-01-04 08:44:54 claes Exp $
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
 */

/* xtt_c_pb_db_slave.cpp -- xtt methods for Pb_DP_Slave. */

#include "pwr_baseclasses.h"
#include "pwr_profibusclasses.h"
#include "flow_std.h"

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "co_cdh.h"
#include "xtt_menu.h"
#include "xtt_xnav.h"
#include "rt_xnav_msg.h"
#include "pwr_privilege.h"

#include "rt_pb_gsd.h"
#include "cow_xhelp_motif.h"
#include "rt_pb_gsd_attr_motif.h"
#include "xtt_c_pb_dp_slave.h"


// Show Configuration
static pwr_tStatus ShowConfiguration( xmenu_sMenuCall *ip)
{
  xtt_slave_sCtx *ctx;
  int edit_mode = 0;
  
  xtt_pb_dp_slave_create_ctx( ip->Pointed, ip->EditorContext, &ctx);
  
  ctx->attr = new GsdAttrMotif( CoXHelpMotif::get_widget(), ctx, 0, ctx->gsd, edit_mode);
  ctx->attr->close_cb = xtt_pb_dp_slave_close_cb;
  ctx->attr->save_cb = xtt_pb_dp_slave_save_cb;
  ctx->attr->help_cb = xtt_pb_dp_slave_help_cb;

  return 1;
}




/*----------------------------------------------------------------------------*\
  Every method to be exported to xtt should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindXttMethods(Pb_DP_Slave) = {
  pwr_BindXttMethod(ShowConfiguration),
  pwr_NullMethod
};





