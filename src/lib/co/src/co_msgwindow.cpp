/** 
 * Proview   $Id: co_msgwindow.cpp,v 1.10 2007-01-04 07:51:42 claes Exp $
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

/* co_msgwindow.cpp -- Message window */

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>

#include "co_cdh.h"
#include "co_time.h"
#include "co_dcli.h"

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

extern "C" {
#include "flow_x.h"
}
#include "co_lng.h"
#include "co_xhelp.h"
#include "co_msgwindow.h"


MsgWindow *MsgWindow::default_window = 0;
int  MsgWindow::hide_info = 0;

void MsgWindow::msgw_find_wnav_cb( void *ctx, pwr_tOid oid)
{
  MsgWindow *msgw = (MsgWindow *)ctx;
  if ( msgw->find_wnav_cb)
    (msgw->find_wnav_cb)( msgw->parent_ctx, oid);
}

void MsgWindow::msgw_find_plc_cb( void *ctx, pwr_tOid oid)
{
  MsgWindow *msgw = (MsgWindow *)ctx;
  if ( msgw->find_plc_cb)
    (msgw->find_plc_cb)( msgw->parent_ctx, oid);
}

void MsgWindow::message( int severity, const char *text, msgw_ePop pop, pwr_tOid oid, bool is_plc)
{

  if ( default_window) {
    if ( severity == 'O')
      severity = 'I';
    default_window->insert( severity, text, oid, is_plc);
  }
  else {
    if ( (hide_info && severity == 'I') || severity == 'O')
      return;
    if ( severity == 'E' || severity == 'W' || severity == 'F' || severity == 'I')
      printf( "%c %s\n", severity, text);
    else
      printf( "%s\n", text);
  }
  if ( default_window &&
       (pop == msgw_ePop_Yes || 
	(pop == msgw_ePop_Default && ( severity == 'E' || severity == 'F' || severity == 'W'))))
    default_window->map();
}

void MsgWindow::message( const co_error& e, const char *text1, const char *text2, pwr_tOid oid, bool is_plc)
{
  pwr_tStatus sts = e.sts();
  int severity;
  switch( sts & 7) {
  case 0:
    severity = 'W';
    break;
  case 1:
    severity = 'S';
    break;
  case 2:
    severity = 'E';
    break;
  case 3:
    severity = 'I';
    break;
  case 4:
    severity = 'F';
    break;
  default:
    severity = ' ';
  }
  message( severity, e.what().c_str(), text1, text2, oid, is_plc);
}

void MsgWindow::message( int severity, const char *text1, const char *text2, const char *text3, 
			 pwr_tOid oid, bool is_plc)
{
  char text[400];
  strncpy( text, text1, sizeof(text));
  text[sizeof(text)-1] = 0;
  if ( text2) {
    strncat( text, " ", sizeof(text) - strlen(text));
    strncat( text, text2, sizeof(text) - strlen(text));
    text[sizeof(text)-1] = 0;
  }
  if ( text3) {
    strncat( text, " ", sizeof(text) - strlen(text));
    strncat( text, text3, sizeof(text) - strlen(text));
    text[sizeof(text)-1] = 0;
  }
  MsgWindow::message( severity, text, msgw_ePop_Default, oid, is_plc);
}

MsgWindow::MsgWindow(
	void *msg_parent_ctx,
	char *msg_name,
	pwr_tStatus *status) :
  parent_ctx(msg_parent_ctx), msgnav(NULL), displayed(0),
  deferred_map(0), nodraw(0), size(0), max_size(500), find_wnav_cb(0), find_plc_cb(0),
  wow(0)
{
  *status = 1;
  strcpy( name, msg_name);
}


void MsgWindow::reset_nodraw()
{
  msgnav->reset_nodraw();
  nodraw--;
  if ( !nodraw && deferred_map)
    map();
}

void MsgWindow::insert( int severity, const char *text, pwr_tOid oid, bool is_plc)
{
  if (  size > max_size - 1)
    msgnav->set_nodraw();

  if ( cdh_ObjidIsNull( oid))
    new ItemMsg( msgnav, "", (char *)text, severity, NULL, flow_eDest_Before);
  else if ( is_plc)
    new ItemMsgObjectPlc( msgnav, "", (char *)text, severity, oid, NULL, flow_eDest_Before);
  else
    new ItemMsgObject( msgnav, "", (char *)text, severity, oid, NULL, flow_eDest_Before);


  if ( size > max_size - 1) {
    msgnav->remove_oldest();
    msgnav->reset_nodraw();
  }
  else
    size++;
}









