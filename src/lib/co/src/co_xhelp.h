#ifndef co_xhelp_h
#define co_xhelp_h

/* co_xhelp.h -- Object attribute editor

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

//#if defined __cplusplus
//extern "C" {
//#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef co_xhelpnav_h
# include "co_xhelpnav.h"
#endif

class CoXHelp {
  public:
    CoXHelp( 
	Widget 		xa_parent_wid, 
	void 		*xa_parent_ctx,
	xhelp_eUtility  utility,
	int             *xa_sts);
    Widget	parent_wid;
    void 	*parent_ctx;
    char 	name[80];
    Widget	brow_widget;
    Widget	form_widget;
    Widget	toplevel;
    CoXHelpNav	*xhelpnav;
    Widget	xhelpnav_form;
    void	*client_data;
    void	(*close_cb) ( void *, void *);
    void 	(*open_URL_cb)( void *, char *);
    int		set_focus_disabled;
    XtIntervalId focus_timerid;
    int         displayed;
    Widget	india_widget;
    Widget	india_label;
    Widget	india_text;
    void	(*india_ok_cb)( CoXHelp *, char *);

    static CoXHelp *default_xhelp;

    void pop();
    void map();
    void clear();
    int help( char *key, char *help_bookmark, navh_eHelpFile file_type,
	      char *file_name, bool strict);
    int	help_index( navh_eHelpFile file_type, char *file_name);
    void set_dimension( int width, int height);    
    static void set_default( CoXHelp *xhelp) { default_xhelp = xhelp;}
    static int dhelp( char *key, char *help_bookmark, navh_eHelpFile file_type,
		      char *file_name, bool strict);
    static int	dhelp_index( navh_eHelpFile file_type, char *file_name);
    ~CoXHelp();
};


//#if defined __cplusplus
//}
//#endif
#endif







