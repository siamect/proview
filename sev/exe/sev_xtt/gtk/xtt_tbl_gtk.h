/* 
 * Proview   $Id: xtt_tbl_gtk.h,v 1.1 2008-07-17 11:18:31 claes Exp $
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

#ifndef xtt_tbl_gtk_h
#define xtt_tbl_gtk_h

#include "../src/xtt_tbl.h"
#include "xtt_tblnav_gtk.h"
#include "co_wow_gtk.h"

/* ge_tbl.h -- Ge tblibute editor */


class XttTblGtk : public XttTbl {
  public:
    XttTblGtk(
      GtkWidget                 *a_parent_wid,
      void			*a_parent_ctx,
      sevcli_tCtx		a_sevcli,
      sevcli_sHistItem         	*itemlist,
      int			item_cnt);
    GtkWidget	*parent_wid;
    GtkWidget	*brow_widget;
    GtkWidget	*form_widget;
    GtkWidget	*toplevel;
    GtkWidget	*msg_label;
    GtkWidget	*pane;

    void message( char severity, char *message);
    XttDsHist *dshist_new( pwr_tOid oid, char *aname);
    
    ~XttTblGtk();
};


#endif
