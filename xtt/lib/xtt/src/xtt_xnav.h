/* 
 * Proview   $Id: xtt_xnav.h,v 1.26 2008-09-18 14:58:26 claes Exp $
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

#ifndef xtt_xnav_h
#define xtt_xnav_h

/* xtt_xnav.h -- Simple navigator */

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

#ifndef flow_ctx_h
#include "flow_ctx.h"
#endif

#ifndef flow_api_h
#include "flow_api.h"
#endif

#ifndef flow_browctx_h
#include "flow_browctx.h"
#endif

#ifndef flow_browapi_h
#include "flow_browapi.h"
#endif

#ifndef xtt_xnav_brow_h
#include "xtt_xnav_brow.h"
#endif

#ifndef xtt_xnav_localdb_h
#include "xtt_localdb.h"
#endif

#ifndef xtt_menu_h
#include "xtt_menu.h"
#endif

#ifndef xtt_logging_h
#include "xtt_logging.h"
#endif

#ifndef pwr_privilege_h
#include "pwr_privilege.h"
#endif

#ifndef rt_sevcli_h
#include "rt_sevcli.h"
#endif

#define xnav_cVersion	"X3.0b"
#define XNAV_BROW_MAX	25
#define XNAV_LOGG_MAX   10

class CoWowTimer;
class XAtt;
class XCrr;
class Block;
class XttTrend;
class XttSevHist;
class XttFast;
class XAttOne;
class GeCurve;
class GeCurveData;
class XCrr;
class CoWow;
class XttAudio;
class Ev;
class Op;
class Hist;
class CLog;
class XttGe;
class RtTrace;
class XttFileview;
class CoLogin;

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

typedef enum {
  menu_ePixmap_Map,
  menu_ePixmap_Graph,
  menu_ePixmap_Script,
  menu_ePixmap_List,
  menu_ePixmap_Leaf
} menu_ePixmap;

typedef struct {
	char	title[80];
	int	item_type;
        int	pixmap;
	void	*action;
} xnav_sStartMenu;

typedef struct xnav_s_Menu {
	char			title[80];
	int			item_type;
	char			command[256];
        menu_ePixmap		pixmap;
	struct xnav_s_Menu	*child_list;
	struct xnav_s_Menu	*parent;
	struct xnav_s_Menu	*next;
} xnav_sMenu;

typedef struct s_trace_node t_trace_node;

struct s_trace_node {
  t_trace_node  *Next;
  RtTrace      *tractx;
  pwr_tObjid    Objid;
};

typedef enum {
	applist_eType_Trace,
	applist_eType_Graph,
	applist_eType_Attr,
	applist_eType_AttrOne,
	applist_eType_Trend,
	applist_eType_Crossref,
	applist_eType_Hist,
	applist_eType_Fast
} applist_eType;

class ApplListElem {
  public:
    ApplListElem( applist_eType al_type, void *al_ctx, pwr_sAttrRef *al_arp,
	char *al_name, char *al_instance);
    applist_eType	type;
    void		*ctx;
    pwr_sAttrRef       	aref;
    char		name[80];
    pwr_tAName          instance;
    ApplListElem 	*next;
};

class ApplList {
  public:
    ApplList() :
       root(NULL) {};
    
    ApplListElem *root;
    void insert( applist_eType type, void *ctx, 
	pwr_sAttrRef *arp, char *name, char *instance);
    void insert( applist_eType type, void *ctx, 
	pwr_tObjid objid, char *name, char *instance);
    void remove( void *ctx);
    int find( applist_eType type, char *name, char *instance, void **ctx);
    int find( applist_eType type, pwr_sAttrRef *arp, void **ctx);
    int find( applist_eType type, pwr_tObjid objid, void **ctx);
    void swap( int mode);
};

class XNavGbl {
  public:
    XNavGbl() :
      priv(0), UserObject(pwr_cNObjid), AlarmAutoLoad(0), AlarmMessage(0),
      AlarmBeep(0), AlarmReturn(0), AlarmAck(0), gdh_started(1),
      verify(0), scantime(0.5), signal_test_mode(0), advanced_user(0), show_truedb(0),
      show_allattr(0), no_graph_ratio(0)
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
    int			show_truedb;
    int			show_allattr;
    int			no_graph_ratio;

