/* 
 * Proview   $Id: xtt_dshist_gtk.h,v 1.1 2008-07-17 11:20:33 claes Exp $
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

#ifndef xtt_dshist_gtk_h
#define xtt_dshist_gtk_h

/* xtt_dshist_gtk.h -- DsHist curves */

#ifndef xtt_dshist_h
# include "xtt_dshist.h"
#endif

class XttDsHistGtk : public XttDsHist {
 public:
  GtkWidget    	*parent_widget;		//!< Parent widget.
  
  XttDsHistGtk( void *xn_parent_ctx,
		GtkWidget *xn_parent_wid,
		char *xn_name,
		GtkWidget **w,
		pwr_tOid *xn_oid,
		pwr_tOName *xn_aname,
		sevcli_tCtx xn_scctx,
		int *sts);
  ~XttDsHistGtk();
};

#endif












