#ifndef wb_gre_h
#define wb_gre_h

/* wb_gre.h -- Gredit info structure

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef wb_ldh_h
#include "wb_ldh.h"
#endif

#ifndef wb_vldh_h
#include "wb_vldh.h"
#endif

#ifndef wb_foe_macros_h
#include "wb_foe_macros.h"
#endif

#define	OT_CON_1	0	/* thin line */
#define	OT_CON_2	1	/* thin line with arrow */
#define	OT_CON_3	2	/* thin dashed line */
#define	OT_CON_4	3	/* thin dashed line with arrow */
#define	OT_CON_5	4	/* thick line */
#define	OT_CON_6	5	/* thick line with arrow */
#define	OT_CON_7	6	/* thick dashed line */
#define	OT_CON_8	7	/* thick dashed line with arrow */
#define	OT_WINDOW	10
#define	OT_DOCUMENT 2


#define	GRE_CLICK	7
#define	GRE_DOUBLECLICK	5
#define	GRE_PRESS	6
#define	GRE_CLICK_TIME		400  /* SG 23.02.91 */
#define	GRE_DOUBLECLICK_TIME	400
#define	GRE_PASTE_CENTER	0
#define	GRE_PASTE_CURSOR	1
#define	MAX_NUMBER_OF_OBJECTTYPES	300
#define	MAX_NUMBER_OF_CONTYPES		70
#define	MAX_SEL_NODE	500
#define	MAX_SEL_CON	500
#define	MAX_DEL_NODE	500
#define	MAX_DEL_CON	500

#define	MOVE_REGION	1
#define PASTE		2

#define GRE_CON_CREATE	1
#define GRE_CON_REDRAW	2
#define GRE_CON_NONROUTE 4
#define GRE_CON_NODEREDRAW 8

#define GRE_SELRECT 100

#define	GRE_POPUPMENUMODE_OBJECT	0
#define	GRE_POPUPMENUMODE_AREA		1

#define tst_neted(sts, text, index)			\
if ( sts != NetedSCsuccess) gre_neted_error( sts, text, index);


typedef struct s_gre_ctx t_gre_ctx;
typedef struct s_gre_ctx *gre_ctx;

#ifndef wb_goen_h
#include "wb_goen.h"
#endif

struct s_gre_ctx {
  t_commonpart	cp;

  Widget	gre_window;
  Widget	flow_widget;
  Widget	form_widget;
  Widget	nav_shell;
  Widget	nav_widget;
  flow_tCtx	flow_ctx;
  goen_t_cursors cursors;
  float		grid_size;
  unsigned long	gridobject;
  unsigned long	grid_on;
  vldh_t_wind	wind;
  unsigned long	conref_nodetypeid;  
  unsigned long	display_nodetypeid;  

   	unsigned long	concreate_mask;
	unsigned long	nodemoved_mask;
	unsigned long	regionsel_mask;
	unsigned long	nodecreate_mask;
	unsigned long	nodecreate_click;
	unsigned long	nodesel_mask;
	unsigned long	nodesel_click;
	unsigned long	nodeseladd_mask;
	unsigned long	nodeseladd_click;
	unsigned long	delete_mask;
	unsigned long	delete_click;
	unsigned long	cut_mask;
	unsigned long	cut_click;
	unsigned long	copy_mask;
	unsigned long	copy_click;
	unsigned long	paste_mask;
	unsigned long	paste_click;
	unsigned long	attribute_mask;
	unsigned long	attribute_click;
	unsigned long	subwindow_mask;
	unsigned long	subwindow_click;
	unsigned long	reserv_mask;
	unsigned long	reserv_click;
	unsigned long	popupmenu_mask;
	unsigned long	popupmenu_click;
	unsigned long	getobj_mask;
	unsigned long	getobj_click;
	unsigned long	help_mask;
	unsigned long	help_click;
	
