/* 
 * Proview   $Id: xtt_block_gtk.h,v 1.1 2007-01-04 08:29:32 claes Exp $
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

#ifndef xtt_block_gtk_h
#define xtt_block_gtk_h

/* xtt_block_gtk.h -- Alarm blocking window */

#ifndef xtt_block_h
# include "xtt_block.h"
#endif

class BlockGtk : public Block {
  public:
    BlockGtk( void *b_parent_ctx,
		GtkWidget *b_parent_wid,
		pwr_sAttrRef *b_oar,
		char *name,
		unsigned int priv,
		pwr_tStatus *status);
    ~BlockGtk();
    int execute();
    void update();

    GtkWidget		*parent_wid;
    GtkWidget		*toplevel;
    GtkWidget		*form;
    GtkWidget		*toggleA;
    GtkWidget		*toggleB;
    GtkWidget		*toggleC;
    GtkWidget		*toggleD;
    GtkWidget		*toggleNo;
    GtkWidget		*buttonOk;
    GtkWidget		*buttonApply;

    static void activate_ok( GtkWidget *w, gpointer data);
    static void activate_cancel( GtkWidget *w, gpointer data);
    static void activate_apply( GtkWidget *w, gpointer data);
};

#endif



