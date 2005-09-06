/** 
 * Proview   $Id: co_wow.c,v 1.8 2005-09-06 14:17:10 claes Exp $
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

/* co_wow -- useful windows */

#if !defined OS_ELN

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <Xm/Xm.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <Xm/List.h>
#include <Xm/PushB.h>
#include <X11/Intrinsic.h>
#include <Xm/MessageB.h>
#include <Xm/MainW.h>
#include <Xm/FileSB.h>
#include <Xm/Form.h>
#include <X11/Xatom.h>
#include <X11/Xmu/Atoms.h>
#include <X11/Xmu/StdSel.h>

#include "pwr.h"
#include "co_dcli.h"
#include "co_wow.h"
#include "co_api.h"
#include "flow_x.h"

#define WOW_MAXNAMES 400

typedef struct {
  void			*ctx;
  void			*data;
  void	         	(* questionbox_ok) ();
  void	         	(* questionbox_cancel) ();
  void	         	(* questionbox_help) ();
} wow_t_question_cb;

typedef struct {
  char		        str[200];
  int 			len;
  int			received;
  pwr_tStatus		sts;
  Atom			atom;
} wow_sSelection;

static void wow_question_ok_cb (
  Widget dialog, 
  XtPointer data, 
  XmAnyCallbackStruct *cbs
);

static void wow_question_cancel_cb (
  Widget dialog, 
  XtPointer data, 
  XmAnyCallbackStruct *cbs
);

static void wow_error_ok_cb (Widget w);

static void wow_error_ok_cb (Widget w)
{
    XtDestroyWidget( w);
}

static void wow_question_ok_cb (
  Widget dialog, 
  XtPointer data, 
  XmAnyCallbackStruct *cbs
)
{
  wow_t_question_cb *cbdata = (wow_t_question_cb *) data;
  
  if (cbdata->questionbox_ok)
    (cbdata->questionbox_ok)( cbdata->ctx, cbdata->data);

  XtFree ((char *)cbdata);
  XtDestroyWidget (dialog);
}

static void wow_question_cancel_cb (
  Widget dialog, 
  XtPointer data, 
  XmAnyCallbackStruct *cbs
)
{
  wow_t_question_cb *cbdata = (wow_t_question_cb *) data;
  
  if (cbdata->questionbox_cancel)
    (cbdata->questionbox_cancel)( cbdata->ctx, cbdata->data);

  XtFree ((char *)cbdata);
  XtDestroyWidget (dialog);
}

static void wow_question_help_cb (
  Widget dialog, 
  XtPointer data, 
  XmAnyCallbackStruct *cbs
)
{
  wow_t_question_cb *cbdata = (wow_t_question_cb *) data;
  
  if (cbdata->questionbox_help)
    (cbdata->questionbox_help)( cbdata->ctx, cbdata->data);
}


/************************************************************************
*
* Name: wow_DisplayQuestion		
*
* Description:	Displays an question box widget 
*
*************************************************************************/

