/* 
 * Proview   $Id: xtt_xnav_help.cpp,v 1.4 2005-09-01 14:57:48 claes Exp $
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

/* xtt_xnav_help.cpp 
   This module contains routines for displaying help in xtt. */

/*_Include files_________________________________________________________*/

#include "flow_std.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

extern "C" {
#include "co_cdh.h"
#include "co_dcli.h"
}

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget.h"

extern "C" {
#include "rt_trace.h"
#include "flow_utils.h"
}
#include "xtt_xnav.h"
#include "xtt_item.h"
#include "xtt_xnav_crr.h"
#include "co_nav_help.h"
#include "co_dcli_msg.h"
#include "rt_xnav_msg.h"

/* Nice functions */
#define ODD(a)	(((int)(a) & 1) != 0)
#define EVEN(a)	(((int)(a) & 1) == 0)
#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))
#ifndef __ALPHA
#define abs(Dragon) ((Dragon) >= 0 ? (Dragon) : (-(Dragon)))
#endif

static int help_cmp_items( const void *node1, const void *node2);


/*************************************************************************
*
* Name:		help()
*
**************************************************************************/

void *xnav_help_insert_cb( void *ctx, navh_eItemType item_type, char *text1,
		      char *text2, char *text3, char *link, 
		      char *bookmark, char *file_name,
		      navh_eHelpFile file_type, int help_index, char *bm)
{
  XNav *xnav = (XNav *)ctx;

  if ( xnav->init_help) {
    xnav->brow_pop();
    brow_SetNodraw( xnav->brow->ctx);
    xnav->init_help = 0;
  }

  switch ( item_type) {
    case navh_eItemType_Help:
    case navh_eItemType_HelpCode:
    {      
      ItemHelp *item = new ItemHelp( xnav->brow, "help", text1, text2, text3,
	     link, bookmark, file_name, file_type, help_index,
	     NULL, flow_eDest_IntoLast);
      return item->node;
    }
    case navh_eItemType_HelpBold:
    {
      ItemHelpBold *item = new ItemHelpBold( xnav->brow, "help", text1, text2,
	     text3, link, bookmark, file_name, file_type, help_index,
	     NULL, flow_eDest_IntoLast);
      return item->node;
    }
    case navh_eItemType_HelpHeader:
    {      
      ItemHelpHeader *item = new ItemHelpHeader( xnav->brow, "help", text1,
	     NULL, flow_eDest_IntoLast);
      return item->node;
    }
     case navh_eItemType_Header:
    {      
      ItemHeader *item = new ItemHeader( xnav->brow, "help", text1,
	     NULL, flow_eDest_IntoLast);
      return item->node;
    }
    case navh_eItemType_HeaderLarge:
    {      
      ItemHeaderLarge *item = new ItemHeaderLarge( xnav->brow, "help", text1,
	     NULL, flow_eDest_IntoLast);
      return item->node;
    }
    default:
      return 0;
  }
}

int	XNav::help( char *help_key, char *help_bookmark, 
		navh_eHelpFile file_type, char *file_name, int pop)
{
  int sts;
  brow_tNode bookmark_node;
  brow_tNode prev, first;
  NavHelp *navhelp = new NavHelp( (void *)this, "$pwr_exe/xtt_help.dat",
				  "$pwrp_exe/xtt_help.dat");
  navhelp->insert_cb = xnav_help_insert_cb;

  if ( pop)
    init_help = 1;
  else {
    init_help = 0;
    brow_SetNodraw( brow->ctx);
  }
  sts = navhelp->help( help_key, help_bookmark, file_type, 
		       file_name, &bookmark_node, false);
  if ( EVEN(sts)) {
    if ( !pop || (pop && !init_help)) {
      brow_push();
      brow_ResetNodraw( brow->ctx);
    }
    return sts;
  }

  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  if ( bookmark_node) {
    brow_CenterObject( brow->ctx, bookmark_node, 0.0);
    sts = brow_GetPrevious( brow->ctx, bookmark_node, &prev);
    if ( ODD(sts))
    {
      brow_SelectClear( brow->ctx);
      brow_SetInverse( prev, 1);
      brow_SelectInsert( brow->ctx, prev);
    }
  }
  else {
    sts = brow_GetFirst( brow->ctx, &first);
    if ( ODD(sts))
      brow_CenterObject( brow->ctx, first, 0.0);
  }
  delete navhelp;

  return XNAV__SUCCESS;
}

/*************************************************************************
*
* Name:		help_index()
*
**************************************************************************/

int	XNav::help_index( navh_eHelpFile file_type, char *file_name, int pop)
{
  int sts;
  brow_tObject 	*object_list;
  int		object_cnt;
  NavHelp *navhelp = new NavHelp( (void *)this, "$pwr_exe/xtt_help.dat",
				  "$pwrp_exe/xtt_help.dat");
  navhelp->insert_cb = xnav_help_insert_cb;

  if (pop)
    brow_pop();
  brow_SetNodraw( brow->ctx);
  new ItemHelpHeader( brow, "help_index", "Index",  NULL, flow_eDest_IntoLast);
  new ItemHelp( brow, "help_index", "",  "", "", "", "", NULL, 
		navh_eHelpFile_Base, 0, NULL, flow_eDest_IntoLast);

  sts = navhelp->help_index( file_type, file_name);

  brow_GetObjectList( brow->ctx, &object_list, &object_cnt);
  flow_qsort( &object_list[2], object_cnt - 2, sizeof(object_list[0]), 
	help_cmp_items);    

  brow_ResetNodraw( brow->ctx);
  brow_Redraw( brow->ctx, 0);
  return sts;
}

static int help_cmp_items( const void *node1, const void *node2)
{
  char	text1[80];
  char	text2[80];

  brow_GetAnnotation( *(brow_tNode *) node1, 0, text1, sizeof(text1));
  brow_GetAnnotation( *(brow_tNode *) node2, 0, text2, sizeof(text2));
  cdh_ToLower( text1, text1);
  cdh_ToLower( text2, text2);

  return ( strcmp( text1, text2));
}



