    int			load_config( void *xnav);
    int			symbolfile_exec( void *xnav);
};

class XNav {
  public:
    XNav(
	void *xn_parent_ctx,
	char *xn_name,
	xnav_sStartMenu *root_menu,
	char *xn_opplace_name,
	int xn_op_close_button,
	pwr_tStatus *status);
    virtual ~XNav();

    XNavGbl		gbl;
    XNavLdb		ldb;
    ApplList		appl;
    XttLogging          logg[XNAV_LOGG_MAX];
    void 		*parent_ctx;
    char 		name[80];
    XNavBrow		*brow;
    XNavBrow		*collect_brow;
    XNavBrow		*brow_stack[XNAV_BROW_MAX];
    int			brow_cnt;
    pwr_tObjid		root_objid;
    void		*root_item;
    t_trace_node	*TraceList;
    CoWowTimer		*trace_timerid;
    int			trace_started;
    void 		(*message_cb)( void *, char, char *);
    void 		(*close_cb)( void *, int);
    void 		(*map_cb)( void *);
    void 		(*change_value_cb)( void *);
    void 		(*set_dimension_cb)( void *, int, int);
    void 		(*selection_changed_cb)( void *);
    int			ccm_func_registred;
    int			verify;    
    xnav_sMenu 		*menu_tree;
    Ev			*ev;
    Op			*op;
    CLog		*clog;
    int			closing_down;
    pwr_tOName		opplace_name;
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
    int			attach_audio;
    XttAudio		*audio;
    CoWow		*wow;
    int			op_close_button;
    static xmenu_sMenuCall *mcp;
    CoLogin		*cologin;
    sevcli_tCtx 	scctx;

