/* wb_wnav_crr.cpp -- <short description>

   PROVIEW/R
   Copyright (C) 1996-98 by Mandator AB.

   This module contains routines for displaying help in wtt. */

/*_Include files_________________________________________________________*/

#include "flow_std.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

extern "C" {
#include "co_cdh.h"
#include "co_dcli.h"
#include "wb_ldh.h"
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
#include "flow_utils.h"
}
#include "wb_wnav.h"
#include "wb_wnav_item.h"
#include "co_nav_crr.h"
#include "co_dcli_msg.h"
#include "wb_wnav_msg.h"

/* Nice functions */
#define ODD(a)	(((int)(a) & 1) != 0)
#define EVEN(a)	(((int)(a) & 1) == 0)
#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))
#ifndef __ALPHA
#define abs(Dragon) ((Dragon) >= 0 ? (Dragon) : (-(Dragon)))
#endif

static void wnav_crr_insert_cb( void *ctx, void *parent_node, 
				navc_eItemType item_type,
				char *text1, char *text2, int write)
{
  if ( parent_node) {
    WNav *wnav = (WNav *)ctx;

    switch( item_type) {
      case navc_eItemType_Crossref:
        new WItemCrossref( wnav, text1, text2, 
			write, parent_node, flow_eDest_IntoLast);
        break;
      case navc_eItemType_Header:
        new WItemHeader( wnav, "crr", text1, parent_node, flow_eDest_IntoLast);
        break;
      case navc_eItemType_Text:
        new WItemText( wnav, "crr", text1, parent_node, flow_eDest_IntoLast);
        break;
    }
  }
  else {
    // Print out crossref info
    switch( item_type) {
      case navc_eItemType_Crossref:
        if ( write)
          printf("# ");
        else
          printf("  ");
        printf( "%s", text1);
        for ( int i = 0; i < (int)(28 - strlen(text1)); i++)
          printf(" ");
        printf( "  ");
        printf( "%s\n", text2);
        break;
      case navc_eItemType_Header:
      case navc_eItemType_Text:
        printf( "%s\n", text1);
        break;
    }
  }
}


int wnav_crr_name_to_objid_cb( void *ctx, char *name, pwr_tObjid *objid)
{
  WNav *wnav = (WNav *)ctx;

  return ldh_NameToObjid( wnav->ldhses, objid, name);
}

int wnav_crr_get_volume_cb( void *ctx, pwr_tVolumeId *volid)
{
  int sts;
  ldh_sVolumeInfo info;
  WNav *wnav = (WNav *)ctx;
  ldh_tVolContext volctx = ldh_SessionToVol( wnav->ldhses);
  sts = ldh_GetVolumeInfo( volctx, &info);

  *volid = info.Volume;
  return WNAV__SUCCESS;
}

int	WNav::crr_signal(
			char	*filename,
			char	*signalname,
			brow_tNode parent_node)
{
  int sts;
  NavCrr *navcrr = new NavCrr( this, parent_node);
  navcrr->insert_cb = wnav_crr_insert_cb;
  navcrr->name_to_objid_cb = wnav_crr_name_to_objid_cb;
  navcrr->get_volume_cb = wnav_crr_get_volume_cb;

  sts = navcrr->crr_signal( filename, signalname);

  delete navcrr;
  return sts;
}

int	WNav::crr_object(
			char	*filename,
			char	*objectname,
			brow_tNode parent_node)
{
  int sts;
  NavCrr *navcrr = new NavCrr( this, parent_node);
  navcrr->insert_cb = wnav_crr_insert_cb;
  navcrr->name_to_objid_cb = wnav_crr_name_to_objid_cb;
  navcrr->get_volume_cb = wnav_crr_get_volume_cb;

  sts = navcrr->crr_object( filename, objectname);

  delete navcrr;
  return sts;
}

int	WNav::crr_code(
			char	*filename,
			char	*str,
			int	brief,
			int	func,
			int	case_sensitive)
{
  int sts;
  NavCrr *navcrr = new NavCrr( this, NULL);
  navcrr->insert_cb = wnav_crr_insert_cb;
  navcrr->name_to_objid_cb = wnav_crr_name_to_objid_cb;
  navcrr->get_volume_cb = wnav_crr_get_volume_cb;

  sts = navcrr->crr_code( filename, str, brief, func, case_sensitive);

  delete navcrr;
  return sts;
}







