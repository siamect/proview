/* 
 * Proview   $Id: xtt_sevhist_gtk.h,v 1.2 2008-10-31 12:51:36 claes Exp $
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

#ifndef xtt_sevhist_gtk_h
#define xtt_sevhist_gtk_h

/* xtt_sevhist_gtk.h -- SevHist curves */

#ifndef xtt_sevhist_h
# include "xtt_sevhist.h"
#endif

class XttSevHistGtk : public XttSevHist {
 public:
  GtkWidget    	*parent_widget;		//!< Parent widget.
  
  XttSevHistGtk( void *xn_parent_ctx,
		GtkWidget *xn_parent_wid,
		const char *xn_name,
		GtkWidget **w,
		pwr_tOid *xn_oid,
		pwr_tOName *xn_aname,
		sevcli_tCtx xn_scctx,
		int *sts);
  ~XttSevHistGtk();
};

#endif












