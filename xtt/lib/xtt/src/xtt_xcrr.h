#ifndef wb_xcrr_h
#define wb_xcrr_h

/* wb_xcrr.h -- Object attribute editor

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif


class XCrr {
  public:
    XCrr( 
	Widget 		xa_parent_wid, 
	void 		*xa_parent_ctx, 
	pwr_tObjid 	xa_objid,
	int 		xa_advanced_user,
	int             *xa_sts);
    ~XCrr();
    Widget	parent_wid;
    void 	*parent_ctx;
    pwr_tObjid	objid;
    char 	name[80];
    Widget	brow_widget;
    Widget	form_widget;
    Widget	toplevel;
    void	*xcrrnav;
    void	*root_item;
    Widget	xcrrnav_form;
    int		input_open;
    int         input_multiline;
    void 	*object;
    void	(*close_cb) ( void *, void *);
    void	(*redraw_cb) (void *);
    void 	(*popup_menu_cb)( void *, pwr_sAttrRef, unsigned long,
				  unsigned long, char *, Widget * );
    void 	(*start_trace_cb)( void *, pwr_tObjid, char *);
    void	*client_data;
    brow_tObject input_node;
    char	input_name[80];
    int		set_focus_disabled;
    XtIntervalId focus_timerid;

    void pop();
};


#if defined __cplusplus
}
#endif
#endif







