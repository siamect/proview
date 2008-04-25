/** 
 * Proview   $Id: xtt_fileview_gtk.h,v 1.1 2008-04-25 11:08:06 claes Exp $
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

#ifndef xtt_fileview_gtk_h
#define xtt_fileview_gtk_h

#include "xtt_fileview.h"

/* xtt_fileview_gtk.h -- view and select files */

class XttFileviewGtk : public XttFileview {
 public:
  XttFileviewGtk( void *xn_parent_ctx, GtkWidget *xn_parent_wid, pwr_tOid xn_oid,
		  char *xn_title, char *xn_dir, char *xn_pattern, int xn_type,
		  char *xn_target_attr, char *xn_trigger_attr);
  ~XttFileviewGtk() {}

  GtkWidget    	*toplevel;
  GtkWidget    	*list;
  GtkWidget     *input_text;
  pwr_tFileName dir;
  pwr_tString40 pattern;
  pwr_tAName	target_attr;
  pwr_tAName	trigger_attr;
  pwr_tString40	*filelist;
  pwr_tString40 selected_file;
  int		filecnt;
  void      	*parent_ctx;
  pwr_tOid 	oid;
  int		type;

  void execute( char *file);

  static void list_cursor_changed_cb( GtkTreeView *tree_view, gpointer data);
  static void list_row_activated_cb( GtkTreeView *tree_view, GtkTreePath *path,
				     GtkTreeViewColumn *column, gpointer data);
  static void list_ok_cb ( GtkWidget *w, gpointer data);
  static void list_input_cb ( GtkWidget *w, gpointer data);
  static void list_cancel_cb( GtkWidget *w, gpointer data);
  static void list_save_cb( void *ctx, void *data);
};

#endif
