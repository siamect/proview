#ifndef ge_graph_h
#define ge_graph_h

/* ge_graph.h -- Simple graphic editor

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif
#ifndef glow_h
#include "glow.h"
#endif

#ifndef glow_growctx_h
#include "glow_growapi.h"
#endif

#ifndef ge_graph_ccm_h
#include "ge_graph_ccm.h"
#endif

#ifndef ge_attr_h
#include "ge_attr.h"
#endif

#if defined OS_VMS || defined OS_LINUX
#define LDH 1
#endif

#if LDH
# ifndef wb_ldh_h
#  include "wb_ldh.h"
# endif
#else
  typedef void *ldh_tSesContext;
  typedef void *ldh_tWBContext;
  typedef void *ldh_tVolContext;
#endif

#define graph_cVersion	"X3.0b"
#define GRAPH_GROW_MAX	25

typedef enum {
	graph_eMode_Development,
	graph_eMode_Runtime
	} graph_eMode;

typedef enum {
	graph_eDatabase_Gdh,
	graph_eDatabase_User,
	graph_eDatabase_Local,
	} graph_eDatabase;

typedef enum {
	graph_eTrace_Inherit		= 0,
	graph_eTrace_Dig		= 1,
	graph_eTrace_DigWithError	= 2,
	graph_eTrace_DigTone		= 3,
	graph_eTrace_DigToneWithError	= 4,
	graph_eTrace_Annot		= 5,
	graph_eTrace_DigWithText	= 6,
	graph_eTrace_Bar		= 7,
	graph_eTrace_Trend		= 8,
	graph_eTrace_DigBorder		= 9,
	graph_eTrace_AnnotWithTone 	= 10,
	graph_eTrace_DigTwo		= 11,
	graph_eTrace_DigToneTwo		= 12,
	graph_eTrace_Invisible		= 13,
	graph_eTrace_Rotate		= 14,
	graph_eTrace_AnalogShift	= 15,
	graph_eTrace_Animation		= 16,
	graph_eTrace_DigAnimation	= 17,
	graph_eTrace_AnimationForwBack	= 18,
	graph_eTrace_DigShift		= 19,
	graph_eTrace_Move		= 20,
	graph_eTrace_SliderBackground	= 21,
	graph_eTrace_No		        = 999,
	graph_eTrace_SetDig		= 1000,
	graph_eTrace_ResetDig		= 1001,
	graph_eTrace_ToggleDig		= 1002,
	graph_eTrace_Slider		= 1003,
	graph_eTrace_AnnotInput		= 1004,
	graph_eTrace_Command		= 1005,
	graph_eTrace_CommandConfirm	= 1006,
	graph_eTrace_SetDigConfirm	= 1007,
	graph_eTrace_ResetDigConfirm	= 1008,
	graph_eTrace_ToggleDigConfirm	= 1009,
	graph_eTrace_SetDigWithTone	= 1010,
	graph_eTrace_ResetDigWithTone	= 1011,
	graph_eTrace_ToggleDigWithTone	= 1012,
	graph_eTrace_AnnotInputWithTone	= 1013,
	graph_eTrace_SetDigConfirmWithTone = 1014,
	graph_eTrace_ResetDigConfirmWithTone = 1015,
	graph_eTrace_ToggleDigConfirmWithTone = 1016,
	graph_eTrace_DigWithCommand	= 1017,
	graph_eTrace_DigWithErrorAndCommand = 1018,
	graph_eTrace_DigToneWithCommand	= 1019,
	graph_eTrace_DigToneWithErrorAndCommand	= 1020,
	graph_eTrace_StoDigWithTone	= 1021,
	graph_eTrace_DigTwoWithCommand	= 1022,
	graph_eTrace_DigToneTwoWithCommand = 1023,
	graph_eTrace_IncrAnalog 	= 1024,
	graph_eTrace_RadioButton 	= 1025,
	graph_eTrace_DigShiftWithToggleDig = 1026
	} graph_eTrace;

typedef struct {
	pwr_tFloat32	*pres_max_limit_p;
	pwr_tFloat32	*pres_min_limit_p;
	pwr_tFloat32	pres_max_limit_old;
	pwr_tFloat32	pres_min_limit_old;
	grow_tObject 	bar_object;
	grow_tObject 	trend_object;
	grow_tObject 	slider_object;
	grow_tObject 	slider_button_object;
	grow_tObject 	hold_button_object;
	float		*scan_time_p;
	float		old_scan_time;
	double		*data_scan_time_p;
	int		*hold_button_p;
	int		*go_button_p;
	int		*slider_button_p;
	int		*hold_p;
	int		*slider_disable_p;
	} graph_sObjectTrend;

typedef struct {
	pwr_tSubid	subid[6];
	char		format[16];
	graph_eTrace	type;
	glow_eDrawType	color;
	glow_eDrawType	color2;
	void		*p[6];
	int		inverted[6];
	char		old_value[6][80];
	int		size[6];
	graph_eDatabase	db[6];
	int		first_scan;
	double		scan_time;
	double		acc_time;
 	int		annot_typeid;
 	int		annot_size;
	unsigned int	access;
        glow_eCycle     cycle;
	char		high_text[80];
	char		low_text[80];
	double		x0;
	double		y0;
	double		x_orig;
	double		y_orig;
	double		factor;
	glow_eRotationPoint rotation_point;
	int		animation_count;
	int		animation_direction;
	int		trend_boolean;
	int		trend_hold;
	int		slider_disabled;
	} graph_sTraceData;

typedef struct s_LocalDb {
	char	name[40];
	int	type;
	char	value[80];
	char	old_value[80];
	s_LocalDb *next;
	} graph_sLocalDb;

#define RECALL_BUFF_SIZE 20

class GraphRecallBuff {
  public:
	  GraphRecallBuff() : cnt(0), size(RECALL_BUFF_SIZE)
             { memset(key,0,sizeof(key)); };
        glow_sTraceData       buff[RECALL_BUFF_SIZE];
        glow_sTraceData       temporary;
        char                  key[RECALL_BUFF_SIZE][80];
        int                   cnt;
        int                   size;

        void insert( glow_sTraceData *data, char *key);
        int get( glow_sTraceData *data, int idx);
        int get( glow_sTraceData *data, char *key);
};

class GraphApplList {
  public:
	GraphApplList( int root, void *appl_ctx) :
		is_root(root), ctx(appl_ctx), prev(NULL), next(NULL) {};
	int			is_root;
	void			*ctx;
	GraphApplList		*prev;
	GraphApplList		*next;

	void 		insert( void *ctx);
	void 		remove( void *ctx);
	int		get_first( void **ctx);
};

class GraphGbl {
  public:
    GraphGbl()
	{ 
	  strcpy( version, graph_cVersion);
	};
    char		version[10];
    int			load_config( void *graph);
};

class GraphGrow {
  public:
    GraphGrow( GrowCtx *grow_ctx, void *xn) : ctx(grow_ctx), graph(xn) {};
    ~GraphGrow();

    GrowCtx		*ctx;
    void		*graph;
    void grow_setup();
    void grow_trace_setup();
};


class Graph {
  public:
    Graph(
	void *xn_parent_ctx,
	Widget	xn_parent_wid,
	char *xn_name,
	Widget *w,
	pwr_tStatus *status,
	char *xn_default_path,
	graph_eMode graph_mode = graph_eMode_Development,
	int scrollbar = 1,
	int xn_gdh_init_done = 0,
	char *xn_object_name = 0,
	int xn_use_default_access = 0,
	unsigned int xn_default_access = 0);

    GraphGbl		gbl;
    GraphApplList	attr_list;
    GraphRecallBuff     recall;
    void 		*parent_ctx;
    Widget		parent_wid;
    char 		name[120];
    char 		object_name[120];
    Widget		grow_widget;
    Widget		form_widget;
    Widget		toplevel;
    Widget		nav_widget;
    GraphGrow		*grow;
    GraphGrow		*grow_stack[GRAPH_GROW_MAX];
    int			grow_cnt;
    ldh_tSesContext	ldhses;
    void 		(*message_cb)( void *, char, char *);
    int 		(*get_current_subgraph_cb)( void *, char *, char *);
    void 		(*close_cb)( void *);
    void 		(*get_current_colors_cb)( void *, glow_eDrawType *, glow_eDrawType *);
    void 		(*set_current_colors_cb)( void *, glow_eDrawType, glow_eDrawType);
    void 		(*init_cb)( void *);
    void 		(*cursor_motion_cb)( void *, double, double);
    void 		(*change_text_cb)( void *, void *, char *);
    void 		(*change_name_cb)( void *, void *, char *);
    void 		(*change_value_cb)( void *, void *, char *);
    void 		(*confirm_cb)( void *, void *, char *);
    void 		(*command_cb)( void *, char *);
    void 		(*load_graph_cb)( void *, char *);
    int 		(*get_plant_select_cb)( void *, char *attr_name);
    void 		(*display_in_xnav_cb)( void *, pwr_tObjid);
    void 		(*message_dialog_cb)( void *, char *);
    int 		(*is_authorized_cb)( void *, unsigned int);
    int 		(*traverse_focus_cb)( void *, void *);
    int 		(*set_focus_cb)( void *, void *);
    int			(*get_ldhses_cb)( void *, ldh_tSesContext *, int);
    int			(*get_current_objects_cb)( void *, pwr_sAttrRef **, int **);
    void                (*popup_menu_cb)(void *, pwr_sAttrRef, unsigned long,
					 unsigned long, char *, Widget *); 
    int			linewidth;
    int			textsize;
    int			textbold;
    int			border_color;
    int			fill_color;
    int			fill;
    int			border;
    int			grid;
    double		grid_size_x;
    double		grid_size_y;
    glow_eConType	con_type;
    glow_eCorner	con_corner;
    glow_eDirection	conpoint_direction;
    grow_tObject	current_polyline;
    grow_tObject	current_slider;
    XtIntervalId	trace_timerid;
    int			trace_started;
    int			gdh_init_done;
    gccm_s_arglist	arglist_stack[20];
    int			arglist_cnt;
    double		corner_round_amount;
    graph_eMode		mode;
    double		scan_time;
    double		fast_scan_time;
    double		animation_scan_time;
    char		default_path[80];
    char		filename[120];
    int			closing_down;
    grow_tObject	current_mb1_down;
    int			slow_scan_cnt;
    int			fast_scan_cnt;
    int			displayed;
    int			ccm_func_registred;
    int			verify;
    int			scriptmode;
    grow_tObject	current_cmd_object;
    void		*graph_object_data;
    void 		(*graph_object_scan)( Graph *graph);
    void 		(*graph_object_close)( Graph *graph);
    graph_sLocalDb	*local_db;
    char                systemname[80];
    int                 use_default_access;
    unsigned int        default_access;

    int create_navigator( Widget parent);
    void print( char *filename);
    void zoom( double zoom_factor);
    void unzoom();
    void set_mode( grow_eMode mode);
    void message( char sev, char *text);
    int grow_pop();
    int grow_push();
    int grow_push_all();
    void set_inputfocus( int focus);
    int setup();
    void set_grid( int grid_on);
    void set_linewidth( int width) { linewidth = width;};
    void set_textsize( int size) { textsize = size;};
    void set_textbold( int bold) { textbold = bold;};
    void set_border_color( int color_idx) { border_color = color_idx;};
    void set_fill_color( int color_idx) { fill_color = color_idx;};
    void set_fill( int fill_on) { fill = fill_on;};
    void set_border( int border_on) { border = border_on;};
    void set_select_fill_color();
    void set_select_border_color();
    void set_select_color_tone( glow_eDrawTone tone);
    void incr_select_color_lightness( int lightness);
    void incr_select_color_intensity( int intensity);
    void incr_select_color_shift( int shift);
    void set_select_linewidth( int linewidth);
    void set_select_textsize( int textsize);
    void set_select_textbold( int textbold);
    void set_select_fill( int fill);
    void set_select_border( int border);
    void set_background_color();
    void set_show_grid( int show);
    void set_nav_background_color();
    void set_name( char *name);
    void get_name( char *name);
    void save( char *filename);
    void save_subgraph( char *filename);
    void open( char *filename);
    void open_subgraph( char *filename);
    void set_subgraph_path( int path_cnt, char *path);
    int get_conclass( glow_eDrawType drawtype, int linewidth,
	glow_eConType contype, glow_eCorner corner, double round_amount, 
	grow_tConClass *cc);
    glow_eDrawType get_border_drawtype();
    glow_eDrawType get_fill_drawtype();
    void set_condir( glow_eDirection dir) { conpoint_direction = dir;};
    void set_contype( glow_eConType type) { con_type = type;};
    void set_concorner( glow_eCorner corner) { con_corner = corner;};
    void set_corner_round_amount( double round_amount) 
	{ corner_round_amount = round_amount;};
    int edit_attributes( grow_tObject object);
    int get_attr_items( grow_tObject object, attr_sItem **itemlist,
	int *item_cnt, void **client_data);
    int edit_subgraph_attributes();
    int edit_graph_attributes();
    void clear_all();
    void rotate( double angel);
    void cut();
    void copy();
    void paste();
    int init_trace();
    void close_trace( int reload);
    void push_select();
    void pop_select();
    void align_select( glow_eAlignDirection direction);
    void equidistance_select( glow_eAlignDirection direction);
    void set_default_layout();
    void set_gridsize( double gridsize);
    void select_all_cons();
    void select_all_objects();
    void change_text( grow_tObject object, char *text);
    void change_name( grow_tObject object, char *name);
    void change_value( grow_tObject object, char *text);
    void change_select_text();
    void change_select_name();
    void confirm_ok( grow_tObject object);
    void set_move_restriction( glow_eMoveRestriction restriction);
    void set_scale_equal( int equal);
    int is_subgraph();
    void connect( grow_tObject object, char *attr_name, int second);
    void get_filename( char *inname, char *outname);
    int is_authorized( unsigned int access);
    int get_selected_object( grow_tObject *object);
    int is_modified();
    void set_scantime( double time) { scan_time = time;};
    graph_eDatabase parse_attr_name( char *name, char *parsed_name,
		int *inverted, int *type, int *size);
    int get_default_size( int *width, int *height);
    int type_to_string( pwr_eType type, char *type_buf, int  *size);
    void string_to_type( char *type_str, pwr_eType *type,
		int *size, int *elements);
    void get_systemname( char *name) { strcpy( name, systemname);};
    void set_systemname( char *name) { strcpy( systemname, name);};

    int get_argnames( char *code, char *argnames, int *argtypes, int *argcnt);
    int exec_dynamic( grow_tObject object, char *code, glow_eDynamicType type);

    int export_javabean( char *filename, char *bean_name);
    int export_javaframe( char *filename, char *bean_name, int applet, int html);
    int export_GejavaObjectTraceAttr( ofstream& fp, grow_tObject object, int cnt);
    int export_ObjectTraceAttr( ofstream& fp, grow_tObject object, int cnt);
    int export_BarTraceAttr( ofstream& fp, grow_tObject object, int cnt);
    int export_TrendTraceAttr( ofstream& fp, grow_tObject object, int cnt);
    int export_SliderTraceAttr( ofstream& fp, grow_tObject object, int cnt);
    int export_gejava( char *filename, char *bean_name, int applet, int html);
    int export_gejava_nodeclass( ofstream& fp, grow_tNodeClass nodeclass);
    void set_java_name( char *name);
    int get_java_name( char *name);
    void get_next_subgraph( char *name);
    void set_next_subgraph( char *name);
    void store_geometry();
    void restore_geometry();
    void set_nodraw() { grow_SetNodraw( grow->ctx);};
    void reset_nodraw() { grow_ResetNodraw( grow->ctx);};
    void redraw() { grow_Redraw( grow->ctx);};
    int group_select( grow_tObject *object, char *last_group);
    int set_recall_data( grow_tObject object, char *name);
    int ungroup_select( int force);
    int set_object_focus( char *name, int empty);
    void get_command( char *in, char *out);
    int is_javaapplet();
    int is_javaapplication();
    int ref_object_info( glow_eCycle cycle, char *name, void **data,
			 pwr_tSubid *subid, unsigned int size);

    // Command module
    int command( char* input_str);
    int readcmdfile( 	char		*incommand);

    // Object graph module
    int init_object_graph( int mode);
    int set_button_command( char *button_name, char *cmd);
    int trend_init( graph_sObjectTrend *td, pwr_tObjid objid);
    void trend_scan( graph_sObjectTrend *td);
    graph_sLocalDb *localdb_add( char *name, int type);
    int localdb_find( char *name, graph_sLocalDb **item);
    void localdb_free();
    void *localdb_ref_or_create( char *name, int type);
    int localdb_set_value( char *name, void *value, int size);
    int create_node( char *node_name, char *subgraph_str, double x1, double y1, 
		     double x2, double y2, grow_tNode *node);

    // Web module
    static int generate_web( ldh_tSesContext);


    ~Graph();
};

#if defined __cplusplus
}
#endif
#endif
