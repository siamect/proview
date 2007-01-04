/* 
 * Proview   $Id: xtt_fast_motif.h,v 1.1 2007-01-04 08:30:03 claes Exp $
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

#ifndef xtt_fast_motif_h
#define xtt_fast_motif_h

/* xtt_fast_motif.h -- Fast curves */

#ifndef xtt_fast_h
# include "xtt_fast.h"
#endif

class XttFastMotif : public XttFast {
 public:
  Widget     	parent_widget;		//!< Parent widget.
  
  XttFastMotif ( void *xn_parent_ctx,
		 Widget xn_parent_wid,
		 char *xn_name,
		 Widget *w,
		 pwr_sAttrRef *fast_arp,
		 int *sts);
  ~XttFastMotif();
};

#endif












