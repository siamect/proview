#ifndef wb_wpkgnav_h
#define wb_wpkgnav_h

/* wb_wpkgnav.h -- Simple navigator

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef wb_ldh_h
# include "wb_ldh.h"
#endif

#ifndef flow_h
#include "flow.h"
#endif

#ifndef flow_browapi_h
#include "flow_browapi.h"
#endif

#ifndef wb_wnav_brow_h
#include "wb_wnav_brow.h"
#endif

#define wpkgnav_cVersion	"X3.3b"
#define WPKGNAV_BROW_MAX	25

typedef enum {
  wpkg_mDisplayMode__ 			= 0,
  wpkg_mDisplayMode_FileOrderTime 	= 1 << 0,
  wpkg_mDisplayMode_FileDiff		= 1 << 1,
  wpkg_mDisplayMode_FilePath		= 1 << 2
} wpkg_mDisplayMode;

class WItemPkg;
class WItemPkgPackage;

class WPkgNav {
  public:
    WPkgNav(
	void 		*wa_parent_ctx,
	Widget		wa_parent_wid,
	char 		*wa_name,
	wb_eUtility	wa_utility,
	Widget 		*w,
	pwr_tStatus 	*status);
    ~WPkgNav();

    void 		*parent_ctx;
    Widget		parent_wid;
    char 		name[80];
    Widget		brow_widget;
    Widget		form_widget;
    Widget		toplevel;
    WNavBrow		*brow;
    void 		(*message_cb)( void *, char, char *);
    void 		(*set_clock_cursor_cb)( void *);
    void 		(*reset_cursor_cb)( void *);
    void 		(*change_value_cb)( void *);
    wb_eUtility		utility;
    int			displayed;
    int			display_mode;

    void message( char sev, char *text);
    void set_inputfocus();
    int root_objects();
    void redraw();
    void enable_events();
    void set_display_mode( int mode) { display_mode = mode;}
    int get_select( WItemPkg ***items, int *item_cnt);
    void refresh_node( WItemPkg *item);
    WItemPkg *get_parent( WItemPkg *item);
    void delete_package( WItemPkg *item);
    void delete_package( WItemPkgPackage *item);
};

class WItemPkg {
  public:
    WItemPkg() : node(0) {};
    virtual int open_children( WNavBrow *brow, double x, double y, int display_mode) 
      { return 1;}
    virtual int close( WNavBrow *brow, double x, double y);

    brow_tNode		node;
    char	 	name[120];
};

class WItemPkgNode : public WItemPkg {
  public:
    WItemPkgNode( WNavBrow *brow, char *item_name, char *item_nodename, int item_bus,
		  pwr_mOpSys item_opsys, brow_tNode dest, flow_eDest dest_code);
    int	 open_children( WNavBrow *brow, double x, double y, int display_mode);
    char nodename[32];
    int bus;
    pwr_mOpSys opsys;
};

class WItemPkgPackage : public WItemPkg {
  public:
    WItemPkgPackage( WNavBrow *brow, char *item_name, char *item_packagename, pwr_tTime item_time,
		  brow_tNode dest, flow_eDest dest_code);
    int	 open_children( WNavBrow *brow, double x, double y, int display_mode);
    char packagename[120];
    pwr_tTime time;
};

class WItemPkgFile : public WItemPkg {
  public:
    WItemPkgFile( WNavBrow *brow, char *item_name, char *item_filename, pwr_tTime item_time,
		  int dmode, brow_tNode dest, flow_eDest dest_code);
    char filename[120];
    pwr_tTime time;
};

#if defined __cplusplus
}
#endif
#endif




