/* 
 * Proview   $Id: wb_pal.h,v 1.7 2008-10-03 14:18:37 claes Exp $
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

#ifndef wb_pal_h
#define wb_pal_h

#ifndef pwr_h
# include "pwr.h"
#endif
#ifndef flow_h
#include "flow.h"
#endif

#ifndef flow_browctx_h
#include "flow_browapi.h"
#endif

#ifndef wb_palfile_h
#include "wb_palfile.h"
#endif


class Pal {
  public:
    Pal(
	void *parent_ctx,
	char *name,
	ldh_tSesContext ldhses,
	char *root_name,
	pwr_tStatus *status
	);
    virtual ~Pal();

    void 	*parent_ctx;
    char 	name[80];
    char 	root_name[80];
    BrowCtx	*brow_ctx;
    ldh_tWBContext wbctx;
    ldh_tSesContext ldhses;
    pwr_tObjid	root_objid;
    void       	*root_item;
    brow_tObject last_selected;
    int	       	selection_owner;
    int        	(*set_focus_cb)( void *, void *);
    int        	(*traverse_focus_cb)( void *, void *);
    void       	(*create_popup_menu_cb)( void *, pwr_tCid, int, int);
    void	(*select_cb)( void *, pwr_tCid);
    int		displayed;
    PalFileMenu *menu;

    brow_tNodeClass nc;
    flow_sAnnotPixmap *pixmap_leaf;
    flow_sAnnotPixmap *pixmap_map;
    flow_sAnnotPixmap *pixmap_openmap;
    flow_sAnnotPixmap *pixmap_ref;
    flow_sAnnotPixmap *pixmap_crossref;
    flow_sAnnotPixmap *pixmap_aarithm;
    flow_sAnnotPixmap *pixmap_arithm;
    flow_sAnnotPixmap *pixmap_bodytext;
    flow_sAnnotPixmap *pixmap_document;
    flow_sAnnotPixmap *pixmap_get;
    flow_sAnnotPixmap *pixmap_getp;
    flow_sAnnotPixmap *pixmap_head;
    flow_sAnnotPixmap *pixmap_initstep;
    flow_sAnnotPixmap *pixmap_logic1_;
    flow_sAnnotPixmap *pixmap_logic2_;
    flow_sAnnotPixmap *pixmap_order;
    flow_sAnnotPixmap *pixmap_orderact;
    flow_sAnnotPixmap *pixmap_setcond;
    flow_sAnnotPixmap *pixmap_ssbegin;
    flow_sAnnotPixmap *pixmap_ssend;
    flow_sAnnotPixmap *pixmap_step;
    flow_sAnnotPixmap *pixmap_sto;
    flow_sAnnotPixmap *pixmap_stop;
    flow_sAnnotPixmap *pixmap_substep;
    flow_sAnnotPixmap *pixmap_text;
    flow_sAnnotPixmap *pixmap_title;
    flow_sAnnotPixmap *pixmap_trans;
    flow_sAnnotPixmap *pixmap_cell;
    flow_sAnnotPixmap *pixmap_condigital;
    flow_sAnnotPixmap *pixmap_conanalog;
    flow_sAnnotPixmap *pixmap_confeedbackdigital;
    flow_sAnnotPixmap *pixmap_confeedbackanalog;
    flow_sAnnotPixmap *pixmap_conexecuteorder;
    flow_sAnnotPixmap *pixmap_congrafcet;
    flow_sAnnotPixmap *pixmap_stepconv;
    flow_sAnnotPixmap *pixmap_stepdiv;
    flow_sAnnotPixmap *pixmap_transconv;
    flow_sAnnotPixmap *pixmap_transdiv;
    flow_sAnnotPixmap *pixmap_frame;
    flow_sAnnotPixmap *pixmap_wait;

    virtual void set_inputfocus( int focus) {}
    virtual void set_selection_owner() {}
    virtual void create_popup_menu( pwr_tCid cid, int x, int y) {}

    void zoom( double zoom_factor);
    void unzoom();
    int get_select( pwr_tClassId *classid);
    void free_pixmaps();
    void allocate_pixmaps();
    void create_nodeclasses();
    int create_item( pwr_tObjid objid, 
	brow_tNode dest, flow_eDest dest_code, void **item,
	int is_root);
    int object_exist( brow_tObject object);
    int session_opened( ldh_tSesContext pal_ldhses, char *pal_root_name);
    int session_closed();

    static int init_brow_cb( FlowCtx *fctx, void *client_data);
    static int brow_cb( FlowCtx *ctx, flow_tEvent event);

};

//
// Definition of item classes
// The following classes are defined:
//    PalItem		superclass.
//    PalItemObject	Object that is not of class Group, GroupRef or 
//			$ClassHier. Doesn't display it's children.
//    PalItemClass		Group or $ClassHier objects. Displays it's children.
//    PalItemClassVolume	GroupRef objects. Display's one of the following
//			- the children of a Group with the same name.
//			- the children of a root object with the same name,
//			- a classvolume width the same name,
//			- it's own children.
//

typedef enum {
	pal_mOpen_All	        = ~0,
	pal_mOpen_Children	= 1 << 0
	} pal_mOpen;

typedef enum {
	pal_ePalItemType_Object,
	pal_ePalItemType_Class,
	pal_ePalItemType_ClassVolume,
	pal_ePalItemType_Menu
	} pal_ePalItemType;

class PalItem {
  public:
    PalItem( pwr_tObjid item_objid, int item_is_root) :
        type( pal_ePalItemType_Object), objid(item_objid), 
	is_root(item_is_root), node(NULL)
	{};
    pal_ePalItemType	type;
    pwr_tObjid		objid;
    int			is_root;
    brow_tNode		node;
    int			open( Pal *pal, double x, double y) 
				{ return 1;};
};


class PalItemClassVolume : public PalItem {
  public:
    PalItemClassVolume( Pal *pal, char *item_name, 
	brow_tNode dest, flow_eDest dest_code, int item_is_root);
    int			open( Pal *pal, double x, double y);
    char	 	name[120];
};

class PalItemClass : public PalItem {
  public:
    PalItemClass( Pal *pal, char *item_name, 
	brow_tNode dest, flow_eDest dest_code, int item_is_root);
    int			open( Pal *pal, double x, double y);
    char	 	name[120];
};

class PalItemObject : public PalItem {
  public:
    PalItemObject( Pal *pal, pwr_tObjid item_objid, 
	brow_tNode dest, flow_eDest dest_code, int item_is_root);
};

class PalItemMenu : public PalItem {
  public:
    PalItemMenu( Pal *pal, char *item_name, 
	brow_tNode dest, flow_eDest dest_code, PalFileMenu **item_child_list,
	int item_is_root);
    char	 	name[120];
    PalFileMenu	        **child_list;
    int			open( Pal *pal, double x, double y);
    int     		close( Pal *pal, double x, double y);
};

#endif




