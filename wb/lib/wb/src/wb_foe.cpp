/* 
 * Proview   $Id: wb_foe.cpp,v 1.14 2008-10-31 12:51:31 claes Exp $
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

//   wb_foe.cpp
//   This module creates function object editor and handles
//   the graphical editor.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "flow.h"
#include "flow_ctx.h"
#include "flow_browapi.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "co_msg.h"
#include "wb_ldh.h"
#include "wb_foe_msg.h"
#include "wb_vldh_msg.h"
#include "co_login_msg.h"
#include "wb_gcg.h"
#include "wb_goen.h"
#include "wb_gre.h"
#include "wb_vldh.h"
#include "wb_foe.h"
#include "wb_gsx.h"
#include "wb_utl_api.h"
#include "co_login.h"
#include "wb_tra.h"
#include "wb_watt.h"
#include "wb_gobj.h"
#include "wb_exo.h"
#include "wb_watt.h"
#include "wb_pal.h"
#include "co_wow.h"
#include "co_xhelp.h"
#include "wb_log.h"

#define	BEEP	    putchar( '\7' );

foe_sAttr	*WFoe::attr_pointer = NULL;
int		WFoe::attr_count = 0;



//	Callback from the menu.
//	The routine first checks that the parent of the window is
//	saved, and then saves the window.

void WFoe::activate_save()
{
  vldh_t_wind	wind;
  vldh_t_node	parent_node;
  int		sts;
  vldh_t_plc	plc;

  if ( msg_label_id != 0 ) message( ""); 

  /* Check that the parent node is saved */
	
  wind = gre->wind;
  plc = wind->hw.plc;
  parent_node = wind->hw.parent_node_pointer;
  if ( parent_node != 0 ) {
    /* Parent is node, not a plc */
    if ( parent_ctx != 0 ) {
      /* parent is alive */
      if ((parent_node->hn.status & VLDH_CREATE) != 0) {
	/* Parentnode is not saved, then it's not allowed to save
	   the window */
	msgbox( "UNABLE TO SAVE \nSave the parent window first.");
	return;
      }
    }
  }
  clock_cursor();
  sts = create_flow();
  sts = gre->undelete_reset();	  
  disable_ldh_cb();
  sts = vldh_wind_save( gre->wind);
  enable_ldh_cb();
  error_msg( sts);
	
  normal_cursor();

  if ( ODD(sts)) {
    unsigned int opt;
    if ( options & foe_mOption_EnableComment)
      opt = log_mOption_Comment;
    else
      opt = 0;

    wb_log::log( (wb_session *)gre->wind->hw.ldhses, wlog_eCategory_PlcSave, 
		 gre->wind->lw.oid, opt);
    message( "Window saved");
  }
  else if ( sts == VLDH__PLCNOTSAVED )
    msgbox( "UNABLE TO SAVE \nSave the plcprogram in the hierarchy editor first.");
  else if (EVEN(sts)) {
    char msg[256];

    msg_GetMsg( sts, msg, sizeof(msg));
    msgbox( msg);
  }
}

//	Callback from the menu.
//	The routine first checks that the parent node of the window
//	is saved, and then saves the window and quits.

void WFoe::activate_exit()
{
}

//	Callback from the menu.
//	It is not allowed to delete orderwindows due to problems with
//	the brotherhood in ldh.
//	Checks first that the window doesn't have any subwindows,
//	and then starts the popupmessagebox "Do you really want to ..."
//	which calls foe_delete if ok.

void WFoe::activate_winddelete()
{
  unsigned long		node_count;
  vldh_t_node		*nodelist;
  vldh_t_node		*node_ptr;
  int			i, subwind_count;
  char			msg[80];
  int			sts;
  vldh_t_wind		wind;
  pwr_tClassId		cid;

  if ( msg_label_id != 0 ) message( ""); 

  /*************************************************/
  /* BUGGFIX, quit on suborder windows not allowed */
  /*************************************************/
  wind = gre->wind;
  sts = ldh_GetObjectClass( wind->hw.ldhses,
			    wind->lw.poid,
			    &cid);
  error_msg( sts);
  if (( cid == pwr_cClass_order) &&
      ((wind->hw.status & VLDH_CREATE) != 0 )) {
    message( "Operation not allowed, save session first");
    return;
  }	  
  /**************************************************/

  /* Check that there is no subwindows in this window */
  sts = vldh_get_nodes( gre->wind, 
			&node_count, &nodelist);
  error_msg( sts);
  node_ptr = nodelist;
  subwind_count = 0;
  for ( i = 0; i < (int)node_count; i++) {
    if ( (*node_ptr)->ln.subwindow != 0 )	
      /* Subwindow */
      subwind_count += (((*node_ptr)->ln.subwindow & 1) != 0)
	+ (((*node_ptr)->ln.subwindow & 2) != 0);
    node_ptr++;
  }
  if ( subwind_count > 0 ) {
    /* Created subwindows found, inform and break */
    sprintf( msg, 
	     " %d subwindow found in this window\nDelete them first",
	     subwind_count);
    msgbox( msg); 
    return;
  }
  if ( node_count > 0) free((char *)nodelist);

  /* You should take care of inconscious users.
     Ask them to confirm the action */

  popupmsg( "Do you really want to delete this window ?",
	    deletewind_ok, NULL, NULL);
      
}

//
// Backcall function to popupmsg for activate_winddelete
//
void WFoe::deletewind_ok( WFoe *foe)
{
  foe->foe_delete();
}


//
//	Callback from the menu.
//	It is not allowed to quit order subwindows due to problems with
//	the brotherhood in ldh.
//	Checks that no node that is not saved contains a subwindow.
//	Then starts the popup messagebox "Do you really want to..."
// 	which calls foe_quit if ok.
// 	SG 27.05.91  add treatement for quit from trace .
//
void WFoe::activate_quit()
{
  unsigned long		wind_count;
  vldh_t_wind		*windlist;
  vldh_t_wind		*wind_ptr;
  int			i, w_create_count, create_count;
  char			msg[80];
  int			sts;
  vldh_t_wind		wind;
  int			vldh_mod;
  ldh_sSessInfo		info;


  if ( msg_label_id != 0 ) message( ""); 

  wind = gre->wind;

  /* Check that there is no subwindows created during this session
     that is not saved while the node also is not saved */
  sts = vldh_get_wind_subwindows( gre->wind, 
				  &wind_count, &windlist);
  error_msg( sts);
  wind_ptr = windlist;
  w_create_count = 0;
  create_count = 0;
  for ( i = 0; i < (int)wind_count; i++) {
    if ( (*wind_ptr)->hw.foe != 0) {
      /* Subwindow created, check if parentnode also is created and
	 not saved */
      create_count++;
    }
    wind_ptr++;
  }
  if ( wind_count > 0) free((char *) windlist);
  
  if ( create_count > 0 && access != ldh_eAccess_SharedReadWrite) {
    /* Created subwindows found, inform and break */
    sprintf( msg, 
	     "Exit subwindows first, %d subwindow found",
	     create_count);
    msgbox( msg); 
    return;
  }

  /* If trace or view, exit without confirm */
  if ( ( function == TRACE ) || ( function == VIEW ) 
       || ( function == SIMULATE )) {
    /* If session is not empty revert it first */
    trace_stop( this);
    quit();
    return;

  }

  /* Check if changes are made */
  sts = ldh_GetSessionInfo( wind->hw.ldhses, &info);
  error_msg( sts);
  sts = vldh_get_wind_modification( wind, &vldh_mod);

  if ( !info.Empty || vldh_mod) {
    popupmsg( "Do you want to save changes", 
		      exit_save, exit_nosave, NULL); 
  }
  else
    /* Dont bother to ask */
    quit();
}

//
//	Callback from the menu.
//	This function finds all document objects in the window
//	and calls gre->print_docobj for each of them.
//
void WFoe::activate_savetrace()
{
  int			sts;

  if ( msg_label_id != 0 ) message( ""); 

  sts = trace_save( gre);
}

void WFoe::activate_restoretrace()
{
  int			sts;

  if ( msg_label_id != 0 ) message( ""); 

  sts = trace_restore( gre);
}

//
//	Callback from the menu.
//	This function finds all document objects in the window
//	and calls gre->print_docobj for each of them.
//
void WFoe::activate_print()
{
  int			sts;

  if ( msg_label_id != 0 ) message( ""); 

  sts = print_document();
  if ( sts == FOE__NOOBJFOUND) {
    message( "No documents found");
  }
}

//
//	Callback from the menu.
//	This function finds all document objects in the window
//	prints an overview of them.
//
void WFoe::activate_printoverv()
{
  int		sts;
  vldh_t_wind	wind;

  wind = gre->wind;

  if ( msg_label_id != 0 ) message( ""); 

  sts = print_overview();
  if ( sts == FOE__NOOBJFOUND) {
    message( "Less than two documents, no overview printed");
  }
}

//
//	Callback from the menu.
//	This function finds all document objects in the window
//	prints an overview of them to pdf.
//
void WFoe::activate_printpdf()
{
  int		sts;
  vldh_t_wind	wind;

  wind = gre->wind;

  if ( msg_label_id != 0 ) message( ""); 

  sts = print_pdf_overview();
  if ( sts == FOE__NOOBJFOUND) {
    message( "No documents found");
  }
}

//
//	Callback from the menu.
//	This function print a portion of the screen defined by the last
//	selected region.
//	Changed 980908 to print the selected document objects.
//
void WFoe::activate_printselect()
{
  int		sts;

  sts = print_selected_document();
  if ( sts == FOE__NOOBJFOUND) {
    message( "No documents found");
  }
}

//
//	Callback from the menu.
//	Calls foe_gcg to control syntax for the window.
//	Displays a message with number of errors or warnings.
//
void WFoe::activate_syntax()
{
  int		sts;
  unsigned long	errcount = 0;
  unsigned long	warncount = 0;
  char		msg[80];

  sts = gcg_plcwindow_compile( gre->wind, 0, 
			       &errcount, &warncount, 0, 0);
  if ( sts == GSX__AMBIGOUS_EXECUTEORDER ) {
    message( "Execute order error");
    BEEP;
    return;
  }
  else if ( sts == GSX__PLCWIND_ERRORS) {
    /* Errors found, inform the user*/
    sprintf( msg, 
	     "Syntax %ld errors  %ld warnings",
	     errcount, warncount);
    message( msg);
    BEEP;
  }
  else if ( EVEN(sts)) {
    message( "Error in class definition, syntax aborted");
    BEEP;
    error_msg( sts);
    return;
  }
  else	{
    if ( warncount > 0 ) {
      /* Errors found, inform the user*/
      sprintf( msg, 
	       "Syntax %ld warnings", warncount);
      message( msg);
      BEEP;
    }
    else
      message( "Syntax successfull completion");
  }

  if ( show_execorder)
    show_executeorder();
}

//
//	Callback from the menu.
//	Calls foe_gcg to make syntax control and generate plc code
//	for the window. Displays a message of the result of the 
//	syntax control.
//
void WFoe::activate_compile()
{
  int		sts;
  unsigned long	errcount = 0;
  unsigned long	warncount = 0;
  unsigned long	plc_errcount = 0;
  unsigned long	plc_warncount = 0;
  char		msg[80];
  vldh_t_wind	wind;
  int		vldh_mod;

  if ( msg_label_id != 0 ) message( ""); 

  wind = gre->wind;

  sts = vldh_get_wind_modification( wind, &vldh_mod);

  if ( vldh_mod) {
    message( "Session is not saved");
    BEEP;
    return;
  }

  disable_ldh_cb();
  sts = gcg_plcwindow_compile( wind, 1, &errcount, &warncount, 0, 0);
  enable_ldh_cb();
  if ( sts == GSX__AMBIGOUS_EXECUTEORDER) {
    message( "Execute order error");
    BEEP;
    return;
  }
  else if ( sts == GSX__NOTSAVED) {
    /* Session is not saved */
    message( "Session not saved, no objectfile created");
    BEEP;
    return;
  }
  else if ( sts == GSX__PLCWIND_ERRORS) {
    /* Errors found, inform the user*/
    sprintf( msg, 
	     "Compile %ld errors  %ld warnings, no objectfile created",
	     errcount, warncount);
    message( msg);
    BEEP;
    return;
  }
  else if ( EVEN(sts)) {
    message( "Ldh error");
    BEEP;
    error_msg( sts);
    return;
  }
	
  sts = gcg_plc_compile( wind->hw.plc, 1, 
			 &plc_errcount, &plc_warncount, 0, 0);
  warncount += plc_warncount;
  errcount += plc_errcount;
  if ( sts == GSX__PLCPGM_ERRORS) {
    /* Errors found, inform the user*/
    sprintf( msg, 
	     "Compile %ld errors  %ld warnings, no objectfile created",
	     errcount, warncount);
    message( msg);
    BEEP;
    return;
  }
  else if ( EVEN(sts)) {
    message( "Ldh error");
    BEEP;
    error_msg( sts);
    return;
  }
  else if ( warncount > 0 ) {
    /* Errors found, inform the user*/
    sprintf( msg, 
	     "Compile %ld warnings", warncount);
    message( msg);
  }

  if ( show_execorder)
    show_executeorder();
}

void WFoe::pal_select_cb( void *ctx, pwr_tCid cid)
{
  WFoe *foe = (WFoe *) ctx;
  vldh_t_node node;
  pwr_tStatus sts;

  if ( foe->function != EDIT)
    return;

  foe->gre->pending_paste_stop();

  foe->disable_ldh_cb();
  sts = foe->gre->create_node_floating( cid, 0, 0, &node);
  foe->enable_ldh_cb();
  foe->error_msg( sts);
}


// 
// Delete from menu
//
void WFoe::delete_ok_cb( void *ctx, void *data)
{
  WFoe *foe = (WFoe *) ctx;

  foe->activate_delete();
}

