#ifndef co_xhelpnav_h
#define co_xhelpnav_h

/* co_xhelpnav.h -- Simple navigator

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef co_nav_help_h
#include "co_nav_help.h"
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
	xhelpnav_eHItemType_HelpHeader,
	xhelpnav_eHItemType_HelpLine,
	xhelpnav_eHItemType_HelpImage
	} xhelpnav_eHItemType;

typedef enum {
	xhelp_eUtility_Xtt,
	xhelp_eUtility_Wtt
	} xhelp_eUtility;

class CoXHelpNavBrow {
  public:
    CoXHelpNavBrow( BrowCtx *brow_ctx, void *brow_userdata) : 
      ctx(brow_ctx), userdata(brow_userdata)
      {}
#if 0
    ~CoXHelpNavBrow();
#endif

    BrowCtx		*ctx;
    void                *userdata;
    brow_tNodeClass 	nc_object;
    brow_tNodeClass 	nc_text;
    brow_tNodeClass 	nc_header;
    brow_tNodeClass 	nc_headerlarge;
    brow_tNodeClass 	nc_line;
    flow_sAnnotPixmap 	*pixmap_morehelp;
    flow_sAnnotPixmap 	*pixmap_closehelp;

    void free_pixmaps();
    void allocate_pixmaps();
    void create_nodeclasses();
    void brow_setup();
};

class CoXHelpNav {
  public:
    CoXHelpNav(
	void *xn_parent_ctx,
	Widget	xn_parent_wid,
	char *xn_name,
	xhelp_eUtility xn_utility,
	Widget *w,
	pwr_tStatus *status);
    ~CoXHelpNav();

    void 		*parent_ctx;
    Widget		parent_wid;
    char 		name[80];
    Widget		brow_widget;
    Widget		form_widget;
    Widget		toplevel;
    CoXHelpNavBrow		*brow;
    CoXHelpNavBrow		*collect_brow;
    CoXHelpNavBrow		*brow_stack[XHELPNAV_BROW_MAX];
    int			brow_cnt;
    int			closing_down;
    int			displayed;
    int                 init_help;
    xhelp_eUtility	utility;
    brow_tNode		search_node;
    char		search_str[80];
    bool 		search_strict;
    void 		(*open_URL_cb)( void *, char *);

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
    void enable_events( CoXHelpNavBrow *brow);
    int help( char *key, char *help_bookmark, navh_eHelpFile file_type,
	      char *file_name, int pop, bool strict);
    int	help_index( navh_eHelpFile file_type, char *file_name, int pop);
    pwr_tStatus search( char *str, bool strict);
    pwr_tStatus search_next();
    pwr_tStatus search_next_reverse();
    pwr_tStatus search_exec( bool reverse);
};

class HItem {
  public:
    HItem() :
	type( xhelpnav_eHItemType_Object),
	node(NULL)
	{};
    virtual int	doubleclick_action( CoXHelpNavBrow *brow, CoXHelpNav *xhelpnav,
				       double x, double y) { return 1;}
    virtual bool search( char *str, bool strict) { return false;}
    xhelpnav_eHItemType	type;
    brow_tNode		node;
};

class HItemHeader : public HItem {
  public:
    HItemHeader( CoXHelpNavBrow *brow, char *item_name, char *title,
	brow_tNode dest, flow_eDest dest_code);
};

class HItemHelpLine : public HItem {
  public:
    HItemHelpLine( CoXHelpNavBrow *brow, char *item_name,
	brow_tNode dest, flow_eDest dest_code);
};

class HItemHelpImage : public HItem {
  public:
    HItemHelpImage( CoXHelpNavBrow *brow, char *item_name, brow_tNodeClass nc,
		    char *item_link, char *item_bookmark, 
		    char *item_file_name, navh_eHelpFile help_file_type, 
		    brow_tNode dest, flow_eDest dest_code);
    int	doubleclick_action( CoXHelpNavBrow *brow, CoXHelpNav *xhelpnav,
				       double x, double y);
    char link[200];
    char bookmark[80];
    char file_name[80];
    navh_eHelpFile	 file_type;
};

class HItemHeaderLarge : public HItem {
  public:
    HItemHeaderLarge( CoXHelpNavBrow *brow, char *item_name, char *title,
	brow_tNode dest, flow_eDest dest_code);
    bool search( char *str, bool strict);
};

class HItemHelp : public HItem {
  public:
    HItemHelp( CoXHelpNavBrow *brow, char *item_name, char *text, char *text2, 
	char *text3, char *item_link, char *item_bookmark, 
	char *item_file_name, navh_eHelpFile help_file_type, int help_index, brow_tNode dest, flow_eDest dest_code);
    int	doubleclick_action( CoXHelpNavBrow *brow, CoXHelpNav *xhelpnav,
				       double x, double y);
    bool search( char *str, bool strict);
    char link[200];
    char bookmark[80];
    char file_name[80];
    navh_eHelpFile	 file_type;
    int  index;
};

class HItemHelpBold : public HItem {
 public:
  HItemHelpBold( CoXHelpNavBrow *brow, char *item_name, char *text, char *text2, 
		char *text3, char *item_link, char *item_bookmark, 
		char *item_file_name, navh_eHelpFile help_file_type, int help_index, 
		brow_tNode dest, flow_eDest dest_code);
  int	doubleclick_action( CoXHelpNavBrow *brow, CoXHelpNav *xhelpnav, 
			    double x, double y);
    bool search( char *str, bool strict);
  char link[200];
  char bookmark[80];
  char file_name[80];
  navh_eHelpFile	 file_type;
  int  index;
};

class HItemHelpHeader : public HItem {
 public:
  HItemHelpHeader( CoXHelpNavBrow *brow, char *item_name, char *title, bool base,
		  brow_tNode dest, flow_eDest dest_code);
  int doubleclick_action( CoXHelpNavBrow *brow, CoXHelpNav *xhelpnav, double x, double y);
    bool search( char *str, bool strict);
};


#endif