void wow_DisplayQuestion (
void	    *ctx,
Widget	    father,
char	    *title,
char	    *text,
void	    (* questionbox_ok) (),
void	    (* questionbox_cancel) (),
void	    *data
)
{
    Arg	    arg[10];
    Widget  question_widget, w;
    XmString CStr2, TitleStr, okstr, cancelstr;
    wow_t_question_cb *cbdata;
    XmFontList fontlist;
    XFontStruct *font;
    XmFontListEntry fontentry;

    // Set default fontlist
    font = XLoadQueryFont( XtDisplay(father),
	      "-*-Helvetica-Bold-R-Normal--12-*-*-*-P-*-ISO8859-1");
    fontentry = XmFontListEntryCreate( "tag1", XmFONT_IS_FONT, font);
    fontlist = XmFontListAppendEntry( NULL, fontentry);
    XtFree( (char *)fontentry);

    CStr2 = XmStringCreateLtoR( text, XmSTRING_DEFAULT_CHARSET);
    TitleStr = XmStringCreateLtoR( title, XmSTRING_DEFAULT_CHARSET);    
    okstr = XmStringCreateLtoR( " Yes ", XmSTRING_DEFAULT_CHARSET );    
    cancelstr = XmStringCreateLtoR( " No  ", XmSTRING_DEFAULT_CHARSET );    
    XtSetArg(arg[0],XmNheight,75);
    XtSetArg(arg[1],XmNwidth,200);
    XtSetArg(arg[2],XmNmessageString, CStr2);
    XtSetArg(arg[3],XmNx,400);
    XtSetArg(arg[4],XmNy,300);
    XtSetArg(arg[5],XmNdialogTitle,TitleStr);
    XtSetArg(arg[6], XmNokLabelString, okstr);
    XtSetArg(arg[7], XmNcancelLabelString, cancelstr);
    XtSetArg(arg[8], XmNbuttonFontList, fontlist);
    XtSetArg(arg[9], XmNlabelFontList, fontlist);

    cbdata = (wow_t_question_cb *) XtCalloc( 1, sizeof(*cbdata));
    cbdata->questionbox_ok = questionbox_ok;
    cbdata->questionbox_cancel = questionbox_cancel;
    cbdata->questionbox_help = 0;
    cbdata->ctx = ctx;
    cbdata->data = data;

    question_widget = XmCreateQuestionDialog( father,"questionDialog",arg,10);
    XtAddCallback( question_widget, XmNokCallback,
		(XtCallbackProc) wow_question_ok_cb, cbdata);
    XtAddCallback( question_widget, XmNcancelCallback, 
		(XtCallbackProc) wow_question_cancel_cb, cbdata);

    XmStringFree( CStr2);
    XmStringFree( TitleStr);
    XmStringFree( okstr);
    XmStringFree( cancelstr);
    XmFontListFree( fontlist);
    
    XtManageChild( question_widget);	       
    
    w = XmMessageBoxGetChild( question_widget, XmDIALOG_HELP_BUTTON);
    XtUnmanageChild(w);    

}


/************************************************************************
*
* Name: DisplayError(Father,Text)		
*
* Type:	void	
*
* Widget	Father		I	The widget that is the father of
*					the message box
* char		*Text		I	Message text
*
* Description:	Displays an error message box widget 
*
*************************************************************************/

void wow_DisplayError (
Widget	    father,
char	    *title,
char	    *text
)
{
    Arg	    arg[10];
    Widget  err_widget, w;
    XmString cstr, ctitle;
    XmFontList fontlist;
    XFontStruct *font;
    XmFontListEntry fontentry;

    // Set default fontlist
    font = XLoadQueryFont( XtDisplay(father),
	      "-*-Helvetica-Bold-R-Normal--12-*-*-*-P-*-ISO8859-1");
    fontentry = XmFontListEntryCreate( "tag1", XmFONT_IS_FONT, font);
    fontlist = XmFontListAppendEntry( NULL, fontentry);
    XtFree( (char *)fontentry);

    cstr = XmStringCreateLtoR( text, XmSTRING_DEFAULT_CHARSET);
    ctitle = XmStringCreateLtoR( title, XmSTRING_DEFAULT_CHARSET);    
    XtSetArg(arg[0],XmNheight,75);
    XtSetArg(arg[1],XmNwidth,200);
    XtSetArg(arg[2],XmNmessageString, cstr);
    XtSetArg(arg[3],XmNx,400);
    XtSetArg(arg[4],XmNy,300);
    XtSetArg(arg[5],XmNdialogTitle, ctitle);
    XtSetArg(arg[6], XmNbuttonFontList, fontlist);
    XtSetArg(arg[7], XmNlabelFontList, fontlist);

    err_widget = XmCreateErrorDialog( father,"err_widget",arg,8);
    XtAddCallback(err_widget, XmNokCallback, 
		(XtCallbackProc) wow_error_ok_cb, NULL);

    XmStringFree( cstr);
    XmStringFree( ctitle);
    XmFontListFree( fontlist);
      
    XtManageChild(err_widget);	       
    
    w = XmMessageBoxGetChild(err_widget, XmDIALOG_CANCEL_BUTTON);
    XtUnmanageChild( w);    
    
    w = XmMessageBoxGetChild(err_widget, XmDIALOG_HELP_BUTTON);
    XtUnmanageChild( w);    

} /* END DisplayErrorBox */

