#ifndef wb_pal_h
#define wb_pal_h

/* wb_pal.h -- Palette

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

#define pal_cPaletteFile "$pwr_exe/pwr_wb_palette.cnf"
#define pal_cLocalPaletteFile "$pwrp_login/pwrp_wb_palette.cnf"

typedef enum {
  pal_eNameType_TopObjects,
  pal_eNameType_Palette
} pal_eNameType;

typedef enum {
  pal_eMenuType_Menu,
  pal_eMenuType_Class,
  pal_eMenuType_ClassVolume
} pal_eMenuType;

typedef struct pal_s_Menu {
	char				title[80];
	int				item_type;
	char				file[120];
 	int				pixmap;
	struct pal_s_Menu	*child_list;
	struct pal_s_Menu	*parent;
	struct pal_s_Menu	*next;
} pal_sMenu;


class Pal {
  public:
    Pal(
	void *parent_ctx,
	Widget	parent_wid,
	char *name,
	ldh_tSesContext ldhses,
	char *root_name,
	Widget *w,
	pwr_tStatus *status
	);
    ~Pal();

    void 	*parent_ctx;
    Widget	parent_wid;
    char 	name[80];
    char 	root_name[80];
    Widget	brow_widget;
    Widget	form_widget;
    Widget	toplevel;
    BrowCtx	*brow_ctx;
    ldh_tWBContext wbctx;
    ldh_tSesContext ldhses;
    pwr_tObjid	root_objid;
    void		*root_item;
    brow_tObject	last_selected;
    int			selection_owner;
    int 		(*set_focus_cb)( void *, void *);
    int 		(*traverse_focus_cb)( void *, void *);
    int		displayed;
    pal_sMenu   *menu;

    brow_tNodeClass nc;
    flow_sAnnotPixmap *pixmap_leaf;
    flow_sAnnotPixmap *pixmap_map;
    flow_sAnnotPixmap *pixmap_openmap;
    flow_sAnnotPixmap *pixmap_ref;
    flow_sAnnotPixmap *pixmap_crossref;
    flow_sAnnotPixmap *pixmap_aarithm;
    flow_sAnnotPixmap *pixmap_arithm;
    flow_sAnnotPixmap *pixmap_bodytext;
    flow_sAnnotPixmap *pixmap_document;
    flow_sAnnotPixmap *pixmap_get;
    flow_sAnnotPixmap *pixmap_getp;
    flow_sAnnotPixmap *pixmap_head;
    flow_sAnnotPixmap *pixmap_initstep;
    flow_sAnnotPixmap *pixmap_logic1_;
    flow_sAnnotPixmap *pixmap_logic2_;
    flow_sAnnotPixmap *pixmap_order;
    flow_sAnnotPixmap *pixmap_orderact;
    flow_sAnnotPixmap *pixmap_setcond;
    flow_sAnnotPixmap *pixmap_ssbegin;
    flow_sAnnotPixmap *pixmap_ssend;
    flow_sAnnotPixmap *pixmap_step;
    flow_sAnnotPixmap *pixmap_sto;
    flow_sAnnotPixmap *pixmap_stop;
    flow_sAnnotPixmap *pixmap_substep;
    flow_sAnnotPixmap *pixmap_text;
    flow_sAnnotPixmap *pixmap_title;
    flow_sAnnotPixmap *pixmap_trans;
    flow_sAnnotPixmap *pixmap_cell;
    flow_sAnnotPixmap *pixmap_condigital;
    flow_sAnnotPixmap *pixmap_conanalog;
    flow_sAnnotPixmap *pixmap_confeedbackdigital;
    flow_sAnnotPixmap *pixmap_confeedbackanalog;
    flow_sAnnotPixmap *pixmap_conexecuteorder;
    flow_sAnnotPixmap *pixmap_congrafcet;
    flow_sAnnotPixmap *pixmap_stepconv;
    flow_sAnnotPixmap *pixmap_stepdiv;
    flow_sAnnotPixmap *pixmap_transconv;
    flow_sAnnotPixmap *pixmap_transdiv;
    flow_sAnnotPixmap *pixmap_frame;
    flow_sAnnotPixmap *pixmap_wait;


    void zoom( double zoom_factor);
    void unzoom();
    int get_select( pwr_tClassId *classid);
    void free_pixmaps();
    void allocate_pixmaps();
    void create_nodeclasses();
    int create_item( pwr_tObjid objid, 
	brow_tNode dest, flow_eDest dest_code, void **item,
	int is_root);
    void set_inputfocus( int focus);
    int object_exist( brow_tObject object);
    void set_selection_owner();
    int session_opened( ldh_tSesContext pal_ldhses, char *pal_root_name);
    int session_closed();

    static pal_sMenu *config_tree_build( ldh_tSession ldhses, char *filename, 
	       pal_eNameType keytype, char *keyname, pal_sMenu *menu);
    static pal_sMenu *config_tree_build_children( ldh_tSession ldhses, 
               ifstream *fp, int *line_cnt, char *filename, pal_sMenu *parent);
    static void config_tree_free( pal_sMenu *menu_tree);
    static void config_tree_free_children( pal_sMenu *first_child);
    static int check_volume( ldh_tSession ldhses, char *name);
};

#if defined __cplusplus
}
#endif
#endif




