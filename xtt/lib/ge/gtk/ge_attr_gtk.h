/* 
 * Proview   $Id: ge_attr_gtk.h,v 1.2 2008-10-31 12:51:33 claes Exp $
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

#ifndef ge_attr_gtk_h
#define ge_attr_gtk_h

#include "ge_attr.h"
#include "ge_attrnav_gtk.h"
#include "co_wow_gtk.h"

/* ge_attr.h -- Ge attribute editor */

/*! \file ge_attr_gtk.h
    \brief Contains the AttrGtk class. */
/*! \addtogroup Ge */
/*@{*/

//! Attribute editor.
/*! Displays grow attributes and dynamic attributes for an object or graph. */
class AttrGtk : public Attr {
  public:
    AttrGtk(
      GtkWidget                 *a_parent_wid,
      void			*a_parent_ctx,
      void 			*a_object,
      attr_sItem  		*itemlist,
      int			item_cnt);
    GtkWidget	*parent_wid;
    GtkWidget	*brow_widget;
    GtkWidget	*form_widget;
    GtkWidget	*toplevel;
    GtkWidget	*msg_label;
    GtkWidget	*cmd_prompt;
    GtkWidget	*cmd_input;
    GtkWidget	*cmd_scrolledinput;
    GtkWidget	*cmd_scrolledtextview;
    GtkWidget	*cmd_scrolled_ok;
    GtkWidget	*cmd_scrolled_ca;
    GtkTextBuffer *cmd_scrolled_buffer;
    GtkWidget	*pane;
    // int         recall_idx;
    // GeDyn 	*original_data;
    // static char	value_recall[30][160];
    // int		value_current_recall;
    static CoWowRecall 	value_recall;
    CoWowEntryGtk	*cmd_entry;

    void message( char severity, const char *message);
    void set_prompt( const char *prompt);
    void change_value();
    int reconfigure_attr();
    
    ~AttrGtk();
};


/*@}*/
#endif
