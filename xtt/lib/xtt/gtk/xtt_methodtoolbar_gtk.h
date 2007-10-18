/* 
 * Proview   $Id: xtt_methodtoolbar_gtk.h,v 1.2 2007-10-18 12:46:35 claes Exp $
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

#ifndef xtt_methodtoolbar_gtk_h
#define xtt_methodtoolbar_gtk_h

#include "xtt_methodtoolbar.h"

class XttMethodToolbarGtk;

typedef struct {
  XttMethodToolbarGtk *mt;
  int idx;
} xtt_sMethodButtonCb;
  
class XttMethodToolbarGtk : public  XttMethodToolbar {
 public:
  GtkWidget *m_toolbar_w;
  GtkWidget *m_button_w[m_size];
  xtt_sMethodButtonCb m_cb[m_size];
  gint 	    m_timerid;

  XttMethodToolbarGtk( XNav *xnav);
  ~XttMethodToolbarGtk();
  GtkWidget *build();
  void set_sensitive();
  void set_current_sensitive();

  static void activate_button( GtkWidget *w, gpointer data);
  static gboolean set_sensitive_cb( void *data);

};

#endif
