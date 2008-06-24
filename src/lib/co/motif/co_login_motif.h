/* 
 * Proview   $Id: co_login_motif.h,v 1.1 2008-06-24 06:51:43 claes Exp $
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

#ifndef co_login_motif_h
#define co_login_motif_h

#ifndef co_login_h
#include "co_login.h"
#endif

struct login_widgets 
{                                             
  Widget	toplevel;
  Widget	login_window;
  Widget	label;
  Widget        adb;
  Widget	usernamevalue;
  Widget	passwordvalue;
};

class CoLoginMotif : public CoLogin {
 public:
  Widget	parent_wid;
  struct login_widgets	widgets;

  CoLoginMotif( void		*wl_parent_ctx,
	       Widget       	wl_parent_wid,
	       char		*wl_name,
	       char		*wl_groupname,
	       void		(* wl_bc_success)( void *),
	       void		(* wl_bc_cancel)( void *),
	       pwr_tStatus   	*sts);
  ~CoLoginMotif();

  pwr_tStatus get_values();
  void message( char *new_label);

  static void activate_ok( Widget w, CoLogin *loginctx, XmAnyCallbackStruct *data);
  static void activate_cancel( Widget w, CoLogin *loginctx, XmAnyCallbackStruct *data);
  static void create_label( Widget w, CoLogin *loginctx, XmAnyCallbackStruct *data);
  static void create_adb( Widget w, CoLogin *loginctx, XmAnyCallbackStruct *data);
  static void create_usernamevalue( Widget w, CoLogin *loginctx, XmAnyCallbackStruct *data);
  static void create_passwordvalue( Widget w, CoLogin *loginctx, XmAnyCallbackStruct *data);
  static void valchanged_passwordvalue( Widget w, CoLogin *loginctx, XmAnyCallbackStruct *data);
  static void valchanged_usernamevalue( Widget w, CoLogin *loginctx, XmAnyCallbackStruct *data);
};

#endif