    virtual void set_inputfocus() {}
    virtual void pop() {}
    virtual void create_popup_menu( pwr_sAttrRef attrref,
				    xmenu_eItemType item_type,
				    xmenu_mUtility caller, unsigned int priv,
				    char *arg, int x, int y) {}
    virtual RtTrace *plctrace_new( pwr_tOid oid, pwr_tStatus *sts) {return 0;}
    virtual XAtt *xatt_new( pwr_tAttrRef *arp, int advanced_user, pwr_tStatus *sts) {return 0;}
    virtual XCrr *xcrr_new( pwr_tAttrRef *arp, int advanced_user, pwr_tStatus *sts) {return 0;}
    virtual Ev *ev_new( char *eve_name, char *ala_name, char *blk_name,
			pwr_tObjid ev_user, int display_ala, int display_eve,
			int display_blk, int display_return, int display_ack,
			int ev_beep, pwr_tStatus *status) { return 0;}
    virtual Hist *hist_new( char *title, pwr_tOid oid, pwr_tStatus *sts) {return 0;}
    virtual Block *block_new( pwr_tAttrRef *arp, char *name, unsigned int priv,
		      pwr_tStatus *sts) {return 0;}
    virtual Op *op_new( char *opplace, pwr_tStatus *sts) {return 0;}
    virtual XttTrend *xtttrend_new( char *name, pwr_tAttrRef *objar, pwr_tAttrRef *plotgroup,
			    pwr_tStatus *sts) {return 0;}
    virtual XttSevHist *xttsevhist_new( char *name, pwr_tOid *oid, pwr_tOName *aname,
				      sevcli_tCtx scctx, pwr_tStatus *sts) {return 0;}
    virtual XttFast *xttfast_new( char *name, pwr_tAttrRef *objar, pwr_tStatus *sts) {return 0;}
    virtual XAttOne *xattone_new( pwr_tAttrRef *objar, char *title, unsigned int priv,
			  pwr_tStatus *sts) {return 0;}
    virtual CLog *clog_new( char *name, pwr_tStatus *sts) {return 0;}
    virtual XttGe *xnav_ge_new( char *name, char *filename, int scrollbar, int menu, 
				int navigator, int width, int height, int x, int y, 
				double scan_time, char *object_name, 
				int use_default_access, unsigned int access,
				int (*xg_command_cb) (XttGe *, char *),
				int (*xg_get_current_objects_cb) (void *, pwr_sAttrRef **, int **),
				int (*xg_is_authorized_cb) (void *, unsigned int)) {return 0;}
    virtual GeCurve *gecurve_new( char *name, char *filename, GeCurveData *data,
				  int pos_right) {return 0;}
    virtual XttFileview *fileview_new( pwr_tOid oid, char *title, char *dir, char *pattern,
				       int type, char *target_attr, char *trigger_attr, 
				       char *filetype) {return 0;}
    virtual CoLogin *login_new( char		*wl_name,
				char		*wl_groupname,
				void		(* wl_bc_success)( void *),
				void		(* wl_bc_cancel)( void *),
				pwr_tStatus  	*status) { return 0;}
    virtual void bell( int time) {}

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
    int collect_remove();
    int collect_show();
    void collect_clear();
    void clear();
    void message( char sev, char *text);
    int brow_pop();
    int brow_push();
    int brow_push_all();
    void set_push_command( char *cmd);
    int find( pwr_tObjid objid, void **item);
    int find( pwr_tObjid objid, char *attr, void **item);
    int display_object( pwr_sAttrRef *arp, int open);
    int display_object( pwr_tObjid objid, int open);
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
    int menu_tree_insert( char *title, int item_type, char *command, menu_ePixmap pixmap,
		char *destination, int dest_code, xnav_sMenu **menu_item);
    int menu_tree_delete( char *name);
    int load_ev_from_opplace();
    int login_from_opplace();
    int login();
    void open_login();
    void logout();
    int is_authorized( unsigned int access = pwr_mAccess_AllRt, int msg = 1);
    static char *get_message( int sts);
    int show_object_as_struct( 
			pwr_tObjid	objid, 
			char		*type_str, 
			char		*file_str);
    void enable_events( XNavBrow *brow);
    int open_object( pwr_sAttrRef *arp);
    int open_crossref( pwr_sAttrRef *arp);
    void swap( int mode);
    int update_alarminfo();
    int sound( pwr_tAttrRef *arp);
    int sound_attached();
    
    
    static int init_brow_base_cb( FlowCtx *fctx, void *client_data);
    static int get_trace_attr( pwr_sAttrRef *arp, char *attr);
    static int attr_string_to_value( int type_id, char *value_str, 
				     void *buffer_ptr, int buff_size, int attr_size);
    static void attrvalue_to_string( int type_id, pwr_tTid tid, void *value_ptr, 
					   char *str, int size, int *len, char *format);
    static void trace_subwindow_cb( void *ctx, pwr_tObjid objid);
    static void trace_display_object_cb( void *ctx, pwr_tObjid objid);
    static int is_authorized_cb( void *xnav, unsigned int access);
    static void trace_collect_insert_cb( void *ctx, pwr_tObjid objid);
    static void trace_close_cb( RtTrace *tractx);
    static void trace_help_cb( RtTrace *tractx, char *key);
    static void xatt_close_cb( void *xnav, void *xatt);
    static void xcrr_close_cb( void *xnav, void *xcrr);
    static int brow_cb( FlowCtx *ctx, flow_tEvent event);
    static void trace_scan( void *data);
    static int trace_scan_bc( brow_tObject object, void *p);
    static int trace_connect_bc( brow_tObject object, char *name, char *attr, 
				 flow_eTraceType type, void **p);
    static int trace_disconnect_bc( brow_tObject object);
    static int init_brow_collect_cb( BrowCtx *ctx, void *client_data);
    static int init_brow_cb( BrowCtx *ctx, void *client_data);

