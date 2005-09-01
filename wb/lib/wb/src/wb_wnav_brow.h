/** 
 * Proview   $Id: wb_wnav_brow.h,v 1.5 2005-09-01 14:57:59 claes Exp $
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

#ifndef wb_wnav_brow_h
#define wb_wnav_brow_h

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

typedef enum {
	wnav_eBrowType_Volume,
	wnav_eBrowType_Setup,
	wnav_eBrowType_Other
	} wnav_eBrowType;

class WNavBrow {
  public:
    WNavBrow( BrowCtx *brow_ctx, void *brow_userdata) : 
	type(wnav_eBrowType_Other), ldh_cb_used(1),
	ctx(brow_ctx), userdata(brow_userdata) {};
#if 0
    ~WNavBrow();
#endif

    wnav_eBrowType	type;
    int			ldh_cb_used;
    BrowCtx		*ctx;
    void		*userdata;
    brow_tNodeClass 	nc_object;
    brow_tNodeClass 	nc_multiobject;
    brow_tNodeClass 	nc_attr;
    brow_tNodeClass 	nc_attr_full;
    brow_tNodeClass 	nc_attr_input;
    brow_tNodeClass 	nc_attr_inputinv;
    brow_tNodeClass 	nc_attr_output;
    brow_tNodeClass 	nc_attr_multiline;
    brow_tNodeClass 	nc_attr_multiline_full;
    brow_tNodeClass 	nc_enum;
    brow_tNodeClass 	nc_table;
    brow_tNodeClass 	nc_text;
    brow_tNodeClass 	nc_header;
    brow_tNodeClass 	nc_headerlarge;
    brow_tNodeClass 	nc_table_header;
    flow_sAnnotPixmap 	*pixmap_leaf;
    flow_sAnnotPixmap 	*pixmap_map;
    flow_sAnnotPixmap 	*pixmap_openmap;
    flow_sAnnotPixmap	*pixmap_ref;
    flow_sAnnotPixmap 	*pixmap_crossref;
    flow_sAnnotPixmap 	*pixmap_openattr;
    flow_sAnnotPixmap 	*pixmap_attr;
    flow_sAnnotPixmap 	*pixmap_attrarray;
    flow_sAnnotPixmap 	*pixmap_attrenum;
    flow_sAnnotPixmap 	*pixmap_attrmask;
    flow_sAnnotPixmap 	*pixmap_attrarrayelem;
    flow_sAnnotPixmap 	*pixmap_crrread;
    flow_sAnnotPixmap 	*pixmap_crrwrite;
    flow_sAnnotPixmap 	*pixmap_script;
    flow_sAnnotPixmap 	*pixmap_graph;
    flow_sAnnotPixmap 	*pixmap_file;
    flow_sAnnotPixmap 	*pixmap_symbol;
    flow_sAnnotPixmap 	*pixmap_morehelp;
    flow_sAnnotPixmap 	*pixmap_closehelp;
    flow_sAnnotPixmap 	*pixmap_objname;
    flow_sAnnotPixmap 	*pixmap_object;
    flow_sAnnotPixmap 	*pixmap_attrref;
    flow_sAnnotPixmap 	*pixmap_attrxref;
    flow_sAnnotPixmap 	*pixmap_ref_err;
    flow_sAnnotPixmap 	*pixmap_crossref_err;
    flow_sAnnotPixmap 	*pixmap_attrref_err;
    flow_sAnnotPixmap 	*pixmap_attrxref_err;
    flow_sAnnotPixmap 	*pixmap_project;
    flow_sAnnotPixmap 	*pixmap_database;
    flow_sAnnotPixmap 	*pixmap_volume;
    flow_sAnnotPixmap 	*pixmap_castattr;
    flow_sAnnotPixmap 	*pixmap_uncastattr;
    flow_sAnnotPixmap 	*pixmap_docblock;

    void free_pixmaps();
    void allocate_pixmaps();
    void create_nodeclasses();
    void brow_setup();
};

#if defined __cplusplus
}
#endif
#endif
