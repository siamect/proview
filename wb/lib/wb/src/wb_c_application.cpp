/* 
 * Proview   $Id: wb_c_application.cpp,v 1.1 2007-01-30 07:13:02 claes Exp $
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

/* wb_c_application.cpp -- work bench methods of the Application class. */

#include "wb_pwrs.h"
#include "pwr_baseclasses.h"
#include "wb_ldh_msg.h"
#include "wb_ldh.h"
#include "wb_pwrb_msg.h"
#include "wb_wsx.h"
#include "wb_wsx_msg.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "wb_wnav.h"
#include "wb_build.h"
#include "cow_msgwindow.h"
#include "ge.h"

static pwr_tStatus Build (
  ldh_sMenuCall *ip
)
{
  wb_build build( *(wb_session *)ip->PointedSession, ip->wnav);

  build.opt = ip->wnav->gbl.build;
  build.application( ip->Pointed.Objid);

  if ( build.sts() == PWRB__NOBUILT)
    ip->wnav->message( 'I', "Nothing to build");
  return build.sts();
}


pwr_dExport pwr_BindMethods(Application) = {
  pwr_BindMethod(Build),
  pwr_NullMethod
};