void WFoe::activate_delete_confirm()
{
  unsigned long		node_count;
  vldh_t_node		*nodelist;
  unsigned long		con_count = 0;
  vldh_t_con		*conlist;
  char			msg[200];

  gre->get_selnodes( &node_count, &nodelist);
  if ( !node_count) {
    gre->get_selcons( &con_count, &conlist);
    if ( !con_count) {
      message( "Nothing to delete");
      return;    
    }
    if ( con_count > 0) free((char *) conlist);
  }
  if ( node_count > 0) free((char *) nodelist);
  
  if ( node_count + con_count > 1)
    strcpy( msg, "Do you want to delete the selected objects");
  else
    strcpy( msg, "Do you want to delete the selected object");

  wow->DisplayQuestion( this, "Delete", msg, delete_ok_cb, 0, 0);
}

//
//	Callback from the popup menu.
//	Deletes the selected nodes and connections.
//	Checks first that a selected node does not contain a subwindow.
//
void WFoe::delete_subwindow_ok_cb( void *ctx, void *data)
{
  WFoe *foe = (WFoe *) ctx;
  vldh_t_node node = (vldh_t_node) data;
  pwr_tStatus sts, rsts;
  pwr_tOid child, nextchild;
  vldh_t_wind wind = foe->gre->wind;
  unsigned long		wind_count;
  vldh_t_wind		*windlist;
  vldh_t_wind		*wind_ptr;
  int			i;

  /* Check that subwindow is not open */
  sts = vldh_get_wind_subwindows( foe->gre->wind, 
				  &wind_count, &windlist);
  if( EVEN(sts)) { foe->error_msg( sts); return;}

  for ( sts = ldh_GetChild( wind->hw.ldhses, node->ln.oid, &child);
	ODD(sts);
	sts = ldh_GetNextSibling( wind->hw.ldhses, child, &child)) {
    wind_ptr = windlist;
    for ( i = 0; i < (int)wind_count; i++) {
      if ( cdh_ObjidIsEqual( (*wind_ptr)->lw.oid, child)) {
	foe->wow->DisplayError( "Window open",
			   "Subwindow is open\nClose subwindow first");
	free((char *) windlist);
	return;
      }
      wind_ptr++;
    }
  }
  if ( wind_count > 0) free((char *) windlist);

  for ( sts = ldh_GetChild( wind->hw.ldhses, node->ln.oid, &child);
	ODD(sts);
	child = nextchild) {
    sts = ldh_GetNextSibling( wind->hw.ldhses, child, &nextchild);

    rsts = ldh_DeleteObjectTree( wind->hw.ldhses, child, 0);
    if ( EVEN(rsts)) { foe->error_msg( rsts); return; }
  }
  node->ln.subwindow = 0;
  foe->gre->delete_selected();
}

void WFoe::activate_delete()
{
  unsigned long		node_count;
  vldh_t_node		*nodelist;
  vldh_t_node		*node_ptr;
  int			i, subwind_found;

  if ( msg_label_id != 0 ) message( ""); 
  
  /* Delete the selected nodes and connections */
  /* Check that any node doesn't contain a subwindow */
  gre->get_selnodes( &node_count, &nodelist);

  if ( node_count == 1 && (*nodelist)->ln.subwindow != 0) {
    /* Single node that has subwindow */
    char msg[200];
    sprintf( msg, 
	     "Object \"%s\" has subwindow\n Do you want to delete the subwindow?",
	     (*nodelist)->hn.name);
    wow->DisplayQuestion( this,
			 "Delete subwindow", msg,
			 delete_subwindow_ok_cb, 0, *nodelist);
    popupmenu_node = 0;
    return;
  }

  node_ptr = nodelist;
  subwind_found = FALSE;
  for( i = 0; i < (int)node_count; i++) {
    if ( (*node_ptr)->ln.subwindow != 0 ) {
      subwind_found = TRUE;
      break;
    }
    node_ptr++;
  }
  if ( node_count > 0) free((char *) nodelist);
  if ( subwind_found ) {
    wow->DisplayError( "Subwindow found",
		       "Subwindow found\nDelete objects with subwindows separately");
    BEEP;
  }
  else {
    /* Delete selected nodes */
    gre->delete_selected();
    popupmenu_node = 0;
  }
}

void WFoe::activate_createobject( float x, float y)
{
  gre_node_created( gre, 0, x, y);
}

//
//	Callback from the menu.
//	Deletes the selected nodes and connections.
//	Checks first that a selected node does not contain a subwindow.
//
void WFoe::activate_changetext()
{
  unsigned long		node_count;
  vldh_t_node		*nodelist;
  WAtt                  *watt;
  int                   sts;

  if ( msg_label_id != 0 ) message( ""); 
  
  /* Delete the selected nodes and connections */
  /* Check that any node doesn't contain a subwindow */
  gre->get_selnodes( &node_count, &nodelist);
  if ( node_count != 1 ) {
    message( "Select one text object");
    BEEP;
    return;
  }

  switch ( nodelist[0]->ln.cid) {
  case pwr_cClass_Text:
  case pwr_cClass_BodyText:
  case pwr_cClass_Head:
  case pwr_cClass_Title:
    attr_create( nodelist[0]);
    sts = attrlist_get_by_node( nodelist[0], (void **)&watt);
    if ( EVEN(sts)) return;
    sts = watt->open_changevalue( "Text");
    break;
  default:
    message( "Select a text object");
    BEEP;
  }
}

//
//	Callback from the menu.
//	Deletes the selected nodes and connections and places them
//	in the vldh paste list.
//	Checks first that a selected node does not contain a subwindow.
//
void WFoe::activate_cut()
{
  int sts;
  unsigned long		node_count;
  vldh_t_node		*nodelist;
  vldh_t_node		*node_ptr;
  int			i, subwind_found;
  

  if ( msg_label_id != 0 ) message( ""); 

  /* Cut */
  /* Check that any node doesn't contain a subwindow */
  gre->get_selnodes( &node_count, &nodelist);
  node_ptr = nodelist;
  subwind_found = FALSE;
  for( i = 0; i < (int)node_count; i++) {
    if ( (*node_ptr)->ln.subwindow != 0 ) {
      subwind_found = TRUE;
      break;
    }
    node_ptr++;
  }
  if ( node_count > 0) free((char *) nodelist);
  if ( subwind_found ) {
    message( "Subwindow found, use copy or delete the subwindow first");
    BEEP;
  }
  else {
    /* Cut selected nodes */
    sts = gre->cut();
    if (sts == GRE__NOREGSEL) {
      message( "Select region before cutting");
      BEEP;
    }
    else if ( EVEN(sts))
      error_msg( sts);
    popupmenu_node = 0;
  }
}

//
//	Callback from the menu.
//	Copies selected nodes to vldh paste list.
//
void WFoe::activate_copy()
{
  int	sts;


  if ( msg_label_id != 0 ) message( ""); 
  
  /* Copy */
  sts = gre->copy();
  if (sts == GRE__NOREGSEL) {
    message( "Select region before copying");
    BEEP;
  }
  else if ( EVEN(sts))
    error_msg( sts);
  else
    message( "Region copied");
}

//
//	Callback from the menu.
//	Calls the gre paste funktion.
//
void WFoe::activate_paste()
{

  if ( msg_label_id != 0 ) message( ""); 

  disable_ldh_cb();
  gre->paste( 0.0, 0.0, GRE_PASTE_CENTER);
  enable_ldh_cb();
}

void WFoe::activate_select_nextobject( flow_eDirection dir)
{
  gre->select_nextobject( dir, 0);
}

void WFoe::activate_select_addnextobject( flow_eDirection dir)
{
  gre->select_nextobject( dir, 1);
}

void WFoe::activate_scroll( flow_eDirection dir)
{
  gre->scroll( dir);
}

void WFoe::activate_select_nextconpoint( flow_eDirection dir)
{
  gre->select_next_conpoint( dir);
}

void WFoe::activate_conpoint_lock()
{
  gre->conpoint_lock(1);
}

void WFoe::activate_move_object( flow_eDirection dir)
{
  gre->move_object( dir);
}

//
//	Callback from the menu.
//	Creates a attribute editor for the selected object
//	unselects it.
//	SG 25.09.91 new message to the user ( see call to WFoe::message )
//
void WFoe::activate_attribute()
{
  unsigned long		node_count;
  vldh_t_node		*nodelist;
  vldh_t_node		object;
  int                   sts;
  WAtt                  *watt;

  if ( msg_label_id != 0 ) message( ""); 

  /* Get selected nodes */
  
  gre->get_selnodes( &node_count, &nodelist);

  if ( node_count == 1 ) {
    /* Create attribute editor */
    if ( msg_label_id != 0 ) message( ""); 
    object = *nodelist;
    sts = attrlist_get_by_node( object, (void **)&watt);
    if ( ODD(sts))
      watt->pop();
    else {
      clock_cursor();
      attr_create( object);
      normal_cursor();
    }
    // gre->unselect();
  }
  
  if ( node_count > 0) free((char *) nodelist);

  if ( node_count == 0) {
    message( "Select an object to edit attributes");
    BEEP;
  }
  else if ( node_count > 1 ) {
    message( "Unable to open object, more than one object selected");
    BEEP;
  }
  return;
}


//
//	Callback from the menu.
//	Creates a subwindow with window index 1 to the selected node by
// 	the function WFoe::subwindow_create.
//	 SG 25.09.91 new message to the user ( see call to WFoe::message )
//
void WFoe::activate_subwindow()
{
  unsigned long		node_count;
  vldh_t_node		*nodelist;
  vldh_t_node		object;
  int			sts;


  if ( msg_label_id != 0 ) message( ""); 

  /* Get selected nodes */
  gre->get_selnodes( &node_count, &nodelist);

  if ( node_count == 1) {
    if ( msg_label_id != 0 ) message( ""); 
    /* Create subwindow, take the first windowindex */
    object = *nodelist;
    clock_cursor();
    sts = subwindow_create( object, 1);
    error_msg( sts);
    normal_cursor();
    gre->unselect();
  }

  if ( node_count > 0) free((char *) nodelist);

  if ( node_count == 0) {
    message( "Select an object to create a subwindow");
    BEEP;
  }
  else if ( node_count > 1 ) {
    message( "Can't create a subwindow, more than one object selected");
    BEEP;
  }
}

//
//	Callback from the menu.
//	Calls the gre undelete function which restores all
//	objects deleted at the last delete or cut operation.
//
void WFoe::activate_undelete()
{
  if ( msg_label_id != 0 ) message( ""); 

  gre->undelete();
}

//
//	Callback from the menu.
//	Unselects all selected nodes.
//
void WFoe::activate_unselect()
{
  if ( msg_label_id != 0 ) message( ""); 

  gre->unselect();
}


//
//	Callback from the menu.
//	Create a node.
//
void WFoe::activate_createnode()
{
  pwr_tClassId		cid;
  int			sts;
  vldh_t_node		node;

  if ( msg_label_id != 0 ) message( ""); 

  gre->unselect();

  /* Get selected nodes from palette */
  sts =  node_palctx->get_select( &cid);

  if ( EVEN(sts)) {
    message( "Choose an object from the palette");
    BEEP;
    return;
  }

  gre->pending_paste_stop();

  disable_ldh_cb();
  sts = gre->create_node_floating( cid, 0, 0, &node);
  enable_ldh_cb();
  error_msg( sts);
  if ( EVEN(sts)) return;
}


//
//	Callback from the menu.
//	Create a connection.
//
void WFoe::activate_createconnection()
{
  unsigned long		node_count;
  vldh_t_node		*nodelist;
  int			*numlist;
  int			sts;

  if ( msg_label_id != 0 ) message( ""); 

  gre->get_conpoint_select( &node_count, &nodelist, &numlist);
  if ( !node_count) {
    message( "No connectionpoint is selected");
    return;
  }
  if ( node_count == 1) {
    double x, y;
    flow_eDirection dir;

    gre->get_conpoint( nodelist[0], numlist[0], &x, &y, &dir);
    switch ( dir) {
    case flow_eDirection_Up:
      y += 0.15;
      break;
    case flow_eDirection_Down:
      y -= 0.15;
      break;
    case flow_eDirection_Right:
      x += 0.1;
      break;
    case flow_eDirection_Left:
      x -= 0.3;
      break;
    default:
      x -= 0.3;
    }

    gre_con_created( gre, x, y, nodelist[0], numlist[0], 0, 0, 1, &sts);
  }
  else {
    gre_con_created( gre, 0, 0, nodelist[0], numlist[0], nodelist[1], numlist[1], 0, &sts);
    gre->unselect();
  }
  if ( ODD(sts)) {
    gre->conpoint_unselect();
    gre->conpoint_lock(0);
  }
  free( nodelist);
  free( numlist);

}

//
//	Callback from the menu.
//
void WFoe::activate_redraw()
{
  int			sts;

  sts = gre->redraw();
  error_msg( sts);
}

//
//	Callback from the menu.
//	Zoom in.
//
void WFoe::activate_zoomin()
{
  if ( msg_label_id != 0 ) message( ""); 
  gre->zoom( 1.43);
}

//
//	Callback from the menu.
//	Zoom out
//
void WFoe::activate_zoomout()
{
  if ( msg_label_id != 0 ) message( ""); 
  gre->zoom( 0.7);
}

//
//	Callback from the menu.
//	Calls the gre unzoom funktion.
//
void WFoe::activate_unzoom()
{
  if ( msg_label_id != 0 ) message( ""); 
  gre->unzoom();
}

//
// 	Callback from the menu.
//	Sets foe->con_drawtype to GOEN_CONUSERREF or GOEN_CONDRAW.
//	If GOEN_CONUSERREF created connections will be referencetype.
// 	SG 24.03.91
//
void WFoe::activate_refcon( int set)
{
  if ( msg_label_id != 0 ) message( ""); 

  con_drawtype = set ? GOEN_CONUSERREF : GOEN_CONDRAW ;
}

