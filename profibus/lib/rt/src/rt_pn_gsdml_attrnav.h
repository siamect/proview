/* 
 * Proview   $Id: rt_pn_gsdml_attrnav.h,v 1.5 2008-10-31 12:51:30 claes Exp $
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

#ifndef rt_pn_gsdml_attrnav_h
#define rt_pn_gsdml_attrnav_h

/* rt_pn_gsdml_attrnav.h -- Profibus gsd configurator navigator */

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

#ifndef rt_pn_gsdml_atttr_h
#include "rt_pn_gsdml_attr.h"
#endif

#ifndef rt_pn_gsdml_data_h
#include "rt_pn_gsdml_data.h"
#endif

typedef enum {
	attrnav_eItemType_PnBase,
	attrnav_eItemType_PnEnumValue,
 	attrnav_eItemType_PnDevice,
 	attrnav_eItemType_PnNetwork,
 	attrnav_eItemType_PnDeviceInfo,
 	attrnav_eItemType_PnDAP,
 	attrnav_eItemType_PnInterfaceSubmodule,
 	attrnav_eItemType_PnPortSubmodule,
 	attrnav_eItemType_PnSlot,
 	attrnav_eItemType_PnSubslot,
 	attrnav_eItemType_PnModuleInfo,
	attrnav_eItemType_PnModuleType,
	attrnav_eItemType_PnParRecord,
	attrnav_eItemType_PnParValue,
	attrnav_eItemType_PnParEnum,
	attrnav_eItemType_PnParEnumBit,
	attrnav_eItemType_PnModuleClass,
 	attrnav_eItemType_PnIOData,
 	attrnav_eItemType_PnInput,
 	attrnav_eItemType_PnOutput,
 	attrnav_eItemType_PnDataItem,
 	attrnav_eItemType_PnBitDataItem,
	attrnav_eItemType_PnEnumByteOrder,
	attrnav_eItemType_PnEnumTimeRatio
	} attrnav_eItemType;

typedef enum {
	attrnav_mOpen_All	= ~0,
	attrnav_mOpen_Children	= 1 << 0,
	attrnav_mOpen_Attributes = 1 << 1
	} attrnav_mOpen;

class ItemPn;

//! Class for handling of brow.
class GsdmlAttrNavBrow {
  public:
    GsdmlAttrNavBrow( BrowCtx *brow_ctx, void *xn) : ctx(brow_ctx), attrnav(xn) {};
    ~GsdmlAttrNavBrow();

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

class CoWow;
class CoWowTimer;

//! The navigation area of the attribute editor.
class GsdmlAttrNav {
  public:
    void 		*parent_ctx;
    pn_gsdml		*gsdml;
    char 		name[80];
    GsdmlAttrNavBrow	*brow;
    attr_sItem  	*itemlist;
    int			item_cnt;
    int 		edit_mode;
    int			trace_started;
    void 		(*message_cb)( void *, char, const char *);
    void 		(*change_value_cb)( void *);
    CoWow		*wow;
    CoWowTimer		*trace_timerid;
    unsigned int	device_num;
    gsdml_DeviceAccessPointItem *device_item;
    int			device_confirm_active;
    int			modified;
    GsdmlDeviceData	dev_data;
    int			device_read;
    int			viewio;
    unsigned int 	time_ratio;
    
    GsdmlAttrNav(
	void *xn_parent_ctx,
	const char *xn_name,
	pn_gsdml  *xn_gsd,
	int	xn_edit_mode,
	pwr_tStatus *status);
    virtual ~GsdmlAttrNav();

    virtual void set_inputfocus() {}
    
    void start_trace( pwr_tObjid Objid, char *object_str);
    int set_attr_value( char *value_str);
    int check_attr_value( char **value);
    int get_select( pwr_sAttrRef *attrref, int *is_attr);
    void message( char sev, const char *text);
    void force_trace_scan();
    int object_attr();
    void print( char *filename);
    int get_select( ItemPn **item);
    void zoom( double zoom_factor);
    void unzoom();
    void get_zoom( double *zoom_factor);
    void set_modified( int value) { modified = value;}
    int is_modified() { return modified;}
    int save( const char *filename);
    int open( const char *filename);
    void redraw();
    void set_viewio( int set) { viewio = set;}

