#ifndef wb_xatt_h
#define wb_xatt_h

/* wb_xatt.h -- Object attribute editor

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif



class XAtt {
  public:
    XAtt( 
	Widget 		xa_parent_wid, 
	void 		*xa_parent_ctx, 
	pwr_tObjid 	xa_objid,
	int 		xa_advanced_user,
	int             *xa_sts);
    ~XAtt();
    Widget	parent_wid;
    void 	*parent_ctx;
    pwr_tObjid	objid;
    char 	name[80];
    Widget	brow_widget;
    Widget	form_widget;
    Widget	toplevel;
    void	*xattnav;
    void	*root_item;
    Widget	msg_label;
    Widget	cmd_prompt;
    Widget	cmd_input;
    Widget	cmd_scrolledinput;
    Widget	cmd_scrolled_ok;
    Widget	cmd_scrolled_ca;
    Widget	xattnav_form;
    int		input_open;
    int         input_multiline;
    void 	*object;
    void	(*close_cb) ( void *, void *);
    void	(*redraw_cb) (void *);
    void 	(*popup_menu_cb)( void *, pwr_sAttrRef, unsigned long,
				  unsigned long, char *, Widget * );
    int         (*call_method_cb)(void *, char *, char *, pwr_sAttrRef,
				  unsigned long, unsigned long, char *);
    int		(*is_authorized_cb)(void *, unsigned int);
    void	*client_data;
    brow_tObject input_node;
    char	input_name[80];
    int		set_focus_disabled;
    XtIntervalId focus_timerid;
    static char	value_recall[30][160];
    int	value_current_recall;

    void message( char severity, char *message);
    void set_prompt( char *prompt);
    void change_value( int set_focus);
    int open_changevalue( char *name);
    void change_value_close();
    void pop();
    void swap( int mode);
};


#if defined __cplusplus
}
#endif
#endif







