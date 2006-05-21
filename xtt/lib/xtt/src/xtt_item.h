/* 
 * Proview   $Id: xtt_item.h,v 1.11 2006-05-21 22:30:50 lw Exp $
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
 */

#ifndef xtt_item_h
#define xtt_item_h

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

#ifndef rt_trace_h
#include "rt_trace.h"
#endif

#include "rt_xnav_msg.h"

typedef enum {
	xnav_eItemType_Object,
	xnav_eItemType_Attr,
	xnav_eItemType_AttrArray,
	xnav_eItemType_AttrArrayElem,
	xnav_eItemType_Header,
	xnav_eItemType_HeaderLarge,
	xnav_eItemType_Text,
	xnav_eItemType_Collect,
	xnav_eItemType_Plc,
	xnav_eItemType_Menu,
	xnav_eItemType_Command,
	xnav_eItemType_File,
	xnav_eItemType_Crossref,
	xnav_eItemType_Local,
	xnav_eItemType_Table,
	xnav_eItemType_TableHeader,
	xnav_eItemType_Device,
	xnav_eItemType_Channel,
	xnav_eItemType_RemNode,
	xnav_eItemType_RemTrans,
	xnav_eItemType_Help,
	xnav_eItemType_HelpBold,
	xnav_eItemType_HelpHeader,
	xnav_eItemType_ObjectStruct,
	xnav_eItemType_AttrObject,
	xnav_eItemType_Enum,
	xnav_eItemType_Mask
	} xnav_eItemType;

typedef enum {
	item_eDisplayType_Attr,
	item_eDisplayType_Path
	} item_eDisplayType;

typedef enum {
	item_eFileType_Unknown,
	item_eFileType_Script,
	item_eFileType_Graph,
	item_eFileType_RttLog
	} item_eFileType;

typedef struct {
	char  	title[10][32];
	int	table_cnt;
	} item_sTableHeader;

typedef struct {
	pwr_tSubid  	subid[8];
	int		subid_cnt;
	} item_sTableSubid;

typedef struct {
	struct {
	  char		fix_str[80];
	  int		type_id;
	  int		size;
	  void		*value_p;
	  char		format[16];
	} elem[10];
	int	elem_cnt;
	} item_sTable;

class Item {
  public:
    Item( pwr_tObjid item_objid, int item_is_root) :
	type( xnav_eItemType_Object), objid(item_objid), is_root(item_is_root),
	node(NULL)
	{}
    virtual ~Item() {}
    virtual int open_attributes( XNavBrow *brow, double x, double y)
      { return 1;}
    virtual int	open_children( XNavBrow *brow, double x, double y)
      { return XNAV__NOCHILD;}
    virtual int	open_trace( XNavBrow *brow, double x, double y)
      { return 1;}
    virtual int open_crossref( XNavBrow *brow, double x, double y)
      { return 1;}
    virtual void close( XNavBrow *brow, double x, double y) {}

    xnav_eItemType	type;
    pwr_tObjid		objid;
    int			is_root;
    brow_tNode		node;
    pwr_tOName	 	name;
};

class ItemBaseAttr : public Item {
  public:
    ItemBaseAttr( pwr_tObjid item_objid,
	char *attr_name, int attr_type_id, pwr_tTid attr_tid,
        int attr_size, int attr_flags,
	int item_is_root, item_eDisplayType item_display_type) :
	Item( item_objid, item_is_root),
        type_id(attr_type_id), tid(attr_tid), size(attr_size), flags(attr_flags),
	subid(pwr_cNSubid), first_scan(1), display_type(item_display_type)
		{ strcpy( attr, attr_name);
		  memset( old_value, 0, sizeof(old_value));};
    virtual int open_children( XNavBrow *brow, double x, double y);
    virtual void close( XNavBrow *brow, double x, double y);

    int type_id;
    pwr_tTid tid;
    int size;
    int flags;
    pwr_tSubid subid;
    char old_value[80];
    int first_scan;
    char attr[40];
    item_eDisplayType display_type;
};

class ItemHeader : public Item {
  public:
    ItemHeader( XNavBrow *brow, char *item_name, char *title,
	brow_tNode dest, flow_eDest dest_code);
};

class ItemHeaderLarge : public Item {
  public:
    ItemHeaderLarge( XNavBrow *brow, char *item_name, char *title,
	brow_tNode dest, flow_eDest dest_code);
};

