/* 
 * Proview   $Id: ge_attrnav.h,v 1.3 2005-09-01 14:57:52 claes Exp $
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

#ifndef ge_attrnav_h
#define ge_attrnav_h

/* xtt_attrnav.h -- Simple navigator */

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef flow_h
#include "flow.h"
#endif

#ifndef glow_h
#include "glow.h"
#endif

#ifndef flow_browctx_h
#include "flow_browapi.h"
#endif

#ifndef glow_growctx_h
#include "glow_growctx.h"
#endif

#ifndef glow_growapi_h
#include "glow_growapi.h"
#endif

#ifndef ge_graph_h
#include "ge_graph.h"
#endif

/*! \file ge_attrnav.h
    \brief Contains the AttrNav class and related classes. */
/*! \addtogroup Ge */
/*@{*/

#define attrnav_cVersion	"X3.0b"
#define ATTRNAV_BROW_MAX	25

typedef enum {
	attrnav_eItemType_Local,
	attrnav_eItemType_Enum,
	attrnav_eItemType_Mask
	} attrnav_eItemType;

typedef enum {
	attrnav_mOpen_All	= ~0,
	attrnav_mOpen_Children	= 1 << 0,
	attrnav_mOpen_Attributes = 1 << 1,
	attrnav_mOpen_Crossref	= 1 << 2
	} attrnav_mOpen;

typedef enum {
	attrnav_eType_GdbNodeFlags	= 10000,
	attrnav_eType_Empty	= 10001,
	attrnav_eType_FixStr	= 10002,
	attrnav_eType_ShortTime	= 10003
	} attrnav_eType;

typedef struct {
	int 	num;
	char	name[40];
	} attrnav_sEnumElement;

typedef struct {
	int			num;
	attrnav_sEnumElement	*elements;
	} attrnav_sEnum;


//! Class for handling of brow.
class AttrNavBrow {
  public:
    AttrNavBrow( BrowCtx *brow_ctx, void *xn) : ctx(brow_ctx), attrnav(xn) {};
    ~AttrNavBrow();

    BrowCtx		*ctx;
    void		*attrnav;
    brow_tNodeClass 	nc_object;
    brow_tNodeClass 	nc_attr;
    brow_tNodeClass 	nc_attr_multiline;
    brow_tNodeClass 	nc_table;
    brow_tNodeClass 	nc_header;
    brow_tNodeClass 	nc_table_header;
    brow_tNodeClass 	nc_enum;
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
class AttrNav {
  public:
    AttrNav(
	void *xn_parent_ctx,
	Widget	xn_parent_wid,
	char *xn_name,
	attr_sItem  *xn_itemlist,
	int xn_item_cnt,
	Widget *w,
	pwr_tStatus *status);
    ~AttrNav();

    void 		*parent_ctx;
    Widget		parent_wid;
    char 		name[80];
    Widget		brow_widget;
    Widget		form_widget;
    Widget		toplevel;
    AttrNavBrow		*brow;
    attr_sItem  	*itemlist;
    int			item_cnt;
    XtIntervalId	trace_timerid;
    int			trace_started;
    void 		(*message_cb)( void *, char, char *);
    void 		(*change_value_cb)( void *);
    int			(*get_subgraph_info_cb)( void *, char *, attr_sItem  **, int *);
    int			(*get_dyn_info_cb)( void *, GeDyn *, attr_sItem  **, int *);
    int 		(*reconfigure_attr_cb)( void *);
    int 		(*get_plant_select_cb)( void *, char *);
    int 		(*get_current_colors_cb)( void *, glow_eDrawType *, glow_eDrawType *,
						  glow_eDrawType *);

    void start_trace( pwr_tObjid Objid, char *object_str);
    int set_attr_value( char *value_str);
    int check_attr_value( int *multiline, char **value);
    int get_select( pwr_sAttrRef *attrref, int *is_attr);
    void message( char sev, char *text);
    void set_inputfocus();
    void force_trace_scan();
    int object_attr();

};

//! Item for a normal attribute.
class ItemLocal {
  public:
    ItemLocal( AttrNav *attrnav, char *item_name, char *attr,
	int attr_type, int attr_size, double attr_min_limit,
	double attr_max_limit, void *attr_value_p, int attr_multiline, 
        int attr_noedit, int attr_mask,
	brow_tNode dest, flow_eDest dest_code);
    attrnav_eItemType	type;
    brow_tNode		node;
    char	 	name[120];
    void		*value_p;
    char 		old_value[80];
    int 		first_scan;
    int			type_id;
    int			size;
    double		min_limit;
    double		max_limit;
    int			multiline;
    int			noedit;
    int			mask;
    int			parent;
    int			subgraph;

    int			open_children( AttrNav *attrnav, double x, double y);
    int			close( AttrNav *attrnav, double x, double y);
};

//! Item for an enum attribute.
class ItemEnum {
  public:
    ItemEnum( AttrNav *attrnav, char *item_name, int item_num, 
	int item_type_id,
	void *attr_value_p, brow_tNode dest, flow_eDest dest_code);
    attrnav_eItemType	type;
    brow_tNode		node;
    char	 	name[40];
    int			num;
    int			type_id;
    void		*value_p;
    int 		old_value;
    int 		first_scan;
};

//! Item for a mask attribute.
class ItemMask {
  public:
    ItemMask( AttrNav *attrnav, char *item_name, unsigned int item_mask, 
	int item_type_id,
	void *attr_value_p, brow_tNode dest, flow_eDest dest_code);
    attrnav_eItemType	type;
    brow_tNode		node;
    char	 	name[40];
    unsigned int	mask;
    int			type_id;
    void		*value_p;
    int 		old_value;
    int 		first_scan;
};

/*@}*/
#if defined __cplusplus
}
#endif
#endif