//
// 	Callback from the menu.
//	Show execute order.
//
void WFoe::activate_showexeord( int set)
{
  int		sts;

  if ( msg_label_id != 0 ) message( ""); 

  show_execorder = set;

  if ( show_execorder) {
    sts = exo_wind_exec( gre->wind);
    if ( sts == GSX__AMBIGOUS_EXECUTEORDER ) {
      message( "Execute order error");
      BEEP;
    }
    else if ( EVEN(sts))
      error_msg( sts);
    else
      show_executeorder();
  }
  else
    gre->undisplay();
}


//
//	Callback from the menu.
// 	Calls the attribute editor for the plcprogram.
//
void WFoe::activate_plcattribute()
{  
  if ( msg_label_id != 0 ) message( ""); 

  attr_create( 0);
}

//
//	Callback from the menu.
//
void WFoe::activate_searchobject()
{
  get_textinput( "Enter object : ", &search_object);
}

//
//	Callback from the menu.
//
void WFoe::activate_searchstring()
{
  get_textinput( "Enter string: ", &search_string);
}

//
//	Callback from the menu.
//
void WFoe::activate_searchnext()
{
  int sts;

  sts = search_string_next();
  error_msg( sts);
}

//
//	Callback from the edit entry of the foe_menu widget
// 	The user hopes to get the id of an object from the hardware hierarchy
//	to connect it to the getsomething object in his graphcet  diagram.
//	The getsomthing object should should be selected first.
// 	SG 11.04.91 
//
void WFoe::activate_getobj()
{
  unsigned long		node_count;
  vldh_t_node		*nodelist;
  int			sts;


  if ( msg_label_id != 0 ) message( ""); 

  /* Get selected nodes */
  gre->get_selnodes( &node_count, &nodelist);

  if ( node_count == 1)  {
    if ( msg_label_id != 0 ) message( ""); 

    /* get the id of the desired object */
    sts = gobj_get_object( this, *nodelist, 1) ;
    error_msg( sts);
    // gre->unselect();
  }

  if ( node_count > 0) free((char *) nodelist);

  if ( node_count == 0)  {
    message( "Select an object first ");
    BEEP;
  }
  else if ( node_count > 1 ) {
    message( "Unable to connect, more than one object selected");
    BEEP;
  }
}

//
//	Callback from the edit entry of the foe_menu widget
//
void WFoe::activate_expand()
{
  unsigned long		node_count;
  vldh_t_node		*nodelist;
  int			sts;


  if ( msg_label_id != 0 ) message( ""); 

  /* Get selected nodes */
  gre->get_selnodes( &node_count, &nodelist);

  if ( node_count == 1) {
    if ( msg_label_id != 0 ) message( ""); 

    /* get the id of the desired object */
    sts = gobj_expand( this, *nodelist, 0);
    error_msg( sts);
  }

  if ( node_count > 0) free((char *) nodelist);

  if ( node_count == 0) {
    message( "Select an object first ");
    BEEP;
  }
  else if ( node_count > 1 )  {
    message( "Unable to expand, more than one object selected");
    BEEP;
  }
}

//
//	Callback from the edit entry of the foe_menu widget
//
void WFoe::activate_compress()
{
  unsigned long		node_count;
  vldh_t_node		*nodelist;
  int			sts;


  if ( msg_label_id != 0 ) message( ""); 

  /* Get selected nodes */
  gre->get_selnodes( &node_count, &nodelist);

  if ( node_count == 1)  {
    if ( msg_label_id != 0 ) message( ""); 

    /* get the id of the desired object */
    sts = gobj_expand( this, *nodelist, 1);
    error_msg( sts);
  }

  if ( node_count > 0) free((char *) nodelist);

  if ( node_count == 0) {
    message( "Select an object first ");
    BEEP;
  }
  else if ( node_count > 1 ) {
    message( "Unable to expand, more than one object selected");
    BEEP;
  }
}

//
//	Display help window for topic plc editor
//
void WFoe::activate_help()
{
  CoXHelp::dhelp( "plc editor", 0, navh_eHelpFile_Base, 0, 1);
}

//
//	Display class help for selected object
//
void WFoe::activate_helpclass()
{
  unsigned long		node_count;
  vldh_t_node		*nodelist;
  vldh_t_wind		wind;
  char			cname[32];
  int			size;
  int			sts;

  wind = gre->wind;

  gre->get_selnodes( &node_count, &nodelist);
  if ( node_count != 1 ) {
    message( "Select one object");
    BEEP;
  }

  sts = ldh_ClassIdToName( wind->hw.ldhses, nodelist[0]->ln.cid, cname,
			   sizeof(cname), &size);
  if ( EVEN(sts)) return;

  CoXHelp::dhelp( cname, 0, navh_eHelpFile_Base, 0, 1);
}

//
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
void WFoe::activate_trace_togg( int set)
{
  int		sts;

  if ( msg_label_id != 0 ) message( ""); 

  if ( set == 1 ) {
    sts = change_mode( TRACE);
  }
}

//
//	Callbacks from the toggle buttons under the customize entry.
//
void WFoe::activate_simulate_togg( int set)
{
  int		sts;

  if ( msg_label_id != 0 ) message( ""); 

  if ( set == 1 ) 
    sts = change_mode( SIMULATE);
  else
    sts = change_mode( TRACE);
}

void WFoe::activate_view_togg( int set)
{
  int		sts;

  if ( msg_label_id != 0 ) message( ""); 

  if ( set == 1 ) {
    sts = change_mode( VIEW);
  }
}

void WFoe::activate_edit_togg( int set)
{
  int		sts;

  if ( msg_label_id != 0 ) message( ""); 

  if ( set == 1 )
    sts = change_mode( EDIT);
  else 
    sts = change_mode( VIEW);	
}

// 	Backcall from questionbox

//
//	Backcall when save is pressed in the questionbox at exit.
//
void WFoe::exit_save( WFoe *foe)
{
  vldh_t_wind	wind;
  vldh_t_node	parent_node;
  int	sts;

  /* Check that the parent node is saved */
  wind = foe->gre->wind;
  parent_node = wind->hw.parent_node_pointer;
  if ( parent_node != 0 ) {
    /* Parent is node, not a plc */
    if ( foe->parent_ctx != 0 ) {
      /* parent is alive */
      if ((parent_node->hn.status & VLDH_CREATE) != 0) {
	/* Parentnode is not saved, then its not allowed to save
	   the window */
	foe->msgbox( "Save the parent window first") ;
	return;
      }
    }
  }
  foe->clock_cursor();
  sts = foe->create_flow();
  foe->disable_ldh_cb();
  sts = vldh_wind_save( foe->gre->wind);
  foe->error_msg( sts);
	
  foe->normal_cursor();
  if ( sts == VLDH__PLCNOTSAVED ) {
    foe->msgbox( "Save the plcprogram in the hierarchy editor first");
    return;
  }
  else if (EVEN(sts)) {
    char msg[256];

    msg_GetMsg( sts, msg, sizeof(msg));
    foe->msgbox( msg);
    return;
  }
  else {
    unsigned int opt;
    if ( foe->options & foe_mOption_EnableComment)
      opt = log_mOption_Comment;
    else
      opt = 0;
    wb_log::log( (wb_session *)foe->gre->wind->hw.ldhses, wlog_eCategory_PlcSave, 
		 foe->gre->wind->lw.oid, opt);
  }

  foe->foe_exit();
}

//
//	Backcall when save is pressed in the questionbox at exit.
//
void WFoe::exit_nosave( WFoe *foe)
{
  vldh_t_wind		wind;
  pwr_tClassId		cid;

  wind = foe->gre->wind;

  /*************************************************/
  /* BUGGFIX, quit on suborder windows not allowed */
  /*************************************************/
  ldh_GetObjectClass( wind->hw.ldhses,
		      wind->lw.poid,
		      &cid);
  if ( cid == pwr_cClass_order) {
    foe->message( 
		 "Operationen not allowed, do save !!");
    return;
  }	  
  /**************************************************/
  foe->quit();
}

//
//   Backcalls from the controlled gre module.
//


//
// Description: Backcall from the controlled gre module
//
void WFoe::gre_setup_window( WGre *gre)
{
	/* not in use at this time */
}

//
// Description: Backcall from the controlled gre module
// when a node will be created.
// Fetches the current class from the palette and 
// creates a node of this class.
//
void WFoe::gre_node_created( WGre *gre, unsigned long current_node_type,
			     float x, float y)
{
  pwr_tClassId		cid;
  WFoe	  	     	*foe;
  int			sts;
  vldh_t_node		node;
  WAtt	                *watt;

  foe = (WFoe *)gre->parent_ctx;

  /* Get selected nodes from palette */
  sts =  foe->node_palctx->get_select( &cid);

  if ( EVEN(sts)) {
    /* SG 20.03.91 Inform the user that he has to choose a node*/
    foe->message( "Choose an object from the palette");
    BEEP;
    return;
  }

  foe->disable_ldh_cb();
  sts = gre->create_node( cid, x, y, &node);
  foe->enable_ldh_cb();
  foe->error_msg( sts);
  if ( EVEN(sts)) return;
        
  switch ( cid) {
  case pwr_cClass_Text:
  case pwr_cClass_BodyText:
  case pwr_cClass_Head:
  case pwr_cClass_Title:
    ((WFoe *)gre->parent_ctx)->attr_create(node);
    sts = attrlist_get_by_node( node, (void **)&watt);
    if ( EVEN(sts)) return;
    sts = watt->open_changevalue( "Text");
    break;
  default:
    ;
  }
}

//
// Description: Backcall from the controlled gre module when 
// a node is moved. Does nothing.
//
void WFoe::gre_node_moved( WGre *gre)
{
}

//
// Description: Backcall from the controlled gre module
// when a node is selected. Does nothing.
//
void WFoe::gre_node_selected( WGre *gre)
{
}

//
// Description: Backcall from the controlled gre module
// when a connection is selected. Does nothing.
//
void WFoe::gre_con_selected( WGre *gre)
{
}

//
// Description: Backcall from the controlled gre module when
//	a connection should be created.
//	Checks syntax and gets a suitable connectionclass by foe_gsx.
//	If foe->con_palette_managed is set the class is fetched 
//	from the connection palette. Creates a connection.
//
void WFoe::gre_con_created( WGre *gre, double x, double y,
			    vldh_t_node source_obj, unsigned long source_point,
			    vldh_t_node destination_obj, unsigned long destination_point, int select, int *sts)
{
  pwr_tClassId	con_class;
  pwr_tClassId 	user_class;
  WFoe 		*foe;
  vldh_t_node	dest;
  unsigned long	destpoint;

  foe = (WFoe *)gre->parent_ctx;

  if ( gre->trace_started) {
    if ( !destination_obj) {
      *sts = trace_create_analyse( gre, x, y, source_obj, source_point);
      if ( EVEN(*sts)) {
	foe->message( "Unable to find trace attribute");
	BEEP;
	return;
      }
    }
    return;
  }

  /* 
     SG 10.03.91 : If the user as asked to choose the connection from 
     the palette ( by setting the toggle buttton "user connection"
     under the menu entry "customize" )  you get the type that he has
     selected otherwise use the one that the routine gsx_check_connection
     gives to you by default 
  */
  if ( foe->con_palette_managed == 0 )
    user_class = 0;
  else {
    *sts = foe->con_palctx->get_select( &user_class);
    if ( EVEN(*sts)) {
      /* SG 20.03.91 Inform the user that he has to choose a conn */
      foe->message( "Choose a connection from the palette");
      BEEP;
      return;
    }
  }
  
  if ( !destination_obj) {
    foe->disable_ldh_cb();
    *sts = gsx_auto_create( foe, x, y, source_obj, source_point,
			   &dest, &destpoint);
    foe->enable_ldh_cb();
    if ( EVEN(*sts)) return;

    if ( select)
      gre->select_node( dest);
    foe->gre->set_node_visible( dest);
  }
  else {
    dest = destination_obj;
    destpoint = destination_point;
  }
  /* Check connection syntax */
  *sts = gsx_check_connection( foe, source_obj, source_point, 
			      dest, destpoint, &con_class, user_class);	
  if ( *sts == GSX__CONTYPE) {
    foe->message( "Connected attributes are not of the same type");
    BEEP;
    return;
  }
  foe->error_msg( *sts);
  if ( EVEN(*sts)) return;

  if ( user_class)
    con_class = user_class;

  *sts = gre->create_con( con_class, source_obj, source_point,
			dest, destpoint, foe->con_drawtype);
  foe->error_msg( *sts);
  if ( EVEN(*sts)) return;

  *sts = FOE__SUCCESS;
}

//
//	Backcall from the controlled gre module when
//	a region is selected. Does nothing.
//
void WFoe::gre_region_selected( WGre *gre)
{
}

//
//	Backcall from the controlled gre module when
//	the cursor enters or leaves the neted widget.
//	Does nothing.
//
void WFoe::gre_enter_leave( WGre *gre)
{
}

//
//	Backcall from the controlled gre module.
//	Delete of selected objects or one single object.
//	If the parameter object is zero all selected objects is deleted.
//	If the parameter object is not zero the object is deleted.
//	Checks first that no object that is to be deleted
//	has a subwindow.
//
void WFoe::gre_delete( WGre *gre, void *object, unsigned long object_type)
{
  unsigned long		node_count;
  vldh_t_node		*nodelist;
  vldh_t_node		*node_ptr;
  int			i, subwind_found;
	

  if ( object == 0 ) {
    /* Check that any node doesn't contain a subwindow */
    gre->get_selnodes( &node_count, &nodelist);
    node_ptr = nodelist;
    subwind_found = FALSE;
    for( i = 0; i < (int)node_count; i++) {
      if ( (*node_ptr)->ln.subwindow != 0 ) {
	subwind_found = TRUE;
	break;
      }
      node_ptr++;
    }
    if ( node_count > 0) free((char *)nodelist);
    if ( subwind_found ) {
      ((WFoe *)gre->parent_ctx)->message( 
		   "Subwindow found, delete the subwindow first");
      BEEP;
    }
    else
      /* Delete selected nodes */
      gre->delete_selected();
  }
  else {
    if ( object_type == VLDH_NODE ) {
      /* Check that the node doesn't have any subwindow!!!! */
      if (((vldh_t_node) object)->ln.subwindow != 0 ) {
	((WFoe *)gre->parent_ctx)->message( 
		     "Subwindow found, delete the subwindow first");
	BEEP;
      }
      else
	gre->delete_node( (vldh_t_node)object);
    }
    else if ( object_type == VLDH_CON ) {
      /* Delete the connection */
      gre->delete_con( (vldh_t_con)object);
    }
  }
}

