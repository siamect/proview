/* 
 * Proview   $Id: co_login_gtk.h,v 1.2 2008-10-31 12:51:30 claes Exp $
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

#ifndef co_login_gtk_h
#define co_login_gtk_h

#ifndef co_login_h
#include "co_login.h"
#endif

struct login_widgets 
{                                             
  GtkWidget	*toplevel;
  GtkWidget	*login_window;
  GtkWidget	*label;
  GtkWidget     *okbutton;
  GtkWidget	*usernamevalue;
  GtkWidget	*passwordvalue;
};

class CoLoginGtk : public CoLogin {
 public:
  GtkWidget	*parent_wid;
  struct login_widgets	widgets;

  CoLoginGtk( void		*wl_parent_ctx,
	       GtkWidget       	*wl_parent_wid,
	       const char      	*wl_name,
	       const char      	*wl_groupname,
	       void		(* wl_bc_success)( void *),
	       void		(* wl_bc_cancel)( void *),
	       pwr_tStatus   	*sts);
  ~CoLoginGtk();

  pwr_tStatus get_values();
  void message( const char *new_label);

  static void activate_ok( GtkWidget *w, gpointer data);
  static void activate_cancel( GtkWidget *w, gpointer data);
  static void valchanged_passwordvalue( GtkWidget *w, gpointer data);
  static void valchanged_usernamevalue( GtkWidget *w, gpointer data);
};

#endif
