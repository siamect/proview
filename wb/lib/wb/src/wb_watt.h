#ifndef wb_watt_h
#define wb_watt_h

/* wb_watt.h -- Object attribute editor

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef wb_h
# include "wb.h"
#endif

#ifndef wb_ldh_h
# include "wb_ldh.h"
#endif


class WAtt {
  public:
    WAtt( 
	Widget 		wa_parent_wid, 
	void 		*wa_parent_ctx, 
	ldh_tSesContext wa_ldhses,
	pwr_tObjid 	wa_objid,
	int 		wa_editmode,
	int 		wa_advanced_user,
	int		wa_display_objectname);
    ~WAtt();
    Widget	parent_wid;
    void 	*parent_ctx;
    ldh_tSesContext ldhses;
    pwr_tObjid	objid;
    int		editmode;
    char 	name[80];
    Widget	brow_widget;
    Widget	form_widget;
    Widget	toplevel;
    void	*wattnav;
    void	*root_item;
    Widget	msg_label;
    Widget	cmd_prompt;
    Widget	cmd_input;
    Widget	cmd_scrolledinput;
    Widget	cmd_scrolled_ok;
    Widget	cmd_scrolled_ca;
    Widget	wattnav_form;
    int		input_open;
    int         input_multiline;
    void 	*object;
    void	(*close_cb) ( void *);
    void	(*redraw_cb) (void *);
    void	*client_data;
    brow_tObject input_node;
    char	input_name[80];
    wb_eUtility	utility;
    int		set_focus_disabled;
    XtIntervalId focus_timerid;
    static char	value_recall[30][160];
    int	value_current_recall;

    void message( char severity, char *message);
    void set_prompt( char *prompt);
    void change_value( int set_focus);
    void set_editmode( int editmode, ldh_tSesContext ldhses);
    int open_changevalue( char *name);
    void change_value_close();
    void pop();
};


#if defined __cplusplus
}
#endif
#endif







