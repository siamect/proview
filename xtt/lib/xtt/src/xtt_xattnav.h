#ifndef wb_xattnav_h
#define wb_xattnav_h

/* wb_xattnav.h -- Simple navigator

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

#ifndef flow_browapi_h
#include "flow_browapi.h"
#endif

#ifndef xtt_xnav_brow_h
#include "xtt_xnav_brow.h"
#endif

#define xattnav_cVersion	"X3.3b"
#define XATTNAV_BROW_MAX	25

typedef enum {
  xattnav_eType_Object,
  xattnav_eType_CrossRef
} xattnav_eType;

class XAttNav {
  public:
    XAttNav(
	void 		*xa_parent_ctx,
	Widget		xa_parent_wid,
        xattnav_eType   xa_type,
	char 		*xa_name,
	pwr_tObjid 	xa_objid,
	int 		xa_advanced_user,
	Widget 		*w,
	pwr_tStatus 	*status);
    ~XAttNav();

    void 		*parent_ctx;
    Widget		parent_wid;
    xattnav_eType       type;
    char 		name[80];
    Widget		brow_widget;
    Widget		form_widget;
    Widget		toplevel;
    XNavBrow		*brow;
    pwr_tObjid		objid;
    int			advanced_user;
    int			bypass;
    XtIntervalId	trace_timerid;
    int			trace_started;
    void 		(*message_cb)( void *, char, char *);
    void 		(*close_cb)( void *);
    void 		(*change_value_cb)( void *);
    void 		(*popup_menu_cb)( void *, pwr_sAttrRef, unsigned long,
					  unsigned long, char *, Widget *);
    void 		(*start_trace_cb)( void *, pwr_tObjid, char *);
    int		        (*is_authorized_cb)(void *, unsigned int);
    int			displayed;

    void start_trace( pwr_tObjid Objid, char *object_str);
    int set_attr_value( brow_tObject node, char *name, char *value_str);
    int check_attr( int *multiline, brow_tObject *node, char *name,
		char **init_value, int *size);
    int get_select( pwr_sAttrRef *attrref, int *is_attr);
    void message( char sev, char *text);
    void set_inputfocus();
    void force_trace_scan();
    int object_attr();
    int crossref();
    int object_exist( brow_tObject object);
    void redraw();
    void enable_events();
    int select_by_name( char *name);
    void start_trace();
};


#if defined __cplusplus
}
#endif
#endif
