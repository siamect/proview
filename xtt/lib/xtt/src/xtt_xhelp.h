#ifndef wb_xhelp_h
#define wb_xhelp_h

/* wb_xhelp.h -- Object attribute editor

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined __cplusplus
extern "C" {
#endif

#ifndef pwr_h
# include "pwr.h"
#endif



class XHelp {
  public:
    XHelp( 
	Widget 		xa_parent_wid, 
	void 		*xa_parent_ctx, 
	int             *xa_sts);
    ~XHelp();
    Widget	parent_wid;
    void 	*parent_ctx;
    char 	name[80];
    Widget	brow_widget;
    Widget	form_widget;
    Widget	toplevel;
    void	*xhelpnav;
    Widget	xhelpnav_form;
    void	*client_data;
    void	(*close_cb) ( void *, void *);
    int		set_focus_disabled;
    XtIntervalId focus_timerid;
    int         displayed;

    void pop();
    void clear();
    int help( char *key, char *help_bookmark, navh_eHelpFile file_type,
	      char *file_name);
    int	help_index( navh_eHelpFile file_type, char *file_name);
    void set_dimension( int width, int height);    
};


#if defined __cplusplus
}
#endif
#endif







