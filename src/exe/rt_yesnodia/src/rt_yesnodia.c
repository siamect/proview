/* rt_yesnodia.c 

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <Xm/Xm.h>
#include <Xm/MessageB.h>

void ok_pushed( Widget w, XtPointer client_data, XtPointer call_data)
{
  exit(0);
}

void cancel_pushed( Widget w, XtPointer client_data, XtPointer call_data)
{
  exit(1);
}

void WarningMsg( Widget parent, char *msg, char *title)
{
  static Widget dialog = 0;
  XmString text, ok_str, cancel_str;

  if ( !dialog) {
    dialog  = XmCreateWarningDialog( parent, "Warning", NULL, 0);

    XtAddCallback( dialog, XmNokCallback, ok_pushed, NULL);
    XtAddCallback( dialog, XmNcancelCallback, cancel_pushed, NULL);
  }
  text = XmStringCreateLtoR( msg, XmFONTLIST_DEFAULT_TAG);
  ok_str = XmStringCreateLocalized( "Yes");
  cancel_str = XmStringCreateLocalized( "No");
  XtVaSetValues( dialog, 
		 XmNmessageString, text,
		 XmNokLabelString, ok_str,
		 XmNcancelLabelString, cancel_str,
		 XmNdefaultButtonType, XmDIALOG_CANCEL_BUTTON,
		 NULL);
  XtVaSetValues( XtParent( dialog), 
		 XmNtitle, title, NULL);
  XmStringFree( text);
  XmStringFree( ok_str);
  XmStringFree( cancel_str);

  XtManageChild( dialog);
  XtPopup( XtParent(dialog), XtGrabNone);

}


void usage()
{
  printf( "\n\nOpen a warning message dialogbox.\n");
  printf( "The program terminates with 0 if OK is pressed and\n");
  printf( "with 1 if No is pressed\n\n");
  printf( "Argument 1: Confirm text\nArgument 2: Title\n\n");
  printf( "Example: rt_yesnodia \"Do you really want to ?\" \"Warning\"\n\n\n");
}


int main( int argc, char *argv[])
{
  XtAppContext  app_ctx;
  char          text[80];
  char          title[80];
  Widget        toplevel;

  /* Look for user and password as arguments */
  if ( argc > 1) {
    if ( strcmp( argv[1], "-h") == 0) {
      usage();
      exit(0);
    }
    strcpy( text, argv[1]);
    if ( argc > 2) 
      strcpy( title, argv[2]);
    else
      strcpy( title, "Warning");
  }
  else {
    usage();
    exit(0);
  }

  // MrmInitialize();

  toplevel = XtVaAppInitialize( &app_ctx, "yesnodia",
		      NULL, 0, &argc, argv, NULL,
		      XmNmappedWhenManaged, False,
		      NULL);
    
  WarningMsg( toplevel, text, title);

  XtRealizeWidget(toplevel);

  XtAppMainLoop(app_ctx);
  return (0);
}














