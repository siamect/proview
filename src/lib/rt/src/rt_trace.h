#ifndef rt_trace_h
#define rt_trace_h

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef flow_h
#include "flow.h"
#endif

#ifndef flow_api_h
#include "flow_api.h"
#endif


typedef struct s_commonpart
{
  void          *parent_ctx;
  Widget        parent_wid;
  char          *name;
} trace_t_commonpart, *commonpart;

typedef struct tra_sCtx *tra_tCtx;
typedef struct trace_s_node trace_t_node;

struct trace_s_node {
  trace_t_node  *Next;
  tra_tCtx    tractx;
};

struct tra_sCtx{
  trace_t_commonpart	cp;
  Widget		flow_widget;
  Widget		toplevel;
  Widget		form;
  Widget		menu;
  Widget		nav_shell;
  Widget		nav_widget;
  flow_tNodeClass	trace_analyse_nc;
  flow_tConClass	trace_con_cc;
  int			trace_started;
  XtIntervalId		trace_timerid;
  flow_tCtx		flow_ctx;
  flow_tNode		trace_changenode;
  pwr_tObjid		objid;
  void			(*close_cb) (tra_tCtx);
  void			(*help_cb) (tra_tCtx, char *);
  void			(*subwindow_cb) (void *parent_ctx, pwr_tObjid objid);
  void			(*display_object_cb) (void *parent_ctx, pwr_tObjid objid);
  void			(*collect_insert_cb) (void *parent_ctx, pwr_tObjid objid);
  int                   (*is_authorized_cb)(void *parent_ctx, unsigned int access); 
  void                  (*popup_menu_cb)(void *parent_ctx, 
					 pwr_sAttrRef attrref,
					 unsigned long item_type,
					 unsigned long utility, 
					 char *arg, Widget *popup); 
  int                   (*call_method_cb)(void *parent_ctx, char *method,
					  char *filter,
					  pwr_sAttrRef attrref,
					  unsigned long item_type,
					  unsigned long utility, 
					  char *arg);
  trace_t_node		*trace_list;
};


tra_tCtx trace_new( 	void 		*parent_ctx, 
			Widget 		parent_wid, 
			pwr_tObjid	objid,
			void 		(*close_cb)(tra_tCtx),
			void 		(*help_cb)(tra_tCtx, char *),
			void 		(*subwindow_cb)(void *, pwr_tObjid),
			void 		(*display_object_cb)(void *, pwr_tObjid),
			void 		(*collect_insert_cb)(void *, pwr_tObjid),
			int             (*is_authorized_cb)(void *, unsigned int) 
);
void	trace_del( tra_tCtx	tractx);

int trace_search_object( 	tra_tCtx	tractx,
				char		*object_str);
void trace_pop( tra_tCtx	tractx);

#endif

