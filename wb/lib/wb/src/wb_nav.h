#ifndef wb_nav_h
#define wb_nav_h

/* wb_nav.h -- Simple navigator

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

#ifndef wb_pal_h
#include "wb_pal.h"
#endif


class Nav {
 public:
  Nav(
	void *parent_ctx,
	Widget	parent_wid,
	char *name,
	ldh_tSesContext ldhses,
	char *root_name,
	Widget *w,
	pwr_tStatus *status
	);
  ~Nav();

  void 		*parent_ctx;
  Widget	parent_wid;
  char 		name[80];
  Widget	brow_widget;
  Widget	form_widget;
  Widget	toplevel;
  BrowCtx	*brow_ctx;
  ldh_tWBContext wbctx;
  ldh_tSesContext ldhses;
  char	        root_name[80];
  void		*root_item;
  brow_tNodeClass nc_object;
  brow_tObject	last_selected;
  flow_sAnnotPixmap *pixmap_leaf;
  flow_sAnnotPixmap *pixmap_map;
  flow_sAnnotPixmap *pixmap_openmap;
  flow_sAnnotPixmap *pixmap_ref;
  flow_sAnnotPixmap *pixmap_crossref;
  flow_sAnnotPixmap *pixmap_openattr;
  flow_sAnnotPixmap *pixmap_attr;
  flow_sAnnotPixmap *pixmap_attrarray;
  flow_sAnnotPixmap *pixmap_attrarrayelem;
  int 		(*get_plant_select_cb)( void *, char *);
  int 		(*set_focus_cb)( void *, void *);
  int 		(*traverse_focus_cb)( void *, void *);
  int		displayed;
  pal_sMenu     *menu;

  void zoom( double zoom_factor);
  void unzoom();
  int get_select( pwr_sAttrRef *attrref, int *is_attr);
  void set_inputfocus( int focus);
  void type_id_to_name( int type_id, char *type_id_name);
  void set_selection_owner();
  int object_exist( brow_tObject object);
  void create_nodeclasses();
  void free_pixmaps();
  void allocate_pixmaps();
  int open_top();
};


#if 0
nav_tCtx nav_new(
	void *parent_ctx,
	Widget	parent_wid,
	char *name,
	ldh_tSesContext ldhses,
	char *root_name,
	Widget *w,
	pwr_tStatus *status
	);
void nav_del( nav_tCtx navctx);
void nav_zoom( nav_tCtx navctx, double zoom_factor);
void nav_unzoom( nav_tCtx navctx);
int nav_get_select( nav_tCtx navctx, pwr_sAttrRef *attrref, int *is_attr);
void nav_set_inputfocus( nav_tCtx navctx, int focus);
#endif

#if defined __cplusplus
}
#endif
#endif