//
//	Backcall from the controlled gre module.
//	Cut of selected objects or one single object.
//	If the parameter object is zero all selected objects are cut.
//	If the parameter object is not zero the object is cut.
//	Checks first that no object that is to be deleted
//	has a subwindow.
//	If the object is connection it is just deleted.
//
void WFoe::gre_cut( WGre *gre, void *object, unsigned long object_type)
{
  unsigned long		node_count;
  vldh_t_node		*nodelist;
  vldh_t_node		*node_ptr;
  int			i, subwind_found;
  int 			sts;
  WFoe	       		*foe;

  if ( object == 0 ) {
    /* Check that any node doesn't contain a subwindow */
    gre->get_selnodes( &node_count, &nodelist);
    node_ptr = nodelist;
    subwind_found = FALSE;
    for( i = 0; i < (int)node_count; i++) {
      if ( (*node_ptr)->ln.subwindow != 0 ) {
	subwind_found = TRUE;
	break;
      }
      node_ptr++;
    }
    if ( node_count > 0) free((char *) nodelist);
    if ( subwind_found ) {
      ((WFoe *)gre->parent_ctx)->message( 
		   "Subwindow found, delete the subwindow first");
      BEEP;
    }
    else   {
      /* Cut selected nodes */
      sts = gre->cut();

      if ( sts == GRE__NOREGSEL) {
	foe = (WFoe *)gre->parent_ctx;
	foe->message( "Select region before cutting");
	BEEP;
      }
      else if ( EVEN(sts)) foe->error_msg( sts);
    }
  }
  else {
    if ( object_type == VLDH_NODE ) {
      /* Check that the node doesn't have any subwindow!!!! */
      if (((vldh_t_node) object)->ln.subwindow != 0 ) {
	((WFoe *)gre->parent_ctx)->message( 
		     "Subwindow found, delete the subwindow first");
      }
      else
	gre->cut_node( (vldh_t_node)object);
    }
    else if ( object_type == VLDH_CON ) {
      /* Just delete the connection */
      gre->delete_con( (vldh_t_con)object);
    }
  }
}

//
//	Backcall from the controlled gre module.
//	Copy of selected objects or one single object.
//	If the parameter object is zero all selected objects are copied.
//	If the parameter object is not zero the object is copied.
//
void WFoe::gre_copy( WGre *gre, void *object, unsigned long object_type)
{
  int	sts;

  if ( object == 0 ) {
    /* Copy selected region */
    sts = gre->copy();
    if ( ODD(sts))
      ((WFoe *)gre->parent_ctx)->message( "Region copied");
  }
  else {
    if ( object_type == VLDH_NODE ) {
      sts = gre->copy_node( (vldh_t_node)object);
      if ( ODD(sts))
	((WFoe *)gre->parent_ctx)->message( "Object copied");
    }
  }
}

//
//	Backcall from the controlled gre module
//	Paste objects in vldh paste singlenode.
//	Paste a nodes that is cut or copied as a single node.
//
void WFoe::gre_paste( WGre *gre, float x, float y)
{
  WFoe	*foe;

  foe = (WFoe *)gre->parent_ctx;

  foe->disable_ldh_cb();
  gre->paste( x, y, GRE_PASTE_CURSOR);
  foe->enable_ldh_cb();
}

//
//	Backcall from the controlled gre module
//	Starts the attribute editor for the object.
//

void WFoe::gre_attribute( WGre *gre, vldh_t_node object)
{
  ((WFoe *)gre->parent_ctx)->attr_create( object);
}

//
//	Backcall from the controlled gre module
//	Create a subwindow with the specified windowindex or the object.
//
void WFoe::gre_subwindow( WGre *gre, vldh_t_node object, unsigned long subwindow_nr)
{
  int	sts;

  sts = ((WFoe *)gre->parent_ctx)->subwindow_create( object, 
			      subwindow_nr);
  ((WFoe *)gre->parent_ctx)->error_msg(sts);
}

//
//	Backcall from the controlled gre module
//
void WFoe::gre_popupmenu( WGre *gre, int x_pix, int y_pix, int popupmenu_type,
			  vldh_t_node node)
{
  WFoe	*foe;

  foe = (WFoe *)gre->parent_ctx;

  foe->popupmenu_node = 0;
  if ( foe->function == EDIT) {
    if ( popupmenu_type == GRE_POPUPMENUMODE_OBJECT) {
      if ( node != 0) {
	gre->node_select( node);
	foe->popupmenu_node = node;
      }
      
      foe->modify_popup( foe_e_popupmenu_attribute |
			 foe_e_popupmenu_subwindow |
			 foe_e_popupmenu_connect |
			 foe_e_popupmenu_delete	|
			 foe_e_popupmenu_helpclass,
			 x_pix + 5, y_pix);

    }
    else {
      foe->modify_popup( foe_e_popupmenu_delete	|
			 foe_e_popupmenu_copy |
			 foe_e_popupmenu_cut |
			 foe_e_popupmenu_printselect |
			 foe_e_popupmenu_createobject,
			 x_pix + 5, y_pix);
    }
  }
  else {
    if ( node != 0) {
      gre->node_select( node);
      foe->popupmenu_node = node;
    }
    foe->modify_popup( foe_e_popupmenu_attribute |
		       foe_e_popupmenu_subwindow |
		       foe_e_popupmenu_helpclass,
		       x_pix + 5, y_pix);
  }
}

//
//	Backcall from the controlled gre module.
//	Calls the module foe_gobj to make connect operation for 
//	node.
//
void WFoe::gre_getobj( WGre *gre, vldh_t_node node, unsigned long index)
{
  WFoe	*foe;
  int sts ;

  foe = (WFoe *)gre->parent_ctx;
  sts = gobj_get_object ( foe , node, index );
  foe->error_msg(sts);
}

//
//	Backcall from the controlled gre module.
//
void WFoe::gre_undelete( WGre *gre)
{
}

//
// Description: Backcall from the controlled gre module
//
void WFoe::gre_unselect( WGre *gre)
{
}

//
//	Backcall from the controlled gre module
//	Starts the helpwidget with the passed subject ( class of an object).
//
void WFoe::gre_help( WGre *gre, char *help_title)
{
}

//
//	Backcall from the controlled gre module
//
void WFoe::gre_regionmoved( WGre *gre)
{
}

//
//	Backcall from the controlled gre module
//	Recieves messages from gre and displays them with
//	foe_message.
//
void WFoe::gre_message( WGre *gre, const char *message)
{
  /* 
     SG 19.03.91
     send the message received from gre modules to the foe routine
     that will write them on the label widget . The backcalls routines
     gre_xxx should never erase a message because there is chance that
     the message to be erased has just been set by a gre routine
  */

  WFoe *foe;
  int l;

  foe = (WFoe *)gre->parent_ctx ;
  l= strlen(message) ;
  if ( foe->msg_label_id != 0 && l ==0 ) 
    /* erase a message only if there is one to be erased */
    foe->message( "");
  else if (l !=0 )
    /* otherwise just send the message to be set */
    foe->message( message);
  else
    foe->gre->search_rectangle_delete();
}

//
//
//   Methods defined for the function block editor.  */
//
//


void WFoe::enable_ldh_cb()
{
  ldh_cb_enabled = 1;
}

void WFoe::disable_ldh_cb()
{
  ldh_cb_enabled = 0;
}

pwr_tStatus WFoe::ldh_this_session_cb( void *ctx, ldh_sEvent *event)
{
  WFoe *foe = (WFoe *) ctx;
  vldh_t_node	node;
  int		sts;

  if ( !foe->ldh_cb_enabled)
    return 1;

  while (event) {
    switch (event->Event) {
    case ldh_eEvent_AttributeModified:
    case ldh_eEvent_ObjectRenamed:
    case ldh_eEvent_BodyModified:
      sts = vldh_get_node_objdid( event->Object, 
				  foe->gre->wind, &node);
      if ( EVEN(sts)) return 1;
      
      sts = foe->gre->node_update( node);
      break;
    default:
      break;
    }
    event = event->nep;
  }
  return 1;
}

//
//	If the message is a error or warning message it is printed.
//
void WFoe::error_msg( unsigned long sts)
{
  char msg[256];

  if ( EVEN(sts)) {
    msg_GetMsg( sts, msg, sizeof(msg));
    printf("%s\n", msg);
  }
}

//
//	If the message is a error, warning or info message it is printed.
//
void WFoe::info_msg( unsigned long sts)
{
  char msg[256];

  if ( ( EVEN(sts)) || ((sts & 1) && (sts & 2)) ) {
    msg_GetMsg( sts, msg, sizeof(msg));
    printf("%s\n", msg);
  }
}


//
//	Start the attribute editor and store the attribute context pointer 
//	together with WFoe and the nodepointer to use when a backcall
//	comes.
//
int WFoe::attr_create( vldh_t_node node)
{
  WAtt		*watt;
  vldh_t_wind	wind;
  vldh_t_plc	plc;
  int		ate_mode;
  int		sts;
  pwr_sAttrRef	aref;

  ate_mode = ( function == EDIT );

  if ( node == 0 ) {	
    /* This is a plc object calling */
    wind = gre->wind;
    plc = wind->hw.plc;

    aref = cdh_ObjidToAref( plc->lp.oid);
    watt = watt_new( this, 
		     plc->hp.ldhsesctx, aref, 0, 
		     advanced_user, 0);

  }
  else {
    sts = attrlist_get_by_node( node, (void **)&watt);
    if ( ODD(sts))
      watt->pop();
    else {
      aref = cdh_ObjidToAref( node->ln.oid); 
      watt = watt_new( this, 
		       (node->hn.wind)->hw.ldhses,
		       aref, ate_mode, advanced_user, 1);
      watt->close_cb = attr_quit;
      
      /* Store in the array */
      sts = attrlist_insert( watt, this, node);
    }
  }
  return FOE__SUCCESS;
}

//
//	Handles the list of attribute editors in foe.
//	A backcall from the attribute editor is identified by the ate_context.
//	To assosiate this with a foe and a node the context is stored in
//	the attribute list when ate is created.
//
int WFoe::attrlist_get( void *a_ctx, WFoe **foe, vldh_t_node *node)
{
  int	i;

  for ( i = 0; i < attr_count; i++) {
    if ( (attr_pointer + i)->a_ctx == a_ctx) {
      /* This is it */
      *foe = (attr_pointer + i)->foe;
      *node = (attr_pointer + i)->node;
      return FOE__SUCCESS;
    }
  }
  return FOE__OBJECT;
}

int WFoe::attrlist_get_by_node( vldh_t_node node, void **a_ctx)
{
  int	i;

  for ( i = 0; i < attr_count; i++) {
    if ( (attr_pointer + i)->node == node) {
      /* This is it */
      *a_ctx = (attr_pointer + i)->a_ctx;
      return FOE__SUCCESS;
    }
  }
  return FOE__OBJECT;
}

int WFoe::attrlist_insert( void *a_ctx, WFoe *foe, vldh_t_node node)
{
  int		sts;
  WFoe		*dum_foe;
  vldh_t_node	dum_node;

  /* Check that it doesn't exist */
  sts = attrlist_get( a_ctx, &dum_foe, &dum_node);
  if ( EVEN(sts)) {
    sts = utl_realloc( (char **)&attr_pointer, 
		       attr_count * sizeof( foe_sAttr), 
		       (attr_count + 1) * sizeof( foe_sAttr));
    if (EVEN(sts)) return sts;

    (attr_pointer + attr_count)->a_ctx = a_ctx;
    (attr_pointer + attr_count)->foe = foe;
    (attr_pointer + attr_count)->node = node;
    attr_count++;	
  }
  return FOE__SUCCESS;
}

int WFoe::attrlist_delete( void *a_ctx)
{
  int		i;
  int		found;
  int		size;
  foe_sAttr	*item_ptr;
  foe_sAttr	*next_item_ptr;

  /* Find this atectx in the attrlist */
  found = 0;
  for ( i = 0; i < attr_count; i++) {
    if ( (attr_pointer + i)->a_ctx == a_ctx) {
      /* This is it */
      found = 1;
      break;
    }
  }
  if ( !found )
    return FOE__OBJECT;

  item_ptr = attr_pointer + i;
  next_item_ptr = item_ptr + 1;
  size = (attr_count - i - 1) * sizeof( foe_sAttr);
  memcpy( item_ptr, next_item_ptr, size);
  attr_count--;

  return FOE__SUCCESS;
}

//
//	Delete all attribute editors and remove them from the
//	attribute list.
//
int WFoe::attr_delete()
{
  int i;
	
  /* Get the foe in the attributectx array and reset it*/
  for ( i = 0; i < attr_count; i++ ) {
    if ( this == (attr_pointer + i)->foe) {
      delete (WAtt *) (attr_pointer + i)->a_ctx;
      attrlist_delete( (attr_pointer + i)->a_ctx);
    }
  }
  return FOE__SUCCESS;
}

