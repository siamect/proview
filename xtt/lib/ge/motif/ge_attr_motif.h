/* 
 * Proview   $Id: ge_attr_motif.h,v 1.2 2008-10-31 12:51:33 claes Exp $
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

#include "ge_attr.h"

#ifndef ge_attr_motif_h
#define ge_attr_motif_h

/* ge_attr_motif.h -- Ge attribute editor */

/*! \file ge_attr_motif.h
    \brief Contains the AttrMotif class. */
/*! \addtogroup Ge */
/*@{*/

//! Attribute editor.
/*! Displays grow attributes and dynamic attributes for an object or graph. */
class AttrMotif : public Attr {
  public:
    AttrMotif(
      Widget                    a_parent_wid,
      void			*a_parent_ctx,
      void 			*a_object,
      attr_sItem  		*itemlist,
      int			item_cnt);
    Widget	parent_wid;
    Widget	brow_widget;
    Widget	form_widget;
    Widget	toplevel;
    Widget	msg_label;
    Widget	cmd_prompt;
    Widget	cmd_input;
    Widget	cmd_scrolledinput;
    Widget	cmd_scrolled_ok;
    Widget	cmd_scrolled_ca;
    Widget	attrnav_form;
    static char	value_recall[30][160];
    int		value_current_recall;

    void message( char severity, const char *message);
    void set_prompt( const char *prompt);
    void change_value();
    int reconfigure_attr();
    
    ~AttrMotif();
};


/*@}*/
#endif

