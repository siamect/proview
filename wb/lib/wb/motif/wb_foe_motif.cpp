/* 
 * Proview   $Id: wb_foe_motif.cpp,v 1.2 2008-06-24 07:52:21 claes Exp $
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

//   wb_foe_motif.cpp
//   This module creates function object editor and handles
//   the graphical editor.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "flow.h"
#include "flow_ctx.h"
#include "flow_browapi.h"
#include "pwr.h"
#include "pwr_baseclasses.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "wb_ldh.h"
#include "wb_foe_msg.h"
#include "wb_vldh_msg.h"
#include "wb_vldh.h"

#include <X11/Xlib.h>
#include <Xm/Xm.h>
#include <Xm/Text.h>
#include <Xm/ToggleB.h>
#include <Xm/Protocols.h>
#include <Xm/RowColumn.h>
#include <Xm/PushBG.h>
#include <Xm/SashP.h>

#include "wb_watt_motif.h"
#include "wb_gre_motif.h"
#include "wb_foe_motif.h"
#include "co_login.h"
#include "co_mrm_util.h"
#include "wb_nav_motif.h"
#include "wb_pal_motif.h"
#include "co_wow_motif.h"
#include "flow_x.h"

#define	BEEP	    putchar( '\7' );

#define	CLOCK_CURSOR	

#define	NORMAL_CURSOR	

#define	GRE_CLOCK_CURSOR	

#define	GRE_NORMAL_CURSOR	

static Pixmap icon_pixmap = 0;

//	Callback from the menu.
void WFoeMotif::activate_save( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_save();
}

//	Callback from the menu.
void WFoeMotif::activate_exit( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
}

//	Callback from the menu.
void WFoeMotif::activate_winddelete( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_winddelete();
}


//	Callback from the menu.
void WFoeMotif::activate_quit( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_quit();
}

//	Callback from the menu.
void WFoeMotif::activate_savetrace( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_savetrace();
}

void WFoeMotif::activate_restoretrace( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_restoretrace();
}

//	Callback from the menu.
void WFoeMotif::activate_print( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_print();
}

//	Callback from the menu.
void WFoeMotif::activate_printoverv( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_printoverv();
}

//	Callback from the menu.
void WFoeMotif::activate_printselect( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_printselect();
}

//	Callback from the menu.
void WFoeMotif::activate_syntax( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_syntax();
}

//	Callback from the menu.
void WFoeMotif::activate_compile( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_compile();
}

//	Callback from the menu.
void WFoeMotif::activate_delete( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_delete();
}

//	Callback from the menu.
void WFoeMotif::activate_changetext( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_changetext();
}

//	Callback from the menu.
void WFoeMotif::activate_cut( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_cut();
}

//	Callback from the menu.
void WFoeMotif::activate_copy( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_copy();
}

//	Callback from the menu.
void WFoeMotif::activate_paste( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_paste();
}

//	Callback from the menu.
void WFoeMotif::activate_attribute( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_attribute();
}


//	Callback from the menu.
void WFoeMotif::activate_subwindow( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_subwindow();
}

//	Callback from the menu.
void WFoeMotif::activate_undelete( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_undelete();
}

//	Callback from the menu.
void WFoeMotif::activate_unselect( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_unselect();
}

//
//	Callback from the menu.
//	Sets gridsize.
//
void WFoeMotif::activate_gridsize1( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->gre->set_grid_size( 0.01);
}

void WFoeMotif::activate_gridsize2( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->gre->set_grid_size( 0.02);
}

void WFoeMotif::activate_gridsize3( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->gre->set_grid_size( 0.05);
}

void WFoeMotif::activate_gridsize4( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->gre->set_grid_size( 0.10);
}

void WFoeMotif::activate_gridsize5( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->gre->set_grid_size( 0.20);
}

//
//	Callback from the menu.
//	Draws or deletes the grid.
//
void WFoeMotif::activate_showgrid( Widget w, WFoe *foe, XmToggleButtonCallbackStruct *data)
{
#if 0
  if ( data->set > 1) return;
  if ( foe->msg_label_id != 0 ) foe->message( ""); 

  if(data->set == 1) {
    foe->gre->drawgrid();
  }
  else  /* data->set == 0 */ {
    foe->gre->undrawgrid();
  }
#endif
}

//	Callback from the menu.
void WFoeMotif::activate_redraw( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_redraw();
}

//	Callback from the menu.
void WFoeMotif::activate_zoomin( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_zoomin();
}

//	Callback from the menu.
void WFoeMotif::activate_zoomout( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_zoomout();
}

//	Callback from the menu.
void WFoeMotif::activate_unzoom( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_unzoom();
}

//
//	Callback from the menu.
//	Display or hide the connection palette.
//
void WFoeMotif::activate_palette_con( Widget w, WFoe *foe, XmToggleButtonCallbackStruct *data)
{

  if ( data->set > 1) return;
  if ( foe->msg_label_id != 0 ) foe->message( ""); 

  if ( data->set)
    XtManageChild( ((WFoeMotif *)foe)->widgets.con_palette);
  else
    XtUnmanageChild( ((WFoeMotif *)foe)->widgets.con_palette);
  foe->con_palette_managed = data->set;
  foe->con_palctx->set_inputfocus( data->set);
}

//
//	Callback from the menu.
//	Display or hide the connection palette.
//
void WFoeMotif::activate_palette_object( Widget w, WFoe *foe, XmToggleButtonCallbackStruct *data)
{

  if ( data->set > 1) return;
  if ( foe->msg_label_id != 0 ) foe->message( ""); 

  if ( data->set)
    XtManageChild( ((WFoeMotif *)foe)->widgets.node_palette);
  else
    XtUnmanageChild( ((WFoeMotif *)foe)->widgets.node_palette);
  foe->node_palette_managed = data->set;
  foe->node_palctx->set_inputfocus( data->set);
}