//
//	Tell all the subwindows on the screen that I am quitting now
//	by setting the parentctx in foe zero.
//
int WFoe::parent_quit()
{
  unsigned long		wind_count;
  vldh_t_wind		*windlist;
  vldh_t_wind		*wind_ptr;
  int			i;
  int			sts;
  WFoe		        *subw_foe;
  
  /* Get all the subwindows and reset parentctx */
  sts = vldh_get_wind_subwindows( gre->wind, 
				  &wind_count, &windlist);
  if( EVEN(sts)) return sts;
  wind_ptr = windlist;

  for ( i = 0; i < (int)wind_count; i++) {
    subw_foe = (WFoe *)(*wind_ptr)->hw.foe;
    subw_foe->parent_ctx = 0;
    wind_ptr++;
  }
  if ( wind_count > 0) free((char *) windlist);
  
  return FOE__SUCCESS;
}

//
//	Tell the parent of a subwindow that I am quitting by setting the 
//	subwindow pointer in the nodeobject to zero. 
//	Redraw the node if the window isn't saved.
//
int WFoe::child_quit( vldh_t_node node, unsigned long windowindex)
{
  int	sts;

  /* If the parent is not active there is nothing to do */
  if ( this == 0) return FOE__SUCCESS;

  /* Check if the window is created and not saved, 
     if it is saved don't do anything */

  if ( ((node->hn.subwindowobject[ windowindex ])->hw.status &
	VLDH_CREATE) != 0 ) {
    /* The subwindow in the node will not be saved until the
       subwindow itself is saved so just take away all 
       tracks of the window and redraw the node, don't care
       about ldh */

    if ( node->ln.subwindow & ( windowindex + 1 ) )
      node->ln.subwindow -= ( windowindex + 1 );
    node->ln.subwind_oid[ windowindex ] = pwr_cNObjid;
    
    /* Redraw without subwindowmark */
    sts = gre->subwindow_mark( node);
    if(EVEN(sts)) return sts;
  }

  /* Indicate that the subwindow quits by setting the
     subwindowpointer to zero */
  node->hn.subwindowobject[ windowindex ] = 0;

  return FOE__SUCCESS;
}

//
//	Tell the parent that I am deleteing myself. If the parent is
//	alive take away all tracks of the subwindow. If the subwindow
//	is created and not saved thats all, byt if it is saved it should
//	also be taken away in ldh.
//
int WFoe::child_delete( vldh_t_node node, vldh_t_wind subwind)
{
  vldh_t_plc		plc;
  int			sts, size;
  pwr_sPlcNode		*nodebuffer;
  pwr_tClassId		cid;
  ldh_tSesContext      	ldhsession;
  unsigned long		windowindex;

  windowindex = subwind->lw.subwindowindex;
	
  if ( this != 0 ) {
    /* Parent is alive, take away in vldh and on the screen */
    node->hn.subwindowobject[ windowindex ] = 0;
    if ( (node->ln.subwindow & (1 << windowindex)) != 0 )
      node->ln.subwindow -= ( 1 << windowindex );
    node->ln.subwind_oid[ windowindex ] = pwr_cNObjid;
    
    /* Redraw without subwindowmark */
    gre->subwindow_mark( node);
  }

  if ( ( subwind->hw.status & VLDH_CREATE) != 0 ) {
    /* The subwindow is not saved so we don't have to worry
       about ldh */
  }
  else {
    /* The subwindow should be taken away in ldh,
       open a new session for this purpos */ 
    plc = subwind->hw.plc;
    sts = ldh_OpenSession(&ldhsession, 
			  ldh_SessionToVol( plc->hp.ldhsesctx),
			  ldh_eAccess_SharedReadWrite, ldh_eUtility_PlcEditor);
    if( EVEN(sts)) return sts;
    sts = ldh_GetObjectBuffer( ldhsession,
			       subwind->lw.poid, "DevBody", "PlcNode", 
			       (pwr_eClass *) &cid,	
			       (char **)&nodebuffer, &size);
    if( EVEN(sts)) return sts;
    if ( (nodebuffer->subwindow & (windowindex + 1)) != 0 )
      nodebuffer->subwindow -= (windowindex + 1);
    if ( (nodebuffer->subwindow & (1 << windowindex)) != 0 )
      nodebuffer->subwindow -= ( 1 << windowindex );
    nodebuffer->subwind_oid[ windowindex ] = pwr_cNObjid;
    sts = ldh_SetObjectBuffer(
			      ldhsession,
			      subwind->lw.poid,
			      "DevBody",
			      "PlcNode",
			      (char *)nodebuffer);
    if( EVEN(sts)) return sts;
    free((char *) nodebuffer);
    sts = ldh_SaveSession(ldhsession);
    sts = ldh_CloseSession(ldhsession);
  }
  return FOE__SUCCESS;
}

//
//	Is called when quit is activated and confirmed.
//	Tells the parent and children that I am quitting
//	and destroys myself...
//
void WFoe::quit()
{
  vldh_t_wind	wind;
  int		sts;

  gre->pending_paste_stop();

  /* Tell my parent that his child is quitting, if parent is a node */
  wind = gre->wind;
  if ( wind->hw.parent_node_pointer != 0 )
    ((WFoe *)parent_ctx)->child_quit( wind->hw.parent_node_pointer, 
					 wind->lw.subwindowindex);

  /* Get my children and tell them that their parent is quitting */
  sts = parent_quit();
  error_msg(sts);
  
  /* Avoid any pending backcall from a attribute editor */
  attr_delete();

  /* Delete window in vldh */
  sts = vldh_wind_quit_all( gre->wind );
  error_msg(sts);

  /* Destroy the widget and controlled modules */
  destroy();
}

//
//	Is called when exit is activated and confirmed.
//	Tells the parent and children that I am exiting
//	and destroys myself...
//
void WFoe::foe_exit()
{
  int		windowindex;
  vldh_t_wind	wind;
  vldh_t_node	parent_node;
  int		sts;

  gre->pending_paste_stop();

  /* Tell my parent that his child is quitting, if parent is a node */
  wind = gre->wind;
  windowindex = wind->lw.subwindowindex;
  parent_node = wind->hw.parent_node_pointer;
  if ( parent_node != 0 )
    if ( parent_ctx != 0)
      parent_node->hn.subwindowobject[ windowindex ] = 0;

  /* Get my children and tell them that their parent is exiting */
  sts = parent_quit();
  error_msg(sts);

  /* Avoid any pending backcall from a attribute editor */
  attr_delete();

  /* Delete window in vldh */
  vldh_wind_quit( gre->wind );

  /* Destroy the widget and controlled modules */
  destroy();
}

//
//	Is called when window delete is activated and confirmed.
//	Tells the parent and children that I am to delete myself,
//	and do so.
//
void WFoe::foe_delete()
{
  vldh_t_wind	wind;
  int		sts;

  /* Tell my parent that his child is deleted, if parent is a node */
  wind = gre->wind;

  gre->pending_paste_stop();

  sts = ldh_SaveSession( wind->hw.ldhses);
  if ( EVEN(sts)) return;

  if ( wind->hw.parent_node_pointer != 0 ) {
    sts = ((WFoe *)parent_ctx)->child_delete( wind->hw.parent_node_pointer, 
						 wind);
    error_msg(sts);
  }

  /* Avoid any pending backcall from a attribute editor */
  attr_delete();

  if ( (wind->hw.status & VLDH_CREATE) != 0 ) {
    /* The window is not saved in ldh, just quit in vldh */
    sts = vldh_wind_quit_all(wind);
    error_msg(sts);
  }
  else {
    /* Delete in ldh as well */	
    sts = vldh_wind_delete_all( wind );
    error_msg(sts);
  }

  /* Destroy the widget and controlled modules */
  destroy();
}

//
//	Initiates a window when it is created.
//	Creates the default nodes for the windows specified in
//	graphbody of the classobject.
//
int WFoe::init_window()
{
  vldh_t_wind		wind;
  pwr_sGraphPlcWindow 	*graphbody;
  pwr_tClassId		bodyclass;
  int			i, sts, size;
  vldh_t_node		node;

  /* Create the nodes in the window class object */
  /* Get the classinformation for this window */
  wind = gre->wind;

  sts = ldh_GetClassBody( wind->hw.ldhses, wind->lw.cid, 
			  "GraphPlcWindow", &bodyclass, (char **)&graphbody, &size);
  if ( EVEN(sts)) return sts;

  /* Create the nodes specified there */
  for ( i = 0; i < (int)graphbody->defaultobjects; i++) {
    sts = gre->create_node( graphbody->defobj_class[i],
			       graphbody->defobj_x[i],
			       graphbody->defobj_y[i], &node);
  }
  return FOE__SUCCESS;
}

//
//	Creates a subwindow.
//	Calls the module foe_gsx to check sytax and return a suitable
//	window index.
//
int WFoe::subwindow_create( vldh_t_node object, unsigned long subwindow_nr)
{
  WFoe		       	*newfoe;
  int			sts, size;
  pwr_tClassId		bodyclass;
  pwr_sGraphPlcNode 	*graphbody;
  int			subwindow_count;
  unsigned long		new_window;
  vldh_t_plc		plcobject;
  vldh_t_wind		subwind;
  foe_eFuncAccess		function_access;

  /* Check if this is a correct window to create */
  sts = gsx_check_subwindow( object, &subwindow_nr, &function_access);
 
  /* Check if the window exists */
  if ( object->hn.subwindowobject[ subwindow_nr - 1 ] != 0 ) {
    /* Yes, there is already a window */
    message( "Window already exists");
    /* GS 11.04.91 Little trick to put the window	
       on top of the stack in any case ( ie to make is visible ) */
	
    subwind = object->hn.subwindowobject[ subwindow_nr - 1 ];
    newfoe = (WFoe *)subwind->hw.foe;
    newfoe->pop();
    return FOE__WINDEXIST;
  }

  /* Get graphbody for the class */
  sts = ldh_GetClassBody( (object->hn.wind)->hw.ldhses, 
			  object->ln.cid, "GraphPlcNode", 
			  &bodyclass, (char **)&graphbody, &size);
  if (EVEN(sts)) return sts;

  /* Get number of subwindows for this object */
  subwindow_count = graphbody->subwindows;

  if ( subwindow_count == 0 ) {
    /* Sorry, no subwindow in this class, message and return */
    message( "No subwindow in this object");
    BEEP;
    return FOE__SUCCESS;
  }

  if( subwindow_count == 1 ) {
    /* Get the subwindow type */
	  
    /* Is this a new window ? */
    if ( object->ln.subwindow == 0 )
      new_window = TRUE;
    else
      new_window = FALSE;
    
    /* Create subwindow */
    plcobject = (object->hn.wind)->hw.plc;
    newfoe = subwindow_new( this, object->hn.name, pwr_cNObjid, 
			    0, plcobject->hp.ldhsesctx,
			    object, 0, new_window,
			    map_window, access, function_access, options, &sts);
    if ( sts == FOE__WINDNOTFOUND) {
      message( "Subwindow does not exist");
      BEEP;
      return FOE__SUCCESS;
    }
    else if ( EVEN(sts)) return sts;

    /* Draw subwindowmark in node */
    if ( new_window )
      gre->subwindow_mark( object);
  }
  else if ( subwindow_count == 2 ) {
    /* Is this a new window ? */
    if ( (object->ln.subwindow & subwindow_nr) == 0 )
      new_window = TRUE;
    else
      new_window = FALSE;
    
    /* Create subwindow */
    plcobject = (object->hn.wind)->hw.plc;
    newfoe = subwindow_new( this, object->hn.name, pwr_cNObjid, 
			    0, plcobject->hp.ldhsesctx, 
			    object, (subwindow_nr - 1),
			    new_window, map_window, access,
			    function_access, options, &sts);
    if ( sts == FOE__WINDNOTFOUND) {
      message( "Subwindow does not exist");
      BEEP;
      return FOE__SUCCESS;
    }
    else if ( EVEN(sts)) return sts;

    /* Draw subwindowmark in node */
    if ( new_window )
      gre->subwindow_mark( object);
  }
  return FOE__SUCCESS;
}


//
//_Callbacks_____________________________________________________________
//

//
//	Backcall from the attribute editor when the quit button is pressed.
//
void WFoe::attr_quit( void *a_ctx)
{
  int	sts;

  /* Delete from attribute list */
  sts = attrlist_delete( a_ctx);
  error_msg(sts);

  /* Delete the attribute editor */
  delete (WAtt *)a_ctx;
}

//
// Constructor
//
WFoe::WFoe( void		*f_parent_ctx,
	    const char		*f_name,
	    pwr_tObjid		plcprogram,
	    ldh_tWBContext 	ldhwbctx,
	    ldh_tSesContext	ldhsesctx,
	    vldh_t_node		nodeobject,
	    unsigned long	windowindex,
	    unsigned long	new_window,
	    int			f_map_window,
	    ldh_eAccess		f_access,
	    foe_eFuncAccess	function_access,
	    unsigned int	f_options,
	    pwr_tStatus		*sts) :
  WUtility(wb_eUtility_PlcEditor),
  parent_ctx(f_parent_ctx),
  node_palctx(0), con_palctx(0), navctx(0), tractx(0), gre(0), msg_label_id(0),
  function(0), wanted_function(0), plcobject(0), con_palette_managed(0),
  node_palette_managed(0), nav_palette_managed(0), con_drawtype(GOEN_CONDRAW),
  show_execorder(0), searchindex(0), popupmenu_mask(~0), popupmenu_node(0),
  access(f_access), map_window(f_map_window), advanced_user(1), ldh_cb_enabled(0),
  classeditor(0), options(f_options)
{
  strcpy( name, f_name);
}


//
// Constructor
//
WFoe::WFoe( void *f_parent_ctx,
	    const char *f_name,
	    pwr_tObjid plcprogram,
	    ldh_tWBContext ldhwbctx,
	    ldh_tSesContext ldhsesctx,
	    int f_map_window,
	    ldh_eAccess	f_access,
	    unsigned int	f_options,
	    pwr_tStatus *sts) :
  WUtility(wb_eUtility_PlcEditor),
  parent_ctx(f_parent_ctx),
  node_palctx(0), con_palctx(0), navctx(0), tractx(0), gre(0), msg_label_id(0),
  function(0), wanted_function(0), plcobject(0), con_palette_managed(0),
  node_palette_managed(0), nav_palette_managed(0), con_drawtype(GOEN_CONDRAW),
  show_execorder(0), searchindex(0), popupmenu_mask(~0), popupmenu_node(0),
  access(f_access), map_window(f_map_window), advanced_user(1), ldh_cb_enabled(0),
  classeditor(0), options(f_options)
{
  strcpy( name, f_name);
}

