#ifndef wb_xattone_h
#define wb_xattone_h

/* wb_xattone.h -- Single attribute editor

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif



class XAttOne {
  public:
    XAttOne( 
	Widget 		xa_parent_wid, 
	void 		*xa_parent_ctx, 
	pwr_sAttrRef 	*xa_objar,
	char 		*xa_title,
	unsigned int	xa_priv,
	int             *xa_sts);
    ~XAttOne();
    Widget	parent_wid;
    void 	*parent_ctx;
    pwr_sAttrRef aref;
    char 	name[80];
    unsigned int priv;
    Widget	form_widget;
    Widget	toplevel;
    Widget	msg_label;
    Widget	cmd_prompt;
    Widget	cmd_label;
    Widget	cmd_input;
    Widget	cmd_scrolledinput;
    Widget	cmd_scrolled_ok;
    Widget	cmd_scrolled_ap;
    Widget	cmd_scrolled_ca;
    int		input_open;
    int		access_rw;
    int         input_multiline;
    void 	*object;
    void	(*close_cb) ( void *, XAttOne *);
    int		(*is_authorized_cb)(void *, unsigned int);
    void	*client_data;
    brow_tObject input_node;
    char	input_name[80];
    int		set_focus_disabled;
    XtIntervalId focus_timerid;
    static char	value_recall[30][160];
    int	value_current_recall;
    pwr_tTid	atype;
    unsigned int asize;
    unsigned int aoffs;
    unsigned int aelem;

    void message( char severity, char *message);
    void set_prompt( char *prompt);
    int change_value( int set_focus);
    int open_changevalue( char *name);
    void change_value_close();
    void pop();
    void swap( int mode);
    int set_value();
};


#if defined __cplusplus
}
#endif
#endif