/************************************************************************
*
* Name: CreatePushButton(Father,Text,Callback)
*
* Type:	void	
*
* Widget	Father		I	The widget that is the father of
*					the message box
* char		*Text		I	Message text
*
* Description: Create a pushbutton 
*
*************************************************************************/

void wow_CreatePushButton (
Widget	    father,
char	    *text,
char	    *widget_name,
void	    (callback)(),
void	    *ctx
)
{
    Arg	    arg[10];
    Widget  widget;
    XmString cstr;


    cstr = XmStringCreateLtoR( text, XmSTRING_DEFAULT_CHARSET);
    XtSetArg(arg[0],XmNlabelString, cstr);

    widget = XmCreatePushButton( father,widget_name,arg,1);
    XtAddCallback( widget, XmNactivateCallback,
		(XtCallbackProc) callback, ctx);

    XmStringFree( cstr);
    
    XtManageChild(widget);	       

} /* END CreatePushButton */


/************************************************************************
*
* Description: Create a window with a scrolled list and Ok and Cancel
*              buttons.
*
*************************************************************************/

static void wow_list_ok_cb (
  Widget w, 
  XtPointer data, 
  XmAnyCallbackStruct *cbs
)
{
  wow_tListCtx ctx = (wow_tListCtx) data;
  int 		*pos_list, pos_cnt;
  static char   selected_text[80];
  
  if ( ctx->action_cb) {
    if (XmListGetSelectedPos( ctx->list, &pos_list, &pos_cnt)) {
      strcpy( selected_text, ctx->texts + (pos_list[0] - 1) * 80);
      (ctx->action_cb)( ctx->parent_ctx, selected_text);

      XtFree( (char *)pos_list);
    }
  }

  XtDestroyWidget( ctx->toplevel);
  free( ctx->texts);
  free( ctx);
}

static void wow_list_cancel_cb (
  Widget w, 
  XtPointer data, 
  XmAnyCallbackStruct *cbs
)
{
  wow_tListCtx ctx = (wow_tListCtx) data;
  
  XtDestroyWidget( ctx->toplevel);
  free( ctx->texts);
  free( ctx);
}

static void wow_list_action_cb (
  Widget w,
  XtPointer data,
  XmListCallbackStruct *cbs)
{
  wow_tListCtx ctx = (wow_tListCtx) data;
  char          *item_str;
  static char   action_text[80];
  
  if ( cbs->event->type == KeyPress)
    // The ok callback will be called later
    return;

  if ( ctx->action_cb) {
    XmStringGetLtoR( cbs->item, XmSTRING_DEFAULT_CHARSET, &item_str);
    strcpy( action_text, item_str);

    XtFree( item_str);
    (ctx->action_cb)( ctx->parent_ctx, action_text);
  }

  XtDestroyWidget( ctx->toplevel);
  free( ctx->texts);
  free( ctx);
}

