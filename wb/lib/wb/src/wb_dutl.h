#ifndef wb_dutl_h
#define wb_dutl_h

/* wb_dutl.h -- dialog widget

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include "pwr_class.h"

typedef enum {
  dutl_eDialogButton__ ,
  dutl_eDialogButton_1,
  dutl_eDialogButton_2,
  dutl_eDialogButton_3,
  dutl_eDialogButton_
} dutl_eDialogButton;


typedef enum {
  dutl_eDialogModality__ ,
  dutl_eDialogModality_Modeless,
  dutl_eDialogModality_PrimaryApplication,
  dutl_eDialogModality_FullApplication,
  dutl_eDialogModality_System,
  dutl_eDialogModality_
} dutl_eDialogModality;


typedef enum {
  dutl_eDialogType__ = 0,
  dutl_eDialogType_Error,
  dutl_eDialogType_Information,
  dutl_eDialogType_Message,
  dutl_eDialogType_Question,
  dutl_eDialogType_Warning,
  dutl_eDialogType_
} dutl_eDialogType;

/************************************************************************
*
* Name: dutl_MessageDialog
*
*
* Description: Handles a three button message dialog. If no button name is 
*	       given then the button will be unmanged (invisible).
* 
*              ParentWindow must be a widget.
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
);

#endif
