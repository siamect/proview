/* 
 * Proview   $Id: xtt_tblnav.h,v 1.1 2008-07-17 11:18:31 claes Exp $
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

#ifndef xtt_tblnav_h
#define xtt_tblnav_h

/* xtt_attrnav.h -- Sev Table Viewer */

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef flow_h
#include "flow.h"
#endif

#ifndef flow_browctx_h
#include "flow_browapi.h"
#endif

#ifndef rt_qcom_h
#include "rt_qcom.h"
#endif

#ifndef rt_sevcli_h
#include "rt_sevcli.h"
#endif


typedef enum {
  tblnav_eItemType_Local,
  tblnav_eItemType_LocalAttr
} tblnav_eItemType;

typedef enum {
  tblnav_mOpen_All	= ~0,
  tblnav_mOpen_Attributes = 1 << 1
} tblnav_mOpen;


//! Class for handling of brow.
class TblNavBrow {
  public:
    TblNavBrow( BrowCtx *brow_ctx, void *xn) : ctx(brow_ctx), tblnav(xn) {};
    ~TblNavBrow();

    BrowCtx		*ctx;
    void		*tblnav;
    brow_tNodeClass 	nc_object;
    brow_tNodeClass 	nc_attr;
    flow_sAnnotPixmap 	*pixmap_leaf;
    flow_sAnnotPixmap 	*pixmap_map;
    flow_sAnnotPixmap 	*pixmap_openmap;
    flow_sAnnotPixmap 	*pixmap_attr;
    flow_sAnnotPixmap 	*pixmap_attrarray;

    void free_pixmaps();
    void allocate_pixmaps();
    void create_nodeclasses();
    void brow_setup();
};


//! The navigation area of the attribute editor.
class TblNav {
  public:
    TblNav( void *xn_parent_ctx,
	    sevcli_sHistItem  *xn_itemlist,
	    int xn_item_cnt,
	    pwr_tStatus *status);
    virtual ~TblNav();

    void 		*parent_ctx;
    TblNavBrow		*brow;
    sevcli_sHistItem  	*itemlist;
    int			item_cnt;
    void 		(*message_cb)( void *, char, char *);

    int	create_items();
    int get_select( sevcli_sHistItem **hi);

    virtual void message( char sev, char *text);
    virtual void set_inputfocus() {}
    static int init_brow_cb( FlowCtx *fctx, void *client_data);
};

//! Item for a normal attribute.
class ItemLocal {
 public:
  ItemLocal( TblNav *tblnav, sevcli_sHistItem *item, brow_tNode dest, flow_eDest dest_code);
  
  sevcli_sHistItem 	item;
  tblnav_eItemType	type;
  brow_tNode		node;

  int			open_attributes( TblNav *tblnav, double x, double y);
  int			close( TblNav *tblnav, double x, double y);
};

class ItemLocalAttr {
 public:
  ItemLocalAttr( TblNav *tblnav, char *iname, char *ivalue, brow_tNode dest, flow_eDest dest_code);
  
  sevcli_sHistItem 	item;
  tblnav_eItemType	type;
  brow_tNode		node;
};

#endif