wow_tListCtx wow_CreateList (
  Widget    parent,
  char	    *title,
  char      *texts,
  void	    (action_cb)( void *, char *),
  void	    *parent_ctx
)
{
  Arg	    args[15];
  XmString cstr;
  Widget mainwindow;
  Widget ok_button;
  Widget cancel_button;
  Widget form;
  char *name_p;
  int i;
  wow_tListCtx ctx;
  XmFontList fontlist;
  XFontStruct *font;
  XmFontListEntry fontentry;

  ctx = (wow_tListCtx) calloc( 1, sizeof(*ctx));
  ctx->action_cb = action_cb;
  ctx->parent_ctx = parent_ctx;
  
  i=0;
  XtSetArg( args[i], XmNiconName, title); i++;

  ctx->toplevel = XtCreatePopupShell (
        title, topLevelShellWidgetClass, parent, args, i);

  // Set default fontlist
  font = XLoadQueryFont( XtDisplay(ctx->toplevel),
	      "-*-Helvetica-Bold-R-Normal--12-*-*-*-P-*-ISO8859-1");
  fontentry = XmFontListEntryCreate( "tag1", XmFONT_IS_FONT, font);
  fontlist = XmFontListAppendEntry( NULL, fontentry);
  XtFree( (char *)fontentry);

  i=0;
  XtSetArg( args[i], XmNbuttonFontList, fontlist); i++;
  XtSetArg( args[i], XtNallowShellResize, TRUE); i++;
  XtSetValues( ctx->toplevel, args, i);

  mainwindow = XmCreateMainWindow( ctx->toplevel, "mainWindow", NULL, 0);
  XtManageChild( mainwindow);

  i=0;
  XtSetArg(args[i],XmNwidth, 200);i++;
  XtSetArg(args[i],XmNheight, 400);i++;
  XtSetArg(args[i],XmNresizePolicy,XmRESIZE_NONE); i++;

  form = XmCreateForm( mainwindow, "form", args, i);
  XtManageChild( form);

  cstr = XmStringCreateLtoR( "Ok", XmSTRING_DEFAULT_CHARSET);

  i=0;
  XtSetArg( args[i], XmNbottomAttachment, XmATTACH_FORM); i++;
  XtSetArg( args[i], XmNbottomOffset, 20); i++;
  XtSetArg( args[i], XmNleftAttachment, XmATTACH_FORM); i++;
  XtSetArg( args[i], XmNleftOffset, 20); i++;
  XtSetArg( args[i], XmNwidth, 50); i++;
  XtSetArg( args[i], XmNlabelString, cstr); i++;

  ok_button = XmCreatePushButton( form, "okButton", args, i);
  XtAddCallback( ok_button, XmNactivateCallback,
		(XtCallbackProc) wow_list_ok_cb, ctx);
  XtManageChild( ok_button);

  XmStringFree( cstr);

  cstr = XmStringCreateLtoR( "Cancel", XmSTRING_DEFAULT_CHARSET);

  i=0;
  XtSetArg( args[i], XmNbottomAttachment, XmATTACH_FORM); i++;
  XtSetArg( args[i], XmNbottomOffset, 20); i++;
  XtSetArg( args[i], XmNrightAttachment, XmATTACH_FORM); i++;
  XtSetArg( args[i], XmNrightOffset, 20); i++;
  XtSetArg( args[i], XmNwidth, 50); i++;
  XtSetArg( args[i], XmNlabelString, cstr); i++;

  cancel_button = XmCreatePushButton( form, "okButton", args, i);
  XtAddCallback( cancel_button, XmNactivateCallback,
		(XtCallbackProc) wow_list_cancel_cb, ctx);
  XtManageChild( cancel_button);

  XmStringFree( cstr);

  i = 0;
  XtSetArg( args[i], XmNdefaultButton, ok_button); i++;
  XtSetArg( args[i], XmNcancelButton, cancel_button); i++;
  XtSetValues( form, args, i);

  i=0;
  XtSetArg( args[i], XmNbottomAttachment, XmATTACH_WIDGET); i++;
  XtSetArg( args[i], XmNbottomWidget, ok_button); i++;
  XtSetArg( args[i], XmNbottomOffset, 15); i++;
  XtSetArg( args[i], XmNrightAttachment, XmATTACH_FORM); i++;
  XtSetArg( args[i], XmNrightOffset, 15); i++;
  XtSetArg( args[i], XmNtopAttachment, XmATTACH_FORM); i++;
  XtSetArg( args[i], XmNtopOffset, 15); i++;
  XtSetArg( args[i], XmNleftAttachment, XmATTACH_FORM); i++;
  XtSetArg( args[i], XmNleftOffset, 15); i++;
  XtSetArg( args[i], XmNselectionPolicy, XmSINGLE_SELECT); i++;
  XtSetArg( args[i], XmNfontList, fontlist); i++;
  ctx->list = XmCreateScrolledList( form, "scrolledList", args, i);
  XtAddCallback( ctx->list, XmNdefaultActionCallback,
		(XtCallbackProc) wow_list_action_cb, ctx);

  XmFontListFree( fontlist);
  XtManageChild( ctx->list);

  name_p = texts;
  i = 0;
  while ( strcmp( name_p, "") != 0) {
    cstr = XmStringCreateSimple( name_p);
    XmListAddItemUnselected( ctx->list, cstr, 0);
    XmStringFree(cstr);	  
    name_p += 80;
    i++;
  }

  ctx->texts = calloc( i+1, 80);
  memcpy( ctx->texts, texts, (i+1) * 80); 
  XtPopup( ctx->toplevel, XtGrabNone);

  // Set input focus to the scrolled list widget
  XmProcessTraversal( ctx->list, XmTRAVERSE_CURRENT);


  return ctx;
}


