/* 
 * Proview   $Id: ge_subgraphs.h,v 1.3 2005-09-01 14:57:53 claes Exp $
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

#ifndef ge_subgraphs_h
#define ge_subgraphs_h

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif
#ifndef flow_h
#include "flow.h"
#endif

#ifndef flow_browctx_h
#include "flow_browapi.h"
#endif

#ifndef ge_attr_h
#include "ge_attr.h"
#endif

/*! \file ge_subgraphs.h
    \brief Contains the SubGraphs class and related classes. */
/*! \addtogroup Ge */
/*@{*/

#define subgraphs_cVersion	"X3.0b"

typedef enum {
	subgraphs_eItemType_SubGraph
	} subgraphs_eItemType;

typedef enum {
	subgraphs_mOpen_All	= ~0,
	subgraphs_mOpen_Children	= 1 << 0,
	subgraphs_mOpen_Attributes = 1 << 1,
	subgraphs_mOpen_Crossref	= 1 << 2
	} subgraphs_mOpen;

typedef struct subgraph_sAttr {
	Attr	        *attrctx;
	subgraph_sAttr	*next;
	} *subgraphs_tAttr;

class SubGraphsBrow {
  public:
    SubGraphsBrow( BrowCtx *brow_ctx, void *xn) : ctx(brow_ctx), subgraphs(xn) {};
    ~SubGraphsBrow();

    BrowCtx		*ctx;
    void		*subgraphs;
    brow_tNodeClass 	nc_object;
    brow_tNodeClass 	nc_attr;
    brow_tNodeClass 	nc_table;
    brow_tNodeClass 	nc_header;
    brow_tNodeClass 	nc_table_header;
    flow_sAnnotPixmap 	*pixmap_leaf;
    flow_sAnnotPixmap 	*pixmap_map;
    flow_sAnnotPixmap 	*pixmap_openmap;
    flow_sAnnotPixmap 	*pixmap_attr;

    void free_pixmaps();
    void allocate_pixmaps();
    void create_nodeclasses();
    void brow_setup();
};


//! Display loaded subgraphs.
class SubGraphs {
  public:
    SubGraphs(
	void *xn_parent_ctx,
	Widget	xn_parent_wid,
	char *xn_name,
	void *grow_ctx,
	Widget *w,
	pwr_tStatus *status);

    void 		*parent_ctx;
    Widget		parent_wid;
    char 		name[80];
    Widget		subgraphs_form;
    Widget		toplevel;
    Widget		brow_widget;
    Widget		form_widget;
    SubGraphsBrow	*brow;
    XtIntervalId	trace_timerid;
    int			trace_started;
    void 		(*message_cb)( void *, char, char *);
    void 		(*close_cb)( SubGraphs *);
    void		*grow_ctx;
    subgraphs_tAttr	attrlist;

    int get_select( pwr_sAttrRef *attrref, int *is_attr);
    void message( char sev, char *text);
    void set_inputfocus();
    int object_attr();
    int get_select( void **subgraph_item);
    int edit_attributes( void *object);

    ~SubGraphs();
};

//! Base item class.
class SubGraphBaseItem {
  public:
    SubGraphBaseItem( subgraphs_eItemType	item_type) :
	type( item_type) {};
    subgraphs_eItemType	type;
};

//! Item for a subgraph.
class ItemSubGraph : public SubGraphBaseItem {
  public:
    ItemSubGraph( SubGraphs *subgraphs, char *item_name, int *item_extern_p,
	void *item_nodeclass,
	brow_tNode dest, flow_eDest dest_code);
    brow_tNode		node;
    char	 	name[120];
    void		*nodeclass;
    int			*extern_p;
    int			old_extern;
    int			first_scan;

    void		set_extern( int value) { *extern_p = value;};
};

/*@}*/
#if defined __cplusplus
}
#endif
#endif
