/* co_mrm_util.c -- Motif utitlities

   PROVIEW/R  
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if !defined OS_ELN

#include <stdio.h>
#include <stdlib.h>


#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/Text.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#define XK_MISCELLANY
#include <X11/keysymdef.h>

#if defined OS_VMS
#include <Xm/DECWmHints.h>
#endif

#include "flow_x.h"
#include "co_mrm_util.h"

//
// Init motif text widget with recall.
//
void mrm_TextInit( Widget w, XtActionProc value_changed_cb,
	mrm_eUtility utility)
{ 
  // Wtt, Xtt and Admin will not appear in the same executable ?
  switch ( utility)
  {
    case mrm_eUtility_Wtt:
    case mrm_eUtility_Xtt:
    case mrm_eUtility_Admin:
    {
      static XtTranslations compiled_translations_wtt = NULL;
      static char translations_wtt[] = "<KeyDown>: key_down_wtt()";
      static XtActionsRec actions_wtt[1] =
      {
        {"key_down_wtt",      0}
      };
      actions_wtt[0].proc = value_changed_cb;

      if (compiled_translations_wtt == NULL) 
        XtAppAddActions( XtWidgetToApplicationContext(w), 
		actions_wtt, XtNumber(actions_wtt));

      if (compiled_translations_wtt == NULL) 
        compiled_translations_wtt = XtParseTranslationTable(translations_wtt);
      XtOverrideTranslations( w, compiled_translations_wtt);
      break;
    }
    case mrm_eUtility_Ge:
    {
      static XtTranslations compiled_translations_ge = NULL;
      static char translations_ge[] = "<KeyDown>: key_down_ge()";
      static XtActionsRec actions_ge[1] =
      {
        {"key_down_ge",      0}
      };
      actions_ge[0].proc = value_changed_cb;
  
      if (compiled_translations_ge == NULL) 
        XtAppAddActions( XtWidgetToApplicationContext(w), 
	    actions_ge, XtNumber(actions_ge));

      if (compiled_translations_ge == NULL) 
        compiled_translations_ge = XtParseTranslationTable(translations_ge);
      XtOverrideTranslations( w, compiled_translations_ge);
      break;
    }
    case mrm_eUtility_WAtt:
    {
      static XtTranslations compiled_translations_watt = NULL;
      static char translations_watt[] = "<KeyDown>: key_down_watt()";
      static XtActionsRec actions_watt[1] =
      {
        {"key_down_watt",      0}
      };
      actions_watt[0].proc = value_changed_cb;
  
      if (compiled_translations_watt == NULL) 
        XtAppAddActions( XtWidgetToApplicationContext(w), 
	    actions_watt, XtNumber(actions_watt));

      if (compiled_translations_watt == NULL) 
        compiled_translations_watt = XtParseTranslationTable(translations_watt);
      XtOverrideTranslations( w, compiled_translations_watt);
      break;
    }
    case mrm_eUtility_Wda:
    {
      static XtTranslations compiled_translations_wda = NULL;
      static char translations_wda[] = "<KeyDown>: key_down_wda()";
      static XtActionsRec actions_wda[1] =
      {
        {"key_down_wda",      0}
      };
      actions_wda[0].proc = value_changed_cb;
  
      if (compiled_translations_wda == NULL) 
        XtAppAddActions( XtWidgetToApplicationContext(w), 
	    actions_wda, XtNumber(actions_wda));

      if (compiled_translations_wda == NULL) 
        compiled_translations_wda = XtParseTranslationTable(translations_wda);
      XtOverrideTranslations( w, compiled_translations_wda);
      break;
    }
    case mrm_eUtility_Attr:
    {
      static XtTranslations compiled_translations_attr = NULL;
      static char translations_attr[] = "<KeyDown>: key_down_attr()";
      static XtActionsRec actions_attr[1] =
      {
        {"key_down_attr",      0}
      };
      actions_attr[0].proc = value_changed_cb;
  
      if (compiled_translations_attr == NULL) 
        XtAppAddActions( XtWidgetToApplicationContext(w), 
	    actions_attr, XtNumber(actions_attr));

      if (compiled_translations_attr == NULL) 
        compiled_translations_attr = XtParseTranslationTable(translations_attr);
      XtOverrideTranslations( w, compiled_translations_attr);
      break;
    }
    default:
      ;
  }
}

//
// Input from motif text-widget with recall
// Returns 1 if return i pressed, else 0.
//
int mrm_TextInput( Widget w, XEvent *event, char *recall, int line_size, 
	int recall_size, int *current_recall_line)
{
  KeySym keysym;
  Modifiers mod;
  int 	pos;
  char 	*text;
  char 	newtext[160];
  int	i;
  XmTextPosition   left, right;
  char  *s, *t;

  text = XmTextGetString( w);
  XtTranslateKeycode( flow_Display(w), event->xkey.keycode, event->xkey.state, 
	&mod, &keysym);
  keysym &= 0xFFFF;
  switch ( keysym) 
  {
    case XK_Return:
    case XK_Linefeed:
      // Insert in recall buffer
      if ( strcmp( text, "") != 0)
      {
        for ( i = recall_size - 2; i >= 0; i--)
          strcpy( recall + (i+1) * line_size, recall + i * line_size);
        strncpy( recall, text, line_size);
        recall[line_size-1] = 0;
      }
      *current_recall_line = 0;
      return 1;
    case XK_Up:
      (*current_recall_line)++;
      if ( *current_recall_line > recall_size - 1)
        *current_recall_line = recall_size - 1;
      XmTextSetString( w, recall + (*current_recall_line - 1) * line_size);
      XmTextSetCursorPosition( w, 
		strlen(recall + (*current_recall_line - 1) * line_size));
      break;
    case XK_Down:
      if ( *current_recall_line == 0)
        XmTextSetString( w, "");
      else if ( *current_recall_line == 1)
      {
        XmTextSetString( w, "");
        (*current_recall_line)--;
      }
      else
      {
        (*current_recall_line)--;
        XmTextSetString( w, recall + (*current_recall_line - 1) * line_size);
        XmTextSetCursorPosition( w, 
		strlen(recall + (*current_recall_line - 1) * line_size));
      }
      break;
    case XK_Left:
      XmTextClearSelection( w, CurrentTime);
      pos = XmTextGetCursorPosition( w);
      if ( pos == 0)
        break;
      pos--;
      XmTextSetCursorPosition( w, pos);
      break;
    case XK_Right:
      XmTextClearSelection( w, CurrentTime);
      pos = XmTextGetCursorPosition( w);
      pos++;
      if ( pos > strlen(text))
        break;
      XmTextSetCursorPosition( w, pos);
      break;
    case XK_BackSpace:
    case 65535:
      if ( XmTextGetSelectionPosition( w, &left, &right)) {
        for ( s = text + left, t = text + right; *t; s++,t++)
	  *s = *t; 
        *s = 0;
        XmTextSetString( w, text);
        XmTextSetCursorPosition( w, left);
      }
      else {
        pos = XmTextGetCursorPosition( w);
        if ( pos == 0)
          break;
        if ( pos == 1)
          strcpy( newtext, "");
        else
          strncpy( newtext, text, pos-1);
        newtext[pos-1] = 0;
        strncat( newtext, &text[pos], sizeof( newtext));
        XmTextSetString( w, newtext);
        XmTextSetCursorPosition( w, pos-1);
      }
      break;
    default:
      if ( event->xkey.state & ControlMask)
        return 0;
      if ( keysym > 256)
        return 0;
      if ( XmTextGetSelectionPosition( w, &left, &right)) {
        for ( s = text + left, t = text + right; *t; s++,t++)
	  *s = *t; 
        *s = 0;
        XmTextSetCursorPosition( w, left);
      }
      pos = XmTextGetCursorPosition( w);
      if ( pos == 0)
        strcpy( newtext, "");
      else
        strncpy( newtext, text, pos);
      newtext[pos] = keysym;
      newtext[pos+1] = 0;
      strncat( newtext, &text[pos], sizeof( newtext));
      XmTextSetString( w, newtext);
      XmTextSetCursorPosition( w, pos+1);
  }
  return 0;
}

int mrm_IsIconicState( Widget W)
{
#if defined OS_VMS
   static Atom	    WmStateAtom = 0;
   int		    state, sts;
   int		    Iconic = 0;
   WmIconStateRec   *IconStateData;
   Atom		    atom_ret;
   int		    format_ret;
   unsigned long    nitems_ret, bytesleft;
   Widget	    Shell;

   if (W == NULL)
      return FALSE;

   Shell = W;
   while(!XtIsShell(Shell))    
      Shell = XtParent(Shell);

   if (WmStateAtom == 0)
   {
      WmStateAtom = XInternAtom(XtDisplay(Shell), "WM_STATE", 1);
   }

   if (WmStateAtom)
   {    
      sts = XGetWindowProperty(
	    XtDisplay(Shell), 
	    XtWindow(Shell),
	    WmStateAtom, 
	    0L, 
	    (sizeof(*IconStateData)/sizeof(long)), 
	    FALSE,
	    AnyPropertyType, 
	    &atom_ret, 
	    &format_ret, 
	    &nitems_ret, 
	    &bytesleft, 
	    (unsigned char **)&IconStateData
	    );

      if ( sts == Success && 
	   IconStateData != NULL && 
	   IconStateData->state == IconicState)
	Iconic = TRUE;
      else
	Iconic = FALSE;

      if (sts == Success && IconStateData != NULL)
	 XFree(IconStateData);
   }
   return Iconic;
#else
   return 0;
#endif
}

void mrm_PositionPopup( Widget popup, Widget parent, int x, int y)
{
  short x0, y0, x1, y1;
  Arg args[5];
  Widget grandparent;

  x0 = (short) x;
  y0 = (short) y;
  grandparent = XtParent(parent);
  while( grandparent) {
    XtSetArg( args[0], XmNx, &x1);
    XtSetArg( args[1], XmNy, &y1);
    XtGetValues( parent, args, 2);
    if ( XtIsShell( parent))
      break;
    x0 += x1;
    y0 += y1;
    parent = grandparent;
    grandparent = XtParent( parent);
  }
  XtSetArg(args[0], XmNx, x0);
  XtSetArg(args[1], XmNy, y0);
  XtSetValues( popup, args, 2);
}
#endif