void wow_file_ok_cb( Widget widget, XtPointer udata, XtPointer data)
{
  XmFileSelectionBoxCallbackStruct *cbs = (XmFileSelectionBoxCallbackStruct *) data;
  wow_tFileSelCtx ctx = (wow_tFileSelCtx) udata;
  char *filename;

  if ( !XmStringGetLtoR( cbs->value, XmFONTLIST_DEFAULT_TAG, &filename))
    return;

  if ( !filename)
    return;
  
  if ( ctx->file_selected_cb)
    (ctx->file_selected_cb)( ctx->parent_ctx, filename, ctx->file_type);
  free( (char *)ctx);
  XtDestroyWidget( widget);
  XtFree( filename);
}

void wow_file_cancel_cb( Widget widget, XtPointer udata, XtPointer data)
{
  free( (char *)udata);
  XtDestroyWidget( widget);
}

void wow_file_search_cb( Widget widget, XtPointer data)
{
  XmFileSelectionBoxCallbackStruct *cbs = (XmFileSelectionBoxCallbackStruct *) data;
  XmString names[WOW_MAXNAMES];
  char *mask;
  char found_file[200];
  int sts;
  int file_cnt;
  int i;

  if ( !XmStringGetLtoR( cbs->mask, XmFONTLIST_DEFAULT_TAG, &mask))
    return;

  file_cnt = 0;
  sts = dcli_search_file( mask, found_file, DCLI_DIR_SEARCH_INIT);
  while( ODD(sts)) {
    if ( file_cnt >= WOW_MAXNAMES)
      break;
    names[file_cnt++] = XmStringCreateLocalized( found_file);
    sts = dcli_search_file( mask, found_file, DCLI_DIR_SEARCH_NEXT);
  }
  sts = dcli_search_file( mask, found_file, DCLI_DIR_SEARCH_END);
  XtFree( mask);

  if ( file_cnt) {
    XtVaSetValues( widget, 
		   XmNfileListItems, names,
		   XmNfileListItemCount, file_cnt,
		   XmNdirSpec, names[0],
		   XmNlistUpdated, True,
		   NULL);
    for ( i = 0; i < file_cnt; i++)
      XmStringFree( names[i]);
  }
  else
    XtVaSetValues( widget,
		   XmNfileListItems, NULL,
		   XmNfileListItemCount, 0,
		   XmNlistUpdated, True,
		   NULL);
}

void wow_CreateFileSelDia( Widget parent_wid,
			   char *title,
			   void *parent_ctx,
			   void (*file_selected_cb)(void *, char *, wow_eFileSelType),
			   wow_eFileSelType file_type)
{
  Arg args[10];
  XmString ctitle, cdirectory, cpattern;
  char directory[200];
  int i;
  wow_tFileSelCtx ctx;
  Widget help;

  ctx = (wow_tFileSelCtx) calloc( 1, sizeof(*ctx));
  ctx->file_selected_cb = file_selected_cb;
  ctx->parent_ctx = parent_ctx;
  ctx->file_type = file_type;

  ctitle = XmStringCreateLtoR( title, XmSTRING_DEFAULT_CHARSET);    

  i = 0;
  XtSetArg( args[i], XmNdialogTitle, ctitle); i++;
  XtSetArg( args[i], XmNfileTypeMask, XmFILE_REGULAR); i++;
  XtSetArg( args[i], XmNfileSearchProc, wow_file_search_cb); i++;

  switch( file_type) {
    case wow_eFileSelType_All:
      break;
    case wow_eFileSelType_Dbs:
      dcli_translate_filename( directory, "$pwrp_load/");
      cdirectory = XmStringCreateLtoR( directory, XmSTRING_DEFAULT_CHARSET);
      cpattern = XmStringCreateLtoR( "*.dbs", XmSTRING_DEFAULT_CHARSET);
      XtSetArg( args[i], XmNdirectory, cdirectory); i++;
      XtSetArg( args[i], XmNpattern, cpattern); i++;
      break;
    case wow_eFileSelType_Wbl:
    case wow_eFileSelType_WblClass:
      dcli_translate_filename( directory, "$pwrp_db/");
      cdirectory = XmStringCreateLtoR( directory, XmSTRING_DEFAULT_CHARSET);
      cpattern = XmStringCreateLtoR( "*.wb_load", XmSTRING_DEFAULT_CHARSET);
      XtSetArg( args[i], XmNdirectory, cdirectory); i++;
      XtSetArg( args[i], XmNpattern, cpattern); i++;
      break;
  }

  ctx->dialog = XmCreateFileSelectionDialog( parent_wid, "fileseldia", args, i);
  XtAddCallback( ctx->dialog, XmNokCallback, wow_file_ok_cb, ctx);
  XtAddCallback( ctx->dialog, XmNcancelCallback, wow_file_cancel_cb, ctx);

  help = XmFileSelectionBoxGetChild( ctx->dialog, XmDIALOG_HELP_BUTTON);
  XtUnmanageChild( help);
  XtManageChild( ctx->dialog);

  XmStringFree( ctitle);
  XmStringFree( cdirectory);
  XmStringFree( cpattern);
}