//
// Get the foe of a plcpgm, return 0 if not found.
//
WFoe *WFoe::get( pwr_tOid oid)
{
  vldh_t_plc plc;
  pwr_tStatus sts;

  sts = vldh_get_plc_objdid( oid, &plc);
  if ( EVEN(sts)) return 0;

  if ( !plc->hp.wind)
    return 0;
  else
    return (WFoe *)(plc->hp.wind)->hw.foe;
}

//
//	Create a new foe object
// 	SG : 09.02.91  Define a pixmap for the icon
//	SG : 24.02.91  change the call to create_window
//
int WFoe::new_local( pwr_tObjid	plcprogram,
		     ldh_tWBContext 	ldhwbctx,
		     ldh_tSesContext	ldhsesctx,
		     vldh_t_node	nodeobject,
		     unsigned long	windowindex,
		     unsigned long	new_window,
		     foe_eFuncAccess	function_access)
{
  WFoe		*old_foe;
  pwr_tObjid	windowobjdid;
  vldh_t_plc	plc;
  vldh_t_wind 	windowobject;
  pwr_tClassId	wind_class;
  int		sts, size;
  pwr_tClassId	bodyclass;
  pwr_sGraphPlcProgram *plcgraphbody;
  pwr_sGraphPlcNode *nodegraphbody;
  pwr_sGraphPlcWindow *windgraphbody;
  char		*windbuffer;
  char		*plcbuffer;
  pwr_tFullName	subwind_name;
  int		window_found;
  pwr_eClass	windclass;
  pwr_tClassId	plcclass;
  int		plc_loaded;
  pwr_tClassId	classid;

  if ( cdh_ObjidIsNotNull( plcprogram)) {
    if ( !ldh_LocalObject( ldhsesctx, plcprogram))
      function_access = foe_eFuncAccess_View;

    /* This is probably a plcprogram, check that it really is */
    sts = ldh_GetObjectBuffer( ldhsesctx,
			       plcprogram, "DevBody", "PlcProgram", (pwr_eClass *)&plcclass,	
			       &plcbuffer, &size);
    if( EVEN(sts)) 
      /* This is not a plcprogram object, return */
      return FOE__NOPLC;
    
    free((char *) plcbuffer);

    sts = ldh_GetObjectClass(ldhsesctx, plcprogram, &plcclass);
    if ( EVEN(sts)) return sts;

    /* Get the windowtype for this plcprogram */
    sts = ldh_GetClassBody(ldhsesctx, plcclass, 
			   "GraphPlcPgm", &bodyclass, (char **)&plcgraphbody, &size);
    if ( EVEN(sts)) return sts;
    wind_class = plcgraphbody->subwindow_class; 

    /* Get graphbody for the window */
    sts = ldh_GetClassBody(ldhsesctx, 
			   wind_class, "GraphPlcWindow", &bodyclass, 
			   (char **)&windgraphbody, &size);
    if ( EVEN(sts)) return sts;
    
    /* Check if this plcprogram already is loaded in vldh */
    plc_loaded = 0;
    sts = vldh_get_plc_objdid( plcprogram, &plc);
    if ( ODD(sts) ) {
      /* Loaded in vldh, do nothing */
      if ((plc->hp.wind)->hw.foe != 0) {
	// Already loaded, this should not happen
	return FOE__WINDEXIST;
      }
      else
	plc_loaded = 1;
    }
    if ( (sts == VLDH__OBJNOTFOUND) || plc_loaded ) {
      /* Check if windowobject in ldh exists */
      /* Get the first child to the plc */
      sts = ldh_GetChild( ldhsesctx, plcprogram, &windowobjdid);
      window_found = 0;
      while ( ODD(sts) ) {
	/* Check if window */
	sts = ldh_GetObjectBuffer( ldhsesctx,
				   windowobjdid, "DevBody", "PlcWindow", &windclass,	
				   &windbuffer, &size);
	if ( ODD(sts))  {
	  free((char *) windbuffer);
	  window_found = 1;	
	  break;
	}
	sts = ldh_GetNextSibling( ldhsesctx, windowobjdid, &windowobjdid);
      }
      
      if ( window_found ) {
	/* Window exists in ldh, load the plcprogram to vldh */
	if ( !plc_loaded) {
	  sts = vldh_plc_load( plcprogram, ldhwbctx, ldhsesctx,
			       &plc);
	  if ( EVEN(sts)) return sts;
	}
	sts = vldh_get_wind_objdid( windowobjdid, &windowobject);
	if ( sts == VLDH__OBJNOTFOUND ) {
	  sts = vldh_wind_load( plc, 0, windowobjdid, 0,
				&windowobject, access);
	  if ( EVEN(sts)) return sts;
	  
	  sts = vldh_wind_load_all( windowobject);
	  if ( sts == VLDH__WINDCORRUPT) {}
	  else if ( EVEN(sts)) return sts;
	}
	/* Create the foe context */
	/* SG 02.05.91 by default give the view function */
	function = VIEW;
	sts = create_window( 0, 50, 920, 750, windowobject->hw.ldhses,
			     function_access);
	if ( EVEN(sts)) return sts;

	gre->wind = windowobject;
	windowobject->hw.foe = this;
	gre->window_draw();

	gre->wind = windowobject;
	plcobject = plc;
	plcobject->hp.wind = windowobject;
	plcobject->hp.hinactx = parent_ctx;
	windowobject->hw.foe = this;
      }
      else {
	/* This is a brandnew plcprogram */

	/* Check authorization to open a new plcpgm */
	if ( !(CoLogin::privilege() & pwr_mPrv_DevPlc )) {
	  return LOGIN__USERNOTAU;
	}
	access = ldh_eAccess_ReadWrite;
	sts = vldh_plc_create( plcprogram, ldhwbctx, ldhsesctx,
			       &plc);
	if ( EVEN(sts)) return sts;

	/* Create window object in vldh */
	sts = vldh_wind_create(plc, 0, 0, wind_class,
			       0, &windowobject, access);
	if ( EVEN(sts)) {
	  vldh_plc_delete(plc);
	  return sts;
	}
	
	/* Create the foe context */
	function = EDIT;
	sts = create_window( windgraphbody->x, windgraphbody->y, 
			     windgraphbody->width, windgraphbody->height, 
			     windowobject->hw.ldhses, function_access);
	if ( EVEN(sts)) return sts;
	
	gre->wind = windowobject;
	windowobject->hw.foe = this;
	windowobject->lw.x = windgraphbody->x;
	windowobject->lw.y = windgraphbody->y;
	windowobject->lw.width = windgraphbody->width;
	windowobject->lw.height = windgraphbody->height;
	plcobject = plc;
	plcobject->hp.wind = windowobject;
	plcobject->hp.hinactx = parent_ctx;
	windowobject->hw.foe = this;	    	    

	sts = init_window();
	if ( EVEN(sts)) return sts;
      }
    }
  }
  else {
    /* This is a subwindow to a node in gre */
    plcobject = (nodeobject->hn.wind)->hw.plc;
    /* Get the windowtype for this plcprogram */
    sts = ldh_GetClassBody(plcobject->hp.ldhsesctx, 
			   nodeobject->ln.cid, 
			   "GraphPlcNode", &bodyclass, (char **)&nodegraphbody, &size);
    if ( EVEN(sts)) return sts;
    wind_class = nodegraphbody->subwindow_class[windowindex];

    /* Get graphname for the window and add to the nodename */
    sts = ldh_GetClassBody(plcobject->hp.ldhsesctx, 
			   wind_class, 
			   "GraphPlcWindow", &bodyclass, (char **)&windgraphbody, &size);
    if ( EVEN(sts)) return sts;
    sprintf(subwind_name, "%s  %s", nodeobject->hn.name, 
	    windgraphbody->graphname);
    
    if ( new_window ) {
      if ( access == ldh_eAccess_ReadOnly ||
	   function_access == foe_eFuncAccess_View ||
	   !ldh_LocalObject( plcobject->hp.ldhsesctx, 
			     nodeobject->ln.oid))
	/* A subwindow can not be created */
	return FOE__WINDNOTFOUND;
      
      access = ldh_eAccess_SharedReadWrite;

      /* Create the foe module */
      old_foe = (WFoe *)parent_ctx ;
      function = old_foe->function;
      if ( function_access == foe_eFuncAccess_View) {
	if ( function == EDIT)
	  function = VIEW;
	if ( function == SIMULATE)
	  function = TRACE;
      }
      else
	function = VIEW;
      
      /* Create window object in vldh */
      sts = vldh_wind_create( (nodeobject->hn.wind)->hw.plc,
			      nodeobject, 0, wind_class,
			      windowindex,
			      &windowobject, access);
      if ( EVEN(sts)) return sts;

      strcpy( name, subwind_name);
      sts = create_window( windgraphbody->x, windgraphbody->y, 
			   windgraphbody->width, windgraphbody->height, 
			   windowobject->hw.ldhses,
			   function_access);
      if ( EVEN(sts)) return sts;

      vldh_node_subwindow_created( nodeobject, windowobject, windowindex);
      gre->wind = windowobject;
      windowobject->hw.foe = this;
      plcobject = 0;
      windowobject->hw.foe = this;	    	    
      windowobject->lw.x = windgraphbody->x;
      windowobject->lw.y = windgraphbody->y;
      windowobject->lw.width = windgraphbody->width;
      windowobject->lw.height = windgraphbody->height;

      sts = init_window();
      if ( EVEN(sts)) return sts;

      sts = ldh_SetSession( windowobject->hw.ldhses, ldh_eAccess_ReadOnly);
      if ( EVEN(sts)) return sts;
    }
    else {
      /* Subwindow already exists, check if window exists */
      if ( nodeobject->hn.subwindowobject[ windowindex ] == 0 ) {
	/* No, create new foe window and load the window */	
	old_foe = (WFoe *)parent_ctx ;

	if ( access == ldh_eAccess_ReadWrite)
	  access = ldh_eAccess_ReadOnly;

	sts = vldh_get_wind_objdid( nodeobject->ln.subwind_oid[windowindex], &windowobject); 
	if ( sts == VLDH__OBJNOTFOUND ) {
	  sts = vldh_wind_load( (nodeobject->hn.wind)->hw.plc, 
				nodeobject, 
				nodeobject->ln.subwind_oid[windowindex], 0,
				&windowobject, access);
	  if ( EVEN(sts)) return sts;
	  
	  sts = vldh_wind_load_all( windowobject);
	  if ( sts == VLDH__WINDCORRUPT) {}
	  else if ( EVEN(sts)) return sts;
	}

	function = old_foe->function;
	if ( function_access == foe_eFuncAccess_View) {
	  if ( function == EDIT)
	    function = VIEW;
	  if ( function == SIMULATE)
	    function = TRACE;
	}
	else
	  function = VIEW;

	strcpy( name, subwind_name);
	sts = create_window( windgraphbody->x, windgraphbody->y, 
			     windgraphbody->width, windgraphbody->height, 
			     windowobject->hw.ldhses,
			     function_access);
	if ( EVEN(sts)) return sts;

	gre->wind = windowobject;
	gre->window_draw();

	plcobject = 0;
	windowobject->hw.foe = this;
	nodeobject->hn.subwindowobject[ windowindex ] = windowobject;
      }
      else {
	/* foewindow exists, do nothing */
	return FOE__WINDEXIST;
      } 
    }
  }
  ldh_AddThisSessionCallback( windowobject->hw.ldhses,
			      this, ldh_this_session_cb);
  enable_ldh_cb();

  /* SG 22.05.91 is this the good position to start trace ??? */
  if ( function == TRACE) 
    trace_start( this);
  else if ( function == SIMULATE) {
    trace_start( this);
    gre->disable_button_events();
    trace_simsetup( this);
  }

  // Check if class editor
  sts = ldh_GetVolumeClass( ldh_SessionToWB(windowobject->hw.ldhses),
			    windowobject->lw.oid.vid, &classid);
  if ( EVEN(sts)) return sts;
	
  if ( ldh_VolRepType( windowobject->hw.ldhses) == ldh_eVolRep_Mem &&
       cdh_isClassVolumeClass( classid))
    classeditor = 1;
  if ( function == EDIT)
    edit_set_entries();
	     	     
  /* Write document headers /CJ 050415 */
  gre->init_docobjects();
  
  return FOE__SUCCESS;
}

//
//	Setup for the view mode.
//	Defines the events and backcallfunktions for gre in view.
//
int WFoe::view_setup()
{
  int sts ;

  /* the same entries are disables for trace and view */
  view_set_entries();

  gre->view_setup();

  sts = gre->setup_backcalls( gre_setup_window,
				 gre_node_created,
				 gre_con_created,
				 gre_node_moved,
				 gre_delete,
				 gre_cut,
				 gre_copy,
				 gre_paste,
				 gre_attribute,
				 gre_subwindow,
				 0,
				 gre_popupmenu,
				 gre_getobj,
				 gre_undelete,
				 gre_unselect,
				 gre_help,
				 gre_regionmoved,
				 gre_message);

  return sts;
}

//
//	Setup for the edit mode.
//	Defines the events and backcallfunktions for gre in edit.
//
int WFoe::edit_setup()
{
  int sts;

  /* enable the menu entries that were disable 
  for trace the same entries are disables for trace and view */
   
  edit_set_entries();

  gre->edit_setup();

  /* setup the backcalls from gre */
  sts = gre->setup_backcalls( gre_setup_window,
				 gre_node_created,
				 gre_con_created,
				 gre_node_moved,
				 gre_delete,
				 gre_cut,
				 gre_copy,
				 gre_paste,
				 gre_attribute,
				 gre_subwindow,
				 0,
				 gre_popupmenu,
				 gre_getobj,
				 gre_undelete,
				 gre_unselect,
				 gre_help,
				 gre_regionmoved,
				 gre_message);
  
  return FOE__SUCCESS;
}