    static void trace_scan( void *data);
    static int trace_scan_bc( brow_tObject object, void *p);
    static int trace_connect_bc( brow_tObject object, char *name, char *attr, 
				 flow_eTraceType type, void **p);
    static int trace_disconnect_bc( brow_tObject object);
    static int init_brow_cb( FlowCtx *fctx, void *client_data);
    static int attr_string_to_value( int type_id, char *value_str, 
				     void *buffer_ptr, int buff_size, int attr_size);
    static void attrvalue_to_string( int type_id, void *value_ptr, 
				     char *str, int size, int *len, char *format);
    static int brow_cb( FlowCtx *ctx, flow_tEvent event);
    static void device_changed_ok( void *ctx, void *data);
    static void device_changed_cancel( void *ctx, void *data);
};

class ItemPn {
 public:
  ItemPn() : parent(0) {}
  attrnav_eItemType	type;
  brow_tNode		node;
  char	 		name[120];
  int			parent;

  virtual ~ItemPn() {}
  
  virtual int	       	open_children( GsdmlAttrNav *attrnav, double x, double y) { return 1;}
  virtual int	       	close( GsdmlAttrNav *attrnav, double x, double y);
  virtual int 	       	scan( GsdmlAttrNav *attrnav, void *p) { return 1;}
  virtual void 	       	update( GsdmlAttrNav *attrnav) {}
  virtual void 		value_changed( GsdmlAttrNav *attrnav, char *value_str) {}
};

//! Item for a normal attribute.
class ItemPnBase : public ItemPn {
  public:
    ItemPnBase( GsdmlAttrNav *attrnav, const char *item_name, const char *attr,
	int attr_type, int attr_size, double attr_min_limit,
	double attr_max_limit, void *attr_value_p, int attr_noedit,
	brow_tNode dest, flow_eDest dest_code);
    void		*value_p;
    char 		old_value[80];
    int 		first_scan;
    int			type_id;
    int			size;
    double		min_limit;
    double		max_limit;
    int			noedit;
    int			subgraph;

    virtual int 	scan( GsdmlAttrNav *attrnav, void *p);
    virtual void 	value_changed( GsdmlAttrNav *attrnav, char *value_str);
};

//! Item for an enum attribute.
class ItemPnEnumValue : public ItemPn {
  public:
    ItemPnEnumValue( GsdmlAttrNav *attrnav, const char *item_name, int item_num, 
	int item_type_id,
	void *attr_value_p, brow_tNode dest, flow_eDest dest_code);
    int			num;
    int			type_id;
    void		*value_p;
    int 		old_value;
    int 		first_scan;

    int scan( GsdmlAttrNav *attrnav, void *p);
};

//! Item for a device.
class ItemPnDevice : public ItemPn {
 public:
  ItemPnDevice( GsdmlAttrNav *attrnav, const char *item_name,
		brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemPnDevice() {}

  int 	    old_value;
  int 	    first_scan;
  
  int open_children( GsdmlAttrNav *attrnav, double x, double y);
  int scan( GsdmlAttrNav *attrnav, void *p);
};

//! Item for a slot.
class ItemPnSlot : public ItemPn {
 public:
  ItemPnSlot( GsdmlAttrNav *attrnav, const char *item_name, GsdmlSlotData *item_slotdata,
	      brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemPnSlot() {}

  GsdmlSlotData *slotdata;
  int 	    old_value;
  int 	    first_scan;
  
  int open_children( GsdmlAttrNav *attrnav, double x, double y);
  int scan( GsdmlAttrNav *attrnav, void *p);
};

//! Item for a subslot.
class ItemPnSubslot : public ItemPn {
 public:
  ItemPnSubslot( GsdmlAttrNav *attrnav, const char *item_name, GsdmlSubslotData *item_subslotdata, 
		 gsdml_VirtualSubmoduleItem *item_virtualsubmodule,
		 brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemPnSubslot() {}

  GsdmlSubslotData *subslotdata;
  gsdml_VirtualSubmoduleItem *virtualsubmodule;
  
  int open_children( GsdmlAttrNav *attrnav, double x, double y);
};

//! Item for the DeviceAccessPoint, slot 0
class ItemPnDAP : public ItemPn {
 public:
  ItemPnDAP( GsdmlAttrNav *attrnav, const char *item_name, GsdmlSlotData *item_slotdata,
	     brow_tNode dest, flow_eDest dest_code);
  GsdmlSlotData *slotdata;
  virtual ~ItemPnDAP() {}