void wow_GetLabel( Widget w, char *label)
{
  Arg			arg[1];
  XmString 		latinstr;

  XtSetArg(arg[0], XmNlabelString, &latinstr);
  XtGetValues( w, arg, 1);
  wow_GetCSText( latinstr, label);
  XmStringFree( latinstr);
}

void wow_GetCSText( XmString ar_value, char *t_buffer)
{
    char			*first_seg;
    XmStringContext		context;
    XmStringCharSet 		charset;
    XmStringDirection 		dir_r_to_l;
    Boolean 			sep;

    if (ar_value == 0)
    {
	strcpy(t_buffer,"");
	return;
    }
    
    
    XmStringInitContext( &context, ar_value);
    
    if ( XmStringGetNextSegment( context, &first_seg, &charset, &dir_r_to_l,
			&sep) != TRUE )
    {
      *first_seg = 0;
    }
    else
    {
      strcpy(t_buffer,first_seg);
      XtFree(first_seg);
#if 0
    /* I don't think that this should be freed, ML 950120 */
      XtFree (charset);
#endif
    }
    XmStringFreeContext (context);

}
#endif

static void wow_get_selection_cb( Widget w, XtPointer clientdata, Atom *selection,
				   Atom *type, XtPointer value, unsigned long *len,
				   int *format)
{
  wow_sSelection *data = (wow_sSelection *)clientdata;

  if ( *len != 0 && value != NULL) {
    if ( *type == data->atom) {
      if ( *len > sizeof(data->str) - 1) {
	data->sts = 0;
	return;
      }
      strncpy( data->str, (char *)value, *len);
      data->str[*len] = 0;
      data->len = *len;
      data->sts = 1;
    }
    else
      data->sts = 0;
  }
  else
    data->sts = 0;
  XtFree( (char *)value);
  data->received = 1;
}

int wow_GetSelection( Widget w, char *str, int size, Atom atom)
{
  wow_sSelection data;

  data.received = 0;
  data.atom = atom;
  XtGetSelectionValue( w, XA_PRIMARY, atom,
		       wow_get_selection_cb, &data, CurrentTime);
  
  while( !data.received) {
    XEvent e;
    XtAppNextEvent( XtWidgetToApplicationContext(w), &e);
    XtDispatchEvent( &e);
  }
  if ( data.sts && data.len < size)
    strcpy( str, data.str);
  return data.sts;
}

void wow_GetAtoms( Widget w, Atom *graph_atom, Atom *objid_atom, Atom *attrref_atom)
{
  if ( graph_atom)
    *graph_atom = XInternAtom( flow_Display(w), "PWR_GRAPH", False); 
  if ( objid_atom)
    *objid_atom = XInternAtom( flow_Display(w), "PWR_OBJID", False); 
  if ( attrref_atom)
    *attrref_atom = XInternAtom( flow_Display(w), "PWR_ATTRREF", False); 
}

