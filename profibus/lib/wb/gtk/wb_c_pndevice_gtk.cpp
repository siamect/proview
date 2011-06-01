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
#include "cow_pn_gsdml.h"
#include "cow_pn_gsdml_attr_gtk.h"
#include "wb_c_pndevice.h"


#include "wb_pwrs.h"
#include "wb_ldh_msg.h"
#include "wb_ldh.h"
#include "wb_pwrb_msg.h"
#include "rt_pb_msg.h"
#include "wb_wnav.h"
#include "wb_wsx.h"

using namespace std;

char *id_to_string( pwr_tOid oid)
{
  unsigned char	vid[4];
  static char	str[40];

  memcpy( &vid, &oid.vid, sizeof(vid));
  sprintf( str, "%3.3u_%3.3u_%3.3u_%3.3u_%8.8x", vid[3], vid[2], vid[1], vid[0], oid.oix);
  return str;
}

static pwr_tStatus Configure (
  ldh_sMenuCall *ip
)
{
  pwr_tStatus sts;
  device_sCtx *ctx;
  pwr_tFileName datafile;

  sprintf( datafile, "$pwrp_load/pwr_pn_%s.xml", id_to_string( ip->Pointed.Objid));

  sts = pndevice_create_ctx( ip->PointedSession, ip->Pointed, ip->wnav, &ctx);
  if ( EVEN(sts)) return sts;

  ctx->attr = new GsdmlAttrGtk( CoXHelpGtk::get_widget(), ctx, 0, ctx->gsdml, 
				ctx->edit_mode, datafile, &sts);
  if ( sts == PB__CONFIGABORTED) {
    delete ctx->attr;
    return 1;
  }
  ctx->attr->close_cb = pndevice_close_cb;
  ctx->attr->save_cb = pndevice_save_cb;
  ctx->attr->help_cb = pndevice_help_cb;

  if ( ODD(sts))
    sts = pndevice_init( ctx);
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

//
//  Get value.
//

static pwr_tStatus GetIoDeviceData (
  pwr_tAttrRef Object,
  const char *Attr,
  char *Buf,
  int BufSize
) {
  pwr_tFileName datafile;
  pwr_tStatus sts;

  sprintf( datafile, "$pwrp_load/pwr_pn_%s.xml", id_to_string( Object.Objid));
  dcli_translate_filename( datafile, datafile);

  GsdmlDeviceData *data = new GsdmlDeviceData();
  sts = data->read( datafile);
  if ( EVEN(sts)) return sts;

  sts = data->get_value( Attr, Buf, BufSize);

  delete data;

  return sts;
}

static pwr_tStatus SetIoDeviceData (
  pwr_tAttrRef Object,
  const char *Attr,
  const char *Value
) {
  pwr_tFileName datafile;
  pwr_tStatus sts;
  
  sprintf( datafile, "$pwrp_load/pwr_pn_%s.xml", id_to_string( Object.Objid));
  dcli_translate_filename( datafile, datafile);

  GsdmlDeviceData *data = new GsdmlDeviceData();
  sts = data->read( datafile);
  if ( EVEN(sts)) return sts;

  sts = data->modify_value( Attr, Value);
  if ( ODD(sts))
    data->print( datafile);
       

  delete data;

  return sts;
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindMethods(PnDevice) = {
  pwr_BindMethod(Configure),
  pwr_BindMethod(ConfigureFilter),
  pwr_BindMethod(SyntaxCheck),
  pwr_BindMethod(GetIoDeviceData),
  pwr_BindMethod(SetIoDeviceData),
  pwr_NullMethod
};