  virtual int  open_children( GsdmlAttrNav *attrnav, double x, double y);
};

//! Item for Network settings
class ItemPnNetwork : public ItemPn {
 public:
  ItemPnNetwork( GsdmlAttrNav *attrnav, const char *item_name,
		 brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemPnNetwork() {}

  virtual int  open_children( GsdmlAttrNav *attrnav, double x, double y);
};

//! Item for Network settings
class ItemPnDeviceInfo : public ItemPn {
 public:
  ItemPnDeviceInfo( GsdmlAttrNav *attrnav, const char *item_name,
		 brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemPnDeviceInfo() {}

  virtual int  open_children( GsdmlAttrNav *attrnav, double x, double y);
};

//! Item for an InterfaceSubmoduleItem
class ItemPnInterfaceSubmodule : public ItemPn {
 public:
  ItemPnInterfaceSubmodule( GsdmlAttrNav *attrnav, const char *item_name,
			    gsdml_InterfaceSubmoduleItem *item_ii, 
			    GsdmlSubslotData *item_subslotdata,			    
			    brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemPnInterfaceSubmodule() {}

  gsdml_InterfaceSubmoduleItem *ii;
  GsdmlSubslotData *subslotdata;			    
  virtual int  open_children( GsdmlAttrNav *attrnav, double x, double y);
};

//! Item for a PortSubmoduleItem
class ItemPnPortSubmodule : public ItemPn {
 public:
  ItemPnPortSubmodule( GsdmlAttrNav *attrnav, const char *item_name,
		       gsdml_PortSubmoduleItem *item_pi,
		       GsdmlSubslotData *item_subslotdata,			    
		       brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemPnPortSubmodule() {}

  gsdml_PortSubmoduleItem *pi;
  GsdmlSubslotData *subslotdata;			    
  virtual int  open_children( GsdmlAttrNav *attrnav, double x, double y);
};

//! Item for a moduleinfo.
class ItemPnModuleInfo : public ItemPn {
 public:
  ItemPnModuleInfo( GsdmlAttrNav *attrnav, const char *item_name, gsdml_ModuleInfo *item_info,
		brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemPnModuleInfo() {}

  gsdml_ModuleInfo *info;

  virtual int  open_children( GsdmlAttrNav *attrnav, double x, double y);
};

//! Item for module type selection.
class ItemPnModuleType : public ItemPn {
 public:
  ItemPnModuleType( GsdmlAttrNav *attrnav, const char *item_name, int item_slot_number,
		    int item_slot_idx, brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemPnModuleType() {}

  int	    slot_number;
  int	    slot_idx;
  int 	    old_value;
  int 	    first_scan;
  
  int open_children( GsdmlAttrNav *attrnav, double x, double y);
  int scan( GsdmlAttrNav *attrnav, void *p);
};

//! Item for module type selection.
class ItemPnParRecord : public ItemPn {
 public:
  ItemPnParRecord( GsdmlAttrNav *attrnav, const char *item_name, 
		   gsdml_ParameterRecordDataItem *item_par_record, 
		   GsdmlDataRecord *item_datarecord, brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemPnParRecord() {}

  gsdml_ParameterRecordDataItem *par_record;
  GsdmlDataRecord *datarecord;
  
  virtual int	       	open_children( GsdmlAttrNav *attrnav, double x, double y);
};

//! Item for module type selection.
class ItemPnParValue : public ItemPn {
 public:
  ItemPnParValue( GsdmlAttrNav *attrnav, const char *item_name, gsdml_Ref *item_value_ref,
		  gsdml_eValueDataType item_datatype, unsigned char *item_data, 
		  brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemPnParValue() {}

  gsdml_Ref *value_ref;
  gsdml_eValueDataType datatype;
  unsigned char *data;
  unsigned int byte_offset;
  unsigned int size;
  int first_scan;
  char old_value[80];
  int noedit;

  int scan( GsdmlAttrNav *attrnav, void *p);
  void value_changed( GsdmlAttrNav *attrnav, char *value_str);
};

class ParEnumValue {
 public:
  ParEnumValue() {}
  unsigned int value;
  char text[160];
};

//! Item for module type selection.
class ItemPnParEnum : public ItemPn {
 public:
  ItemPnParEnum( GsdmlAttrNav *attrnav, const char *item_name, gsdml_Ref *item_value_ref,
		 gsdml_eValueDataType item_datatype, unsigned char *item_data, 
		 brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemPnParEnum() {}

  gsdml_Ref *value_ref;
  gsdml_eValueDataType datatype;
  unsigned char *data;
  unsigned int byte_offset;
  unsigned int bit_offset;
  unsigned int bit_length;
  vector<ParEnumValue> values;
  unsigned int mask;
  unsigned int old_value;
  int first_scan;
  int noedit;
  
  virtual int	       	open_children( GsdmlAttrNav *attrnav, double x, double y);
  int 			scan( GsdmlAttrNav *attrnav, void *p);
};

//! Item for module type selection.
class ItemPnParEnumBit : public ItemPn {
 public:
  ItemPnParEnumBit( GsdmlAttrNav *attrnav, const char *item_name, 
		    gsdml_eValueDataType item_datatype, 
		    unsigned char *item_data, unsigned int item_byte_offset, 
		    unsigned int item_value, unsigned int item_mask,
		    int item_noedit, brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemPnParEnumBit() {}

  gsdml_Assign *assign;
  gsdml_eValueDataType datatype;
  unsigned char *data;
  unsigned int byte_offset;
  unsigned int value;
  unsigned int mask;
  int first_scan;
  unsigned int old_value;
  int noedit;

  int 			scan( GsdmlAttrNav *attrnav, void *p);
  void 			update( GsdmlAttrNav *attrnav);
};

//! Item for a IOData.
class ItemPnIOData : public ItemPn {
 public:
  ItemPnIOData( GsdmlAttrNav *attrnav, const char *item_name, gsdml_IOData *item_iodata, 
		 brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemPnIOData() {}

  gsdml_IOData *iodata;
  
  int open_children( GsdmlAttrNav *attrnav, double x, double y);
};

//! Item for a Input.
class ItemPnInput : public ItemPn {
 public:
  ItemPnInput( GsdmlAttrNav *attrnav, const char *item_name, gsdml_Input *item_input, 
	       brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemPnInput() {}

  gsdml_Input *input;
  
  int open_children( GsdmlAttrNav *attrnav, double x, double y);
};

//! Item for a Output.
class ItemPnOutput : public ItemPn {
 public:
  ItemPnOutput( GsdmlAttrNav *attrnav, const char *item_name, gsdml_Output *item_output, 
	       brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemPnOutput() {}

  gsdml_Output *output;
  
  int open_children( GsdmlAttrNav *attrnav, double x, double y);
};

//! Item for a DataItem.
class ItemPnDataItem : public ItemPn {
 public:
  ItemPnDataItem( GsdmlAttrNav *attrnav, const char *item_name, gsdml_DataItem *item_dataitem, 
	       brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemPnDataItem() {}

  gsdml_DataItem *dataitem;
  
  int open_children( GsdmlAttrNav *attrnav, double x, double y);
};

//! Item for a BitDataItem.
class ItemPnBitDataItem : public ItemPn {
 public:
  ItemPnBitDataItem( GsdmlAttrNav *attrnav, const char *item_name, gsdml_BitDataItem *item_bitdataitem, 
	       brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemPnBitDataItem() {}

  gsdml_BitDataItem *bitdataitem;
  
  int open_children( GsdmlAttrNav *attrnav, double x, double y);
};

//! Item for module class selection.
class ItemPnModuleClass : public ItemPn {
 public:
  ItemPnModuleClass( GsdmlAttrNav *attrnav, const char *item_name, GsdmlSlotData *item_slotdata,
		     brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemPnModuleClass() {}

  GsdmlSlotData *slotdata;
  int 	    old_value;
  int 	    first_scan;
  
  int open_children( GsdmlAttrNav *attrnav, double x, double y);
  int scan( GsdmlAttrNav *attrnav, void *p);
};

//! Item for slave byte order.
class ItemPnEnumByteOrder : public ItemPnBase {
 public:
  ItemPnEnumByteOrder( GsdmlAttrNav *attrnav, const char *item_name, const char *attr, 
		       int attr_type, int attr_size, void *attr_value_p, int attr_noedit,
		       brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemPnEnumByteOrder() {}
  
  int 	    old_value;
  
  int	open_children( GsdmlAttrNav *attrnav, double x, double y);
  int scan( GsdmlAttrNav *attrnav, void *p);
};

//! Item for time ratio.
class ItemPnEnumTimeRatio : public ItemPn {
 public:
  ItemPnEnumTimeRatio( GsdmlAttrNav *attrnav, const char *item_name, 
		       gsdml_InterfaceSubmoduleItem *item_interfacesubmodule,
		       void *attr_value_p, brow_tNode dest, flow_eDest dest_code);
  virtual ~ItemPnEnumTimeRatio() {}

  gsdml_InterfaceSubmoduleItem *interfacesubmodule;
  void		*value_p;
  int 		first_scan;
  int 	    old_value;

  int open_children( GsdmlAttrNav *attrnav, double x, double y);
  int scan( GsdmlAttrNav *attrnav, void *p);
};

/*@}*/
#if defined __cplusplus
}
#endif
#endif
