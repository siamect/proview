#ifndef xtt_xnav_h
#define xtt_xnav_h

/* xtt_xnav.h -- Simple navigator

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef co_regex_h
# include "co_regex.h"
#endif

#ifndef co_nav_help_h
#include "co_nav_help.h"
#endif

#if defined __cplusplus
}
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

#ifndef flow_browwidget_h
#include "flow_browwidget.h"
#endif

#ifndef xtt_xnav_brow_h
#include "xtt_xnav_brow.h"
#endif

extern "C" {

#ifndef rt_trace_h
#include "rt_trace.h"
#endif

}

#ifndef xtt_xnav_localdb_h
#include "xtt_localdb.h"
#endif

#ifndef xtt_ev_h
#include "xtt_ev.h"
#endif

#ifndef xtt_hist_h
#include "xtt_hist.h"
#endif

#ifndef xtt_op_h
#include "xtt_op.h"
#endif

#ifndef xtt_logging_h
#include "xtt_logging.h"
#endif

#ifndef xtt_menu_h
#include "xtt_menu.h"
#endif

#ifndef xtt_clog_h
#include "xtt_clog.h"
#endif

#define xnav_cVersion	"X3.0b"
#define XNAV_BROW_MAX	25
#define XNAV_LOGG_MAX   10

typedef enum {
	xnav_mOpen_All		= ~0,
	xnav_mOpen_Children	= 1 << 0,
	xnav_mOpen_Attributes	= 1 << 1,
	xnav_mOpen_Crossref	= 1 << 2
	} xnav_mOpen;

typedef enum {
	xnav_eType_GdbNodeFlags	= 10000,
	xnav_eType_Empty	= 10001,
	xnav_eType_FixStr	= 10002,
	xnav_eType_ShortTime	= 10003
	} xnav_eType;

typedef enum {
	xnav_eDestCode_After,
	xnav_eDestCode_Before,
	xnav_eDestCode_FirstChild,
	xnav_eDestCode_LastChild
	} xnav_eDestCode;

typedef enum {
	xnav_eSearchType_No,
	xnav_eSearchType_Name,
	xnav_eSearchType_RegularExpr
	} xnav_eSearchType;

typedef struct {
	char	title[80];
	int	item_type;
	void	*action;
} xnav_sStartMenu;

typedef struct xnav_s_Menu {
	char			title[80];
	int			item_type;
	char			command[256];
	struct xnav_s_Menu	*child_list;
	struct xnav_s_Menu	*parent;
	struct xnav_s_Menu	*next;
} xnav_sMenu;

typedef struct s_trace_node t_trace_node;

struct s_trace_node {
  t_trace_node  *Next;
  tra_tCtx      tractx;
  pwr_tObjid    Objid;
};

typedef enum {
	applist_eType_Trace,
	applist_eType_Graph,
	applist_eType_Attr,
	applist_eType_Trend,
	applist_eType_Crossref,
	applist_eType_Hist
} applist_eType;

class ApplListElem {
  public:
    ApplListElem( applist_eType al_type, void *al_ctx, pwr_tObjid al_objid,
	char *al_name, char *al_instance);
    applist_eType	type;
    void		*ctx;
    pwr_tObjid		objid;
    char		name[80];
    char                instance[80];
    ApplListElem 	*next;
};

class ApplList {
  public:
    ApplList() :
       root(NULL) {};
    
    ApplListElem *root;
    void insert( applist_eType type, void *ctx, 
	pwr_tObjid objid, char *name, char *instance);
    void remove( void *ctx);
    int find( applist_eType type, char *name, char *instance, void **ctx);
    int find( applist_eType type, pwr_tObjid objid, void **ctx);
    void swap( int mode);
};

class XNavGbl {
  public:
    XNavGbl() :
	priv(0), UserObject(pwr_cNObjid), AlarmAutoLoad(0), AlarmMessage(0),
	AlarmBeep(0), AlarmReturn(0), AlarmAck(0), gdh_started(1),
	verify(0), scantime(0.5), signal_test_mode(0), advanced_user(0)
	{ 
	  strcpy( version, xnav_cVersion); strcpy( time, "");
	  strcpy( ConfigureObject, ""); strcpy( AlarmLastMessage, "");
	  strcpy( AlarmText1, ""); strcpy( AlarmText2, ""); strcpy( AlarmText3, ""); 
	  strcpy( AlarmText4, ""); strcpy( AlarmText5, ""); 
	  strcpy( platform, ""); strcpy( os, ""); strcpy( hw, "");
	  strcpy( node, ""); strcpy( sys, ""); strcpy( default_directory, "");
	  strcpy( symbolfilename, "");
	};
    char		version[10];
    char		time[80];
    unsigned long	priv;
    char		ConfigureObject[80];
    pwr_tObjid		UserObject;
    int			AlarmAutoLoad;
    int			AlarmMessage;
    int			AlarmBeep;
    int			AlarmReturn;
    int			AlarmAck;
    int 		gdh_started;
    char		AlarmLastMessage[80];
    char		AlarmText1[80];
    char		AlarmText2[80];
    char		AlarmText3[80];
    char		AlarmText4[80];
    char		AlarmText5[80];
    char		platform[20];
    char		os[20];
    char		hw[20];
    int			verify;
    char		node[20];
    char		sys[20];
    char		default_directory[80];
    double		scantime;
    char		symbolfilename[80];
    int			signal_test_mode;
    int			advanced_user;

    int			load_config( void *xnav);
    int			symbolfile_exec( void *xnav);
};


class XNav {
  public:
    XNav(
	void *xn_parent_ctx,
	Widget	xn_parent_wid,
	char *xn_name,
	Widget *w,
	xnav_sStartMenu *root_menu,
	char *xn_opplace_name,
	pwr_tStatus *status);
    ~XNav();

    XNavGbl		gbl;
    XNavLdb		ldb;
    ApplList		appl;
    XttLogging          logg[XNAV_LOGG_MAX];
    void 		*parent_ctx;
    Widget		parent_wid;
    char 		name[80];
    Widget		brow_widget;
    Widget		form_widget;
    Widget		toplevel;
    XNavBrow		*brow;
    XNavBrow		*collect_brow;
    XNavBrow		*brow_stack[XNAV_BROW_MAX];
    int			brow_cnt;
    pwr_tObjid		root_objid;
    void		*root_item;
    t_trace_node	*TraceList;
    XtIntervalId	trace_timerid;
    int			trace_started;
    void 		(*message_cb)( void *, char, char *);
    void 		(*close_cb)( void *);
    void 		(*map_cb)( void *);
    void 		(*change_value_cb)( void *);
    void 		(*set_dimension_cb)( void *, int, int);
    int			ccm_func_registred;
    int			verify;    
    xnav_sMenu 		*menu_tree;
    Ev			*ev;
    Op			*op;
    CLog		*clog;
    int			closing_down;
    char		opplace_name[80];
    char		base_user[80];
    char		user[80];
    unsigned int	base_priv;
    unsigned int	priv;
    int			displayed;
    int                 current_logging_index;
    pwr_tObjid		search_last;
    char		search_string[200];
    int			search_last_found;
    regex_t		search_creg;
    int			search_compiled;
    xnav_eSearchType	search_type;
    int                 init_help;

    void start_trace( pwr_tObjid Objid, char *object_str);
    void start_trace_selected();
    void show_crossref();
    int create_object_item( pwr_tObjid objid, 
		brow_tNode dest, flow_eDest dest_code, void **item,
		int is_root);
    void print( char *filename);
    void zoom( double zoom_factor);
    void get_zoom( double *zoom_factor);
    void unzoom();
    int set_attr_value( char *value_str);
    int check_attr_value();
    int get_select( pwr_sAttrRef *attrref, int *is_attr);
    int get_select_all( pwr_sAttrRef **attrref, int **is_attr);
    int get_all_objects( pwr_sAttrRef **attrref, int **is_attr);
    int get_all_collect_objects( pwr_sAttrRef **attrref, int **is_attr);
    int collect_insert( pwr_sAttrRef *attrref);
    int collect_show();
    void collect_clear();
    void clear();
    void message( char sev, char *text);
    int brow_pop();
    int brow_push();
    int brow_push_all();
    void set_push_command( char *cmd);
    int find( pwr_tObjid objid, void **item);
    int display_object( pwr_tObjid objid, int open);
    void set_inputfocus();
    int setup();
    int	show_logging( int entry);
    void force_trace_scan();
    void menu_tree_build( xnav_sStartMenu *root);
    xnav_sMenu *menu_tree_build_children( xnav_sStartMenu *first_child,
		xnav_sMenu *parent);
    void menu_tree_free();
    void menu_tree_free_children( xnav_sMenu *first_child);
    int menu_tree_search( char *name, xnav_sMenu **menu_item);
    int menu_tree_search_children( char *name, xnav_sMenu *child_list,
		xnav_sMenu **menu_item);
    int menu_tree_insert( char *title, int item_type, char *command,
		char *destination, int dest_code, xnav_sMenu **menu_item);
    int menu_tree_delete( char *name);
    void pop();
    int load_ev_from_opplace();
    int login_from_opplace();
    static char *get_message( int sts);
    int show_object_as_struct( 
			pwr_tObjid	objid, 
			char		*type_str, 
			char		*file_str);
    void enable_events( XNavBrow *brow);
    int open_object( pwr_tObjid objid);
    int open_crossref( pwr_tObjid objid);
    void swap( int mode);


    // Command module member functions
    int command( char *cmd);
    int readcmdfile( char *incommand);
    int show_database();
    int get_current_object(
			pwr_tObjid	*objid,
			char		*objectname,
			int		size,
			pwr_tBitMask	nametype);
    int	show_file(
			char		*filename,
			char		*intitle,
			int		hide_dir);
    void update_time();
    int	find_name(
			char		*name,
			pwr_tObjid	*objid);
    int show_par_hier_class_name(
			char		*parametername,
			char		*hiername,
			char		*classname,
			char		*name,
			int		add,
			int		global,
			int		max_objects);
    int store(		char		*filename,
			int		collect);
    int show_symbols();
    void open_graph( char *name, char *filename, int scrollbar, int menu, 
	int navigator, int width, int height, int x, int y, char *object_name,
        char *focus, int inputempty, int use_default_access, 
        unsigned int access);
    void close_graph( char *filename, char *object_name);
    int exec_xttgraph( pwr_tObjid xttgraph, char *instance, char *focus,
        int inputemtpy);
    int set_parameter( char *name_str, char *value_str, int bypass);
    void open_rttlog( char *name, char *filename);
    int search( char *search_str, int regexp);
    int search_root( char *search_str, pwr_tObjid *found_objid, int next);
    int search_object( pwr_tObjid objid, char *search_str, 
	pwr_tObjid *found_objid, int next);
    int search_next();
    int create_object( char *classname, char *name);
    int delete_object( char *name);

    // Table module member functions
    int show_plcthreads();
    int show_nethandler();
    int show_subsrv();
    int show_subcli();
    int show_device();
    int show_channels( pwr_tObjid card_objid);
    int show_object( pwr_tObjid objid, brow_tNode node);
    int show_remnode();
    int show_remtrans( pwr_tObjid remnode_objid);
    int show_plcpgm();

    // Help module member functions
    int help( char *key, char *help_bookmark, navh_eHelpFile file_type,
	      char *file_name, int pop);
    int	help_index( navh_eHelpFile file_type, char *file_name, int pop);
};

int xnav_cut_segments (
  char	*outname,
  char	*name,
  int	segments);

int  xnav_attr_string_to_value( int type_id, char *value_str, 
	void *buffer_ptr, int buff_size, int attr_size);
void  xnav_attrvalue_to_string( int type_id, void *value_ptr, 
	char *str, int size, int *len, char *format);

Widget xnav_create_popup_menu( XNav *xnav, pwr_sAttrRef attrref,
			       xmenu_eItemType item_type,
			       xmenu_mUtility caller, unsigned int priv,
			       char *arg);
int xnav_call_method( XNav *xnav, char *method, char *filter,
		      pwr_sAttrRef attrref, 
		      xmenu_eItemType item_type, 
		      xmenu_mUtility caller,
		      unsigned int priv, char *arg);
void xnav_popup_menu_cb( void *xnav, pwr_sAttrRef attrref, 
			 unsigned long item_type,
			 unsigned long utility, char *arg, Widget *popup);
int xnav_call_method_cb( void *xnav, char *method, char *filter,
			 pwr_sAttrRef attrref,
			 unsigned long item_type,
			 unsigned long utility, char *arg);
void xnav_start_trace_cb( void *xnav, pwr_tObjid objid, char *name);
int xnav_get_trace_attr( pwr_tObjid objid, char *attr);

#endif