void warranty_ok_cb( void *ctx, void *data)
{
}

void warranty_cancel_cb( void *ctx, void *data)
{
  exit(0);
}

void warranty_help_cb( void *ctx, void *data)
{
  wow_DisplayLicense( (Widget) data);
}

int wow_DisplayWarranty( Widget father)
{
    char    text[4000];
    Arg	    arg[12];
    Widget  question_widget;
    XmString CStr2, TitleStr, okstr, cancelstr, helpstr;
    wow_t_question_cb *cbdata;
    XmFontList fontlist;
    XFontStruct *font;
    XmFontListEntry fontentry;
    char 	title[80];
    FILE 	*fp;
    int 	i;
    char 	fname[256];

    sprintf( fname, "$pwr_exe/%s/acceptlicense.txt", lng_get_language_str());
    dcli_translate_filename( fname, fname);

    fp = fopen( fname, "r");
    if ( !fp) {
      strcpy( fname, "$pwr_exe/en_us/acceptlicense.txt");
      dcli_translate_filename( fname, fname);
      fp = fopen( fname, "r");
      if ( !fp) return 1;
    }

    for ( i = 0; i < sizeof(text) - 1; i++) {
      text[i] = fgetc( fp);
      if ( text[i] == EOF)
	break;
    }
    text[i] = 0;
    fclose( fp);

    strcpy( title, lng_translate("Accept License Terms"));

    // Set default fontlist
    font = XLoadQueryFont( XtDisplay(father),
	      "-*-Helvetica-Bold-R-Normal--12-*-*-*-P-*-ISO8859-1");
    fontentry = XmFontListEntryCreate( "tag1", XmFONT_IS_FONT, font);
    fontlist = XmFontListAppendEntry( NULL, fontentry);
    XtFree( (char *)fontentry);
    
    CStr2 = XmStringCreateLtoR( text, XmSTRING_DEFAULT_CHARSET);
    TitleStr = XmStringCreateLtoR( title, XmSTRING_DEFAULT_CHARSET);    
    okstr = XmStringCreateLtoR( lng_translate( "I Accept"), XmSTRING_DEFAULT_CHARSET );    
    cancelstr = XmStringCreateLtoR( lng_translate( "Quit"), XmSTRING_DEFAULT_CHARSET );    
    helpstr = XmStringCreateLtoR( lng_translate( "Show License"), XmSTRING_DEFAULT_CHARSET );    
    XtSetArg(arg[0],XmNheight,75);
    XtSetArg(arg[1],XmNwidth,700);
    XtSetArg(arg[2],XmNmessageString, CStr2);
    XtSetArg(arg[3],XmNx,400);
    XtSetArg(arg[4],XmNy,300);
    XtSetArg(arg[5],XmNdialogTitle,TitleStr);
    XtSetArg(arg[6], XmNokLabelString, okstr);
    XtSetArg(arg[7], XmNcancelLabelString, cancelstr);
    XtSetArg(arg[8], XmNhelpLabelString, helpstr);
    XtSetArg(arg[9], XmNbuttonFontList, fontlist);
    XtSetArg(arg[10], XmNlabelFontList, fontlist);
    XtSetArg(arg[11], XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL);

    cbdata = (wow_t_question_cb *) XtCalloc( 1, sizeof(*cbdata));
    cbdata->questionbox_ok = warranty_ok_cb;
    cbdata->questionbox_cancel = warranty_cancel_cb;
    cbdata->questionbox_help = warranty_help_cb;
    cbdata->ctx = 0;
    cbdata->data = (void *)father;

    question_widget = XmCreateMessageDialog( father,"questionDialog",arg, 12);
    XtAddCallback( question_widget, XmNokCallback,
		(XtCallbackProc) wow_question_ok_cb, cbdata);
    XtAddCallback( question_widget, XmNcancelCallback, 
		(XtCallbackProc) wow_question_cancel_cb, cbdata);
    XtAddCallback( question_widget, XmNhelpCallback, 
		(XtCallbackProc) wow_question_help_cb, cbdata);

    XmStringFree( CStr2);
    XmStringFree( TitleStr);
    XmStringFree( okstr);
    XmStringFree( cancelstr);
    XmStringFree( helpstr);
    XmFontListFree( fontlist);
   
    XtManageChild( question_widget);	       
    return 1;
}