class ItemTableHeader : public Item {
  public:
    ItemTableHeader( XNavBrow *brow, XNav *tab_xnav, char *item_name, 
        item_sTableHeader *title, brow_tNode dest, flow_eDest dest_code);
    XNav		*xnav;
    void 		(*scan)(XNav *);
    void 		(*disconnect)(XNav *);
    void		add_bc(	void (*tab_scan)(XNav *), void (*tab_disconnect)(XNav *))
			{ scan = tab_scan; disconnect = tab_disconnect;};
};

class ItemText : public Item {
  public:
    ItemText( XNavBrow *brow, char *item_name, char *text,
	brow_tNode dest, flow_eDest dest_code);
};

class ItemHelp : public Item {
  public:
    ItemHelp( XNavBrow *brow, char *item_name, char *text, char *text2, 
	char *text3, char *item_link, char *item_bookmark, 
	char *item_file_name, navh_eHelpFile help_file_type, int help_index, brow_tNode dest, flow_eDest dest_code);
    int			open_children( XNavBrow *brow, double x, double y);
    char link[200];
    char bookmark[80];
    char file_name[80];
    navh_eHelpFile	 file_type;
    int  index;
};

class ItemHelpBold : public Item {
  public:
    ItemHelpBold( XNavBrow *brow, char *item_name, char *text, char *text2, 
	char *text3, char *item_link, char *item_bookmark, 
	char *item_file_name, navh_eHelpFile help_file_type, int help_index, 
	brow_tNode dest, flow_eDest dest_code);
    int			open_children( XNavBrow *brow, double x, double y);
    char link[200];
    char bookmark[80];
    char file_name[80];
    navh_eHelpFile	 file_type;
    int  index;
};

class ItemHelpHeader : public Item {
  public:
    ItemHelpHeader( XNavBrow *brow, char *item_name, char *title,
	brow_tNode dest, flow_eDest dest_code);
    void 		close( XNavBrow *brow, double x, double y);
};

class ItemFile : public Item {
  public:
    ItemFile( XNavBrow *brow, char *item_name, char *text, char *file,
	item_eFileType item_filetype, brow_tNode dest, flow_eDest dest_code);
    int			open_children( XNavBrow *brow, double x, double y);
    char file_name[120];
    item_eFileType file_type;

};


class ItemBaseObject : public Item {
  public:
    ItemBaseObject( pwr_tObjid item_objid, int item_is_root):
	Item( item_objid, item_is_root), cid(0) {};
    virtual int	open_children( XNavBrow *brow, double x, double y);
    virtual int open_attributes( XNavBrow *brow, double x, double y);
    virtual int open_trace( XNavBrow *brow, double x, double y);
    virtual int open_crossref( XNavBrow *brow, double x, double y);
    virtual void close( XNavBrow *brow, double x, double y);
    virtual int open_attribute( XNavBrow *brow, double x, double y, 
				char *attr_name, int element);
    pwr_tCid 	cid;
};

class ItemObject : public ItemBaseObject {
  public:
    ItemObject( XNavBrow *brow, pwr_tObjid item_objid, 
	brow_tNode dest, flow_eDest dest_code, int item_is_root);
    pwr_tUInt32 alarm_level;
    pwr_tUInt32 max_alarm_level;
    pwr_tUInt32 block_level;
    pwr_tUInt32 max_block_level;
};


class ItemAttr : public ItemBaseAttr {
  public:
    ItemAttr( XNavBrow *brow, pwr_tObjid item_objid,
	brow_tNode dest, flow_eDest dest_code,
        char *attr_name, int attr_type_id, pwr_tTid attr_tid,
	int attr_size, int attr_flags,
	int item_is_root, item_eDisplayType item_display_type);
};

class ItemAttrArray : public Item {
  public:
    int elements;
    int type_id;
    pwr_tTid tid;
    int size;
    int flags;
    ItemAttrArray( XNavBrow *brow, pwr_tObjid item_objid,
	brow_tNode dest, flow_eDest dest_code,
     	char *attr_name, int attr_elements, int attr_type_id, pwr_tTid attr_tid,
	int attr_size, int attr_flags, int item_is_root);
    int     open_children( XNavBrow *brow, double x, double y) {return 1;};
    int     open_attributes( XNavBrow *brow, double x, double y);
    void     close( XNavBrow *brow, double x, double y);
};

