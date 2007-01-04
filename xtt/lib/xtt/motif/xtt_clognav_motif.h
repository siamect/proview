/* 
 * Proview   $Id: xtt_clognav_motif.h,v 1.1 2007-01-04 08:30:03 claes Exp $
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

#ifndef xtt_clognav_motif_h
#define xtt_clognav_motif_h

/* xtt_clognav_motif.h -- Console message window. */


// Status is defined as int i xlib...

#include <vector>
using namespace std;

#ifdef Status
# undef Status
#endif

#ifndef xtt_clognav_h
# include "xtt_clognav.h"
#endif

class CLogNavMotif : public CLogNav {
  public:
    CLogNavMotif( void *ev_parent_ctx,
	     Widget	ev_parent_wid,
	     Widget *w);
    ~CLogNavMotif();

    Widget		parent_wid;
    Widget		brow_widget;
    Widget		form_widget;
    Widget		toplevel;

    void set_input_focus();
};

#endif


