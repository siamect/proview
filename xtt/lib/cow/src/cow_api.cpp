/** 
 * Proview   $Id: co_api.cpp,v 1.13 2008-10-31 12:51:30 claes Exp $
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

#if defined OS_VMS && defined __ALPHA
# pragma message disable (NOSIMPINT,EXTROUENCUNNOBJ)
#endif

#if defined OS_VMS && !defined __ALPHA
# pragma message disable (LONGEXTERN)
#endif

#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "pwr.h"
}

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"

#include "cow_msgwindow.h"
#include "cow_xhelp.h"

extern "C" {
#include "cow_api.h"
#include "co_dcli.h"
}


//
// c api to co_msgwindow
//
void msgw_message( int severity, const char *text, msgw_ePop pop)
{
  MsgWindow::message( severity, text, pop);
}

void msgw_message_sts( pwr_tStatus sts, const char *text1, const char *text2)
{
  MsgWindow::message( co_error(sts), text1, text2);
}

void msgw_message_object( pwr_tStatus sts, const char *text1, const char *text2, pwr_tOid oid)
{
  MsgWindow::message( co_error(sts), text1, text2, oid);
}

void msgw_message_plcobject( pwr_tStatus sts, const char *text1, const char *text2, pwr_tOid oid)
{
  MsgWindow::message( co_error(sts), text1, text2, oid, true);
}

void msgw_set_nodraw()
{
  MsgWindow::dset_nodraw();
}

void msgw_reset_nodraw()
{
  MsgWindow::dreset_nodraw();
}

//
// c api to co_xhelp
//

int xhelp_help( char *key, char *help_bookmark, navh_eHelpFile file_type,
		char *file_name, int strict)
{
  return CoXHelp::dhelp( key, help_bookmark, file_type, file_name, strict != 0);
}

