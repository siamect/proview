#ifndef xtt_item_h
#define xtt_item_h

/* xtt_item.h -- Simple navigator

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

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
	xnav_eItemType_ObjectStruct
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
	{};
    int     	open_attributes( XNavBrow *brow, double x, double y)
				{ return 1;};
    int		open_children( XNavBrow *brow, double x, double y)
				{ return 1;};
    int		open_trace( XNavBrow *brow, XNav *xnav, double x, double y)
				{ return 1;};
    void     	close( XNavBrow *brow, double x, double y) {};
    xnav_eItemType	type;
    pwr_tObjid		objid;
    int			is_root;
    brow_tNode		node;
    char	 	name[120];
};

class ItemBaseAttr : public Item {
  public:
    ItemBaseAttr( pwr_tObjid item_objid,
	char *attr_name, int attr_type_id, int attr_size, 
	int item_is_root, item_eDisplayType item_display_type) :
	Item( item_objid, item_is_root),
	type_id(attr_type_id), size(attr_size),
	subid(pwr_cNSubid), first_scan(1), display_type(item_display_type)
		{ strcpy( attr, attr_name);
		  memset( old_value, 0, sizeof(old_value));};
    int type_id;
    int size;
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
    int			open_children( XNavBrow *brow, XNav *xnav,
				       double x, double y);
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
    int			open_children( XNavBrow *brow, XNav *xnav, 
				       double x, double y);
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
    int 		close( XNavBrow *brow, XNav *xnav, double x, double y);
};

class ItemFile : public Item {
  public:
    ItemFile( XNavBrow *brow, char *item_name, char *text, char *file,
	item_eFileType item_filetype, brow_tNode dest, flow_eDest dest_code);
    int			open_children( XNavBrow *brow, XNav *xnav, 
				       double x, double y);
    char file_name[120];
    item_eFileType file_type;

};


class ItemBaseObject : public Item {
  public:
    ItemBaseObject( pwr_tObjid item_objid, int item_is_root):
	Item( item_objid, item_is_root) {};
    int			open_children( XNavBrow *brow, double x, double y);
    int     		open_attributes( XNavBrow *brow, double x, double y);
    int     		open_trace( XNavBrow *brow, XNav *xnav, 
				    double x, double y);
    int     		open_crossref( XNavBrow *brow, XNav *xnav, 
				       double x, double y);
    int     		close( XNavBrow *brow, double x, double y);
    int 		open_attribute( XNavBrow *brow, double x, double y, 
				char *attr_name, int element);
};

class ItemObject : public ItemBaseObject {
  public:
    ItemObject( XNavBrow *brow, pwr_tObjid item_objid, 
	brow_tNode dest, flow_eDest dest_code, int item_is_root);
};


class ItemAttr : public ItemBaseAttr {
  public:
    ItemAttr( XNavBrow *brow, pwr_tObjid item_objid,
	brow_tNode dest, flow_eDest dest_code,
	char *attr_name, int attr_type_id, int attr_size, 
	int item_is_root, item_eDisplayType item_display_type);
};

class ItemAttrArray : public Item {
  public:
    int elements;
    int type_id;
    int size;
    ItemAttrArray( XNavBrow *brow, pwr_tObjid item_objid,
	brow_tNode dest, flow_eDest dest_code,
	char *attr_name, int attr_elements, int attr_type_id, 
	int attr_size, int item_is_root);
    int     open_children( XNavBrow *brow, double x, double y) {return 1;};
    int     open_attributes( XNavBrow *brow, double x, double y);
    int     close( XNavBrow *brow, double x, double y);
};

class ItemAttrArrayElem : public ItemBaseAttr {
  public:
    int element;
    ItemAttrArrayElem( XNavBrow *brow, pwr_tObjid item_objid,
	brow_tNode dest, flow_eDest dest_code,
	char *attr_name, int attr_element, int attr_type_id,
	int attr_size, int item_is_root, item_eDisplayType item_display_type);
};

class ItemCollect : public ItemBaseAttr {
  public:
    ItemCollect( XNavBrow *brow, pwr_tObjid item_objid, char *attr_name,
	brow_tNode dest, flow_eDest dest_code, int attr_type_id, 
	int attr_size, int item_is_root);
};

class ItemMenu : public Item {
 public:
  ItemMenu( XNavBrow *brow, char *item_name,
	brow_tNode dest, flow_eDest dest_code, xnav_sMenu **item_child_list,
	int item_is_root);
  xnav_sMenu		**child_list;
  int			open_children( XNavBrow *brow, double x, double y);
  int     		close( XNavBrow *brow, double x, double y);
};

class ItemCommand : public Item {
  public:
    ItemCommand( XNavBrow *brow, char *item_name, char *item_text,
	brow_tNode dest, flow_eDest dest_code, char *item_command, 
	int item_is_root, flow_sAnnotPixmap *pixmap);
    char		command[200];
    int			open_children( XNavBrow *brow, XNav *xnav, 
				       double x, double y);
};


class ItemCrossref : public Item {
  public:
    ItemCrossref( XNavBrow *brow, char *item_ref_name, char *item_ref_class, 
	int item_write, brow_tNode dest, flow_eDest dest_code);
    char		ref_name[32];
    char		ref_class[32];
    int			write;
    int     		open_trace( XNavBrow *brow, XNav *xnav, 
				    double x, double y);
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
    int			open_children( XNavBrow *brow, XNav *xnav, 
				       double x, double y);
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
    int			open_children( XNavBrow *brow, XNav *xnav, 
				       double x, double y);
    int     		open_crossref( XNavBrow *brow, XNav *xnav, 
				       double x, double y);
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
    int			open_children( XNavBrow *brow, XNav *xnav, 
				       double x, double y);
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
    int			open_children( XNavBrow *brow, XNav *xnav, 
				       double x, double y);
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
    int			open_children( XNavBrow *brow, XNav *xnav, 
				       double x, double y);
    int     		open_attributes( XNavBrow *brow, double x, double y);
    int     		open_trace( XNavBrow *brow, XNav *xnav, 
				    double x, double y);
    int     		close( XNavBrow *brow, double x, double y);
};

#if defined __cplusplus
}
#endif
#endif