class ItemAttrArrayElem : public ItemBaseAttr {
  public:
    int element;
    ItemAttrArrayElem( XNavBrow *brow, pwr_tObjid item_objid,
	brow_tNode dest, flow_eDest dest_code,
	char *attr_name, int attr_element, int attr_type_id, pwr_tTid attr_tid,
	int attr_size, int attr_flags, int item_is_root, item_eDisplayType item_display_type);
};

class ItemAttrObject : public Item {
  public:
    pwr_tCid cid;
    int size;
    int flags;
    int element;
    ItemAttrObject( XNavBrow *brow, pwr_tObjid item_objid,
	brow_tNode dest, flow_eDest dest_code,
	char *attr_name, int attr_cid, 
	int attr_size, int attr_flags, int attr_element, int item_is_root);
    int     open_attributes( XNavBrow *brow, double x, double y);
    int     open_crossref( XNavBrow *brow, double x, double y);
    void    close( XNavBrow *brow, double x, double y);
};

class ItemCollect : public ItemBaseAttr {
  public:
    ItemCollect( XNavBrow *brow, pwr_tObjid item_objid, char *attr_name,
	brow_tNode dest, flow_eDest dest_code, int attr_type_id, pwr_tTid attr_tid,
	int attr_size, int item_is_root);
};

class ItemMenu : public Item {
 public:
  ItemMenu( XNavBrow *brow, char *item_name,
	brow_tNode dest, flow_eDest dest_code, xnav_sMenu **item_child_list,
	int item_is_root);
  xnav_sMenu		**child_list;
  int			open_children( XNavBrow *brow, double x, double y);
  void     		close( XNavBrow *brow, double x, double y);
};

class ItemCommand : public Item {
  public:
    ItemCommand( XNavBrow *brow, char *item_name, char *item_text,
	brow_tNode dest, flow_eDest dest_code, char *item_command, 
	int item_is_root, flow_sAnnotPixmap *pixmap);
    char		command[400];
    int			open_children( XNavBrow *brow, double x, double y);
};


class ItemCrossref : public Item {
  public:
    ItemCrossref( XNavBrow *brow, char *item_ref_name, char *item_ref_class, 
	int item_write, brow_tNode dest, flow_eDest dest_code);
    char		ref_name[32];
    char		ref_class[32];
    int			write;
    int     		open_trace( XNavBrow *brow, double x, double y);
};

class ItemLocal : public Item {
  public:
    ItemLocal( XNavBrow *brow, char *item_name, char *attr,
	int attr_type, int attr_size, double attr_min_limit,
	double attr_max_limit, int attr_nochange,
	void *attr_value_p, brow_tNode dest, flow_eDest dest_code);
    void		*value_p;
    char 		old_value[80];
    int 		first_scan;
    int			type_id;
    int			size;
    double		min_limit;
    double		max_limit;
    int                 nochange;
};

class ItemObjectStruct : public Item {
  public:
    ItemObjectStruct( XNavBrow *brow, char *item_name, char *attr,
	int attr_type, int attr_size, int attr_nochange,
	void *attr_value_p, pwr_tObjid attr_objid, pwr_tRefId attr_subid,
        brow_tNode dest, flow_eDest dest_code);
    void		*value_p;
    char 		old_value[80];
    int 		first_scan;
    int			type_id;
    int			size;
    int                 nochange;
    pwr_tRefId          subid;
};

class ItemTable : public ItemBaseObject {
  public:
    ItemTable( XNavBrow *brow, XNav *tab_xnav, pwr_tObjid objid, 
	       item_sTable *table_columns,
	item_sTableSubid *table_subid,
	int tab_change_value_idx,
	double tab_min_limit, double tab_max_limit, int relative_pos,
	brow_tNode dest, flow_eDest dest_code);
    XNav		*xnav;
    item_sTable		col;
    item_sTableSubid	subid;
    int			change_value_idx;
    double		min_limit;
    double		max_limit;
    void 		(*scan)(XNav *);
    void 		(*disconnect)(XNav *);
    char 		old_value[10][80];
    int 		first_scan;
    int			found;
    void		add_bc(	void (*tab_scan)(XNav *), void (*tab_disconnect)(XNav *))
			{ scan = tab_scan; disconnect = tab_disconnect;};
};

