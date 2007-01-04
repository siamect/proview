/* 
 * Proview   $Id: wb_wcast.cpp,v 1.4 2007-01-04 07:29:04 claes Exp $
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

/* wb_wcast.cpp -- Attribute object casting window. */

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"
#include "pwr_baseclasses.h"
#include "wb_ldh.h"
#include "co_wow.h"
#include "wb_erep.h"
#include "wb_session.h"
#include "wb_attribute.h"
#include "co_msg.h"
#include "wb_wcast.h"


WCast::WCast( 
	void	*wc_parent_ctx,
	char 	*wc_name,
	ldh_tSesContext wc_ldhses,
	pwr_sAttrRef wc_aref,
	pwr_tStatus *status
	) :
	parent_ctx(wc_parent_ctx), ldhses(wc_ldhses), aref(wc_aref)
{
  strcpy( name, wc_name);

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

  wow->CreateList( name, (char *)class_vect, selected_cb, (void *)this);

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
    wcast->wow->DisplayError( "Cast Error", msg);
  }

  sts = ldh_CastAttribute( wcast->ldhses, &wcast->aref, cid);
  if ( EVEN(sts)) {
    char msg[256];

    msg_GetMsg( sts, msg, sizeof(msg));
    wcast->wow->DisplayError( "Cast Error", msg);
  }
}