    // Command module member functions
    int command( char *cmd);
    pwr_tStatus get_command_sts();
    int readcmdfile( char *incommand);
    int show_database();
    int get_current_object(
			pwr_tObjid	*objid,
			char		*objectname,
			int		size,
			pwr_tBitMask	nametype);
    int get_current_aref(
			pwr_sAttrRef	*arp,
			char		*arname,
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
    void print_methods();

    // Table module member functions
    int show_plcthreads();
    int show_nethandler();
    int show_subsrv();
    int show_subcli();
    int show_device();
    int show_channels( pwr_tObjid card_objid);
    int show_object( pwr_tAttrRef *oarp, brow_tNode node);
    int show_remnode();
    int show_remtrans( pwr_tObjid remnode_objid);
    int show_plcpgm();

    // Help module member functions
    int help( char *key, char *help_bookmark, navh_eHelpFile file_type,
	      char *file_name, int pop);
    int	help_index( navh_eHelpFile file_type, char *file_name, int pop);

    // Methods and Popup menu functions
    virtual void get_popup_menu( pwr_sAttrRef attrref,
				 xmenu_eItemType item_type, 
				 xmenu_mUtility caller, 
				 unsigned int priv, char *arg, int x, int y) {}
    void get_popup_menu_items( pwr_sAttrRef attrref,
			       xmenu_eItemType item_type, 
			       xmenu_mUtility caller, 
			       unsigned int priv, char *arg);
    static int CallMenuMethod( xmenu_sMenuCall *ip, int idx);
    static int CheckMenuMethodFilter( xmenu_sMenuCall *ip, int idx);
    int call_method( char *method, char *filter,
		     pwr_sAttrRef attrref, 
		     xmenu_eItemType item_type, 
		     xmenu_mUtility caller,
		     unsigned int priv, char *arg);
    int call_object_method( pwr_sAttrRef attrref, 
			    xmenu_eItemType item_type, 
			    xmenu_mUtility caller, 
			    unsigned int priv, char *method_name);
    int check_object_methodfilter( pwr_sAttrRef attrref, 
				   xmenu_eItemType item_type, 
				   xmenu_mUtility caller, 
				   unsigned int priv, char *method_name);
    static int GetObjectMenu( xmenu_sMenuCall *ip,
			      pwr_tCid classid,
			      xmenu_sMenuItem **Item,
			      pwr_tUInt32 Level,
			      int *nItems,
			      int AddSeparator,
			      pwr_sAttrRef *CurrentObject);
    static int GetMethod( char *name, 
			  pwr_tStatus (**method)( xmenu_sMenuCall *));
    static int getAllMenuItems( xmenu_sMenuCall *ip,
				xmenu_sMenuItem	**Item,
				pwr_tObjid objid,
				pwr_tUInt32 Level,
				int *nItems,
				int AddSeparator,
				pwr_sAttrRef *CurrentObject);
    static int GetMenu( xmenu_sMenuCall *ip);

};

int xnav_cut_segments (
  char	*outname,
  char	*name,
  int	segments);

int  xnav_attr_string_to_value( int type_id, char *value_str, 
	void *buffer_ptr, int buff_size, int attr_size);
void  xnav_attrvalue_to_string( int type_id, pwr_tTid tid, void *value_ptr, 
	char *str, int size, int *len, char *format);

void xnav_create_popup_menu( XNav *xnav, pwr_sAttrRef attrref,
			     xmenu_eItemType item_type,
			     xmenu_mUtility caller, unsigned int priv,
			     char *arg, int x, int y);
int xnav_call_method( XNav *xnav, char *method, char *filter,
		      pwr_sAttrRef attrref, 
		      xmenu_eItemType item_type, 
		      xmenu_mUtility caller,
		      unsigned int priv, char *arg);
int xnav_call_object_method( XNav *xnav, pwr_sAttrRef attrref, 
			     xmenu_eItemType item_type, 
			     xmenu_mUtility caller, 
			     unsigned int priv, char *method_name);
int xnav_check_object_methodfilter( XNav *xnav, pwr_sAttrRef attrref, 
				    xmenu_eItemType item_type, 
				    xmenu_mUtility caller, 
				    unsigned int priv, char *method_name);

void xnav_popup_menu_cb( void *xnav, pwr_sAttrRef attrref, 
			 unsigned long item_type,
			 unsigned long utility, char *arg, int x, int y);
int xnav_call_method_cb( void *xnav, char *method, char *filter,
			 pwr_sAttrRef attrref,
			 unsigned long item_type,
			 unsigned long utility, char *arg);
void xnav_start_trace_cb( void *xnav, pwr_tObjid objid, char *name);
int xnav_get_trace_attr( pwr_sAttrRef *arp, char *attr);

#endif


