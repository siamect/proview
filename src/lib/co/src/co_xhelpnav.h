#ifndef co_xhelpnav_h
#define co_xhelpnav_h

/* co_xhelpnav.h -- Simple navigator

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef co_nav_help_h
#include "co_nav_help.h"
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

#ifndef flow_browwidget_h
#include "flow_browwidget.h"
#endif

#define xhelp_cFile_BaseXtt "$pwr_exe/xtt_help.dat"
#define xhelp_cFile_BaseWtt "$pwr_exe/wtt_help.dat"
#define xhelp_cFile_Project "$pwrp_exe/xtt_help.dat"
#define XHELPNAV_BROW_MAX	25

typedef enum {
	xhelpnav_eHItemType_Object,
	xhelpnav_eHItemType_Header,
	xhelpnav_eHItemType_HeaderLarge,
	xhelpnav_eHItemType_Help,
	xhelpnav_eHItemType_HelpBold,
	xhelpnav_eHItemType_HelpHeader
	} xhelpnav_eHItemType;

typedef enum {
	xhelp_eUtility_Xtt,
	xhelp_eUtility_Wtt
	} xhelp_eUtility;

class XHelpNavBrow {
  public:
    XHelpNavBrow( BrowCtx *brow_ctx, void *brow_userdata) : 
      ctx(brow_ctx), userdata(brow_userdata)
      {}
#if 0
    ~XHelpNavBrow();
#endif

    BrowCtx		*ctx;
    void                *userdata;
    brow_tNodeClass 	nc_object;
    brow_tNodeClass 	nc_text;
    brow_tNodeClass 	nc_header;
    brow_tNodeClass 	nc_headerlarge;
    flow_sAnnotPixmap 	*pixmap_morehelp;
    flow_sAnnotPixmap 	*pixmap_closehelp;

    void free_pixmaps();
    void allocate_pixmaps();
    void create_nodeclasses();
    void brow_setup();
};

class XHelpNav {
  public:
    XHelpNav(
	void *xn_parent_ctx,
	Widget	xn_parent_wid,
	char *xn_name,
	xhelp_eUtility xn_utility,
	Widget *w,
	pwr_tStatus *status);
    ~XHelpNav();

    void 		*parent_ctx;
    Widget		parent_wid;
    char 		name[80];
    Widget		brow_widget;
    Widget		form_widget;
    Widget		toplevel;
    XHelpNavBrow		*brow;
    XHelpNavBrow		*collect_brow;
    XHelpNavBrow		*brow_stack[XHELPNAV_BROW_MAX];
    int			brow_cnt;
    int			closing_down;
    int			displayed;
    int                 init_help;
    xhelp_eUtility	utility;

    void print( char *filename);
    void zoom( double zoom_factor);
    void unzoom();
    void get_zoom( double *zoom_factor);
    void clear();
    void message( char sev, char *text);
    int brow_pop();
    int brow_push();
    int brow_push_all();
    void set_inputfocus();
    void pop();
    void enable_events( XHelpNavBrow *brow);
    int help( char *key, char *help_bookmark, navh_eHelpFile file_type,
	      char *file_name, int pop, bool strict);
    int	help_index( navh_eHelpFile file_type, char *file_name, int pop);

};

class HItem {
  public:
    HItem() :
	type( xhelpnav_eHItemType_Object),
	node(NULL)
	{};
    virtual int	doubleclick_action( XHelpNavBrow *brow, XHelpNav *xhelpnav,
				       double x, double y) { return 1;}
    xhelpnav_eHItemType	type;
    brow_tNode		node;
};

class HItemHeader : public HItem {
  public:
    HItemHeader( XHelpNavBrow *brow, char *item_name, char *title,
	brow_tNode dest, flow_eDest dest_code);
};

class HItemHeaderLarge : public HItem {
  public:
    HItemHeaderLarge( XHelpNavBrow *brow, char *item_name, char *title,
	brow_tNode dest, flow_eDest dest_code);
};

class HItemHelp : public HItem {
  public:
    HItemHelp( XHelpNavBrow *brow, char *item_name, char *text, char *text2, 
	char *text3, char *item_link, char *item_bookmark, 
	char *item_file_name, navh_eHelpFile help_file_type, int help_index, brow_tNode dest, flow_eDest dest_code);
    int	doubleclick_action( XHelpNavBrow *brow, XHelpNav *xhelpnav,
				       double x, double y);
    char link[200];
    char bookmark[80];
    char file_name[80];
    navh_eHelpFile	 file_type;
    int  index;
};

class HItemHelpBold : public HItem {
 public:
  HItemHelpBold( XHelpNavBrow *brow, char *item_name, char *text, char *text2, 
		char *text3, char *item_link, char *item_bookmark, 
		char *item_file_name, navh_eHelpFile help_file_type, int help_index, 
		brow_tNode dest, flow_eDest dest_code);
  int	doubleclick_action( XHelpNavBrow *brow, XHelpNav *xhelpnav, 
			    double x, double y);
  char link[200];
  char bookmark[80];
  char file_name[80];
  navh_eHelpFile	 file_type;
  int  index;
};

class HItemHelpHeader : public HItem {
 public:
  HItemHelpHeader( XHelpNavBrow *brow, char *item_name, char *title, bool base,
		  brow_tNode dest, flow_eDest dest_code);
  int doubleclick_action( XHelpNavBrow *brow, XHelpNav *xhelpnav, double x, double y);
};


#endif



