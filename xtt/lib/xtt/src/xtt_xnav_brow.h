#ifndef xtt_xnav_brow_h
#define xtt_xnav_brow_h

/* xtt_xnav_brow.h -- Simple navigator brow

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined __cplusplus
extern "C" {
#endif

class XNavBrow {
  public:
    XNavBrow( BrowCtx *brow_ctx, void *brow_userdata) : 
      ctx(brow_ctx), userdata(brow_userdata) 
      { strcpy( push_command, "");};
#if 0
    ~XNavBrow();
#endif

    BrowCtx		*ctx;
    void		*userdata;
    brow_tNodeClass 	nc_object;
    brow_tNodeClass 	nc_attr;
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
    char                push_command[200];

    void free_pixmaps();
    void allocate_pixmaps();
    void create_nodeclasses();
    void brow_setup();
    void set_push_command( char *cmd) { strcpy(push_command, cmd);};
};

#if defined __cplusplus
}
#endif
#endif
