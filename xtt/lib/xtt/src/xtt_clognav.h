#ifndef xtt_clognav_h
#define xtt_clognav_h

/* xtt_clognav.h -- Console message window.

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */


// Status is defined as int i xlib...

#include <vector>
using namespace std;

#ifdef Status
# undef Status
#endif

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef rt_errh_h
# include "rt_errh.h"
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
  clognav_eItemType_Msg,
  clognav_eItemType_Restart
} clognav_eItemType;

class CLogNavFilter {
 public:
  CLogNavFilter() :
    show_success(true), show_info(true), show_warning(true), show_error(true),
    show_fatal(true), show_text(true) 
    { strcpy( str, "");}
  bool show_success;
  bool show_info;
  bool show_warning;
  bool show_error;
  bool show_fatal;
  bool show_text;
  char str[200];
};

class CLogNavBrow {
  public:
    CLogNavBrow( BrowCtx *brow_ctx, void *evl) : ctx(brow_ctx), clognav(evl) {};
    ~CLogNavBrow();

    BrowCtx		*ctx;
    void		*clognav;
    brow_tNodeClass 	nc_event;
    brow_tNodeClass 	nc_msg_info;
    brow_tNodeClass 	nc_msg_warning;
    brow_tNodeClass 	nc_msg_error;
    brow_tNodeClass 	nc_msg_fatal;
    brow_tNodeClass 	nc_restart;
    brow_tNodeClass 	nc_text;
    flow_sAnnotPixmap 	*pixmap_leaf;
    flow_sAnnotPixmap 	*pixmap_map;
    flow_sAnnotPixmap 	*pixmap_openmap;

    void free_pixmaps();
    void allocate_pixmaps();
    void create_nodeclasses();
    void brow_setup();
};

class CLogMsg {
 public:
  CLogMsg( errh_eSeverity msg_severity, char *msg_logger, int msg_pid,
       pwr_tTime msg_time, char *msg_text) :
    severity(msg_severity), pid(msg_pid), time(msg_time)
    { 
      strncpy( logger, msg_logger, sizeof(logger));
      strncpy( text, msg_text, sizeof(text)); 
    }
  errh_eSeverity 	severity;
  char 			logger[40];
  int			pid;
  pwr_tTime		time;
  char			text[200];
};

class CLogFile {
 public:
  CLogFile() 
    {
      strcpy( name, "");
    }
  CLogFile( char *file_name, pwr_tTime file_time) :
    time( file_time)
    {
      strcpy( name, file_name);
    }
  char name[200];
  pwr_tTime time;
};

class CLogNav {
  public:
    CLogNav(
	void *ev_parent_ctx,
	Widget	ev_parent_wid,
	Widget *w);
    ~CLogNav();

    void 		*parent_ctx;
    Widget		parent_wid;
    Widget		brow_widget;
    Widget		form_widget;
    Widget		toplevel;
    CLogNavBrow		*brow;
    CLogNavFilter	filter;
    int			clog_size;
    int			max_size;
    vector<CLogMsg>    	msg_list;		
    vector<CLogFile>    file_list;		
    int			current_pos_high;
    int			current_pos_low;

    void set_input_focus();
    void zoom( double zoom_factor);
    void unzoom();
    void set_nodraw();
    void reset_nodraw();
    void read( int *idx_list, int idx_cnt);
    void set_filter( bool success, bool info, bool warning, bool error, bool fatal,
		 bool text, char *str);
    void get_filter( bool *success, bool *info, bool *warning, bool *error, bool *fatal,
		 bool *text);
    void draw();
    void get_files();
    int next_file();
    int prev_file();
};

class ItemMsgBase {
 public:
  ItemMsgBase( CLogNav *item_clognav, char *item_name, brow_tNode dest) :
    clognav(item_clognav), node(dest)
    {
      strcpy( name, item_name);
    }
    clognav_eItemType	type;
    CLogNav		*clognav;
    brow_tNode		node;
    char	 	name[40];
};

class ItemMsgRestart : public ItemMsgBase {
 public:
    ItemMsgRestart( CLogNav *clognav, char *item_name, pwr_tTime item_time,
	brow_tNode dest, flow_eDest dest_code);
    pwr_tTime time;
};

class ItemMsg : public ItemMsgBase {
  public:
    ItemMsg( CLogNav *clognav, char *item_name, errh_eSeverity item_severity, 
	char *item_logger, int item_pid, pwr_tTime item_time, char *item_text,
	brow_tNode dest, flow_eDest dest_code);
    errh_eSeverity	severity;
    char		logger[40];
    int			pid;
    pwr_tTime		time;
    char		text[200];
};

#endif


