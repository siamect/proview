#ifndef xtt_msglist_h
#define xtt_msglist_h

/* xtt_msglist.h -- Alarm and event windows in xtt

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */


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
	void *ev_parent_ctx,
	Widget	ev_parent_wid,
	Widget *w);
    ~MsgList();

    void 		*parent_ctx;
    Widget		parent_wid;
    Widget		brow_widget;
    Widget		form_widget;
    Widget		toplevel;
    MsgListBrow		*brow;
    void 		(*find_wnav_cb)( void *, pwr_tObjid);
    void 		(*find_plc_cb)( void *, pwr_tObjid);

    void set_input_focus();
    void zoom( double zoom_factor);
    void unzoom();
    void set_nodraw();
    void reset_nodraw();
    void remove_oldest();
    void clear();
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



