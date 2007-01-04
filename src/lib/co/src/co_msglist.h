/** 
 * Proview   $Id: co_msglist.h,v 1.5 2007-01-04 07:51:42 claes Exp $
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

#ifndef xtt_msglist_h
#define xtt_msglist_h

/* xtt_msglist.h -- Message list */

// Status is defined as int i xlib...
#ifdef Status
# undef Status
#endif

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#if defined __cplusplus
}
#endif

#ifndef flow_h
#include "flow.h"
#endif

#ifndef flow_browctx_h
#include "flow_browctx.h"
#endif

#ifndef flow_browapi_h
#include "flow_browapi.h"
#endif

typedef enum {
	msglist_eItemType_Msg
	} msglist_eItemType;

class MsgListBrow {
  public:
    MsgListBrow( BrowCtx *brow_ctx, void *msgl) : ctx(brow_ctx), msglist(msgl) {};
    ~MsgListBrow();

    BrowCtx		*ctx;
    void		*msglist;
    brow_tNodeClass 	nc_info;
    brow_tNodeClass 	nc_error;
    brow_tNodeClass 	nc_warning;
    brow_tNodeClass 	nc_success;
    brow_tNodeClass 	nc_fatal;
    brow_tNodeClass 	nc_default;
    flow_sAnnotPixmap 	*pixmap_morehelp;

    void free_pixmaps();
    void allocate_pixmaps();
    void create_nodeclasses();
    void brow_setup();
};

class MsgList {
  public:
    MsgList(
	void *ev_parent_ctx);
    virtual ~MsgList();

    void 		*parent_ctx;
    MsgListBrow		*brow;
    void 		(*find_wnav_cb)( void *, pwr_tObjid);
    void 		(*find_plc_cb)( void *, pwr_tObjid);

    virtual void set_input_focus() {}

    void zoom( double zoom_factor);
    void unzoom();
    void set_nodraw();
    void reset_nodraw();
    void remove_oldest();
    void clear();

    static int init_brow_cb( FlowCtx *fctx, void *client_data);
    static int brow_cb( FlowCtx *ctx, flow_tEvent event);
};

class ItemMsg {
  public:
    ItemMsg( MsgList *msglist, char *item_name,
	     char *item_text, int item_severity,
	     brow_tNode dest, flow_eDest dest_code);
    msglist_eItemType	type;
    MsgList		*msglist;
    brow_tNode		node;
    pwr_tTime		time;
    char		text[200];
    int			severity;

    virtual void find() {}

    virtual ~ItemMsg() {}
};

class ItemMsgObject : public ItemMsg {
 public:
  ItemMsgObject( MsgList *msglist, char *item_name,
		 char *item_text, int item_severity, pwr_tOid item_oid,
		 brow_tNode dest, flow_eDest dest_code);

  pwr_tOid		oid;

  void find();
};

class ItemMsgObjectPlc : public ItemMsg {
 public:
  ItemMsgObjectPlc( MsgList *msglist, char *item_name,
		 char *item_text, int item_severity, pwr_tOid item_oid,
		 brow_tNode dest, flow_eDest dest_code);

  pwr_tOid		oid;

  void find();
};

#endif



