/* 
 * Proview   $Id: ge_subpalette.h,v 1.8 2007-01-11 11:40:30 claes Exp $
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

#ifndef ge_subpalette_h
#define ge_subpalette_h

#ifndef pwr_h
# include "pwr.h"
#endif
#ifndef flow_h
#include "flow.h"
#endif

#ifndef flow_browapi_h
#include "flow_browapi.h"
#endif

/*! \file ge_subpalette.h
    \brief Contains the SubPalette class and related classes. */
/*! \addtogroup Ge */
/*@{*/

#define SUBP_PIXMAPS_SIZE	100

typedef struct subpalette_s_Menu {
	char				title[80];
	int				item_type;
	char				file[120];
 	int				pixmap;
	struct subpalette_s_Menu	*child_list;
	struct subpalette_s_Menu	*parent;
	struct subpalette_s_Menu	*next;
} subpalette_sMenu;

typedef enum {
	subpalette_eItemType_LocalSubGraphs,
	subpalette_eItemType_Menu,
	subpalette_eItemType_File
	} subpalette_eItemType;

typedef enum {
	subpalette_mOpen_All	= ~0,
	subpalette_mOpen_Children	= 1 << 0,
	subpalette_mOpen_Attributes = 1 << 1,
	subpalette_mOpen_Crossref	= 1 << 2
	} subpalette_mOpen;

class SubPaletteBrow {
  public:
    SubPaletteBrow( BrowCtx *brow_ctx, void *xn) : ctx(brow_ctx), 
	subpalette(xn) { memset( pixmaps, 0, sizeof(pixmaps)); };
    ~SubPaletteBrow();

    BrowCtx		*ctx;
    void		*subpalette;
    brow_tNodeClass 	nc_object;
    brow_tNodeClass 	nc_attr;
    brow_tNodeClass 	nc_table;
    brow_tNodeClass 	nc_header;
    brow_tNodeClass 	nc_table_header;
    flow_sAnnotPixmap 	*pixmap_leaf;
    flow_sAnnotPixmap 	*pixmap_map;
    flow_sAnnotPixmap 	*pixmap_openmap;
    flow_sAnnotPixmap 	*pixmaps[SUBP_PIXMAPS_SIZE];

    void free_pixmaps();
    void allocate_pixmaps();
    void create_nodeclasses();
    void brow_setup();
};


//! Display the subgraph palette.
class SubPalette {
  public:
    SubPalette(
	void *xn_parent_ctx,
	char *xn_name,
	pwr_tStatus *status);
    virtual ~SubPalette();

    void 		*parent_ctx;
    char 		name[80];
    SubPaletteBrow	*brow;
    int			trace_started;
    void 		(*message_cb)( void *, char, char *);
    int 		(*traverse_focus_cb)( void *, void *);
    int 		(*set_focus_cb)( void *, void *);
    void		*root_item;
    subpalette_sMenu	*menu_tree;
    char		path[10][80];
    int			path_cnt;
    int			displayed;

    virtual void set_inputfocus( int focus) {}

    void message( char sev, char *text);
    int get_select( pwr_sAttrRef *attrref, int *is_attr);
    void set_inputfocus();
    int object_attr();
    int get_select( char *text, char *filename);
    void menu_tree_build( char *filename);
    subpalette_sMenu *menu_tree_build_children( ifstream *fp, int *line_cnt,
		char *filename, subpalette_sMenu *parent);
    void menu_tree_free();
    void menu_tree_free_children( subpalette_sMenu *first_child);
    void get_path( int *path_count, char **path_vect) 
		{ *path_count = path_cnt; *path_vect = (char *)path;}; 
    void select_by_name( char *name);
    static int init_brow_cb( FlowCtx *fctx, void *client_data);
};

class Item {
  public:
    Item( subpalette_eItemType	item_type) :
	type( item_type), node(0) 
      { strcpy( name, "");}
    virtual ~Item() {}
    subpalette_eItemType	type;
    char	 	        name[120];
    brow_tNode			node;

    virtual int	 open_children( SubPalette *subpalette, double x, double y) { return 0;}
    virtual int	 close( SubPalette *subpalette, double x, double y) { return 0;}
};

class ItemLocalSubGraphs : public Item {
  public:
    ItemLocalSubGraphs( SubPalette *subpalette, char *item_name, 
	char *item_filename,
	brow_tNode dest, flow_eDest dest_code);
    char	 	filename[120];

    int			open_children( SubPalette *subpalette, double x, double y);
    int			close( SubPalette *subpalette, double x, double y);
};

class ItemFile : public Item {
  public:
    ItemFile( SubPalette *subpalette, char *item_name, 
	char *item_filename, int item_pixmap,
	brow_tNode dest, flow_eDest dest_code);
    char	 	filename[120];
    int			pixmap;
};

class ItemMenu : public Item {
  public:
    ItemMenu( SubPalette *subpalette, char *item_name, 
	brow_tNode dest, flow_eDest dest_code, subpalette_sMenu **item_child_list,
	int item_is_root);
    subpalette_sMenu	**child_list;
    int			is_root;
    int			open_children( SubPalette *subpalette, double x, double y);
    int     		close( SubPalette *subpalette, double x, double y);
};

/*@}*/
#endif






