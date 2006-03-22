/** 
 * Proview   $Id: co_mrm_util.h,v 1.5 2006-03-22 14:45:57 claes Exp $
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

#ifndef co_mrm_util_h
#define co_mrm_util_h

/* co_mrm_util.h -- Motif utitlities */

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	mrm_eUtility_Xtt,
	mrm_eUtility_Admin,
	mrm_eUtility_Wtt,
	mrm_eUtility_Ge,
	mrm_eUtility_PlcEditor,
	mrm_eUtility_WAtt,
	mrm_eUtility_Attr,
	mrm_eUtility_Wda,
	mrm_eUtility_XAtt,
	mrm_eUtility_XAttOne,
	mrm_eUtility_GsdAttr
	} mrm_eUtility;

void mrm_TextInit( Widget w, XtActionProc value_changed_cb, mrm_eUtility utility);
int mrm_TextInput( Widget w, XEvent *event, char *recall, int line_size, 
                   int recall_size, int *current_recall_line);
int mrm_IsIconicState( Widget W);
void mrm_PositionPopup( Widget popup, Widget parent, int x, int y);

#ifdef __cplusplus
}
#endif

#endif
