#ifndef ge_adminnav_h
#define ge_adminnav_h

/* xtt_adminnav.h -- Simple navigator

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

#ifndef flow_browapi_h
#include "flow_browapi.h"
#endif

#ifndef wb_wnav_brow_h
#include "wb_wnav_brow.h"
#endif

#define adminnav_cVersion	"X3.0b"

typedef struct adminnav_s_Menu {
	char			name[80];
	int			item_type;
	char			project[120];
	char			base[120];
	char			root[120];
	char			hierarchy[120];
	char			description[120];
 	int			pixmap;
	struct adminnav_s_Menu	*child_list;
	struct adminnav_s_Menu	*parent;
	struct adminnav_s_Menu	*next;
} adminnav_sMenu;

typedef struct adminnav_s_Base {
	char			name[80];
	char			root[120];
	struct adminnav_s_Base	*next;
} adminnav_sBase;

typedef enum {
	adminnav_eItemType_ProjHier,
	adminnav_eItemType_Project,
	adminnav_eItemType_Db,
	adminnav_eItemType_Volume
	} adminnav_eItemType;

typedef enum {
	adminnav_mOpen_All	= ~0,
	adminnav_mOpen_Children	= 1 << 0,
	adminnav_mOpen_Attributes = 1 << 1,
	adminnav_mOpen_Crossref	= 1 << 2
	} adminnav_mOpen;

class AdminNav {
  public:
    AdminNav(
	void *xn_parent_ctx,
	Widget	xn_parent_wid,
	char *xn_name,
	Widget *w,
	pwr_tStatus *status);
    ~AdminNav();

    void 		*parent_ctx;
    Widget		parent_wid;
    char 		name[80];
    Widget		brow_widget;
    Widget		form_widget;
    Widget		toplevel;
    WNavBrow		*brow;
    XtIntervalId	trace_timerid;
    int			trace_started;
    void 		(*message_cb)( void *, char, char *);
    int 		(*set_focus_cb)( void *, void *);
    adminnav_sMenu	*menu_tree;
    adminnav_sBase	*baselist;
    int			displayed;

    void message( char sev, char *text);
    void tree_build();
    void tree_load();
    void tree_free();
    void set_inputfocus( int focus);
    void insert_tree( adminnav_sMenu *menu, adminnav_sMenu *parent, char *hierarchy);
    void enable_events();
    int get_select( void **item);
    int basename_to_baseroot( char *name, char *root);
    void refresh();
    int print( char *filename);
    void zoom( double zoom_factor);
    void get_zoom( double *zoom_factor);
    void unzoom();

};

class Item {
  public:
    Item( adminnav_eItemType	item_type) :
	type( item_type) {};
    adminnav_eItemType	type;
};

class ItemProjHier : public Item {
  public:
    ItemProjHier( AdminNav *adminnav, char *item_name, char *item_hier,
	adminnav_sMenu *item_child_list,
	brow_tNode dest, flow_eDest dest_code);
    brow_tNode		node;
    char	 	name[120];
    char		hierarchy[120];
    adminnav_sMenu	*child_list;

    int			open_children( AdminNav *adminnav, double x, double y);
    int			close( AdminNav *adminnav, double x, double y);
};

class ItemProject : public Item {
  public:
    ItemProject( AdminNav *adminnav, char *item_name,
	char *item_project, char *item_base, char *item_root, 
	char *item_hierarchy, char *item_descr,
	brow_tNode dest, flow_eDest dest_code);
    brow_tNode		node;
    char	 	name[120];
    char	 	project[120];
    char	 	base[120];
    char	 	root[120];
    char	 	hierarchy[120];
    char	 	description[120];
    int			open_children( AdminNav *adminnav, double x, double y);
    int     		close( AdminNav *adminnav, double x, double y);
};

class ItemDb : public Item {
  public:
    ItemDb( AdminNav *adminnav, char *item_name,
	char *item_project, char *item_db, char *item_root,
	brow_tNode dest, flow_eDest dest_code);
    brow_tNode		node;
    char	 	name[120];
    char	 	project[120];
    char	 	db[120];
    char	 	hierarchy[120];
    char	 	root[120];
    int			open_children( AdminNav *adminnav, double x, double y);
    int     		close( AdminNav *adminnav, double x, double y);
};

class ItemVolume : public Item {
  public:
    ItemVolume( AdminNav *adminnav, char *item_name,
	char *item_project, char *item_db, char *item_volume, char *item_root,
	brow_tNode dest, flow_eDest dest_code);
    brow_tNode		node;
    char	 	name[120];
    char	 	project[120];
    char	 	db[120];
    char	 	hierarchy[120];
    char	 	volume[120];
    char	 	root[120];
};

#if defined __cplusplus
}
#endif
#endif