//
//	Show the execute order in a display rectangle in each node.
//
int WFoe::show_executeorder()
{
  vldh_t_node 	*node_list;
  vldh_t_node 	*node_ptr;
  unsigned long	node_count;
  int		i, sts;
  char		text[10];

  /* Get all nodes */
  sts = vldh_get_nodes( gre->wind, 
			&node_count, &node_list);
  if ( EVEN(sts)) return sts;
  node_ptr = node_list;

  for ( i = 0; i < (int)node_count; i++) {
    if ( (*node_ptr)->hn.executeorder > 0) {
      sprintf( text, "%ld", (*node_ptr)->hn.executeorder);
      gre->set_display_value( *node_ptr, text);
    }
    node_ptr++;
  }
  gre->display();
  if ( node_count > 0) free((char *)node_list);
  return FOE__SUCCESS;
}

//
// Search next for search in node annotations
//
int WFoe::search_string_next()
{
  char		searchstr[80];
  char		annotations[1300];
  int		sts, size;
  vldh_t_node 	*node_list;
  vldh_t_node 	*node_ptr;
  unsigned long	node_count;
  int		i, j, found;
  vldh_t_node	node;
  int		len1, len2;
  vldh_t_wind	wind;
  pwr_tObjName	class_name;
  pwr_tClassId	cid;
  pwr_tObjid	class_objid;

  wind = gre->wind; 
  strcpy( searchstr, searchstring);

  if( searchstr[0] == 0) {
    message( "Start with SearchString");
    BEEP;
    return FOE__SUCCESS;
  }

  /* Get all nodes */
  sts = vldh_get_nodes( wind, &node_count, &node_list);
  if ( EVEN(sts)) return sts;

  found = 0;
  for ( j = searchindex; j < (int)node_count; j++) {
    node_ptr = node_list + j;
    sts = gre->node_annot_message( *node_ptr, annotations, 
				      sizeof(annotations)-200, 1024);
    strcat( annotations, " ");
    strcat( annotations, (*node_ptr)->hn.name);
    strcat( annotations, " ");
    sts = ldh_GetObjectClass( wind->hw.ldhses,
			      (*node_ptr)->ln.oid, &cid);
    if ( EVEN(sts)) return sts;
    class_objid = cdh_ClassIdToObjid( cid);
    sts = ldh_ObjidToName( wind->hw.ldhses, class_objid, 
			   ldh_eName_Object, class_name, sizeof( class_name), &size);
    if ( EVEN(sts)) return sts;
    strcat( annotations, class_name);
    strcat( annotations, " ");
    
    utl_toupper( annotations, annotations);

    len1 = strlen( annotations);
    len2 = strlen( searchstr);

    if ( len1 >= len2) {
      for ( i = 0; i <= (len1 - len2); i++) {
	if ( strncmp( &annotations[i], searchstr, strlen(searchstr)) == 0) {
	  /* the string matches */
	  searchindex = j + 1;
	  node = *node_ptr;
	  found = 1;
	  break;
	}
      }
    }
    if ( found)
      break;
  }
  if ( node_count > 0) free((char *)node_list);

  if ( !found) {
    gre->unselect();
    message( "String not found");
    BEEP;
    searchindex = 0;
    return FOE__SUCCESS;
  }	

  /* Center and select the object */
  gre->unselect();
  message( "");
  sts = gre->center_node( node);
  gre->search_rectangle_create( node);

  return FOE__SUCCESS;
}

//
// Search for a string i node annotations
//
int WFoe::search_string( WFoe *foe, char *searchstr)
{

  /* Insert the search string in foe */
  utl_toupper( foe->searchstring, searchstr);

  /* Reset the search index */
  foe->searchindex = 0;

  /* Search */
  foe->search_string_next();

  return FOE__SUCCESS;
}

//
// Search for an object specified by name
//
int WFoe::search_object( WFoe *foe, char *searchstr)
{
  int		sts, size;
  pwr_tFullName	hiername;
  vldh_t_wind	wind;
  pwr_tObjid	objdid;
  vldh_t_node	node;

  wind = foe->gre->wind;
  sts = ldh_ObjidToName( wind->hw.ldhses, wind->lw.oid, 
			 ldh_eName_Hierarchy,
			 hiername, sizeof( hiername), &size); 
  if ( EVEN(sts)) return sts;

  strcat( hiername, "-");
  strcat( hiername, searchstr);
  sts = ldh_NameToObjid( wind->hw.ldhses, &objdid, hiername);
  if ( EVEN(sts)) {
    foe->message( "Object does not exist");
    BEEP;
    return FOE__SUCCESS;
  }
  sts = vldh_get_node_objdid( objdid, wind, &node);
  if ( EVEN(sts)) {
    foe->message( "Object not found in this window");
    BEEP;
    return FOE__SUCCESS;
  }
  foe->gre->unselect();
  foe->message( "");
  
  sts = foe->gre->center_node( node);
  foe->gre->search_rectangle_create( node);

  return FOE__SUCCESS;
}

//
// Center an object
//
int WFoe::center_object( pwr_tOid oid)
{
  int		sts;
  vldh_t_wind	wind;
  vldh_t_node	node;

  wind = gre->wind;
  sts = vldh_get_node_objdid( oid, wind, &node);
  if ( EVEN(sts)) return sts;

  gre->unselect();
  message( "");
  sts = gre->center_node( node);
  gre->search_rectangle_create( node);

  return FOE__SUCCESS;
}


//
//  	Prints an overview.
//
int WFoe::print_overview()
{
  unsigned long		node_count;
  vldh_t_node		*nodelist;
  vldh_t_node		*node_ptr;
  int			i, sts;
  float		ll_x_min;
  float		ll_y_min;
  float		ur_x_max;
  float		ur_y_max;
  float		ll_x;
  float		ll_y;
  float		width;
  float		height;
  int		doc_count;
  vldh_t_wind	wind;
  char		file_id[80];

  ll_x_min = 10000.;
  ll_y_min = 10000.;
  ur_x_max = -10000.;
  ur_y_max = -10000.;

  wind = gre->wind;

  sts = vldh_get_nodes( wind, &node_count, &nodelist);
  if ( EVEN(sts)) return sts;
  
  /* Init the document objects */
  //	gre_init_docobjects( gre);

  doc_count = 0;
  node_ptr = nodelist;
  for ( i = 0; i < (int)node_count; i++) {
    if ( vldh_check_document( wind->hw.ldhses, 
			      (*node_ptr)->ln.oid)) {
      /* Calculate coordinates for an overview */
      gre->measure_object( *node_ptr, &ll_x, &ll_y, &width, &height);
      ll_x_min = co_min( ll_x_min, ll_x);
      ll_y_min = co_min( ll_y_min, ll_y);
      ur_x_max = co_max( ur_x_max, ll_x + width);
      ur_y_max = co_max( ur_y_max, ll_y + height);
      
      doc_count++;	  
    }
    node_ptr++;
  }

  if ( doc_count > 1) {
    /* Print the overview */
    strcpy( file_id, vldh_IdToStr(0, wind->lw.oid)); 
    gre->print_rectangle( ll_x_min, ll_y_min, ur_x_max, 
			     ur_y_max, file_id);
  }

  if ( node_count > 0) free((char *) nodelist);

  if ( doc_count <= 1)
    return FOE__NOOBJFOUND;
	
  return FOE__SUCCESS;
}

//
//  	Prints an overview as pdf.
//
int WFoe::print_pdf_overview()
{
  unsigned long		node_count;
  vldh_t_node		*nodelist;
  vldh_t_node		*node_ptr;
  int			i, sts;
  float		ll_x_min;
  float		ll_y_min;
  float		ur_x_max;
  float		ur_y_max;
  float		ll_x;
  float		ll_y;
  float		width;
  float		height;
  int		doc_count;
  vldh_t_wind	wind;
  char		file_id[80];

  ll_x_min = 10000.;
  ll_y_min = 10000.;
  ur_x_max = -10000.;
  ur_y_max = -10000.;

  wind = gre->wind;

  sts = vldh_get_nodes( wind, &node_count, &nodelist);
  if ( EVEN(sts)) return sts;
  
  /* Init the document objects */
  //	gre_init_docobjects( gre);

  doc_count = 0;
  node_ptr = nodelist;
  for ( i = 0; i < (int)node_count; i++) {
    if ( vldh_check_document( wind->hw.ldhses, 
			      (*node_ptr)->ln.oid)) {
      /* Calculate coordinates for an overview */
      gre->measure_object( *node_ptr, &ll_x, &ll_y, &width, &height);
      ll_x_min = co_min( ll_x_min, ll_x);
      ll_y_min = co_min( ll_y_min, ll_y);
      ur_x_max = co_max( ur_x_max, ll_x + width);
      ur_y_max = co_max( ur_y_max, ll_y + height);
      
      doc_count++;	  
    }
    node_ptr++;
  }

  if ( doc_count >= 1) {
    /* Print the overview */
    strcpy( file_id, vldh_IdToStr(0, wind->lw.oid)); 
    gre->print_pdf_rectangle( ll_x_min, ll_y_min, ur_x_max, 
			      ur_y_max, file_id);
  }

  if ( node_count > 0) free((char *) nodelist);

  if ( doc_count <= 0)
    return FOE__NOOBJFOUND;
	
  return FOE__SUCCESS;
}

//
//	Enable events and menu entris depending on the function
//	in which the user works.
//      Author : SG 15.05.91
//
void WFoe::function_setup()
{

  /* SG 02.05.91 set the default toggel button corresponding to the mode on */

  if ( function == EDIT ) {
    set_mode_button_state( EDIT, 1);
    edit_setup();
  }
  else if ( function == TRACE) {
    set_mode_button_state( TRACE, 1);
    /* SG 22.05.91 
    It is more that a setup: we want also to initialise the tracing.
    For this the differents contexts have to be defined.
    so trace_Start is not called here */
  }
  else if ( function == VIEW ) {
    set_mode_button_state( VIEW, 1);
    view_setup(); 
  }
  else if ( function == SIMULATE ) {
    set_mode_button_state( SIMULATE, 1);
  }
}

//
//  	Prints the documents in a window.
//
int WFoe::print_document()
{
  unsigned long		node_count;
  vldh_t_node		*nodelist;
  vldh_t_node		*node_ptr;
  int			i, sts;
  vldh_t_wind	wind;
  int		doc_count;

  wind = gre->wind;

  sts = vldh_get_nodes( wind, &node_count, &nodelist);
  if ( EVEN(sts)) return sts;

//	gre_init_docobjects( gre);

  doc_count = 0;
  node_ptr = nodelist;
  for ( i = 0; i < (int)node_count; i++) {
    if ( vldh_check_document( wind->hw.ldhses, 
			      (*node_ptr)->ln.oid)) {
      gre->print_docobj( *node_ptr);
      doc_count++;	  
    }
    node_ptr++;
  }
  
  if ( node_count > 0) free((char *) nodelist);

  if ( doc_count == 0)
    return FOE__NOOBJFOUND;

  return FOE__SUCCESS;
}

//
//  	Prints the documents in a window.
//
int WFoe::print_selected_document()
{
  unsigned long		node_count;
  vldh_t_node		*nodelist;
  vldh_t_node		*node_ptr;
  int			i, sts;
  vldh_t_wind	wind;
  int		doc_count;

  wind = gre->wind;

  sts = gre->get_selnodes( &node_count, &nodelist);
  if ( EVEN(sts)) return sts;

  gre->unselect();
//	gre_init_docobjects( gre);

  doc_count = 0;
  node_ptr = nodelist;
  for ( i = 0; i < (int)node_count; i++) {
    if ( vldh_check_document( wind->hw.ldhses, 
			      (*node_ptr)->ln.oid)) {
      gre->print_docobj( *node_ptr);
      doc_count++;	  
    }
    node_ptr++;
  }

  if ( node_count > 0) free((char *) nodelist);

  if ( doc_count == 0)
    return FOE__NOOBJFOUND;

  return FOE__SUCCESS;
}