	flow_tNodeClass	nodeclass_list[MAX_NUMBER_OF_OBJECTTYPES][4];
	flow_tConClass	conclass_list[MAX_NUMBER_OF_CONTYPES];
	vldh_t_node	sel_node_list[MAX_SEL_NODE];
	int		sel_node_count;
	vldh_t_con	sel_con_list[MAX_SEL_CON];
	int		sel_con_count;
	vldh_t_node	del_node_list[MAX_DEL_NODE];
	int		del_node_count;
	vldh_t_con	del_con_list[MAX_DEL_CON];
	int		del_con_count;
	flow_tNode	searchrect_node_id;
	int		popupmenu_mode;
	int		trace_started;
	XtIntervalId	trace_timerid;
	flow_tNodeClass	trace_analyse_nc;
	flow_tConClass	trace_con_cc;
	flow_tNode	trace_changenode;

/* Callbacks */
	void (*gre_setup_window)(gre_ctx);
	void (*gre_node_created)(gre_ctx, unsigned long, float, float);
	void (*gre_con_created)(gre_ctx, double, double, vldh_t_node, unsigned long, vldh_t_node,  unsigned long);
	void (*gre_node_moved)(gre_ctx);
	void (*gre_delete)(gre_ctx, void *, unsigned long);
	void (*gre_cut)(gre_ctx, void *, unsigned long);
	void (*gre_copy)(gre_ctx, void *, unsigned long);
	void (*gre_paste)(gre_ctx, float, float);
	void (*gre_attribute)(gre_ctx, vldh_t_node);
	void (*gre_subwindow)(gre_ctx, vldh_t_node, unsigned long);
	voidp (*gre_reserv);
	void (*gre_popupmenu)(gre_ctx, int, int, int, vldh_t_node);
	void (*gre_getobj)(gre_ctx, vldh_t_node, unsigned long);
	void (*gre_undelete)(gre_ctx);
	void (*gre_unselect)(gre_ctx);
	void (*gre_help)(gre_ctx, char *);
	void (*gre_regionmoved)(gre_ctx);
	void (*gre_message)(gre_ctx, char *);
};

/* Methods */
/*----------------------------------------------------------------------
 * Create a new gredit object
 */
gre_ctx	gre_new( 
	void *,		/* parent_ctx */
	Widget,		/* parent_wid */
	char *		/* name */
);

int gre_new_navigator( gre_ctx grectx, Widget parent);
int gre_new_navigator_popup( gre_ctx grectx);
int gre_init( gre_ctx grectx);


void	gre_del(
	gre_ctx	grectx
);

int gre_setup_backcalls (
	gre_ctx	grectx,
	void (*gre_setup_window)(gre_ctx),
	void (*gre_node_created)(gre_ctx, unsigned long, float, float),
	void (*gre_con_created)(gre_ctx, double, double, vldh_t_node, unsigned long, vldh_t_node,  unsigned long),
	void (*gre_node_moved)(gre_ctx),
	void (*gre_delete)(gre_ctx, void *, unsigned long),
	void (*gre_cut)(gre_ctx, void *, unsigned long),
	void (*gre_copy)(gre_ctx, void *, unsigned long),
	void (*gre_paste)(gre_ctx, float, float),
	void (*gre_attribute)(gre_ctx, vldh_t_node),
	void (*gre_subwindow)(gre_ctx, vldh_t_node, unsigned long),
	voidp (*gre_reserv),
	void (*gre_popupmenu)(gre_ctx, int, int, int, vldh_t_node),
	void (*gre_getobj)(gre_ctx, vldh_t_node, unsigned long),
	void (*gre_undelete)(gre_ctx),
	void (*gre_unselect)(gre_ctx),
	void (*gre_help)(gre_ctx, char *),
	void (*gre_regionmoved)(gre_ctx),
	void (*gre_message)(gre_ctx, char *)
	);
/*
 * Setup neted window
 */
void	gre_setup_window(
	gre_ctx		grectx,
	unsigned long	zoom_mask,
	unsigned long	scroll_mask,
	unsigned long	concreate_mask,
	unsigned long	nodemoved_mask,
	unsigned long	regionsel_mask,
	unsigned long	nodecreate_mask,
	unsigned long	nodecreate_click,
	unsigned long	nodesel_mask,
	unsigned long	nodesel_click,
	unsigned long	nodeseladd_mask,
	unsigned long	nodeseladd_click,
	unsigned long	delete_mask,
	unsigned long	delete_click,
	unsigned long	cut_mask,
	unsigned long	cut_click,
	unsigned long	copy_mask,
	unsigned long	copy_click,
	unsigned long	paste_mask,
	unsigned long	paste_click,
	unsigned long	attribute_mask,
	unsigned long	attribute_click,
	unsigned long	subwindow_mask,
	unsigned long	subwindow_click,
	unsigned long	reserv_mask,
	unsigned long	reserv_click,
	unsigned long	popupmenu_mask,
	unsigned long	popupmenu_click,
	unsigned long	getobj_mask,
	unsigned long	getobj_click,
	unsigned long	help_mask,
	unsigned long	help_click,
	unsigned char	enterleave_flag
);

void gre_disable_button_events (
	gre_ctx grectx) ;

/*
 * Create node
 */
int	gre_create_node( 
	gre_ctx	grectx,
	pwr_tClassId  classid,
	float		x,
	float		y,
	vldh_t_node	*node
);
/*
 * Create connection
 */
