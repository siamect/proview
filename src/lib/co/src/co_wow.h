/** 
 * Proview   $Id: co_wow.h,v 1.7 2005-09-06 14:17:10 claes Exp $
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

#ifndef co_wow_h
#define co_wow_h

/* co_wow -- useful windows */

#include <Xm/Xm.h>
#include <X11/Xatom.h>

#if defined __cplusplus
extern "C" {
#endif

typedef enum {
  wow_eFileSelType_All,
  wow_eFileSelType_Dbs,
  wow_eFileSelType_Wbl,
  wow_eFileSelType_WblClass
} wow_eFileSelType;

typedef struct {
  Widget    dialog;
  void      (*file_selected_cb)( void *, char *, wow_eFileSelType);
  void      *parent_ctx;
  wow_eFileSelType file_type;
} *wow_tFileSelCtx;

typedef struct {
  Widget    toplevel;
  Widget    list;
  char      *texts;
  void      (* action_cb) ( void *, char *);
  void      *parent_ctx;
} *wow_tListCtx;

void wow_DisplayQuestion (
  void	    *ctx,
  Widget    father,
  char	    *title,
  char	    *text,
  void	    (* questionbox_ok) ( void *, void *),
  void	    (* questionbox_cancel) ( void *, void *),
  void	    *data
);

void wow_DisplayError (
  Widget    father,
  char	    *title,
  char	    *text
);

void wow_CreatePushButton (
  Widget    father,
  char	    *text,
  char	    *widget_name,
  void	    (callback)(),
  void	    *ctx
);

wow_tListCtx wow_CreateList (
  Widget    parent,
  char	    *title,
  char      *texts,
  void	    (action_cb)( void *, char *),
  void	    *ctx
);

void wow_CreateFileSelDia( Widget parent_wid,
			   char *title,
			   void *parent_ctx,
			   void (*file_selected_cb)(void *, char *, wow_eFileSelType),
			   wow_eFileSelType file_type);

void wow_GetLabel( Widget w, char *label);
void wow_GetCSText( XmString ar_value, char *t_buffer);
int wow_GetSelection( Widget w, char *str, int size, Atom atom);
void wow_GetAtoms( Widget w, Atom *graph_atom, Atom *objid_atom, Atom *attrref_atom);
int wow_DisplayWarranty( Widget father);
void wow_DisplayLicense( Widget father);

#if defined __cplusplus
}
#endif
#endif








