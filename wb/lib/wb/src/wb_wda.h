/* 
 * Proview   $Id: wb_wda.h,v 1.4 2005-09-06 10:43:32 claes Exp $
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

#ifndef wb_wda_h
#define wb_wda_h

/* wb_wda.h -- Spreadsheet editor */

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


class Wda {
  public:
    Wda( 
	Widget 		wa_parent_wid, 
	void 		*wa_parent_ctx, 
	ldh_tSesContext wa_ldhses,
	pwr_tObjid 	wa_objid,
	pwr_tClassId 	wa_classid,
        char            *wa_attribute,
	int 		wa_editmode,
	int 		wa_advanced_user,
	int		wa_display_objectname);
    ~Wda();
    Widget	parent_wid;
    void 	*parent_ctx;
    ldh_tSesContext ldhses;
    pwr_tObjid	objid;
    pwr_tClassId classid;
    char        attribute[80];
    int		editmode;
    char 	name[80];
    Widget	brow_widget;
    Widget	form_widget;
    Widget	toplevel;
    void	*wdanav;
    void	*root_item;
    Widget	msg_label;
    Widget	cmd_prompt;
    Widget	cmd_input;
    Widget	cmd_scrolledinput;
    Widget	cmd_scrolled_ok;
    Widget	cmd_scrolled_ca;
    Widget	wdanav_form;
    Widget      wdaclass_dia;
    Widget      wdaclass_hiervalue;
    Widget      wdaclass_classvalue;
    Widget      wdaclass_attrobjects;
    Widget      wdaattr_dia;
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
    int attrobjects;

    void message( char severity, char *message);
    void set_prompt( char *prompt);
    void change_value( int set_focus);
    void set_editmode( int editmode, ldh_tSesContext ldhses);
    int open_changevalue( char *name);
    void change_value_close();
    void pop();
    void open_class_dialog();
    void open_attr_dialog();
    int next_attr();
    int prev_attr();
};


#if defined __cplusplus
}
#endif
#endif







