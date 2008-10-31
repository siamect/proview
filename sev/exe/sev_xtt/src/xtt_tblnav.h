/* 
 * Proview   $Id: xtt_tblnav.h,v 1.3 2008-10-31 12:51:30 claes Exp $
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

#include <fstream>
#include <vector>

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef pwr_privilege_h
#include "pwr_privilege.h"
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
  tblnav_eItemType_LocalAttr,
  tblnav_eItemType_TreeLocal,
  tblnav_eItemType_TreeNode
} tblnav_eItemType;

typedef enum {
  tblnav_mOpen_All	  = ~0,
  tblnav_mOpen_Children  = 1 << 0,
  tblnav_mOpen_Attributes = 1 << 1
} tblnav_mOpen;


class TblTreeNode {
public:
  TblTreeNode() : fth(0), fch(0), fws(0), bws(0), item(0), deleted(0)
  { strcpy( sname, "");}
  int fth;
  int fch;
  int fws;
  int bws;
  char sname[80];
  sevcli_sHistItem *item;
  int deleted;
};

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
    void 		(*message_cb)( void *, char, const char *);
    int 		(*is_authorized_cb)( void *, unsigned int, int);
    vector<TblTreeNode> tree;
    int 		list_layout;

    void print( char *filename);
    int is_authorized( unsigned int access = pwr_mAccess_AllSev, int msg = 1);
    int	create_items();
    void build_tree();
    int get_select( sevcli_sHistItem **hi);
    void get_zoom( double *zoom_factor);
    void zoom( double zoom_factor);
    void unzoom();
    void show_tree();
    void show_list();
    void delete_item( sevcli_sHistItem *hi);

    virtual void message( char sev, const char *text);
    virtual void set_inputfocus() {}
    static int init_brow_cb( FlowCtx *fctx, void *client_data);
};

class ItemBase {
 public: 
  ItemBase( tblnav_eItemType t) : type(t) {}
  virtual ~ItemBase() {}
  tblnav_eItemType type;
  virtual int  close( TblNav *tblnav, double x, double y) {return 1;}
};

//! Item for a normal attribute.
class ItemLocal : public ItemBase {
 public:
  ItemLocal( TblNav *tblnav, sevcli_sHistItem *item, brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemLocal() {}
  
  sevcli_sHistItem 	item;
  brow_tNode		node;

  int			open_attributes( TblNav *tblnav, double x, double y);
  int			close( TblNav *tblnav, double x, double y);
};

class ItemLocalAttr : public ItemBase {
 public:
  ItemLocalAttr( TblNav *tblnav, const char *iname, char *ivalue, brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemLocalAttr() {}
  
  sevcli_sHistItem 	item;
  brow_tNode		node;
};

//! Item for a normal attribute.
class ItemTreeLocal : public ItemLocal {
 public:
  ItemTreeLocal( TblNav *tblnav, sevcli_sHistItem *item, int index, brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemTreeLocal() {}
  
  int			idx;
};

//! Item for a normal attribute.
class ItemTreeNode : public ItemBase {
 public:
  ItemTreeNode( TblNav *tblnav, char *name, int index, brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemTreeNode() {}
  
  brow_tNode		node;
  int			idx;

  int			open_children( TblNav *tblnav, double x, double y);
  int			close( TblNav *tblnav, double x, double y);
};



#endif
