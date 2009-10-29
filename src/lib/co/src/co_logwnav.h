/** 
 * Proview   $Id$
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

#ifndef co_logwnav_h
#define co_logwnav_h

/* co_logwnav.h -- History log widnow */

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef flow_h
#include "flow.h"
#endif

#ifndef flow_browctx_h
#include "flow_browctx.h"
#endif

#ifndef flow_browapi_h
#include "flow_browapi.h"
#endif

typedef enum {
	logwitem_eItemType_Log
	} logwitem_eItemType;

class CoLogWNavBrow {
  public:
    CoLogWNavBrow( BrowCtx *brow_ctx, void *lwnav) : ctx(brow_ctx), logwnav(lwnav) {};
    ~CoLogWNavBrow();

    BrowCtx		*ctx;
    void		*logwnav;
    brow_tNodeClass 	nc_log;
    flow_sAnnotPixmap 	*pixmap_save;
    flow_sAnnotPixmap 	*pixmap_build;
    flow_sAnnotPixmap 	*pixmap_package;
    flow_sAnnotPixmap 	*pixmap_copy;
    flow_sAnnotPixmap 	*pixmap_export;

    void free_pixmaps();
    void allocate_pixmaps();
    void create_nodeclasses();
    void brow_setup();
};

class CoLogWNav {
  public:
    CoLogWNav(
	      void *l_parent_ctx, int l_show_item);
    virtual ~CoLogWNav();

    void 		*parent_ctx;
    CoLogWNavBrow	*brow;
    int			show_item;

    virtual void set_input_focus() {}

    void show( char categories[][20], char *item);
    void zoom( double zoom_factor);
    void unzoom();

    static int init_brow_cb( FlowCtx *fctx, void *client_data);
    static int brow_cb( FlowCtx *ctx, flow_tEvent event);
    static void item_cb( void *ctx, pwr_tTime time, char *category, char *user, char *item, 
			 char *comment);
};

class ItemLog {
  public:
    ItemLog( CoLogWNav *logwnav, const char *item_name,
	     pwr_tTime item_time, char *item_catogory, char *item_user, char *item_comment,
	     brow_tNode dest, flow_eDest dest_code);
    logwitem_eItemType	type;
    CoLogWNav		*logwnav;
    brow_tNode		node;
    pwr_tTime		time;
    char		category[20];
    char		user[80];
    char		comment[256];

    virtual ~ItemLog() {}
};

#endif



