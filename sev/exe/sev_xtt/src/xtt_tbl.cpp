/* 
 * Proview   $Id: xtt_tbl.cpp,v 1.2 2008-09-05 08:38:58 claes Exp $
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

/* xtt_tbl.cpp -- Display object attributes */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <vector.h>
#include "co_cdh.h"
#include "co_time.h"
#include "co_xhelp.h"
#include "co_dcli.h"
#include "co_wow.h"
#include "co_cnf.h"

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"

#include "glow.h"
#include "glow_growctx.h"
#include "glow_growapi.h"
#include "flow_msg.h"

#include "xtt_tbl.h"
#include "xtt_tblnav.h"

XttTbl::~XttTbl()
{
}

XttTbl::XttTbl( void *xn_parent_ctx, sevcli_tCtx xn_sevcli) :
  parent_ctx(xn_parent_ctx), sevcli(xn_sevcli), command_open(0), close_cb(0),
  base_priv(pwr_mPrv_System), priv(pwr_mPrv_System), verify(0), ccm_func_registred(0),
  quiet(0)
{
  char default_priv[80];

  strcpy( base_user, "");
  strcpy( user, "");

  // Get default privilete from proview.cnf
  if ( cnf_get_value( "sevXttDefaultPriv", default_priv)) {
    if ( cdh_NoCaseStrcmp( default_priv, "READ") == 0)
      priv = pwr_mPrv_SevRead;
    else if ( cdh_NoCaseStrcmp( default_priv, "ADMIN") == 0)
      priv = pwr_mPrv_SevAdmin;
    else if ( cdh_NoCaseStrcmp( default_priv, "NONE") == 0)
      priv = 0;
    else
      priv = 0;
  }
  else
    priv = 0;
}

void XttTbl::message( void *xtttbl, char severity, char *message)
{
  ((XttTbl *)xtttbl)->message( severity, message);
}

int XttTbl::is_authorized( void *ctx, unsigned int access, int msg)
{
  return ((XttTbl *)ctx)->is_authorized( access, msg);
}

int XttTbl::is_authorized( unsigned int access, int msg)
{
  if (!(priv & access)) {
    if ( msg)
      message( 'I', "Not authorized for this operation");
    return 0;
  }
  return 1;
}

void XttTbl::open_login()
{
  pwr_tCmd cmd;
  strcpy( cmd, "login");
  command( cmd);
}

void XttTbl::logout()
{
  pwr_tCmd cmd;
  strcpy( cmd, "logout");
  command( cmd);
}

void XttTbl::activate_print()
{
  pwr_tFileName filename;
  pwr_tCmd cmd;

  dcli_translate_filename( filename, "$pwrp_tmp/sevxtt.ps");
  tblnav->print( filename);

  sprintf( cmd, "$pwr_exe/rt_print.sh %s", filename);
  system(cmd);
}

void XttTbl::activate_opendshist()
{
  sevcli_sHistItem *hi;

  tblnav->get_select( &hi);

  dshist_new( hi->oid, hi->aname);
}

void XttTbl::delete_item_yes( void *ctx, void *data)
{
  XttTbl *tbl = (XttTbl *)ctx;
  sevcli_sHistItem *hi = (sevcli_sHistItem *)data;
  pwr_tStatus sts;

  printf("Deleting %s\n", hi->oname);
  sevcli_delete_item( &sts, tbl->sevcli, hi->oid, hi->aname);
  if ( EVEN(sts)) {
    tbl->message( 'E', "Delete error");
    return;
  }
  tbl->tblnav->delete_item( hi);

  tbl->message( 'I', "Item deleted");
}

void XttTbl::activate_delete_item()
{
  sevcli_sHistItem *hi;
  char msg[300];

  if ( !tblnav->get_select( &hi)) {
    message( 'E', "Select an storage item");
    return;
  }

  sprintf( msg, "Do you really wan't to delete all stored data for item\n\n%s.%s\n", hi->oname, hi->aname);

  wow->DisplayQuestion( this, "Confirm Delete Item", msg, delete_item_yes, 0, hi);
}

void XttTbl::activate_zoom_in()
{
  double zoom_factor;

  tblnav->get_zoom( &zoom_factor);
  if ( zoom_factor > 40)
    return;

   tblnav->zoom( 1.18);
}

void XttTbl::activate_zoom_out()
{
  double zoom_factor;

  tblnav->get_zoom( &zoom_factor);
  if ( zoom_factor < 15)
    return;

  tblnav->zoom( 1.0 / 1.18);
}

void XttTbl::activate_zoom_reset()
{
  tblnav->unzoom();
}

void XttTbl::activate_help()
{
  CoXHelp::dhelp("overview", "", navh_eHelpFile_Base, NULL, 0);
}

void XttTbl::activate_help_project()
{
  CoXHelp::dhelp("index", "", navh_eHelpFile_Project, NULL, 0);
}

void XttTbl::activate_help_proview()
{
  CoXHelp::dhelp( "version", "", navh_eHelpFile_Other, "$pwr_exe/sev_xtt_version_help.dat", 0);
}

