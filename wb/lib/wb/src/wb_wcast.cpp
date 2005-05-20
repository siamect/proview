/* wb_wcast.cpp -- Attribute object casting window.

   PROVIEW/R  
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "pwr_baseclasses.h"
#include "wb_ldh.h"
#include "flow_x.h"
#include "co_wow.h"
}

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <Xm/ToggleB.h>
#include <Xm/List.h>
#include <X11/cursorfont.h>
#define XK_MISCELLANY
#include <X11/keysymdef.h>

extern "C" {
#include "co_mrm_util.h"
}
#include "wb_erep.h"
#include "wb_session.h"
#include "wb_attribute.h"
#include "co_msg.h"
#include "wb_wcast.h"


WCast::WCast( 
	void	*wc_parent_ctx,
	Widget 	wc_parent_wid,
	char 	*wc_name,
	ldh_tSesContext wc_ldhses,
	pwr_sAttrRef wc_aref,
	pwr_tStatus *status
	) :
	parent_ctx(wc_parent_ctx), parent_wid( wc_parent_wid),
	ldhses(wc_ldhses), aref(wc_aref)
{
  strcpy( name, wc_name);

  open_castlist();
  *status = 1;
}

WCast::~WCast()
{
}

pwr_tStatus WCast::open_castlist()
{
  pwr_tStatus sts;
  pwr_tCid cid;
  int size;

  // Get all possible classes
  int class_cnt;
  char (*class_vect)[80];

  wb_session *sp = (wb_session *)ldhses;
  wb_attribute a = sp->attribute(&aref);
  if (!a) return a.sts();
  cid = a.originalTid();


  cidlist.push_back( cid);
  get_subcid( cid);

  class_vect = (char (*)[80]) calloc( cidlist.size() + 1, 80);

  class_cnt = 0;
  for ( int i = 0; i < (int) cidlist.size(); i++) {
    // Check size
    wb_cdef c = sp->cdef( cidlist[i]);
    if ( !c) return c.sts();

    if ( c.size( pwr_eBix_rt) > a.size())
      continue;

    sts = ldh_ClassIdToName( ldhses, cidlist[i], class_vect[class_cnt++], sizeof(class_vect[0]), 
			     &size);
    if ( EVEN(sts)) return sts;
  }
  strcpy( class_vect[class_cnt], "");


  wow_CreateList( parent_wid, name, (char *)class_vect,
		  selected_cb, (void *)this);

  return 1;
}

void WCast::get_subcid( pwr_tCid cid)
{
  pwr_tCid subcid;
  pwr_tStatus sts;

  for ( sts = ldh_GetSubClass( ldhses, cid, pwr_cNCid, &subcid);
	ODD(sts);
	sts = ldh_GetSubClass( ldhses, cid, subcid, &subcid)) {
    cidlist.push_back( subcid);
    get_subcid( subcid);
  }
}

void WCast::selected_cb( void *ctx, char *text)
{
  WCast *wcast = (WCast *)ctx;
  pwr_tStatus sts;
  pwr_tCid cid;

  sts = ldh_ClassNameToId( wcast->ldhses, &cid, text);
  if ( EVEN(sts)) {
    char msg[256];

    msg_GetMsg( sts, msg, sizeof(msg));
    wow_DisplayError( wcast->parent_wid, "Cast Error", msg);
  }

  sts = ldh_CastAttribute( wcast->ldhses, &wcast->aref, cid);
  if ( EVEN(sts)) {
    char msg[256];

    msg_GetMsg( sts, msg, sizeof(msg));
    wow_DisplayError( wcast->parent_wid, "Cast Error", msg);
  }
}