void wow_DisplayLicense( Widget father)
{
    char text[20000];
    Arg	    arg[11];
    Widget  question_widget;
    XmString CStr2, TitleStr, cancelstr;
    XmFontList fontlist;
    XFontStruct *font;
    XmFontListEntry fontentry;
    char title[80];
    Widget w;
    FILE *fp;
    char fname[200];
    int i;
    Widget wcancel;

    strcpy( title, lng_translate("License"));

    sprintf( fname, "$pwr_exe/%s/license.txt", lng_get_language_str());
    dcli_translate_filename( fname, fname);

    fp = fopen( fname, "r");
    if ( !fp) {
      strcpy( fname, "$pwr_exe/en_us/lincense.txt");
      dcli_translate_filename( fname, fname);
      fp = fopen( fname, "r");
      if ( !fp)
	return;
    }

    for ( i = 0; i < sizeof(text) - 1; i++) {
      text[i] = fgetc( fp);
      if ( text[i] == EOF)
	break;
    }
    fclose( fp);
    text[i] = 0;

    // Set default fontlist
    font = XLoadQueryFont( XtDisplay(father),
	      "-*-Helvetica-Bold-R-Normal--12-*-*-*-P-*-ISO8859-1");
    fontentry = XmFontListEntryCreate( "tag1", XmFONT_IS_FONT, font);
    fontlist = XmFontListAppendEntry( NULL, fontentry);
    XtFree( (char *)fontentry);

    CStr2 = XmStringCreateLtoR( "", XmSTRING_DEFAULT_CHARSET);
    TitleStr = XmStringCreateLtoR( title, XmSTRING_DEFAULT_CHARSET);    
    cancelstr = XmStringCreateLtoR( " Close ", XmSTRING_DEFAULT_CHARSET );    
    XtSetArg(arg[0],XmNheight,400);
    XtSetArg(arg[1],XmNwidth,600);
    XtSetArg(arg[2],XmNmessageString, CStr2);
    XtSetArg(arg[3],XmNx,400);
    XtSetArg(arg[4],XmNy,300);
    XtSetArg(arg[5],XmNdialogTitle,TitleStr);
    XtSetArg(arg[6], XmNcancelLabelString, cancelstr);
    XtSetArg(arg[7], XmNbuttonFontList, fontlist);
    XtSetArg(arg[8], XmNlabelFontList, fontlist);

    question_widget = XmCreateMessageDialog( father,"questionDialog",arg,9);
    XmStringFree( CStr2);
    XmStringFree( TitleStr);
    XmStringFree( cancelstr);
    XmFontListFree( fontlist);
    wcancel = XmMessageBoxGetChild(question_widget, XmDIALOG_CANCEL_BUTTON);
   

    XtSetArg(arg[0], XmNscrollHorizontal, True);
    XtSetArg(arg[1], XmNscrollVertical, True);
    XtSetArg(arg[2], XmNeditMode, XmMULTI_LINE_EDIT);
    XtSetArg(arg[3], XmNeditable, False);
    XtSetArg(arg[4], XmNcursorPositionVisible, False);
    XtSetArg(arg[5], XmNrows, 30);
    XtSetArg(arg[6], XmNvalue, text);
    XtSetArg(arg[7], XmNfontList, fontlist);
    w = XmCreateScrolledText( question_widget, "text", arg, 7);
    XtVaSetValues( XtParent(w), 
		   XmNleftAttachment, XmATTACH_FORM,
		   XmNrightAttachment, XmATTACH_FORM,
		   XmNtopAttachment, XmATTACH_FORM,
		   XmNbottomAttachment, XmATTACH_WIDGET,
		   XmNbottomWidget, wcancel,
		   NULL);
    XtManageChild(w);

    w = XmMessageBoxGetChild(question_widget, XmDIALOG_OK_BUTTON);
    XtUnmanageChild( w);    
    
    w = XmMessageBoxGetChild(question_widget, XmDIALOG_HELP_BUTTON);
    XtUnmanageChild( w);    

    XtManageChild( question_widget);	       
}




