class ItemDevice : public ItemTable {
  public:
    ItemDevice( XNavBrow *brow, XNav *tab_xnav, pwr_tObjid objid, 
		item_sTable *table_columns,
	item_sTableSubid *table_subid,
	int tab_change_value_idx,
	double tab_min_limit, double tab_max_limit, int relative_pos,
	brow_tNode dest, flow_eDest dest_code) :
        ItemTable( brow, tab_xnav, objid, table_columns, table_subid, 
	tab_change_value_idx, tab_min_limit, tab_max_limit, 
	relative_pos, dest, dest_code)
	{ type = xnav_eItemType_Device;};
    int			open_children( XNavBrow *brow, double x, double y);
};

class ItemChannel : public ItemTable {
  public:
    ItemChannel( XNavBrow *brow, XNav *tab_xnav, pwr_tObjid objid, 
		 item_sTable *table_columns,
	item_sTableSubid *table_subid,
	int tab_change_value_idx,
	double tab_min_limit, double tab_max_limit, int relative_pos, 
	brow_tNode dest, flow_eDest dest_code) :
        ItemTable( brow, tab_xnav, objid, table_columns, table_subid, 
	tab_change_value_idx, tab_min_limit, tab_max_limit, 
	relative_pos, dest, dest_code)
	{ type = xnav_eItemType_Channel;};
    pwr_tObjid		signal_objid;
    int			open_children( XNavBrow *brow, double x, double y);
    int     		open_crossref( XNavBrow *brow, double x, double y);
};

class ItemRemNode : public ItemTable {
  public:
    ItemRemNode( XNavBrow *brow,XNav *tab_xnav, pwr_tObjid objid, 
		 item_sTable *table_columns,
	item_sTableSubid *table_subid,
	int tab_change_value_idx,
	double tab_min_limit, double tab_max_limit, int relative_pos, 
	brow_tNode dest, flow_eDest dest_code) :
        ItemTable( brow, tab_xnav, objid, table_columns, table_subid, 
	tab_change_value_idx, tab_min_limit, tab_max_limit, relative_pos, 
	dest, dest_code)
	{ type = xnav_eItemType_RemNode;};
    int			open_children( XNavBrow *brow, double x, double y);
};

class ItemRemTrans : public ItemTable {
  public:
    ItemRemTrans( XNavBrow *brow, XNav *tab_xnav, pwr_tObjid objid, 
		  item_sTable *table_columns,
	item_sTableSubid *table_subid,
	int tab_change_value_idx,
	double tab_min_limit, double tab_max_limit, int relative_pos, 
	brow_tNode dest, flow_eDest dest_code) :
        ItemTable( brow, tab_xnav, objid, table_columns, table_subid, 
	tab_change_value_idx, tab_min_limit, tab_max_limit, relative_pos, 
	dest, dest_code)
	{ type = xnav_eItemType_RemTrans;};
    int			open_children( XNavBrow *brow, double x, double y);
};

class ItemPlc : public ItemTable {
  public:
    ItemPlc( XNavBrow *brow, XNav *tab_xnav, pwr_tObjid objid, 
	     item_sTable *table_columns,
	item_sTableSubid *table_subid,
	int tab_change_value_idx,
	double tab_min_limit, double tab_max_limit, int relative_pos,
	brow_tNode dest, flow_eDest dest_code) :
        ItemTable( brow, tab_xnav, objid, table_columns, table_subid, 
	tab_change_value_idx, tab_min_limit, tab_max_limit, relative_pos,
	dest, dest_code)
	{ type = xnav_eItemType_Plc;};
    int     		open_children( XNavBrow *brow, double x, double y);
};


class ItemEnum : public ItemBaseAttr {
 public:
  ItemEnum( 
	XNavBrow *brow,
	pwr_tObjid item_objid, 
	brow_tNode dest, flow_eDest dest_code,
	char *attr_enum_name, char *attr_name, 
	int attr_type_id, pwr_tTid attr_tid, 
	int attr_size, int attr_flags,
	unsigned int item_num, int item_is_element, int item_element);
  int set_value();

  int num;
  char enum_name[32];
  int is_element;
  int element;
};

class ItemMask : public ItemBaseAttr {
 public:
  ItemMask( 
	XNavBrow *brow,
	pwr_tObjid item_objid, 
	brow_tNode dest, flow_eDest dest_code,
	char *attr_enum_name, char *attr_name, 
	int attr_type_id, pwr_tTid attr_tid, 
	int attr_size, int attr_flags,
	unsigned int item_num, int item_is_element, int item_element);
  int set_value( int bittrue);
  int toggle_value();

  int num;
  char mask_name[32];
  int is_element;
  int element;
};

#if defined __cplusplus
}
#endif
#endif