//
//	Callback from the menu.
//	Display or hide the connection palette.
//
void WFoeMotif::activate_palette_plant( Widget w, WFoe *foe, XmToggleButtonCallbackStruct *data)
{

  if ( data->set > 1) return;
  if ( foe->msg_label_id != 0 ) foe->message( ""); 

  if ( data->set)
    XtManageChild( ((WFoeMotif *)foe)->widgets.nav_palette);
  else
    XtUnmanageChild( ((WFoeMotif *)foe)->widgets.nav_palette);
  foe->nav_palette_managed = data->set;
  foe->navctx->set_inputfocus( data->set);
}

// 	Callback from the menu.
void WFoeMotif::activate_refcon( Widget w, WFoe *foe, XmToggleButtonCallbackStruct *data)
{
  if ( data->set > 1) return;
  foe->activate_refcon( data->set);
}

// 	Callback from the menu.
void WFoeMotif::activate_showexeord( Widget w, WFoe *foe, XmToggleButtonCallbackStruct *data)
{
  if ( data->set > 1) return;
  foe->activate_showexeord( data->set);
}


//	Callback from the menu.
void WFoeMotif::activate_plcattribute( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{  
  foe->activate_plcattribute();
}

//	Callback from the menu.
//	Not yet implemented.
void WFoeMotif::activate_search( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
}

//	Callback from the menu.
void WFoeMotif::activate_searchobject( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_searchobject();
}

//	Callback from the menu.
void WFoeMotif::activate_searchstring( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_searchstring();
}

//	Callback from the menu.
void WFoeMotif::activate_searchnext( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_searchnext();
}

//	Callback from the edit entry of the foe_menu widget
void WFoeMotif::activate_getobj( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_getobj();
}

//	Callback from the edit entry of the foe_menu widget
void WFoeMotif::activate_expand( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_expand();
}

//	Callback from the edit entry of the foe_menu widget
void WFoeMotif::activate_compress( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_compress();
}

//
//	Display help window for topic plc editor
//
void WFoeMotif::activate_help( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_help();
}

//
//	Display class help for selected object
//
void WFoeMotif::activate_helpclass( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  foe->activate_helpclass();
}

//	Callbacks from the toggle buttons under the function entry.
//	This callbacks are called when the value of a toggle button is changed.
//	The function pulldown menu is a radio box which ensure that 
//	one and only one toggle button is set at any time:
//	If the callback is generated by the radio entry 
//	(and not directly by the user ) the event is 0.
//
//	These callbacks are use to determine in which mode the user works: 
// 	SG 01.05.91
//
void WFoeMotif::activate_trace_togg( Widget w, WFoe *foe, XmToggleButtonCallbackStruct *data)
{
  if ( data->set > 1 || data->event == 0)
    return;

  foe->activate_trace_togg( data->set);
}

//
//	Callbacks from the toggle buttons under the customize entry.
//
void WFoeMotif::activate_simulate_togg( Widget w, WFoe *foe, XmToggleButtonCallbackStruct *data)
{
  if ( data->set > 1 || data->event == 0)
    return;

  foe->activate_simulate_togg( data->set);
}

void WFoeMotif::activate_view_togg( Widget w, WFoe *foe, XmToggleButtonCallbackStruct *data)
{
  if ( data->set > 1 || data->event == 0)
    return;

  foe->activate_view_togg( data->set);
}

void WFoeMotif::activate_edit_togg( Widget w, WFoe *foe, XmToggleButtonCallbackStruct *data)
{
  if ( data->set > 1 || data->event == 0)
    return;

  foe->activate_edit_togg( data->set);
}


//
//	Store the id of the created widget in the context
// 	Author : SG
// 	Date : 25.01.91
//
void WFoeMotif::create_label( Widget w, WFoe *foe, XmAnyCallbackStruct *data) 
{ 
  ((WFoeMotif *)foe)->widgets.label= w ; 
}

void WFoeMotif::create_textinput( Widget w, WFoe *foe, XmAnyCallbackStruct *data) 
{
  ((WFoeMotif *)foe)->widgets.textinput = w ; 
}

void WFoeMotif::create_widget_cb( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  int key;

  XtVaGetValues (w, XmNuserData, &key, NULL);

  switch ( key) {
  case 1:
    ((WFoeMotif *)foe)->widgets.file_entry = w;
    break;
  case 2:
    ((WFoeMotif *)foe)->widgets.save = w;
    break;
  case 3:
    ((WFoeMotif *)foe)->widgets.print = w;
    break;
  case 6:
    ((WFoeMotif *)foe)->widgets.syntax = w;
    break;
  case 7:
    ((WFoeMotif *)foe)->widgets.compile = w;
    break;
  case 8:
    ((WFoeMotif *)foe)->widgets.plcattribute = w;
    break;
  case 9:
    ((WFoeMotif *)foe)->widgets.winddelete = w;
    break;
  case 10:
    ((WFoeMotif *)foe)->widgets.savetrace = w;
    break;
  case 11:
    ((WFoeMotif *)foe)->widgets.restoretrace = w;
    break;
  case 12:
    ((WFoeMotif *)foe)->widgets.quit = w;
    break;
  case 13:
    ((WFoeMotif *)foe)->widgets.exit = w;
    break;
  case 14:
    ((WFoeMotif *)foe)->widgets.edit_entry = w;
    break;
  case 15:
    ((WFoeMotif *)foe)->widgets.cut = w;
    break;
  case 16:
    ((WFoeMotif *)foe)->widgets.copy = w;
    break;
  case 17:
    ((WFoeMotif *)foe)->widgets.paste = w;
    break;
  case 18:
    ((WFoeMotif *)foe)->widgets.undelete = w;
    break;
  case 19:
    ((WFoeMotif *)foe)->widgets.unselect = w;
    break;
  case 20:
    ((WFoeMotif *)foe)->widgets.connect = w;
    break;
  case 21:
    ((WFoeMotif *)foe)->widgets.del = w;
    break;
  case 22:
    ((WFoeMotif *)foe)->widgets.changetext = w;
    break;
  case 23:
    ((WFoeMotif *)foe)->widgets.expand = w;
    break;
  case 24:
    ((WFoeMotif *)foe)->widgets.compress = w;
    break;
  case 25:
    ((WFoeMotif *)foe)->widgets.search_entry = w;
    break;
  case 26:
    ((WFoeMotif *)foe)->widgets.customize_entry = w;
    break;
  case 28:
    ((WFoeMotif *)foe)->widgets.redraw = w;
    break;
  case 30:
    ((WFoeMotif *)foe)->widgets.view_togg = w;
    break;
  case 31:
    ((WFoeMotif *)foe)->widgets.edit_togg = w;
    break;
  case 32:
    ((WFoeMotif *)foe)->widgets.trace_togg = w;
    break;
  case 33:
    ((WFoeMotif *)foe)->widgets.simulate_togg = w;
    break;
  }
}

void WFoeMotif::create_adb( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  ((WFoeMotif *)foe)->widgets.adb= w ;
}

void WFoeMotif::create_edit_form( Widget w, WFoe *foe, XmAnyCallbackStruct data)
{
  ((WFoeMotif *)foe)->widgets.pane = w ;
  /* XtUninstallTranslations( w);*/
}

void WFoeMotif::create_subpane( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  ((WFoeMotif *)foe)->widgets.subpane = w ;
  XtUninstallTranslations( w);
}

void WFoeMotif::create_palette_object( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  XmToggleButtonSetState( w, 1, 0);
}


//
//	Callback from the popup menu when poping down.
//
void WFoeMotif::reset_current_node( WFoe *foe)
{

  if ( foe->popupmenu_node != 0) {
    foe->gre->node_unselect( foe->popupmenu_node);
  }
}

void WFoeMotif::popdown_pop( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  XtIntervalId		TimerID;

  if ( foe->popupmenu_node != 0) {
    /* Wait some time and take away select if nobody
       else has done that */
    TimerID = XtAppAddTimeOut( 
	        XtWidgetToApplicationContext(((WFoeMotif *)foe)->parent_wid),
		300, (XtTimerCallbackProc)reset_current_node, foe);
  }
  foe->normal_cursor();	
}

//
//	Callback from the menu.
//
void WFoeMotif::valchanged_textinput( Widget w, WFoeMotif *foe, XmAnyCallbackStruct *data)
{
  char	*value;
  int	len, sts;

  if ( foe->widgets.textinput == 0)
    return;

  value = XmTextGetString( foe->widgets.textinput);
  if ( *value == 0)
    return;

  len = strlen( value);
  if ( *(value + len - 1) == 10) {
    /* Return */
    *(value + len - 1) = 0;
    strcpy( foe->searchstring, value);

    foe->message( "");
    XmTextSetString( foe->widgets.textinput, "");
    XtUnmanageChild(foe->widgets.textinput);

    /* Call the specified function */
    sts = (foe->textinput_func) (foe, value);
    foe->error_msg( sts);
  }
  XtFree((char *) value);
}

//
//      callback from the caution widget 
//	call any function interessed by the yes aknowledge from the caution
//      box. The address of this function has been previoulsly carefully saved
//      if the context. ( foe->Yes_procedure )
// 	Author : SG
//	Date : 13.02.91

void WFoeMotif::yes_popupmsg( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  XtUnmanageChild( ((WFoeMotif *)foe)->widgets.caution);

  if ( foe->popupmsg_yes_p != NULL)
    (foe->popupmsg_yes_p) (foe ) ;
}

//
//      callback from the caution widget 
//	call any function interessed by the no aknowledge from the caution
//      box. The address of this function has been previoulsly carefully saved
//      if the context. ( foe->Yes_procedure )
// 	Author : SG
// 	Date : 13.02.91
//
void WFoeMotif::no_popupmsg( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  XtUnmanageChild( ((WFoeMotif *)foe)->widgets.caution);

  if ( foe->popupmsg_no_p != NULL)
    (foe->popupmsg_no_p)( foe);
}

//
//       Callback form the caution widget 
//
void WFoeMotif::cancel_popupmsg( Widget w, WFoe *foe, XmAnyCallbackStruct *data)
{
  XtUnmanageChild( ((WFoeMotif *)foe)->widgets.caution);

  if ( foe->popupmsg_cancel_p != NULL)
    (foe->popupmsg_cancel_p)( foe);
}

//
//	This routine registers the callbackfunktions for the 
//	objects in the foe menu.
//
int WFoeMotif::register_callbacks()
{
  static MrmRegisterArg	reglist[] = {
    /* First the context variable */
    { "foe_ctx", 0 },

    /* Callbacks for the controlled foe widget */
    {"foe_activate_save",(XtPointer)activate_save},
    {"foe_activate_exit",(XtPointer)activate_exit},
    {"foe_activate_print",(XtPointer)activate_print},
    {"foe_activate_printselect",(XtPointer)activate_printselect},
    {"foe_activate_printoverv",(XtPointer)activate_printoverv},
    {"foe_activate_compile",(XtPointer)activate_compile},
    {"foe_activate_syntax",(XtPointer)activate_syntax},
    {"foe_activate_winddelete",(XtPointer)activate_winddelete},
    {"foe_activate_savetrace",(XtPointer)activate_savetrace},
    {"foe_activate_restoretrace",(XtPointer)activate_restoretrace},
    {"foe_activate_quit",(XtPointer)activate_quit},
    {"foe_activate_delete",(XtPointer)activate_delete},
    {"foe_activate_changetext",(XtPointer)activate_changetext},
    {"foe_activate_cut",(XtPointer)activate_cut},
    {"foe_activate_copy",(XtPointer)activate_copy},
    {"foe_activate_paste",(XtPointer)activate_paste},
    {"foe_activate_attribute",(XtPointer)activate_attribute},
    {"foe_activate_subwindow",(XtPointer)activate_subwindow},
    {"foe_activate_undelete",(XtPointer)activate_undelete},
    {"foe_activate_unselect",(XtPointer)activate_unselect},
    {"foe_activate_gridsize1",(XtPointer)activate_gridsize1},
    {"foe_activate_gridsize2",(XtPointer)activate_gridsize2},
    {"foe_activate_gridsize3",(XtPointer)activate_gridsize3},
    {"foe_activate_gridsize4",(XtPointer)activate_gridsize4},
    {"foe_activate_gridsize5",(XtPointer)activate_gridsize5},
    {"foe_activate_showgrid",(XtPointer)activate_showgrid},
    {"foe_activate_zoomin",(XtPointer)activate_zoomin},
    {"foe_activate_zoomout",(XtPointer)activate_zoomout},
    {"foe_activate_unzoom",(XtPointer)activate_unzoom},
    {"foe_activate_redraw",(XtPointer)activate_redraw},
    {"foe_activate_palette_plant",(XtPointer)activate_palette_plant},
    {"foe_activate_palette_object",(XtPointer)activate_palette_object},
    {"foe_activate_palette_con",(XtPointer)activate_palette_con},
    {"foe_activate_refcon",(XtPointer)activate_refcon},
    {"foe_activate_showexeord",(XtPointer)activate_showexeord},
    {"foe_activate_plcattribute",(XtPointer)activate_plcattribute},
    {"foe_activate_search",(XtPointer)activate_search},
    {"foe_activate_searchobject",(XtPointer)activate_searchobject},
    {"foe_activate_searchstring",(XtPointer)activate_searchstring},
    {"foe_activate_searchnext",(XtPointer)activate_searchnext},
    {"foe_activate_getobj",(XtPointer)activate_getobj}, /* sg 11.04.91 */
    {"foe_activate_expand",(XtPointer)activate_expand},
    {"foe_activate_compress",(XtPointer)activate_compress},
    {"foe_activate_helpclass",(XtPointer)activate_helpclass},
    {"foe_activate_help",(XtPointer)activate_help},
    {"foe_activate_trace_togg",(XtPointer)activate_trace_togg},  
    {"foe_activate_simulate_togg",(XtPointer)activate_simulate_togg},  
    {"foe_activate_view_togg",(XtPointer)activate_view_togg},  
    {"foe_activate_edit_togg",(XtPointer)activate_edit_togg},  
    {"foe_create_adb",(XtPointer)create_adb},  /* SG 24.02.91 */
    {"foe_create_widget_cb",(XtPointer)create_widget_cb},  
    {"foe_create_label",(XtPointer)create_label},  
    {"foe_create_textinput",(XtPointer)create_textinput},  
    {"foe_create_edit_form",(XtPointer)create_edit_form},  
    {"foe_create_subpane",(XtPointer)create_subpane},  
    {"foe_create_palette_object",(XtPointer)create_palette_object},  
    {"foe_valchanged_textinput",(XtPointer)valchanged_textinput},  
    {"foe_yes_popupmsg",(XtPointer)yes_popupmsg},
    {"foe_no_popupmsg",(XtPointer)no_popupmsg},
    {"foe_cancel_popupmsg",(XtPointer)cancel_popupmsg},  
    {"foe_popdown_pop",(XtPointer)popdown_pop}
  };
  
  static int	reglist_num = XtNumber(reglist);

  reglist[0].value = (XtPointer) this;

  MrmRegisterNames(reglist, reglist_num);

  return FOE__SUCCESS;
}

// 	Backcall from questionbox

void WFoeMotif::popup_unmap_cb(Widget w, WFoe *foe)
{
  XtDestroyWidget(w);
}

//
//  Modifies the popupmenu.
//
int WFoeMotif::modify_popup( unsigned long popupmenu_mask, int x, int y)
{
  static char buttontext[][40] = { "ObjectEditor", "SubWindow", "Connect", "Delete",
				   "Paste", "Copy", "Cut", "PrintSelect", "HelpClass"};
  static XtCallbackProc callback[] = { (void (*)(Widget,void *,void*))activate_attribute,
				       (void (*)(Widget,void *,void*))activate_subwindow,
				       (void (*)(Widget,void *,void*))activate_getobj,
				       (void (*)(Widget,void *,void*))activate_delete,
				       (void (*)(Widget,void *,void*))activate_paste,
				       (void (*)(Widget,void *,void*))activate_copy,
				       (void (*)(Widget,void *,void*))activate_cut,
				       (void (*)(Widget,void *,void*))activate_printselect,
				       (void (*)(Widget,void *,void*))activate_helpclass};

  Widget Menu, w;
  XEvent event;
  int i;
  Arg ArgList[5]; 
  XmFontList fontlist;
  XFontStruct *font;
  XmFontListEntry fontentry;

  popupmenu_mask = popupmenu_mask;

  event.xbutton.x_root = x + 5;
  event.xbutton.y_root = y;
  event.xbutton.x = 0;
  event.xbutton.y = 0;

  // Set default fontlist
  font = XLoadQueryFont( flow_Display( widgets.pane),
			 "-*-Helvetica-Bold-R-Normal--12-*-*-*-P-*-ISO8859-1");
  fontentry = XmFontListEntryCreate( "tag1", XmFONT_IS_FONT, font);
  fontlist = XmFontListAppendEntry( NULL, fontentry);
  XtFree( (char *)fontentry);

  i = 0;
  XtSetArg(ArgList[i], XmNbuttonFontList, fontlist); i++;
  XtSetArg(ArgList[i], XmNlabelFontList, fontlist); i++;
  Menu = XmCreatePopupMenu( widgets.pane, "_popup", ArgList, i);  
  XtAddCallback( Menu, XmNunmapCallback, 
		 (XtCallbackProc)popup_unmap_cb, this);

  XmFontListFree( fontlist);

  for ( i = 0; i < 9; i++) {
    if ( popupmenu_mask & (1 << i)) {
      w = XtVaCreateManagedWidget( buttontext[i], xmPushButtonGadgetClass, 
			       Menu, NULL);

      XtAddCallback( w, XmNactivateCallback, (XtCallbackProc) callback[i], 
		    (XtPointer) this);
    }
  }
  widgets.popupmenu = Menu;
  XmMenuPosition( widgets.popupmenu, &event.xbutton);
  XtManageChild( widgets.popupmenu);
  return FOE__SUCCESS ;
}

void WFoeMotif::enable_set_focus( WFoeMotif *foe)
{
  foe->set_focus_disabled--;
}

void WFoeMotif::disable_set_focus( WFoeMotif *foe, int time)
{
  foe->set_focus_disabled++;
  foe->focus_timerid = XtAppAddTimeOut(
	XtWidgetToApplicationContext( foe->parent_wid), time,
	(XtTimerCallbackProc)enable_set_focus, foe);
}

void WFoeMotif::action_inputfocus( Widget w, XmAnyCallbackStruct *data)
{
  Arg args[1];
  WFoeMotif *foe;

  XtSetArg    (args[0], XmNuserData, &foe);
  XtGetValues (w, args, 1);

  if ( mrm_IsIconicState(w))
    return;

  if ( foe->set_focus_disabled)
    return;

  if ( foe->nav_palette_managed)
    foe->navctx->set_inputfocus( 1);
  else if ( foe->node_palette_managed)
    foe->node_palctx->set_inputfocus( 1);
  else if ( foe->con_palette_managed)
    foe->con_palctx->set_inputfocus( 1);

  foe->disable_set_focus( foe, 400);
}


//
//	Start the attribute editor and store the attribute context pointer 
//	together with WFoe and the nodepointer to use when a backcall
//	comes.
//
WAtt *WFoeMotif::watt_new( void *a_parent_ctx,
		      ldh_tSesContext a_ldhses,
		      pwr_sAttrRef a_aref,
		      int a_editmode,
		      int a_advanced_user,
		      int a_display_objectname)
{
  WAttMotif *watt = new WAttMotif( parent_wid, a_parent_ctx,
				   a_ldhses, a_aref, a_editmode, a_advanced_user,
				   a_display_objectname);
  return watt;
}

//
//	Destroys a foe instance.
//	Destroys the widget and frees allocated memory for the 
//	context.
//
void WFoeMotif::destroy()
{
  if ( set_focus_disabled)
    XtRemoveTimeOut( focus_timerid);

  gre->wind->hw.foe = 0;

  /* Delete controled modules */
  delete gre;
  delete node_palctx;
  delete con_palctx;
  delete navctx;

  /* Destroy the widget */
  XtDestroyWidget( parent_wid);

  /* Delete the context */
  delete this;
}

void WFoeMotif::set_title()
{
  Arg 		args[2];

  if ( function == EDIT) {
    pwr_tOName new_title;
    
    strcpy( new_title, "*** ");
    cdh_StrncpyCutOff( &new_title[4], name, sizeof(new_title)-4, 0);

    XtSetArg(args[0],XmNtitle, new_title);
    XtSetValues( parent_wid, args, 1);
  }
  else if ( function == VIEW) {
    pwr_tOName new_title;

    strcpy( new_title, name);

    XtSetArg(args[0],XmNtitle, new_title);
    XtSetValues( parent_wid, args, 1);
  }
}

WFoe *WFoeMotif::subwindow_new( void			*f_parent_ctx,
			   char			*f_name,
			   pwr_tObjid     	plcprogram,
			   ldh_tWBContext 	ldhwbctx,
			   ldh_tSesContext	ldhsesctx,
			   vldh_t_node    	nodeobject,
			   unsigned long	windowindex,
			   unsigned long	new_window,
			   int	       		f_map_window,
			   ldh_eAccess    	f_access,
			   foe_eFuncAccess	function_access,
			   pwr_tStatus 	*sts)
{
  return new WFoeMotif( f_parent_ctx, widgets.foe_window, f_name, plcprogram,
			ldhwbctx, ldhsesctx, nodeobject, windowindex,
			new_window, f_map_window, f_access, function_access,
			sts);
}


//
//	Displays a message in the foe window.
//	Changes the label of a label widget which id is stored in the context.
//	Author: SG
// 	Date : 25.01.91 
//    	To avoid unnecessary call to this routine the context variable :
//     	foe.msg_label_id is set to null if the the message to be displayed 
//     	is "" . This case occurs when you want to erase a previous message.
//
void WFoeMotif::message( char *new_label) 
{
  Arg		args[2];
  int 		l;
  XmString	cstr;

  gre->search_rectangle_delete();
  l= strlen (new_label);
  msg_label_id = ( l !=0 );
  if ( l == 0 ) new_label = " " ;
  XtSetArg(args[0], XmNlabelString, 
	   cstr=XmStringCreateLtoR(new_label , "ISO8859-1"));
  XtSetValues( widgets.label ,args,1);
  XmStringFree( cstr);
}

//
// This routine is called by any one who wants to send a little message.
// in a ( modal) message box. No callback is expected. We just want to
// be sure that the user see the message.
// Author: SG
// Date : 24.02.91
//
void WFoeMotif::msgbox( char *new_label) 
{
  Arg		args[10];
  int 		i;
  XmString	cstr;

  i=0;
  XtSetArg(args[0], XmNmessageString,
	   cstr=XmStringCreateLtoR(new_label , "ISO8859-1") ); i++;
  XtSetValues( widgets.msgbox ,args,i);
  XmStringFree( cstr);
  XtManageChild( widgets.msgbox);
}

//
// This routine is called by any one who wants to send a little caution message.
// The calling routine send also the address of the function it want to
// be executed when the yes button is pressed in the caution box.
// Author: SG
// Date : 13.02.91
//
void WFoeMotif::popupmsg( char *new_label, void (* yes_procedure)(WFoe *), 
			  void (* no_procedure)(WFoe *),
			  void (* cancel_procedure)(WFoe *)) 
{
  Arg		args[10];
  int 		i;
  XmString	cstr;

  cstr = XmStringCreateLtoR(new_label , "ISO8859-1");
  i=0;
  XtSetArg(args[0], XmNmessageString, cstr); i++;
  XtSetValues( widgets.caution ,args,i);
  XtManageChild( widgets.caution);
  XmStringFree( cstr);

  popupmsg_yes_p = yes_procedure ;        
  popupmsg_no_p = no_procedure ;        
  popupmsg_cancel_p = cancel_procedure ;        
}

//
// Constructor
//
WFoeMotif::WFoeMotif( void		*f_parent_ctx,
		      Widget		f_parent_wid,
		      char		*f_name,
		      pwr_tObjid       	plcprogram,
		      ldh_tWBContext 	ldhwbctx,
		      ldh_tSesContext	ldhsesctx,
		      vldh_t_node      	nodeobject,
		      unsigned long	windowindex,
		      unsigned long	new_window,
		      int	       	f_map_window,
		      ldh_eAccess      	f_access,
		      foe_eFuncAccess	function_access,
		      pwr_tStatus      	*sts) :
  WFoe(f_parent_ctx,f_name,plcprogram,ldhwbctx,ldhsesctx,nodeobject,
       windowindex,new_window,f_map_window,f_access,function_access,sts),
  parent_wid(f_parent_wid), set_focus_disabled(0), focus_timerid(0)
{

  memset( &widgets, 0, sizeof(widgets));

  *sts = new_local( plcprogram, ldhwbctx, ldhsesctx,
		    nodeobject, windowindex, new_window, function_access);

  wow = new CoWowMotif( parent_wid);
}


//
// Constructor
//
WFoeMotif::WFoeMotif( void *f_parent_ctx,
		      Widget f_parent_wid,
		      char *f_name,
		      pwr_tObjid plcprogram,
		      ldh_tWBContext ldhwbctx,
		      ldh_tSesContext ldhsesctx,
		      int f_map_window,
		      ldh_eAccess	f_access,
		      pwr_tStatus *sts) :
  WFoe(f_parent_ctx,f_name,plcprogram,ldhwbctx,ldhsesctx,f_map_window,
       f_access,sts),
  parent_wid(f_parent_wid), set_focus_disabled(0), focus_timerid(0)
{
  int		size;
  pwr_tFullName	new_name;
  ldh_tSesContext ldhses;

  memset( &widgets, 0, sizeof(widgets));

  /* Check that it is possible to open a session */
  *sts = ldh_OpenSession(&ldhses, ldh_SessionToVol( ldhsesctx),
			access, ldh_eUtility_PlcEditor);
  if ( EVEN(*sts)) return;

  *sts = ldh_CloseSession(ldhses);

  /* New title */
  *sts = ldh_ObjidToName( ldhsesctx, plcprogram, ldh_eName_Hierarchy,
			  new_name, sizeof( new_name), &size);
  error_msg(*sts);
  if( EVEN(*sts)) return;

  strcpy( name, new_name);
  *sts = new_local( plcprogram,
		    ldhwbctx, ldhsesctx, 0, 0, 0,
		    foe_eFuncAccess_Edit);

  wow = new CoWowMotif( parent_wid);
}

//
// Pop the window
//
void WFoeMotif::pop()
{
  XtUnmapWidget( parent_wid );
  XtMapWidget( parent_wid );
}

//
// Create a new foe window
// SG 09.02.91 : Create a new toplevel widget and give it an pixmap icon
// SG : 13.02.91 Fetch a caution box and save the id of the widget in the 
//               foe context for futher use
// SG : 24.02.91 Fetch a widget ( again !) . This time a message box
// SG : 24.03.91 Send the routine the desired position (x_top , y_top) 
// and the desired size ( width_adb , height_adb ) for the window to be created.
// The two first parameters are set to the top level widget
// and the two last paramters are set to the adb widget (see in uil file
// the widget hierarchy )
// SG 14.04.91 A new widget to fetch , the help widget ! 
// SG 2.05.91 New parameter : function ( edit , trace , view , simulate ..)
// to indicate which neted events and which menu entries should be alllowed.
//
pwr_tStatus WFoeMotif::create_window( int x_top,
				 int y_top,
				 int width_adb,
				 int height_adb, 
				 ldh_tSesContext ldhsession,
				 foe_eFuncAccess function_access)
{
  Arg	args[20];
  int	sts;
  Widget *children;
  int i, num_children;
  XmString   	TitleStr;
  Dimension icon_w, icon_h;

  /* DRM database hierarchy related variables */
  MrmHierarchy s_MrmH;
  MrmType dclass;
  char		uid_filename[200] = {"pwr_exe:wb_foe.uid"};
  char		*uid_filename_p = uid_filename;
  static Atom	MwmDeleteWind = 0;
  static char translations[] = "\
<FocusIn>: foe_inputfocus()\n";
  static XtTranslations compiled_translations = NULL;

  static XtActionsRec actions[] =
    {
      {"foe_inputfocus",      (XtActionProc) action_inputfocus}
    };
       
  sts = dcli_translate_filename( uid_filename, uid_filename);
  if ( EVEN(sts)) {
    printf( "** pwr_exe is not defined\n");
    exit(0);
  }

  /*
   * Now start the module creation
   */

  /* Save the context structure in the widget */
  XtSetArg (args[0], XmNuserData, (unsigned int) this);

  /*
   * Create a new main widget
   */
  /*
   * Open the UID files (the output of the UIL compiler) in the hierarchy
   * Register the items DRM needs to bind for us.
   * Create a new neted-widget
   * Close the hierarchy
   * Compile the additional button translations and augment and add actions
   */ 
  sts = MrmOpenHierarchy( 1, &uid_filename_p, NULL, &s_MrmH);
  if (sts != MrmSUCCESS) {
    printf("can't open hierarchy\n");
    return 0;
  }

  register_callbacks();

  /* SG 09.02.91 : create the new hierarchy. By the way use a pixmap define
  with help of wb_foe_ptrn.h */

  /* create a pixmap for the icon */
  if ( icon_pixmap == 0 )
    sts = MrmFetchBitmapLiteral(s_MrmH,"icon",
				XtScreen(parent_wid), XtDisplay(parent_wid),
				&icon_pixmap, &icon_w, &icon_h);  
 
  i=0;
  XtSetArg(args[i],XmNiconName,name);  i++;
  XtSetArg(args[i],XtNiconPixmap,icon_pixmap);  i++;
  
  /* SG 11.02.91 Save the id of the top in the context */ 
  parent_wid = XtCreatePopupShell("plcEditor", 
		     topLevelShellWidgetClass, parent_wid, args, i);

  if (compiled_translations == NULL) 
    XtAppAddActions( XtWidgetToApplicationContext( parent_wid), 
						actions, XtNumber(actions));

  /* the positioning of a top level can only be define after the creation
      of the widget . So i do it know: 
     SG 24.02.91 use the parameters received x and y 
  */

  i=0;
  XtSetArg(args[i],XmNx,x_top);i++;
  XtSetArg(args[i],XmNy,y_top);i++;
  XtSetArg(args[i],XtNallowShellResize,TRUE), i++;

  if ( ! map_window) {
    XtSetArg(args[i],XmNmappedWhenManaged,FALSE); i++;
  }

  XtSetValues( parent_wid, args, i);

  XtSetArg (args[0], XmNuserData, (unsigned int) this);

  /* now that we have a top level we can get the main window */
  sts = MrmFetchWidgetOverride(s_MrmH, "foe_window", parent_wid ,
		name, args, 1,
		&widgets.foe_window, &dclass);
  if (sts != MrmSUCCESS) printf("can't fetch foedit widget\n");

  /* SG 24.02.91 Set the width and hight desired for the main window to his
	child the adb widget */
  i=0;
  XtSetArg(args[i],XmNwidth,width_adb);i++;
  XtSetArg(args[i],XmNheight,height_adb);i++;
  XtSetValues( widgets.adb  ,args,i);

  /* SG 24.02.91
     Get a litle message box widget that you will make appears when you want by
     calling foe_msgbox
  */

  if (MrmFetchWidget (s_MrmH, 
			"foe_msgbox", 
			widgets.foe_window,
			&widgets.msgbox,
			&dclass) != MrmSUCCESS)
	printf("can't fetch msgbox \n");

  TitleStr = XmStringCreateLtoR("PlcEdit Message" , "ISO8859-1");    
  XtSetArg(args[0],XmNdialogTitle,TitleStr);
  XtSetValues( widgets.msgbox, args, 1);
  XmStringFree(TitleStr);


  /* SG 13.02.91 
     Get a litle caution widget that you will make appears when you want by
     calling foe_popupmsg 
  */

  if (MrmFetchWidget (s_MrmH, 
			"foe_popupmsg", 
			widgets.foe_window,
			&widgets.caution,
			&dclass) != MrmSUCCESS)
    printf("can't fetch popupmsg\n");

  TitleStr = XmStringCreateLtoR("PlcEdit Confirm" , "ISO8859-1");    
  XtSetArg(args[0],XmNdialogTitle,TitleStr);
  XtSetValues( widgets.caution, args, 1);
  XmStringFree(TitleStr);


  /* create the pane widgets */
  /* create the palette module */
  /* SG 07.03.91 put two palletes in the sub pane widget */

  node_palctx = new PalMotif( this, widgets.subpane, "Objects",
		ldhsession, "PlcEditorPalette", 
		&widgets.node_palette, &sts);
  node_palette_managed = 1;

  navctx = new NavMotif( this, widgets.subpane, "Plant",
		ldhsession, "NavigatorW1",
		&widgets.nav_palette, &sts);
  XtUnmanageChild( widgets.nav_palette);

  con_palctx = new PalMotif( this, widgets.subpane, "Connections",
		ldhsession, "PlcEditorConPalette", 
		&widgets.con_palette, &sts);
  XtUnmanageChild( widgets.con_palette);


  /* Turn off traversal for sash widgets */
  XtVaGetValues(widgets.subpane, 
		XmNchildren, &children,
		XmNnumChildren, &num_children,
		NULL);
  for (i = 0; i < num_children; i++) {
    if (XmIsSash(children[i]))
      XtVaSetValues(children[i], XmNtraversalOn, False, NULL);
  }

  /* create the gredit module */
  XtManageChild(widgets.foe_window);


  gre = new WGreMotif( this, widgets.pane, "Name");

  ((WGreMotif *)gre)->new_navigator( widgets.subpane);

  if ( map_window)
    XtPopup(parent_wid, XtGrabNone);
  else
    XtRealizeWidget( parent_wid);

  gre->init();

  /* create the setup ie:
     enable netedevents,
     enable menu entries
     setup backcalls from the gredit module */

  function_setup();

  MrmCloseHierarchy(s_MrmH);

  if (compiled_translations == NULL) 
    compiled_translations = XtParseTranslationTable(translations);
  XtOverrideTranslations( widgets.foe_window, compiled_translations);

  XtUnmanageChild(widgets.textinput);

  if ( !(CoLogin::privilege() & pwr_mPrv_DevPlc ) || 
       (function_access == foe_eFuncAccess_View)) {
    /* Mode Edit and Simulate is not allowed */
    i=0;
    XtSetArg(args[i],XmNsensitive,0); i++;

    XtSetValues( widgets.edit_togg,args,i);
    XtSetValues( widgets.simulate_togg,args,i);
  }

  if (MwmDeleteWind == 0)
    MwmDeleteWind = XInternAtom(XtDisplay(parent_wid), "WM_DELETE_WINDOW", FALSE);

  if (MwmDeleteWind != 0) {
    XtVaSetValues(parent_wid, XmNdeleteResponse, XmDO_NOTHING, NULL);
    XmAddWMProtocolCallback(parent_wid, MwmDeleteWind, 
			    (XtCallbackProc)activate_quit, this);
  }

  CoWowMotif::GetAtoms( parent_wid, 0, &objid_atom, 0);

  /* SG 09.02.91 a top level should always be realized !
   * ML 16.03.94 changed Realize to Popup
   */

  return FOE__SUCCESS;
}


void WFoeMotif::set_mode_button_state( int mode, int state)
{
  switch ( mode) {
  case VIEW:
    XmToggleButtonSetState( widgets.view_togg, state, 0);
    break;
  case EDIT:
    XmToggleButtonSetState( widgets.edit_togg, state, 0);
    break;
  case TRACE:
    XmToggleButtonSetState( widgets.trace_togg, state, 0);
    break;
  case SIMULATE:
    XmToggleButtonSetState( widgets.simulate_togg, state, 0);
    break;
  }
}

//
//	Show the text input widget.
//
int WFoeMotif::get_textinput( char *msg, int (*function)(WFoe *, char *))
{
  /* Print the text */
  message( msg);

  /* View the input widget */
  XtManageChild( widgets.textinput);
  XtSetKeyboardFocus( parent_wid, widgets.textinput);

  /* Insert the function in foe */
  textinput_func = function;
  
  return FOE__SUCCESS;
}

//
// Disable or enable the menu entries that are relevant for the trace function.
//
int WFoeMotif::edit_set_entries()
{
  Arg	sensitive[1];
  Arg	insensitive[1];
  
  set_title();

  XtSetArg(sensitive[0],XmNsensitive, 1);
  XtSetArg(insensitive[0],XmNsensitive, 0);

  if ( !classeditor) {
    XtSetValues( widgets.save,sensitive,1);
    XtSetValues( widgets.exit,sensitive,1);
    XtSetValues( widgets.syntax,sensitive,1);
    XtSetValues( widgets.compile,sensitive,1);
    XtSetValues( widgets.savetrace,sensitive,1);
    XtSetValues( widgets.restoretrace,sensitive,1);
    XtSetValues( widgets.redraw,sensitive,1);
    XtSetValues( widgets.plcattribute,sensitive,1);
    XtSetValues( widgets.winddelete,sensitive,1);
    XtSetValues( widgets.edit_entry,sensitive,1);
    XtSetValues( widgets.cut,sensitive,1);
    XtSetValues( widgets.copy,sensitive,1);
    XtSetValues( widgets.paste,sensitive,1);
    XtSetValues( widgets.undelete,sensitive,1);
    XtSetValues( widgets.unselect,sensitive,1);
    XtSetValues( widgets.connect,sensitive,1);
    XtSetValues( widgets.del,sensitive,1);
    XtSetValues( widgets.changetext,sensitive,1);
    XtSetValues( widgets.expand,sensitive,1);
    XtSetValues( widgets.compress,sensitive,1);
  }
  else {
    XtSetValues( widgets.save,sensitive,1);
    XtSetValues( widgets.exit,sensitive,1);
    XtSetValues( widgets.syntax,insensitive,1);
    XtSetValues( widgets.compile,insensitive,1);
    XtSetValues( widgets.savetrace,insensitive,1);
    XtSetValues( widgets.restoretrace,insensitive,1);
    XtSetValues( widgets.redraw,sensitive,1);
    XtSetValues( widgets.plcattribute,sensitive,1);
    XtSetValues( widgets.winddelete,sensitive,1);
    XtSetValues( widgets.edit_entry,sensitive,1);
    XtSetValues( widgets.cut,sensitive,1);
    XtSetValues( widgets.copy,sensitive,1);
    XtSetValues( widgets.paste,sensitive,1);
    XtSetValues( widgets.undelete,sensitive,1);
    XtSetValues( widgets.unselect,sensitive,1);
    XtSetValues( widgets.connect,sensitive,1);
    XtSetValues( widgets.del,sensitive,1);
    XtSetValues( widgets.changetext,sensitive,1);
    XtSetValues( widgets.expand,sensitive,1);
    XtSetValues( widgets.compress,sensitive,1);
  }

  return FOE__SUCCESS ;

  /*  XtSetValues( widgets.print,args,i); */

}

//
// Disable or enable the menu entries that are relevant for the trace function.
//
int WFoeMotif::view_set_entries()
{  
  Arg	args[20];

  set_title();

  XtSetArg(args[0],XmNsensitive,0 );

  XtSetValues( widgets.save,args,1);
  XtSetValues( widgets.exit,args,1);
  XtSetValues( widgets.syntax,args,1);
  XtSetValues( widgets.compile,args,1);
  XtSetValues( widgets.redraw,args,1);
  XtSetValues( widgets.plcattribute,args,1);
  XtSetValues( widgets.winddelete,args,1);
  XtSetValues( widgets.cut,args,1);
  XtSetValues( widgets.paste,args,1);
  XtSetValues( widgets.undelete,args,1);
  XtSetValues( widgets.unselect,args,1);
  XtSetValues( widgets.connect,args,1);
  XtSetValues( widgets.del,args,1);
  XtSetValues( widgets.changetext,args,1);
  XtSetValues( widgets.expand,args,1);
  XtSetValues( widgets.compress,args,1);

  return FOE__SUCCESS;
}

int WFoeMotif::get_selection( char *str, int len)
{
  int sts;

  sts = CoWowMotif::GetSelection( parent_wid, str, len, objid_atom);
  if ( EVEN(sts))
    sts = CoWowMotif::GetSelection( parent_wid, str, len, XA_STRING);

  return sts;
}