//
//  	Changed mode.
//
int WFoe::change_mode( int new_mode)
{
  vldh_t_wind	wind;
  int		sts, vldh_mod;
  ldh_sSessInfo	info;

  wind = gre->wind;

  switch( new_mode) {
  case VIEW:
    switch( function) {
    case VIEW:
      /* Mode unchanged, set togglebutton */
      set_mode_button_state( VIEW, 1);
      break;
    case EDIT:
      /* Save changes... */
      wind = gre->wind;

      gre->pending_paste_stop();

      /* Check if changes are made */
      sts = ldh_GetSessionInfo( wind->hw.ldhses, &info);
      if ( EVEN(sts)) return sts;
      sts = vldh_get_wind_modification( wind, &vldh_mod);
      
      if ( !info.Empty || vldh_mod) {
	/* Assume mode change is not successfull */
	wanted_function = VIEW;
	set_mode_button_state( VIEW, 0);
	set_mode_button_state( EDIT, 1);
	popupmsg( "Do you want to save changes",
		  edit_exit_save, edit_exit_nosave,
		  NULL);
      }
      else {
	/* Change funktion */
	set_mode_button_state( EDIT, 0);
	set_mode_button_state( VIEW, 1);
	function = VIEW;
	gre->disable_button_events();
	view_setup();
	sts = ldh_SetSession( wind->hw.ldhses, 
			      ldh_eAccess_ReadOnly);
	if ( EVEN(sts)) return sts;
	access = ldh_eAccess_ReadOnly;
      }
      break;
    case TRACE:
      /* Stop trace */
      set_mode_button_state( TRACE, 0);
      set_mode_button_state( VIEW, 1);
      trace_stop( this);
      function = VIEW;
      gre->disable_button_events();
      view_setup ();
      sts = ldh_SetSession( wind->hw.ldhses, 
			    ldh_eAccess_ReadOnly);
      if ( EVEN(sts)) return sts;
      access = ldh_eAccess_ReadOnly;
      break;
    case SIMULATE:
      /* Stop trace and simulate */
      set_mode_button_state( SIMULATE, 0);
      set_mode_button_state( VIEW, 1);
      trace_stop( this);
      function = VIEW ;
      gre->disable_button_events();
      view_setup();
      sts = ldh_SetSession( wind->hw.ldhses, 
			    ldh_eAccess_ReadOnly);
      if ( EVEN(sts)) return sts;
      access = ldh_eAccess_ReadOnly;
      break;
    }
    break;
  case EDIT:
    if ( !(CoLogin::privilege() & pwr_mPrv_DevPlc )) {
      message( "No privilege to enter Edit");
      BEEP;
      set_mode_button_state( EDIT, 0);
      return FOE__SUCCESS;
    }
    
    switch( function) {
    case VIEW:
      sts = ldh_SetSession( wind->hw.ldhses, 
			    ldh_eAccess_ReadWrite);
      if( EVEN(sts)) {
	message( "No access to edit, close other session");
	BEEP;
	set_mode_button_state( VIEW, 1);
	set_mode_button_state( EDIT, 0);
	ldh_SetSession( wind->hw.ldhses, 
			ldh_eAccess_ReadOnly);
	access = ldh_eAccess_ReadOnly;
	return FOE__SUCCESS;
      }
      ldh_AddThisSessionCallback( wind->hw.ldhses,
				  this, ldh_this_session_cb);
      enable_ldh_cb();
      access = ldh_eAccess_ReadWrite;
      function = EDIT ;
      gre->disable_button_events();
      edit_setup();
      set_mode_button_state( VIEW, 0);
      break;
    case EDIT:
      /* Mode unchanged */
      set_mode_button_state( EDIT, 1);
      break;
    case TRACE:
      /* Stop trace */
      trace_stop( this);
      function = EDIT;
      gre->disable_button_events();
      edit_setup();
      set_mode_button_state( TRACE, 0);
      break;
    case SIMULATE:
      /* Stop trace and simulate */
      trace_stop( this);
      function = EDIT;
      gre->disable_button_events();
      edit_setup();
      set_mode_button_state( SIMULATE, 0);
      break;
    }
    break;
  case TRACE:
    
    switch( function) {
    case VIEW:
      /* Change funktion */
      sts = ldh_SetSession( wind->hw.ldhses, 
			    ldh_eAccess_ReadWrite);
      if( EVEN(sts)) {
	message( "No access to trace, close other session");
	BEEP;
	set_mode_button_state( VIEW, 1);
	set_mode_button_state( TRACE, 0);
	return FOE__SUCCESS;
      }
      ldh_AddThisSessionCallback( wind->hw.ldhses,
				  this, ldh_this_session_cb);
      enable_ldh_cb();
      access = ldh_eAccess_ReadWrite;
      gre->disable_button_events();
      sts = trace_start( this);
      if ( EVEN(sts)) {
	/* Return to view */
	view_setup();
	set_mode_button_state( VIEW, 1);
	set_mode_button_state( TRACE, 0);
	ldh_SetSession( wind->hw.ldhses, 
			ldh_eAccess_ReadOnly);
	access = ldh_eAccess_ReadOnly;
	return sts;
      }
      set_mode_button_state( VIEW, 0);
      function = TRACE;
      break;
    case EDIT:
      /* Save changes... */
      wind = gre->wind;
      
      gre->pending_paste_stop();

      /* Check if changes are made */
      sts = ldh_GetSessionInfo( wind->hw.ldhses, &info);
      if ( EVEN(sts)) return sts;
      sts = vldh_get_wind_modification( wind, &vldh_mod);
      
      if ( !info.Empty || vldh_mod) {
	/* Assume mode change is not successfull */
	wanted_function = TRACE;
	set_mode_button_state( TRACE, 0);
	set_mode_button_state( EDIT, 1);
	popupmsg( "Do you want to save changes", 
		  edit_exit_save, edit_exit_nosave, 
		  NULL); 
      }
      else {
	/* Change function */
	sts = trace_start( this);
	if ( EVEN(sts)) {
	  /* Return to view */
	  edit_setup();
	  set_mode_button_state( EDIT, 1);
	  set_mode_button_state( TRACE, 0);
	  return sts;
	}
	set_mode_button_state( EDIT, 0);
	function = TRACE;
	gre->disable_button_events();
      }
      break;
    case TRACE:
      /* Mode unchanged, set togglebutton */
      set_mode_button_state( TRACE, 1);
      break;
    case SIMULATE:
      set_mode_button_state( SIMULATE, 0);
      gre->disable_button_events();
      trace_trasetup( this);
      function = TRACE;
      break;
    }
    break;
  case SIMULATE:
    
    if ( !(CoLogin::privilege() & pwr_mPrv_DevPlc )) {
      message( "No privilege to enter Simulate");
      BEEP;
      set_mode_button_state( SIMULATE, 0);
      return FOE__SUCCESS;
    }
    switch( function) {
    case VIEW:
      /* Change funktion */
      sts = ldh_SetSession( wind->hw.ldhses, 
			    ldh_eAccess_ReadWrite);
      if( EVEN(sts)) {
	message( "No access to simulate, close other session");
	BEEP;
	set_mode_button_state( VIEW, 1);
	set_mode_button_state( SIMULATE, 0);
	return FOE__SUCCESS;
      }
      ldh_AddThisSessionCallback( wind->hw.ldhses,
				  this, ldh_this_session_cb);
      enable_ldh_cb();
      access = ldh_eAccess_ReadWrite;
      gre->disable_button_events();
      sts = trace_start( this);
      if ( EVEN(sts)) {
	/* Return to view */
	view_setup();
	set_mode_button_state( VIEW, 1);
	set_mode_button_state( SIMULATE, 0);
	return sts;
      }
      set_mode_button_state( VIEW, 0);
      function = SIMULATE;
      gre->disable_button_events();
      trace_simsetup( this);
      message( "Mode is Simulate");
      break;
    case EDIT:
      /* Save changes... */
      wind = gre->wind;
      
      gre->pending_paste_stop();

      /* Check if changes are made */
      sts = ldh_GetSessionInfo( wind->hw.ldhses, &info);
      if ( EVEN(sts)) return sts;
      sts = vldh_get_wind_modification( wind, &vldh_mod);
      
      if ( !info.Empty || vldh_mod) {
	/* Assume mode change is not successfull */
	wanted_function = SIMULATE;
	set_mode_button_state( SIMULATE, 0);
	set_mode_button_state( EDIT, 1);
	popupmsg( "Do you want to save changes", 
		  edit_exit_save, edit_exit_nosave, 
		  NULL); 
      }
      else {
	/* Change function */
	gre->disable_button_events();
	sts = trace_start( this);
	if ( EVEN(sts)) {
	  /* Return to view */
	  view_setup();
	  set_mode_button_state( EDIT, 1);
	  set_mode_button_state( SIMULATE, 0);
	  return sts;
	}
	set_mode_button_state( EDIT, 0);
	function = SIMULATE;
	gre->disable_button_events();
	trace_simsetup( this);
	message( "Mode is Simulate");
      }
      break;
    case TRACE:
      set_mode_button_state( TRACE, 0);
      function = SIMULATE;
      gre->disable_button_events();
      trace_simsetup( this);
      message( "Mode is Simulate");
      break;
    case SIMULATE:
      /* Mode unchanged, set togglebutton */
      set_mode_button_state( SIMULATE, 1);
      break;
      
    }
    break;
  }
  return FOE__SUCCESS;
}

//_Backcall from questionbox___________________________________________

//
//	Backcall when save is pressed in the questionbox at exit.
//
void WFoe::edit_exit_save( WFoe *foe)
{
  vldh_t_wind	wind;
  vldh_t_node	parent_node;
  int	sts;


  /* Check that the parent node is saved */
  wind = foe->gre->wind;
  parent_node = wind->hw.parent_node_pointer;
  if ( parent_node != 0 ) {
    /* Parent is node, not a plc */
    if ( foe->parent_ctx != 0 ) {
      /* parent is alive */
      if ((parent_node->hn.status & VLDH_CREATE) != 0) {
	/* Parentnode is not saved, then its not allowed to save
	   the window */
	foe->msgbox( "Save the parent window first") ;
	return;
      }
    }
  }
  foe->disable_ldh_cb();
  sts = vldh_wind_save( foe->gre->wind);
  foe->error_msg( sts);
  if ( sts == VLDH__PLCNOTSAVED ) {
    foe->msgbox( "Save the plcprogram in the hierarchy editor first") ;
    foe->enable_ldh_cb();
    return;
  }
  else if (EVEN(sts)) {
    char msg[256];

    msg_GetMsg( sts, msg, sizeof(msg));
    foe->msgbox( msg);
    foe->enable_ldh_cb();
    return;
  }

  foe->create_flow();

  unsigned int opt;
  if ( foe->options & foe_mOption_EnableComment)
    opt = log_mOption_Comment;
  else
    opt = 0;
  wb_log::log( (wb_session *)foe->gre->wind->hw.ldhses, wlog_eCategory_PlcSave, 
	       foe->gre->wind->lw.oid, opt);
	
  /* Change the function */
  switch( foe->wanted_function) {
  case SIMULATE:
    foe->gre->disable_button_events();
    sts = trace_start( foe);
    if ( EVEN(sts)) {
      /* Return to edit */
      foe->edit_setup();
      return;
    }
    foe->gre->disable_button_events();
    trace_simsetup( foe);
    foe->set_mode_button_state( EDIT, 0);
    foe->set_mode_button_state( SIMULATE, 1);
    foe->function = SIMULATE;
    break;
  case TRACE:
    foe->gre->disable_button_events();
    sts = trace_start( foe);
    if ( EVEN(sts)) {
      /* Return to edit */
      foe->edit_setup();
      return;
    }
    foe->set_mode_button_state( EDIT, 0);
    foe->set_mode_button_state( TRACE, 1);
    foe->function = TRACE;
    break;
  case VIEW:
    foe->set_mode_button_state( EDIT, 0);
    foe->set_mode_button_state( VIEW, 1);
    foe->function = VIEW;
    foe->gre->disable_button_events();
    foe->view_setup();
    sts = ldh_SetSession( wind->hw.ldhses, 
			  ldh_eAccess_ReadOnly);
    foe->error_msg(sts);
    foe->access = ldh_eAccess_ReadOnly;
    break;
  }
}

//
//	Backcall when no save is pressed in the questionbox at exit.
//
void WFoe::edit_exit_nosave( WFoe *foe)
{
  foe->message( "Mode is still Edit, revert by exiting the PlcEditor");
  BEEP;
}

//
//	Returns the hina ctx.
//
void WFoe::get_hinactx( void **hinactx)
{
  vldh_t_wind	wind;
  vldh_t_plc	plc;

  wind = gre->wind;
  plc = wind->hw.plc;
  *hinactx = plc->hp.hinactx;
}


//
//  	Redraw the window.
//
int WFoe::redraw_and_save()
{
  int sts;

  gre->edit_setup();

  sts = gre->redraw();
  if ( EVEN(sts)) return sts;

  disable_ldh_cb();
  sts = vldh_wind_save( gre->wind);
  enable_ldh_cb();
  if ( EVEN(sts)) return sts;

  sts = create_flow();
  if ( EVEN(sts)) return sts;

  wb_log::log( (wb_session *)gre->wind->hw.ldhses, wlog_eCategory_PlcSave, gre->wind->lw.oid);

  return FOE__SUCCESS;
}


//
//  	Create flowfiles.
//
int WFoe::create_flow()
{
  int sts;

  if ( !classeditor) {
    sts = gre->set_trace_attributes( 0);
    sts = gre->save( 0);
  }
  else {
    vldh_t_wind	wind;
    int 	size;
    pwr_tOName 	name;
    vldh_t_plc	plc;
    pwr_tFileName fname;
    char 	classname[80];
    pwr_tObjid 	classdef;

    wind = gre->wind;
    plc = wind->hw.plc;

    sts = ldh_ObjidToName( wind->hw.ldhses, plc->lp.oid,
			   ldh_eName_Hierarchy, name, sizeof( name), &size); 
    if ( EVEN(sts)) return sts;
    
    sts = ldh_GetParent( wind->hw.ldhses, plc->lp.oid, &classdef);
    if ( EVEN(sts)) return sts;

    sts = ldh_ObjidToName( wind->hw.ldhses, classdef,
			   ldh_eName_Object, classname, sizeof( classname), &size); 
    if ( EVEN(sts)) return sts;
    cdh_ToLower( classname, classname);
    sprintf( fname, "$pwrp_load/pwr_%s.flw", classname);
    sts = gre->set_trace_attributes( name);
    sts = gre->save( fname);
  }

  return sts;
}

/* API routines */

#if 0
/************************************************************************
*
* Name: foe_Open 
*		 
* Description:	Activates an editor context and creates a new instance if 
*		needed 
*
*************************************************************************/

foe_tContext foe_Open(
  void		    *ParentEditorContext,
  void		    *ParentWindowContext,
  ldh_tSesContext   SessCtx,
  pwr_tObjid	    PlcPgmObject
) {

  foe_tContext Foe = NULL;


  foe_new (ParentEditorContext, (Widget)ParentWindowContext,  
	   "PlcProgram", PlcPgmObject, ldh_SessionToWB(SessCtx), SessCtx, 
	   (WFoe **)&Foe, 1, ldh_eAccess_ReadOnly);

  return Foe;

} /* foe_Open */

/************************************************************************
*
* Name: foe_Close 
*		 
* Description:	Closes a foe editor and frees allocated resources
*************************************************************************/
void foe_Close (
  foe_tContext FoeContext
) {

  foe_delete ((WFoe *) FoeContext);

}/* foe_Close */

#endif
