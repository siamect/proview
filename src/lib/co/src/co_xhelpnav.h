/** 
 * Proview   $Id: co_xhelpnav.h,v 1.11 2008-10-31 12:51:30 claes Exp $
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

#ifndef co_xhelpnav_h
#define co_xhelpnav_h

/* co_xhelpnav.h -- Helptext navigator */

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

#ifndef flow_browwidget_motif_h
// #include "flow_browwidget_motif.h"
#endif

#define xhelp_cFile_BaseXtt "$pwr_lang/xtt_help.dat"
#define xhelp_cFile_BaseWtt "$pwr_lang/wtt_help.dat"
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
    navh_eHelpFile	current_filetype;
    char		current_key[200];
    pwr_tFileName      	current_filename;

    void free_pixmaps();
    void allocate_pixmaps();
    void create_nodeclasses();
    void brow_setup();
};

class CoXHelpNav {
  public:
    CoXHelpNav(
	void *xn_parent_ctx,
	char *xn_name,
	xhelp_eUtility xn_utility,
	pwr_tStatus *status);
    virtual ~CoXHelpNav();

    void 		*parent_ctx;
    char 		name[80];
    CoXHelpNavBrow     	*brow;
    CoXHelpNavBrow     	*collect_brow;
    CoXHelpNavBrow     	*brow_stack[XHELPNAV_BROW_MAX];
    int			brow_cnt;
    int			closing_down;
    int			displayed;
    int                 init_help;
    xhelp_eUtility	utility;
    brow_tNode		search_node;
    char		search_str[80];
    bool 		search_strict;
    void 		(*open_URL_cb)( void *, char *);

    virtual void set_inputfocus() {}
    virtual void pop() {}

    void print( char *filename);
    void zoom( double zoom_factor);
    void unzoom();
    void get_zoom( double *zoom_factor);
    void clear();
    void message( char sev, char *text);
    int brow_pop();
    int brow_push();
    int brow_push_all();
    void enable_events( CoXHelpNavBrow *brow);
    int help( const char *key, const char *help_bookmark, navh_eHelpFile file_type,
	      const char *file_name, int pop, bool strict);
    int home();
    int back();
    int next_topic();
    int previous_topic();
    int	help_index( navh_eHelpFile file_type, const char *file_name, int pop);
    pwr_tStatus search( char *str, bool strict);
    pwr_tStatus search_next();
    pwr_tStatus search_next_reverse();
    pwr_tStatus search_exec( bool reverse);

    static int init_brow_cb( BrowCtx *ctx, void *client_data);
    static int init_brow_base_cb( FlowCtx *fctx, void *client_data);

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

    virtual ~HItem() {}
};

class HItemHeader : public HItem {
  public:
    HItemHeader( CoXHelpNavBrow *brow, const char *item_name, const char *title,
	brow_tNode dest, flow_eDest dest_code);
};

class HItemHelpLine : public HItem {
  public:
    HItemHelpLine( CoXHelpNavBrow *brow, const char *item_name,
	brow_tNode dest, flow_eDest dest_code);
};

class HItemHelpImage : public HItem {
  public:
    HItemHelpImage( CoXHelpNavBrow *brow, const char *item_name, brow_tNodeClass nc,
		    const char *item_link, const char *item_bookmark, 
		    const char *item_file_name, navh_eHelpFile help_file_type, 
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
    HItemHeaderLarge( CoXHelpNavBrow *brow, const char *item_name, const char *title,
	brow_tNode dest, flow_eDest dest_code);
    bool search( char *str, bool strict);
};

class HItemHelp : public HItem {
  public:
    HItemHelp( CoXHelpNavBrow *brow, const char *item_name, const char *text, const char *text2, 
	       const char *text3, const char *item_link, const char *item_bookmark, 
	       const char *item_file_name, navh_eHelpFile help_file_type, int help_index, 
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

class HItemHelpBold : public HItem {
 public:
  HItemHelpBold( CoXHelpNavBrow *brow, const char *item_name, const char *text, const char *text2, 
		const char *text3, const char *item_link, const char *item_bookmark, 
		const char *item_file_name, navh_eHelpFile help_file_type, int help_index, 
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
  HItemHelpHeader( CoXHelpNavBrow *brow, const char *item_name, const char *title, bool base,
		  brow_tNode dest, flow_eDest dest_code);
  int doubleclick_action( CoXHelpNavBrow *brow, CoXHelpNav *xhelpnav, double x, double y);
    bool search( char *str, bool strict);
};


#endif