int	gre_create_con(
	gre_ctx		grectx,
	pwr_tClassId	classid,
	vldh_t_node 	source_obj,
	unsigned long	source_point,
	vldh_t_node 	destination_obj,
	unsigned long	destination_point,
	unsigned long	drawtype
);

int	gre_print_docobj( 
	gre_ctx	grectx,
	vldh_t_node doc_obj
);

int	gre_print_selrectangle( 
	gre_ctx		grectx,
	char		*file_id
);

/*
 * Paste
 */
void gre_paste (
    gre_ctx	grectx,
    float	cursor_x,
    float	cursor_y,
    int		paste_type
);

int  gre_paste_node_exec(
	gre_ctx		grectx,
	float		x,
	float		y
);
/*
 * Move region
 */
int	gre_moveregion( 
    gre_ctx	grectx
);




int gre_node_sethighlight ( 
	gre_ctx		grectx,
	vldh_t_node	node,
	unsigned long	highlight_flag
);


int gre_node_gethighlight (	
	gre_ctx		grectx,
	vldh_t_node	node,
	unsigned long	*highlight_flag
);

int gre_get_annot_width(
	flow_tNodeClass	nodeclass,
	float		*annot_width,
	char		*annot_str,
	int		annot_count,
	int		annot_size
);

int gre_get_annotations(
	vldh_t_node	node,
	char		*annot_str,
	int		*annot_nr,
	int		*annot_count,
	int		annot_max,
	int		annot_size
);

int gre_modif_annot ( 
	gre_ctx		grectx,
	vldh_t_node	node,
	int		annotation,
	char		*str
);

int	gre_node_annotations_draw (
	gre_ctx		grectx,
	vldh_t_node	node,
	int		paste
);

int	gre_node_annot_message( 
	gre_ctx		grectx,
	vldh_t_node	node,
	char		*message,
	int		msg_size,
	int		annot_max_size
);

void	gre_unselect(
	gre_ctx		grectx
);

void	gre_search_rectangle_delete( 
	gre_ctx	grectx
);

void	gre_search_rectangle_create(
	gre_ctx		grectx,
	vldh_t_node	node
);

int	gre_undelete( 
	gre_ctx		grectx
);

int	gre_undelete_reset( 
	gre_ctx	    grectx
);

void	gre_delete( 
	gre_ctx		grectx
);

int	gre_delete_node(
	gre_ctx		grectx,	
	vldh_t_node	node
);

void	gre_delete_con(
	gre_ctx		grectx,
	vldh_t_con	con
);

int	gre_cut(
	gre_ctx		grectx
);

int	gre_cut_node(
	gre_ctx		grectx,
	vldh_t_node	node
);

int	gre_copy_node(
	gre_ctx		grectx,
	vldh_t_node	node
);

int	gre_copy(
	gre_ctx		grectx
);

int	gre_window_draw(
	gre_ctx		grectx
);

int gre_get_conclass (
	gre_ctx	grectx,
	pwr_tClassId  classid,
	ldh_tSesContext	    ldhses,
	unsigned long con_type,
	flow_tConClass *con_type_id
);

int  gre_get_selnodes( 
	gre_ctx		grectx,
	unsigned long		*node_count,
	vldh_t_node		**intern_nodelist
);

int	gre_measure_object(
	vldh_t_node	node,
	float		*ll_x,
	float		*ll_y,
	float		*width,
	float		*height
);

int gre_print_rectangle(
	gre_ctx	grectx,
	float	ll_x,
	float	ll_y,
	float	ur_x,
	float	ur_y,
	char	*file_id
);

int gre_subwindow_mark (
	gre_ctx	grectx,
	vldh_t_node object
);


int gre_node_update (
	gre_ctx	grectx,
	vldh_t_node object
);

void gre_zoom (
	gre_ctx	grectx,
	float	zoom
);

void gre_unzoom (
	gre_ctx	grectx
);

int	gre_display(
	gre_ctx		grectx
);

int	gre_set_display_value(
	gre_ctx		grectx,
	vldh_t_node	node,
	char		*string
);

int gre_undisplay (
	gre_ctx grectx
);

int	gre_node_select(
	gre_ctx		grectx,
	vldh_t_node	node
);

int	gre_node_unselect(
	gre_ctx		grectx,
	vldh_t_node	node
);

int	gre_center_node( 
	gre_ctx		grectx,
	vldh_t_node	node
);


int	gre_init_docobjects(
    gre_ctx	grectx
);

int	gre_redraw(
	gre_ctx	grectx
);

int gre_view_setup( gre_ctx grectx);
int gre_edit_setup( gre_ctx grectx);
int gre_set_trace_attributes( gre_ctx grectx, char *host);
int gre_save( gre_ctx grectx, char *filename);
void gre_set_grid_size( gre_ctx grectx, double size);

#endif
