/* 
 * Proview   $Id: wb_wdanav_motif.h,v 1.2 2008-10-31 12:51:31 claes Exp $
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

#ifndef wb_wattnav_motif_h
#define wb_wattnav_motif_h

#ifndef wb_wdanav_h
# include "wb_wdanav.h"
#endif

class WdaNavMotif : public WdaNav {
  public:
    WdaNavMotif(
	void 		*wa_parent_ctx,
	Widget		wa_parent_wid,
	const char     	*wa_name,
	ldh_tSesContext wa_ldhses,
	pwr_tObjid 	wa_objid,
	pwr_tClassId 	wa_classid,
        char            *wa_attribute,
	int 		wa_editmode,
	int 		wa_advanced_user,
	int		wa_display_objectname,
	wb_eUtility	wa_utility,
	Widget 		*w,
	pwr_tStatus 	*status);
    ~WdaNavMotif();

    Widget		parent_wid;
    Widget		brow_widget;
    Widget		form_widget;
    Widget		toplevel;
    XtIntervalId	trace_timerid;

    void set_inputfocus();
};


#endif


