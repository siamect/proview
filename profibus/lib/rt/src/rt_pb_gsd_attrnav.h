/* 
 * Proview   $Id: rt_pb_gsd_attrnav.h,v 1.2 2006-04-24 13:22:23 claes Exp $
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

#ifndef rt_pb_gsd_attrnav_h
#define rt_pb_gsd_attrnav_h

/* rt_pb_gsd_attrnav.h -- Profibus gsd configurator navigator */

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

#ifndef rt_pb_gsd_atttr_h
#include "rt_pb_gsd_attr.h"
#endif

typedef enum {
	attrnav_eItemType_PbBase,
	attrnav_eItemType_PbEnum,
	attrnav_eItemType_PbEnumValue,
	attrnav_eItemType_PbModule,
	attrnav_eItemType_PbModuleType,
	attrnav_eItemType_PbModuleData,
	attrnav_eItemType_PbModuleClass,
	attrnav_eItemType_PbPrmData,
	attrnav_eItemType_PbMoreData
	} attrnav_eItemType;

typedef enum {
	attrnav_mOpen_All	= ~0,
	attrnav_mOpen_Children	= 1 << 0,
	attrnav_mOpen_Attributes = 1 << 1
	} attrnav_mOpen;

class ItemPb;

//! Class for handling of brow.
class GsdAttrNavBrow {
  public:
    GsdAttrNavBrow( BrowCtx *brow_ctx, void *xn) : ctx(brow_ctx), attrnav(xn) {};
    ~GsdAttrNavBrow();

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
    flow_sAnnotPixmap 	*pixmap_attrenum;

    void free_pixmaps();
    void allocate_pixmaps();
    void create_nodeclasses();
    void brow_setup();
};


//! The navigation area of the attribute editor.
class GsdAttrNav {
  public:
    GsdAttrNav(
	void *xn_parent_ctx,
	Widget	xn_parent_wid,
	char *xn_name,
	pb_gsd  *xn_gsd,
	int	xn_edit_mode,
	Widget *w,
	pwr_tStatus *status);
    ~GsdAttrNav();

    void 		*parent_ctx;
    Widget		parent_wid;
    pb_gsd		*gsd;
    char 		name[80];
    Widget		brow_widget;
    Widget		form_widget;
    Widget		toplevel;
    GsdAttrNavBrow	*brow;
    attr_sItem  	*itemlist;
    int			item_cnt;
    int 		edit_mode;
    XtIntervalId	trace_timerid;
    int			trace_started;
    void 		(*message_cb)( void *, char, char *);
    void 		(*change_value_cb)( void *);
    
    void start_trace( pwr_tObjid Objid, char *object_str);
    int set_attr_value( char *value_str);
    int check_attr_value( char **value);
    int get_select( pwr_sAttrRef *attrref, int *is_attr);
    void message( char sev, char *text);
    void set_inputfocus();
    void force_trace_scan();
    int object_attr();
    void print( char *filename);
    int get_select( ItemPb **item);
    void zoom( double zoom_factor);
    void unzoom();
    void get_zoom( double *zoom_factor);

};

class ItemPb {
 public:
  ItemPb() : parent(0) {}
  attrnav_eItemType	type;
  brow_tNode		node;
  char	 		name[120];
  int			parent;

  virtual ~ItemPb() {}
  
  virtual int	       	open_children( GsdAttrNav *attrnav, double x, double y) { return 1;}
  virtual int	       	close( GsdAttrNav *attrnav, double x, double y);
};

//! Item for a normal attribute.
class ItemPbBase : public ItemPb {
  public:
    ItemPbBase( GsdAttrNav *attrnav, char *item_name, char *attr,
	int attr_type, int attr_size, double attr_min_limit,
	double attr_max_limit, void *attr_value_p, 
        int attr_noedit,  gsd_sPrmText *attr_enumtext,
	brow_tNode dest, flow_eDest dest_code);
    void		*value_p;
    char 		old_value[80];
    int 		first_scan;
    int			type_id;
    int			size;
    double		min_limit;
    double		max_limit;
    int			noedit;
    gsd_sPrmText      	*enumtext;
    int			subgraph;
};

class ItemPbEnum : public ItemPbBase {
   public:
    ItemPbEnum( GsdAttrNav *attrnav, char *item_name, char *attr,
	int attr_type, int attr_size, double attr_min_limit,
	double attr_max_limit, void *attr_value_p,
        int attr_noedit,  gsd_sPrmText *attr_enumtext,
	brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemPbEnum() {}

    int 	    old_value;

    virtual int	       	open_children( GsdAttrNav *attrnav, double x, double y);
};

//! Item for an enum attribute.
class ItemPbEnumValue : public ItemPb {
  public:
    ItemPbEnumValue( GsdAttrNav *attrnav, char *item_name, int item_num, 
	int item_type_id,
	void *attr_value_p, brow_tNode dest, flow_eDest dest_code);
    int			num;
    int			type_id;
    void		*value_p;
    int 		old_value;
    int 		first_scan;
};

//! Item for a module.
class ItemPbModule : public ItemPb {
 public:
  ItemPbModule( GsdAttrNav *attrnav, char *item_name, gsd_sModuleConf *item_mconf,
		brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemPbModule() {}

  gsd_sModuleConf *mconf;
  char 		old_value[80];
  char 		old_type[80];
  int 		first_scan;

  virtual int  open_children( GsdAttrNav *attrnav, double x, double y);
};

//! Item for module type selection.
class ItemPbModuleType : public ItemPb {
 public:
  ItemPbModuleType( GsdAttrNav *attrnav, char *item_name, gsd_sModuleConf *item_mconf,
		    brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemPbModuleType() {}

  gsd_sModuleConf *mconf;
  int 	    old_value;
  int 	    first_scan;
  
  virtual int	       	open_children( GsdAttrNav *attrnav, double x, double y);
};

//! Item for module type selection.
class ItemPbModuleData : public ItemPb {
 public:
  ItemPbModuleData( GsdAttrNav *attrnav, char *item_name, gsd_sModuleConf *item_mconf,
		    brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemPbModuleData() {}

  gsd_sModuleConf *mconf;
  
  virtual int	       	open_children( GsdAttrNav *attrnav, double x, double y);
  void 			update( GsdAttrNav *attrnav);
};

//! Item for module class selection.
class ItemPbModuleClass : public ItemPb {
 public:
  ItemPbModuleClass( GsdAttrNav *attrnav, char *item_name, gsd_sModuleConf *item_mconf,
		     brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemPbModuleClass() {}

  gsd_sModuleConf *mconf;
  int 	    old_value;
  int 	    first_scan;
  
  virtual int	       	open_children( GsdAttrNav *attrnav, double x, double y);
};

//! Item for slave prmdata.
class ItemPbPrmData : public ItemPb {
 public:
  ItemPbPrmData( GsdAttrNav *attrnav, char *item_name,
		    brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemPbPrmData() {}

  virtual int	       	open_children( GsdAttrNav *attrnav, double x, double y);
};

//! Item for slave prmdata.
class ItemPbMoreData : public ItemPb {
 public:
  ItemPbMoreData( GsdAttrNav *attrnav, char *item_name,
		  brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemPbMoreData() {}
  virtual int	       	open_children( GsdAttrNav *attrnav, double x, double y);
};

/*@}*/
#if defined __cplusplus
}
#endif
#endif
