#ifndef wb_wpkg_h
#define wb_wpkg_h

/* wb_wpkg.h -- Object attribute editor

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

class WPkgNav;

class WPkg {
  public:
    WPkg( 
	Widget 		wa_parent_wid, 
	void 		*wa_parent_ctx);
    ~WPkg();
    Widget	parent_wid;
    void 	*parent_ctx;
    char 	name[80];
    Widget	brow_widget;
    Widget	form_widget;
    Widget	toplevel;
    WPkgNav	*wpkgnav;
    Widget	msg_label;
    Widget	wpkgnav_form;
    void	(*close_cb) ( void *);
    wb_eUtility	utility;
    int		set_focus_disabled;
    XtIntervalId focus_timerid;
    int		display_mode;
    int		clock_cursor;

    void message( char severity, char *message);
    void pop();
    void set_clock_cursor();
    void reset_cursor();
};


#if defined __cplusplus
}
#endif
#endif







