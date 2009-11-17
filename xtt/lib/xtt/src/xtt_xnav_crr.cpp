/* 
 * Proview   $Id: xtt_xnav_crr.cpp,v 1.3 2007-01-04 08:22:47 claes Exp $
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

/* xtt_xnav_crr.cpp 
   This module contains routines for displaying crossreferences in xtt. */

/*_Include files_________________________________________________________*/

#include "flow_std.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "co_cdh.h"
#include "co_ccm.h"
#include "co_time.h"
#include "co_dcli.h"
#include "pwr_baseclasses.h"
#include "co_ccm_msg.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "xtt_trace.h"
#include "xtt_xnav.h"
#include "xtt_item.h"
#include "xtt_xnav_crr.h"
#include "co_nav_crr.h"
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

static void xnav_crr_insert_cb( void *ctx, void *parent_node, 
				navc_eItemType item_type,
				char *text1, char *text2, int write)
{
  XNavBrow *brow = (XNavBrow *)ctx;

  switch( item_type) {
    case navc_eItemType_Crossref:
      new ItemCrossref( brow, text1, text2, 
			write, parent_node, flow_eDest_IntoLast);
      break;
    case navc_eItemType_Header:
      new ItemHeader( brow, "crr", text1, parent_node, flow_eDest_IntoLast);
      break;
    case navc_eItemType_Text:
      new ItemText( brow, "crr", text1, parent_node, flow_eDest_IntoLast);
      break;
  }
}

int xnav_crr_name_to_objid_cb( void *ctx, char *name, pwr_tObjid *objid)
{
  return gdh_NameToObjid( name, objid);
}

int xnav_crr_get_volume_cb( void *ctx, pwr_tVolumeId *volid)
{
  int sts;
  pwr_tObjid objid;

  sts = gdh_GetNodeObject( 0, &objid);
  if ( EVEN(sts)) return sts;

  *volid = objid.vid;
  return XNAV__SUCCESS;
}

int	xnav_crr_signal(
			XNavBrow *brow,
			char	*filename,
			char	*signalname,
			flow_tNode parent_node)
{
  int sts;
  NavCrr *navcrr = new NavCrr( brow, parent_node);
  navcrr->insert_cb = xnav_crr_insert_cb;
  navcrr->name_to_objid_cb = xnav_crr_name_to_objid_cb;
  navcrr->get_volume_cb = xnav_crr_get_volume_cb;

  sts = navcrr->crr_signal( filename, signalname);

  delete navcrr;
  return sts;
}

int	xnav_crr_object(
			XNavBrow *brow,
			char	*filename,
			char	*objectname,
			flow_tNode parent_node)
{
  int sts;
  NavCrr *navcrr = new NavCrr( brow, parent_node);
  navcrr->insert_cb = xnav_crr_insert_cb;
  navcrr->name_to_objid_cb = xnav_crr_name_to_objid_cb;
  navcrr->get_volume_cb = xnav_crr_get_volume_cb;

  sts = navcrr->crr_object( filename, objectname);

  delete navcrr;
  return sts;
}

int	xnav_crr_code(
			XNavBrow *brow,
			char	*filename,
			char	*str,
			int	brief,
			int	func,
			int	case_sensitive)
{
  int sts;
  NavCrr *navcrr = new NavCrr( brow, NULL);
  navcrr->insert_cb = xnav_crr_insert_cb;
  navcrr->name_to_objid_cb = xnav_crr_name_to_objid_cb;
  navcrr->get_volume_cb = xnav_crr_get_volume_cb;

  sts = navcrr->crr_code( filename, str, brief, func, case_sensitive);

  delete navcrr;
  return sts;
}














