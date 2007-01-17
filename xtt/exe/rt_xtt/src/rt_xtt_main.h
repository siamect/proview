/* 
 * Proview   $Id: rt_xtt_main.h,v 1.2 2007-01-17 06:18:11 claes Exp $
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
 */

#ifndef rt_xtt_main_h
#define rt_xtt_main_h

/* rt_xtt_main.h -- Simple navigator */


#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef flow_h
#include "flow.h"
#endif

#ifndef flow_browctx_h
#include "flow_browapi.h"
#endif

#ifndef rt_qcom_h
#include "rt_qcom.h"
#endif

#ifndef xtt_xnav_h
#include "xtt_xnav.h"
#endif

class CoWow;
class CoWowTimer;
class XttMethodToolbar;

class Xtt {
  public:
    void 	*parent_ctx;
    char 	name[80];
    XNav	*xnav;
    void	*root_item;
    int		input_open;
    int		command_open;
    void	(*india_ok_cb)( Xtt *, char *);
    qcom_sQid	queid;
    CoWowTimer  *timerid;
    CoWow	*wow;
    pwr_tOName	opplace_str;
    int 	quiet;
    int 	attach_audio;
    XttMethodToolbar *methodtoolbar;

    Xtt( int argc, char *argv[], int *return_sts);
    virtual ~Xtt() {}
    virtual void message( char severity, char *msg) {}
    virtual void open_input_dialog( char *text, char *title,
				    char *init_text,
				    void (*ok_cb)( Xtt *, char *)) {}
    virtual void set_prompt( char *prompt) {}
    virtual void open_change_value() {}

    void hotkey_activate_command( char *cmdp);
    static void hotkey_activate_toggledig( char *namep);
    static void hotkey_activate_setdig( char *namep);
    static void hotkey_activate_resetdig( char *namep);
  
    static void open_URL_cb( void *ctx, char *url);
    static void qcom_events( void *data);
    static void findregex_ok( Xtt *xtt, char *search_str);
    static void find_ok( Xtt *xtt, char *search_str);
    static void xtt_message_cb( void *ctx, char severity, char *msg);
    static void close_ok( void *ctx, void *data);
    static void change_value( void *ctx);
    static void selection_changed( void *ctx);
    
    void activate_print();
    void activate_find();
    void activate_findregex();
    void activate_findnext();
    void activate_collapse();
    void activate_openobject();
    void activate_openplc();
    void activate_opengraph();
    void activate_collect_insert();
    void activate_advanceduser();
    void activate_zoom_in();
    void activate_zoom_out();
    void activate_help();
    void activate_help_project();
    void activate_help_proview();
    void activate_method( char *method, char *filter);

    static Xtt *hot_xtt;
    static xnav_sStartMenu alarm_menu[];
    static xnav_sStartMenu nethandler_menu[];
    static xnav_sStartMenu communication_menu[];
    static xnav_sStartMenu logging_menu[];
    static xnav_sStartMenu system_menu[];
    static xnav_sStartMenu root_menu[];
};

#endif




















