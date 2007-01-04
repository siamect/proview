/* 
 * Proview   $Id: xtt_xnav_brow.h,v 1.9 2007-01-04 08:22:47 claes Exp $
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
 */

#ifndef xtt_xnav_brow_h
#define xtt_xnav_brow_h

typedef enum {
  brow_eUserType_XNav,
  brow_eUserType_XAttNav
} brow_eUserType;

class XNavBrow {
  public:
    XNavBrow( BrowCtx *brow_ctx, void *brow_userdata, brow_eUserType brow_usertype) : 
      ctx(brow_ctx), userdata(brow_userdata), usertype(brow_usertype)
      { strcpy( push_command, "");};
#if 0
    ~XNavBrow();
#endif

    BrowCtx		*ctx;
    void		*userdata;
    brow_eUserType	usertype;
    brow_tNodeClass 	nc_object;
    brow_tNodeClass 	nc_attr;
    brow_tNodeClass 	nc_table;
    brow_tNodeClass 	nc_text;
    brow_tNodeClass 	nc_header;
    brow_tNodeClass 	nc_headerlarge;
    brow_tNodeClass 	nc_table_header;
    brow_tNodeClass 	nc_enum;
    flow_sAnnotPixmap 	*pixmap_leaf;
    flow_sAnnotPixmap 	*pixmap_map;
    flow_sAnnotPixmap 	*pixmap_openmap;
    flow_sAnnotPixmap	*pixmap_ref;
    flow_sAnnotPixmap 	*pixmap_crossref;
    flow_sAnnotPixmap 	*pixmap_openattr;
    flow_sAnnotPixmap 	*pixmap_attr;
    flow_sAnnotPixmap 	*pixmap_attrarray;
    flow_sAnnotPixmap 	*pixmap_attrarrayelem;
    flow_sAnnotPixmap 	*pixmap_crrread;
    flow_sAnnotPixmap 	*pixmap_crrwrite;
    flow_sAnnotPixmap 	*pixmap_script;
    flow_sAnnotPixmap 	*pixmap_list;
    flow_sAnnotPixmap 	*pixmap_graph;
    flow_sAnnotPixmap 	*pixmap_curve;
    flow_sAnnotPixmap 	*pixmap_action;
    flow_sAnnotPixmap 	*pixmap_file;
    flow_sAnnotPixmap 	*pixmap_symbol;
    flow_sAnnotPixmap 	*pixmap_morehelp;
    flow_sAnnotPixmap 	*pixmap_closehelp;
    flow_sAnnotPixmap 	*pixmap_object;
    flow_sAnnotPixmap 	*pixmap_attrenum;
    flow_sAnnotPixmap 	*pixmap_attrmask;
    flow_sAnnotPixmap 	*pixmap_block2;
    flow_sAnnotPixmap 	*pixmap_blockd;
    flow_sAnnotPixmap 	*pixmap_blockr;
    flow_sAnnotPixmap 	*pixmap_alarm2;
    flow_sAnnotPixmap 	*pixmap_arrowdown;
    flow_sAnnotPixmap 	*pixmap_arrowright;
    flow_sAnnotPixmap 	*pixmap_attrref;
    flow_sAnnotPixmap 	*pixmap_castattr;
    flow_sAnnotPixmap 	*pixmap_plcpgm;
    char                push_command[200];

    void free_pixmaps();
    void allocate_pixmaps();
    void create_nodeclasses();
    void brow_setup();
    void set_push_command( char *cmd) { strcpy(push_command, cmd);};
};

#endif
