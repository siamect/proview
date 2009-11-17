/** 
 * Proview   $Id: co_xhelp_motif.h,v 1.2 2008-10-31 12:51:30 claes Exp $
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
 **/

#ifndef cow_xhelp_motif_h
#define cow_xhelp_motif_h

#ifndef cow_xhelp_h
# include "cow_xhelp.h"
#endif

class CoXHelpMotif : public CoXHelp {
  public:
    CoXHelpMotif( 
	Widget 		xa_parent_wid, 
	void 		*xa_parent_ctx,
	xhelp_eUtility  utility,
	int             *xa_sts);
    Widget	parent_wid;
    Widget	brow_widget;
    Widget	form_widget;
    Widget	toplevel;
    Widget	xhelpnav_form;
    XtIntervalId focus_timerid;
    Widget	india_widget;
    Widget	india_label;
    Widget	india_text;

    void set_dimension( int width, int height);    
    static void open_input_dialog( CoXHelpMotif *xhelp, const char *text, const char *title,
				   char *init_text,
				   void (*ok_cb)( CoXHelp *, char *));
    static void activate_exit( Widget w, CoXHelpMotif *xhelp, XmAnyCallbackStruct *data);
    static void activate_zoom_in( Widget w, CoXHelpMotif *xhelp, XmAnyCallbackStruct *data);
    static void activate_zoom_out( Widget w, CoXHelpMotif *xhelp, XmAnyCallbackStruct *data);
    static void activate_zoom_reset( Widget w, CoXHelpMotif *xhelp, XmAnyCallbackStruct *data);
    static void activate_search( Widget w, CoXHelpMotif *xhelp, XmAnyCallbackStruct *data);
    static void activate_searchnext( Widget w, CoXHelpMotif *xhelp, XmAnyCallbackStruct *data);
    static void activate_searchprevious( Widget w, CoXHelpMotif *xhelp, XmAnyCallbackStruct *data);
    static void create_india_label( Widget w, CoXHelpMotif *xhelp, XmAnyCallbackStruct *data);
    static void create_india_text( Widget w, CoXHelpMotif *xhelp, XmAnyCallbackStruct *data);
    static void activate_india_ok( Widget w, CoXHelpMotif *xhelp, XmAnyCallbackStruct *data);
    static void activate_india_cancel( Widget w, CoXHelpMotif *xhelp, XmAnyCallbackStruct *data);
    static void activate_help( Widget w, CoXHelpMotif *xhelp, XmAnyCallbackStruct *data);
    static void create_xhelpnav_form( Widget w, CoXHelpMotif *xhelp, XmAnyCallbackStruct *data);
    static void enable_set_focus( CoXHelpMotif *xhelp);
    static void disable_set_focus( CoXHelpMotif *xhelp, int time);
    static void action_inputfocus( Widget w, XmAnyCallbackStruct *data);

    static Widget get_widget() { 
      if ( default_xhelp)
	return ((CoXHelpMotif *)default_xhelp)->parent_wid;
      else
	return 0;
    }

    ~CoXHelpMotif();
};

#endif







