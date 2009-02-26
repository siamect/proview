/* 
 * Proview   $Id: wb_nav.h,v 1.8 2008-10-31 12:51:32 claes Exp $
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

#ifndef wb_nav_h
#define wb_nav_h

/* wb_nav.h -- Simple navigator */

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
	const char *name,
	ldh_tSesContext ldhses,
	const char *root_name,
	pwr_tStatus *status
	);
  virtual ~Nav();

  void 		*parent_ctx;
  char 		name[80];
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
  flow_sAnnotPixmap *pixmap_attrobject;
  int 		(*get_plant_select_cb)( void *, char *, int);
  int 		(*set_focus_cb)( void *, void *);
  int 		(*traverse_focus_cb)( void *, void *);
  int		displayed;
  PalFileMenu   *menu;
  int	       	selection_owner;
  int		show_descrip;

  virtual void set_inputfocus( int focus) {}
  virtual void set_selection_owner() {}

  void zoom( double zoom_factor);
  void unzoom();
  int get_select( pwr_sAttrRef *attrref, int *is_attr);
  void type_id_to_name( int type_id, char *type_id_name);
  int object_exist( brow_tObject object);
  void create_nodeclasses();
  void free_pixmaps();
  void allocate_pixmaps();
  int open_top();

  static int brow_cb( FlowCtx *ctx, flow_tEvent event);
  static int init_brow_cb( FlowCtx *fctx, void *client_data);
};


#endif





