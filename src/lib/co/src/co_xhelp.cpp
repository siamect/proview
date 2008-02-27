/** 
 * Proview   $Id: co_xhelp.cpp,v 1.10 2008-02-27 06:24:37 claes Exp $
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

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_time.h"
#include "co_lng.h"
#include "co_xhelpnav.h"
#include "co_xhelp.h"

CoXHelp *CoXHelp::default_xhelp = 0;

void CoXHelp::open_URL( void *ctx, char *url)
{
  CoXHelp *xhelp = (CoXHelp *)ctx;

  if ( xhelp->open_URL_cb)
    (xhelp->open_URL_cb)( xhelp->parent_ctx, url);
}

void CoXHelp::find_ok( CoXHelp *xhelp, char *search_str)
{
  xhelp->xhelpnav->search( search_str, false);
}

void CoXHelp::pop()
{
  ((CoXHelpNav*)xhelpnav)->pop();
  displayed = 1;
}

void CoXHelp::clear()
{
  xhelpnav->brow_push_all();
  xhelpnav->clear();
}

int CoXHelp::help( char *key, char *help_bookmark, navh_eHelpFile file_type,
	      char *file_name, bool strict)
{
  return xhelpnav->help( key, help_bookmark, file_type,
				   file_name, 0, strict);
}

int CoXHelp::back()
{
  return xhelpnav->back();
}

int CoXHelp::next_topic()
{
  return xhelpnav->next_topic();
}

int CoXHelp::previous_topic()
{
  return xhelpnav->previous_topic();
}

//
//  Static function to call help for the default xhelp
//
int CoXHelp::dhelp( char *key, char *help_bookmark, navh_eHelpFile file_type,
	      char *file_name, bool strict)
{
  if ( !default_xhelp)
    return 0;

  default_xhelp->clear();
  default_xhelp->pop();
  return default_xhelp->help( key, help_bookmark, file_type,
			 file_name, strict);
}


int CoXHelp::help_index( navh_eHelpFile file_type, char *file_name)
{
  return xhelpnav->help_index( file_type, file_name, 0);
}

int CoXHelp::dhelp_index( navh_eHelpFile file_type, char *file_name)
{
  if ( !default_xhelp)
    return 0;

  default_xhelp->clear();
  default_xhelp->pop();
  return default_xhelp->help_index( file_type, file_name);
}

CoXHelp::~CoXHelp()
{
}

CoXHelp::CoXHelp( 
	void 		*xa_parent_ctx,
	xhelp_eUtility	utility,
        int             *xa_sts) :
 	parent_ctx(xa_parent_ctx), 
	client_data(0), close_cb(0), open_URL_cb(0), set_focus_disabled(0), 
	displayed(0)
{
  *xa_sts = 1;
}






