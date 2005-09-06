/* 
 * Proview   $Id: wb_dutl.c,v 1.3 2005-09-06 10:43:31 claes Exp $
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

/* wb_dutl.c -- dialog widget
   This module contains convenience routine for dialog widgets.  */

#include <string.h>
#include <Xm/Xm.h>
#include <Xm/MessageB.h>
#include "pwr.h"
#include "wb_dutl.h"

typedef struct {
  void (*callback)(void *callback_data, dutl_eDialogButton button);
  void *callback_data;
} dialog_cbs;
  
static void dialog_enum_to_motif (
  dutl_eDialogButton d_button,
  unsigned char *m_button,
  dutl_eDialogModality d_modality,
  unsigned char *m_modality,
  dutl_eDialogType d_type,
  unsigned char *m_type
);

static void message_dialog_cb (
  Widget dialog, 
  XtPointer data, 
  XmAnyCallbackStruct *cbs
);

/************************************************************************
*
* Name: dialog_enum_to_motif 
*
*************************************************************************/

static void dialog_enum_to_motif (
  dutl_eDialogButton d_button,
  unsigned char *m_button,
  dutl_eDialogModality d_modality,
  unsigned char *m_modality,
  dutl_eDialogType d_type,
  unsigned char *m_type
)
{

  if (m_button) {
    switch (d_button) {
    case dutl_eDialogButton_1 : *m_button = XmDIALOG_OK_BUTTON; 
      break;
    case dutl_eDialogButton_2 : *m_button = XmDIALOG_CANCEL_BUTTON; 
      break;
    case dutl_eDialogButton_3 : *m_button = XmDIALOG_HELP_BUTTON; 
      break;
    default: *m_button = XmDIALOG_OK_BUTTON; 
      break;
    }
  }



  if (m_modality) {
    switch (d_modality) {
    case dutl_eDialogModality_Modeless : *m_modality = XmDIALOG_MODELESS; 
      break;
    case dutl_eDialogModality_PrimaryApplication : 
      *m_modality = XmDIALOG_PRIMARY_APPLICATION_MODAL;
      break;
    case dutl_eDialogModality_FullApplication : 
      *m_modality = XmDIALOG_FULL_APPLICATION_MODAL; 
      break;
    case dutl_eDialogModality_System : 
      *m_modality = XmDIALOG_SYSTEM_MODAL; 
      break;
    default: *m_modality = XmDIALOG_MODELESS; 
      break;
    }
  }

  if (m_type) {
    switch (d_type) {
    case dutl_eDialogType_Error: *m_type = XmDIALOG_ERROR;
      break;   
    case dutl_eDialogType_Information: *m_type = XmDIALOG_INFORMATION;
      break;   
    case dutl_eDialogType_Message: *m_type = XmDIALOG_MESSAGE;
      break;   
    case dutl_eDialogType_Question: *m_type = XmDIALOG_QUESTION;
      break;   
    case dutl_eDialogType_Warning: *m_type = XmDIALOG_WARNING;
      break;   
    default: *m_type = XmDIALOG_INFORMATION;
      break;
    }
  }

  
} /* dialog_enum_to_motif */

/************************************************************************
*
* Name: dialog_enum_to_motif 
*
*************************************************************************/
static void message_dialog_cb (
  Widget dialog, 
  XtPointer data, 
  XmAnyCallbackStruct *cbs
)
{
  dialog_cbs *dcbs = (dialog_cbs *)data;
  dutl_eDialogButton button;
  
  switch (cbs->reason) {
  case XmCR_OK: button = dutl_eDialogButton_1;
    break;
  case XmCR_CANCEL: button = dutl_eDialogButton_2;
    break;
  case XmCR_HELP: button = dutl_eDialogButton_3;
    break;
  default: button = dutl_eDialogButton__;
    break;
  }

  if (dcbs->callback)
    (*dcbs->callback)(dcbs->callback_data, button);

  XtFree ((char *)dcbs);
  XtDestroyWidget (dialog);

} /* message_dialog_cb */ 


/************************************************************************
*
* Name: dutl_MessageDialog
*
*
* Description: Handles a three button message dialog. If no button name is 
*	       given then the button will be unmanged (invisible).
* 
*************************************************************************/
pwr_tStatus dutl_MessageDialog (
  void *ParentWindow,
  dutl_eDialogType Type,
  dutl_eDialogModality Modality,
  char *Title,
  char *Msg,
  char *Button1Label,
  char *Button2Label,
  char *Button3Label,
  dutl_eDialogButton DefaultButton,
  void (*ButtonCallback)(void *CallbackData, dutl_eDialogButton Button),
  void *CallbackData
)
{
  Widget dialog;
  unsigned char type, style, def_but;
  XmString title, msg, but1str, but2str, but3str;
  Arg  args[20];
  dialog_cbs *d_cbs;
  int n = 0;


  dialog_enum_to_motif (DefaultButton, &def_but, Modality, &style,
    Type, &type);     

  dialog = XmCreateMessageDialog ((Widget) ParentWindow, "dialog", NULL, 0);

  title = XmStringCreateLtoR(Title, XmSTRING_DEFAULT_CHARSET );
  msg = XmStringCreateLtoR(Msg, XmSTRING_DEFAULT_CHARSET );
  XtSetArg(args[n], XmNdefaultButtonType, def_but); n++;
  XtSetArg(args[n], XmNdialogStyle, style); n++;
  XtSetArg(args[n], XmNdialogTitle, title); n++;
  XtSetArg(args[n], XmNdialogType, type); n++;
  XtSetArg(args[n], XmNmessageString, msg); n++;


  if (Button1Label) {
    but1str = XmStringCreateLtoR(Button1Label, XmSTRING_DEFAULT_CHARSET );    
    XtSetArg(args[n], XmNokLabelString, but1str); n++;
  }
  else
    XtUnmanageChild(XmMessageBoxGetChild(dialog, XmDIALOG_OK_BUTTON));
  if (Button2Label) {
    but2str = XmStringCreateLtoR(Button2Label, XmSTRING_DEFAULT_CHARSET );    
    XtSetArg(args[n], XmNcancelLabelString, but2str); n++;
  }
  else
    XtUnmanageChild(XmMessageBoxGetChild(dialog, XmDIALOG_CANCEL_BUTTON));
  if (Button3Label) {
    but3str = XmStringCreateLtoR(Button3Label, XmSTRING_DEFAULT_CHARSET );    
    XtSetArg(args[n], XmNhelpLabelString, but3str); n++;
  }
  else
    XtUnmanageChild (XmMessageBoxGetChild(dialog, XmDIALOG_HELP_BUTTON));


  XtSetValues (dialog, args, n);


  d_cbs = (dialog_cbs*) XtMalloc (sizeof(dialog_cbs));
  d_cbs->callback = ButtonCallback;
  d_cbs->callback_data = CallbackData;

  XtAddCallback (dialog, XmNokCallback, (XtCallbackProc) message_dialog_cb, d_cbs);
  XtAddCallback (dialog, XmNcancelCallback, (XtCallbackProc) message_dialog_cb, d_cbs);
  XtAddCallback (dialog, XmNhelpCallback, (XtCallbackProc) message_dialog_cb, d_cbs);
  

  XmStringFree (title);    
  XmStringFree (msg);    
  if (Button1Label)
    XmStringFree (but1str);    
  if (Button2Label)
    XmStringFree (but2str);    
  if (Button3Label)
    XmStringFree (but3str);    


  XtManageChild (dialog);


  return 1;

} /* dutl_MessageDialog */



