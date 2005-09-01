/** 
 * Proview   $Id: wb_wcast.h,v 1.2 2005-09-01 14:57:59 claes Exp $
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

#ifndef wb_wcast_h
#define wb_wcast_h

/* wb_wcast.h -- Attribute object casting window. */

#include <vector>

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif


#ifndef wb_ldh
# include "wb_ldh.h"
#endif

#ifndef co_wow_h
#include "co_wow.h"
#endif

using namespace std;


class WCast {
  public:
    WCast( 
	void	*wc_parent_ctx,
	Widget 	wc_parent_wid,
	char 	*wc_name,
	ldh_tSesContext wc_ldhses,
	pwr_sAttrRef wc_aref,
	pwr_tStatus *status);

    void 	*parent_ctx;
    Widget	parent_wid;
    char 	name[80];
    ldh_tSesContext ldhses;
    pwr_sAttrRef aref;
    vector<pwr_tCid> cidlist;

    pwr_tStatus open_castlist();
    void get_subcid( pwr_tCid cid);

    ~WCast();

    static void selected_cb( void *ctx, char *text);
};


#if defined __cplusplus
}
#endif
#endif



