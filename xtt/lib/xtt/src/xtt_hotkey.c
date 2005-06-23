/* xtt_hotkey.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   This module implements global action key definitions for Xtoolkit
   based application. These global key definitions are referred to as
   'hotkeys' in the rest of this document.

   Using the 'hotkey' routines an Xtoolkit actionroutine can be
   associated with a specific key. When the users depresses the key
   the action routine is called. This is done regardless of which
   window and/or application has the keyboard focus.

   The hotkey definitions are read from the application X resource
   database. In order to define the hotkey bindings the resource
   'hotkeys' must be specified in the following way ('Class' is the
   application class name) :

   Example
    Class*hotkeys: [hotkey_binding]...

    1: Hotkey resource file translation tables

       The binding between key events and hotkey actions are defined in
       the application resource file by specifying the resource
       'hotkeys' for the application top widget.

       The syntax of the binding definition is closely related to that
       of normal Xtoolkit translation tables. The syntax is defined by
       the following EBNF description:

   Example
     hotkey_binding ::= [modifier]... "<Key>" keyname ":" action
     modifier ::= "Ctrl" | "Shift" | "Alt"
     keyname ::= standard_X11_key_name 
     action ::= action_routine_name "(" argument ")"
     action_routine_name ::= normal_x11_action_routine_name
     argument ::= any_text_value

     Note that there are some differences between this and the normal
     X11 translation table syntax. The differences are listed below:

   Only the modifiers "Ctrl", "Shift" and "Alt" are allowed.
   The modifiers can not be negated (i.e. ~Shift can not be used).
   The 'keyname' can not be specified as a hex value (i.e. x0FF1
   can not be used).
   The action routine must have exactly one argument.

   Below is an example of a hotkey binding:
   Example:
     Myprog*hotkeys: \n\
       Ctrl <Key>F1: PopupAction(foo)\n\
       Ctrl Shift <Key>F7: CloseValve(valve_15)\n\
       <Key>F14: AcknowlegeWarning(ok)

     1: Hotkey action routines.

        Hotkey action routine work exactly the same way as normal
        Xtoolkit action routines and must be declared in the same way (by
        Calling XtAddActions or XtAppAddActions).

     1: Example of using hotkeys 

        The following code example show one way of using the hotkey
        routines. The key sequence "<Ctrl>F1" is bound to the action
        routine show_win() which deiconifies and raises the window. 

        Example:
         #include <Xm/MainW.h>
         #include "TkHotkey.h"

         static void show_win (Widget, void*, String*, Cardinal*);

         Widget topw;

         static String fbr[] = {
           "*hotkeys: Ctrl<Key>F1: show_win(foo)", NULL, };
         static XtActionsRec actions[] = {
           { "show_win",  show_win }};

         main (int argc, char* argv[])
         {
           XtAppContext	ctx;
           hotkeyHandle	hk_handle;
           hotkeySTATUS	rs;
           XEvent		event;

           topw = XtAppInitialize (&ctx,"Xhk",0,0,&argc,argv,fbr,0,0);
           XtManageChild (XmCreateMainWindow (topw,"main",0,0));
           XtAppAddActions (ctx, actions, XtNumber (actions));
           rs = tkhk_initialize (topw, &hk_handle );

           XtRealizeWidget (topw);
           for (;;)
           {
             if ( XtAppPeekEvent (ctx, &event) 
                  && TkSUCCESS == tkhk_process (hk_handle, &event))
              {
		XtAppNextEvent (ctx, &event); / * remove event * /
              }
              else
              {
                XtAppNextEvent (ctx, &event);
                XtDispatchEvent ( &event );
              }
            }
          }

          static void show_win (Widget w, void* arg, String* av, Cardinal* ac)
          {
            XMapRaised (XtDisplay (topw), XtWindow (topw));
          }
   */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#if defined (OS_VMS)
# pragma nostandard
#endif
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/keysym.h>
#if defined (OS_VMS)
# pragma standard
#endif
#include "xtt_hotkey.h"

#define CLONESTR(str)	    ((char*)strcpy(XtMalloc(strlen(str)+1),(str)))
#define CLONE(d,l)	    ((char*)memcpy(XtMalloc((l)),(d),(l)))
#define	TkNhotkeys	    "hotkeys"
#define	TkRHotkeys	    "Hotkeys"

/*---------------------------------------------------------------------*\
* @Structure:	HkRec, *Hk
* @Abstract:	hotkey storage record
* @Description:
*   
* @NOSide_Effects:
* @EndStructure.
\*---------------------------------------------------------------------*/
typedef struct HkStruct {
  int		    kcode;	/* Keycode for hotkey			*/
  unsigned int	    mod;	/* Modified for hotkey			*/
  void*		    arg;	/* Callback argument			*/
  char*		    action;	/* `Action' procedure name		*/
  char*		    args;	/* `Action' procedure textual argument	*/
} HkRec, *Hk;

/*---------------------------------------------------------------------*\
* @Structure:	CtrlRec, *Ctrl
* @Abstract:	Hotkey control record
* @Description:
*	TBS.
* @NOSide_Effects:
* @EndStructure.
\*---------------------------------------------------------------------*/
typedef struct CtrlStruct {
  Display*	    xd;		/* Display				*/
  Widget	    wdg;	/* Widget id passed to action routines	*/
  Hk		    hk;		/* Hotkey storage vector		*/
  int		    nkeys;	/* Nbr of used slots			*/
  int		    hk_len;	/* Allocated len of 'hk' vector		*/
  char*		    bp;		/* String parse pointer			*/
  int		    parse_err;	/* Parser error code			*/
  char*		    token;	/* Current parsed token			*/
} CtrlRec, *Ctrl;

static XtResource tkhk_resources [] = {
 { TkNhotkeys, TkRHotkeys, 
    XtRString, sizeof (String),
    XtOffset(Ctrl,bp),
    XtRImmediate, NULL },
};

    /* --- PRIVATE ROUTINE FOREWARD DECLARATION --- */

static hotkeySTATUS	tkhk_add_grab (Ctrl cp, Hk hk );
static int	tkhk_xerr_hdlr (Display *xd, XErrorEvent* ev );
static int	tkhk_xerror_flag = 0;
static hotkeySTATUS	tkhk_parser (Ctrl cp);
static int	TwpHotkeyStmt (Ctrl cp);
static int	TwpError (Ctrl p);
static int	TwpStrncmp_ncs (char* s1, char* s2, int len);
static int	TwpSetToken (Ctrl p, int len);
static void	TwpSkip (Ctrl p);
static int	TwpCheck (Ctrl p, char* str, int len);
static int	TwpMatch (Ctrl p, char* str);
static int	TwpIsArg (Ctrl p);
static int	TwpIsEmpty (Ctrl p);
static int	TwpIsId (Ctrl p);


/*----------------------------------------------------------------------*\
* @Module:	hotkey_Initialize
* @Abstract:	init hotkey subsystem
* @Description:
*	This routine initializes the 'hotkey' toolkit component. 
*
*	The application resource database is searched for a 'hotkeys'
*	resource specifying a hotkey translation string. This string is
*	parsed and the specified hotkeys are enabled.
*
* @Completion_Status:
*	<@>TkSUCCESS	 -S- Successful completion
*	<@>TkPARSE_ERROR -E- Syntax error in hotkey binding description.
*			     Some key binding where not performed.
*	<@>TkNO_BINDINGS -E- Failed to locate hotkey binding resource.
*	<@>TkERROR	 -E- Failed.
* @Side_Effects:
*	If an error occurs when trying to enable a hotkey this routine
*	outputs an error message to stdout.
*
* @NODesign:
* @EndModule.
\*----------------------------------------------------------------------*/
hotkeySTATUS hotkey_Initialize (
    Widget w,			/*@ARG:RO,VAL,Id of a widget in the	*/
				/* application. This widget is passed	*/
				/* to the `XtActionProc' called		*/
				/* when a hotkey is pressed by the user.*/
    hotkeyHandle* handle )	/*@ARG:WR,REF,Returned handle. This	*/
				/* handle must be used when calling the	*/
				/* tkhk_process() routine.		*/
    {
	Ctrl		cp = (Ctrl) XtCalloc (1, sizeof (CtrlRec));
	XErrorHandler	xerr_hdlr;
	hotkeySTATUS	rs;

	cp->hk_len = 10;
	cp->nkeys = 0;
	cp->hk = (Hk) XtCalloc (cp->hk_len, sizeof (HkRec));
	cp->xd = XtDisplay (w);
	cp->wdg = w;
	*handle = (hotkeyHandle) cp;
	XtGetApplicationResources (w, cp, tkhk_resources,1,0,0);
	if (! cp->bp ) return TkNO_BINDINGS;
	xerr_hdlr = XSetErrorHandler (tkhk_xerr_hdlr);
	rs = tkhk_parser (cp);
	XSync ( cp->xd, False );
	xerr_hdlr = XSetErrorHandler (xerr_hdlr);

	return rs;
    }

static int tkhk_xerr_hdlr (
    Display *xd,
    XErrorEvent* ev )
    {
	printf ("TkHotkeys: failed to grab key.\n");
	tkhk_xerror_flag = True;
	return Success;

    }


/*---------------------------------------------------------------------*\
* @Module:	tkhk_process
* @Abstract:	process hotkey x events
* @Description:
*	This routine tests if an event is a 'hotkey' event. If it is the
*	action routine associated with the key is called and TkSUCCESS is
*	returned. If the event is not a 'hotkey' event this routine
*	returns TkNOOP.
*
*	This routine must be called in the application event dispatch
*	loop. The following example shows how:
*
*	@Example
*	for (;;)
*	{
*	    Xt(App)NextEvent(.. &event);
*	    if (TkNOOP == tk_hk_process (tk_handle, &event)) 
*		XtDispatchEvent (&event);
*	}
*	@End_Example
*
*	If the application does not have complete control over the event
*	dispatching loop the following type of event loop can be used:
*	@Example
*	for (;;)
*	{
*	    if ( XtAppPeekEvent (ctx, &event) 
*	    &&	 TkSUCCESS == tk_hk_process (hk, &event))
*	    {
*		XtAppNextEvent (ctx, &event); // remove event
*	    }
*	    else
*	    {
*		gmsWaitEvent (0);
*		switch (gmsQEventPriority ())
*		{
*		...
*		}
*	    }
*	}
*	@End_Example
*
* @Completion_Status:
*	<@>TkSUCCESS -S- Successful completion, action routine called.
*	<@>TkNOOP    -S- Not a hotkey event.
*	<@>TkINV_ARGS -F- Invalid arguments.
* @Side_Effects:
*	None.
* @NODesign:
* @EndModule.
\*---------------------------------------------------------------------*/
hotkeySTATUS hotkey_Process (
    hotkeyHandle handle,		/*@ARG:RO,VAL,Hotkey handle retured by	*/
				/* tkhk_initialize() routine.		*/
    XEvent *event )		/*@ARG:RO,REF,Address of XEvent record.	*/
    {
	Ctrl	    cp = (Ctrl)handle;
	XKeyEvent*  kev = (XKeyEvent*) event;
	int	    i;

	if (! handle || ! event) return TkINV_ARGS;

	if (KeyPress == event->type )
	{
	    for (i = 0; i < cp->nkeys; ++i )
	    {
		if ( cp->hk[i].kcode == kev->keycode 
		    && cp->hk[i].mod == (kev->state & ~LockMask
		    & ~Mod2Mask & ~Mod3Mask & ~Mod4Mask & ~Mod5Mask)
		)
		{
		    XtCallActionProc (
			cp->wdg, 
			cp->hk[i].action,
			event,
			&cp->hk[i].args,
			1 );
		    return TkSUCCESS;
		}
	    }
	}
	return TkNOOP;
    }


/*---------------------------------------------------------------------*\
* @Module:	tkhk_add_grab
* @Abstract:	add grab for one key
* @Description:
*	Add grabs for one key. The key is defined by the hotkey structure
*	passed as the 2:nd argument. Both the normal and 'caps-locked'
*	keypress is selected.
*
* @Completion_Status:
*	<@>TkSUCCESS -S- success
*	<@>TkERROR -E- failed
* @Side_Effects:
*	None.
* @NODesign:
* @EndModule.
\*---------------------------------------------------------------------*/
static hotkeySTATUS tkhk_add_grab (
    Ctrl cp,			/*@ARG:RO,REF,Address of control record */
    Hk hk )			/*@ARG:RO,REF,Address of new key def	*/
    {
	int	mode, i;
	Window	w;

	if (cp->nkeys >= cp->hk_len )
	{
	    cp->hk_len += 10;
	    cp->hk = (Hk) XtRealloc ((char *)cp->hk, cp->hk_len * sizeof(HkRec));
	}
	cp->hk[cp->nkeys++ ] = *hk;
	mode = GrabModeAsync;

	for (i = 0; i < ScreenCount(cp->xd); i++)
	{ 
	    w = RootWindowOfScreen(ScreenOfDisplay(cp->xd, i));
	    XGrabKey (cp->xd, hk->kcode, hk->mod, w, 1, mode, mode );
	    XGrabKey (cp->xd, hk->kcode, hk->mod | LockMask, w, 1, mode,mode);
	}
	return TkSUCCESS;
    }

/*---------------------------------------------------------------------*\
* @Module:	tkhk_parser
* @Abstract:	parse hotkey binding string
* @Description:
*	
* @Completion_Status:
*	<@>TkSUCCESS -S- success
*	<@>TkERROR -E- failed
* @Side_Effects:
*	None.
* @NODesign:
* @EndModule.
\*---------------------------------------------------------------------*/
static hotkeySTATUS tkhk_parser (Ctrl cp)
    {
	cp->parse_err = False;
	cp->token = 0;
	while ( TwpHotkeyStmt ( cp ));
	return cp->parse_err;
    }

/*---------------------------------------------------------------------*\
* @Module:	TwpHotkeyStmt
* @Abstract:	parse one hotkey statement
* @Description:
*	Process one hotkey binding statement.
*	This code was autogenerated by the META II Bnf compiler-compiler.
* @Completion_Status:
*	<@>True	 -S- statement successfully parsed.
*	<@>False -F- Invalid syntax or reached end of binding string.
*		     Check the 'parse_err' flag in the CtrlRec for 
*		     syntax errors.
* @NOSide_Effects:
* @NODesign:
* @EndModule.
\*---------------------------------------------------------------------*/
static int TwpHotkeyStmt (Ctrl cp)
    {
	KeySym	    ksym;
	HkRec	    hkrec;
	int	    flag = 1;
	hotkeySTATUS  rs;

/*	printf ("\n"); */
	hkrec.mod = 0x00;

	if (TwpIsEmpty (cp)) return 0;

	while ( flag )
	{
	    do
	    {
		if (TwpMatch (cp,"Ctrl"))
		{
		    hkrec.mod |= ControlMask;
/*		    printf ("Ctrl "); */
		    break;
		}
		if (TwpMatch (cp,"Shift"))
		{
		    hkrec.mod |= ShiftMask;
/*		    printf ("Shift "); */
		    break;
		}
		if (TwpMatch (cp,"Alt"))
		{
		    hkrec.mod |= Mod1Mask;
/*		    printf ("Alt "); */
		    break;
		}
		flag=0;
	    } while(0);
	}
	if (! TwpMatch (cp,"<Key>")) return TwpError(cp);
	if (! TwpIsId (cp)) return TwpError (cp);
	ksym = XStringToKeysym ( cp->token );
	hkrec.kcode = XKeysymToKeycode (cp->xd, ksym );
/*	printf ( "%s ---> ", cp->token ); */

	if (! TwpMatch(cp,":")) return TwpError(cp);
	if (! TwpIsId(cp)) return TwpError(cp);
	hkrec.action = CLONESTR (cp->token);
/*	printf ( "%s", cp->token ); */

	if (! TwpMatch(cp,"(")) return TwpError(cp);
	if (! TwpIsArg(cp)) return TwpError(cp);
	hkrec.args = CLONESTR (cp->token);
/*	printf ( "  with ARGS: %s", cp->token ); */
	if (! TwpMatch(cp,")")) return TwpError(cp);

	rs = tkhk_add_grab ( cp, &hkrec );
	return True;
    }

static int TwpError (Ctrl p)
    {
	p->parse_err = TkPARSE_ERROR;
	return 0;
    }
static int TwpStrncmp_ncs (char* s1, char* s2, int len)
    {
	for ( ; *s1 && *s2 && len; ++s1, ++s2, --len)
	    if (_tolower(*s1) != _tolower(*s2)) break;
	return len * (_tolower(*s1) - _tolower(*s2));
    }
static int TwpSetToken (Ctrl p, int len)
    {
	if (0 == len) return 0;
	if (p->token) XtFree ( p->token );
	p->token = CLONE (p->bp, len + 1);
	p->token [len] = '\0';
	p->bp = &p->bp[len];
	return 1;
    }
static void TwpSkip (Ctrl p)
    {
	while (*p->bp && (isspace (*p->bp) || *p->bp == '\n')) p->bp++;
    }
static int TwpCheck (Ctrl p, char* str, int len)
    {
	int ok;
	TwpSkip (p);
	ok = (0 == TwpStrncmp_ncs (p->bp,str,len));
	if (ok) p->bp = &p->bp[len];
	return ok;
    }
static int TwpMatch (Ctrl p, char* str)
    {
	if (!TwpCheck (p, str, strlen (str))) return 0;
	return 1;
    }

static int TwpIsArg(Ctrl p)
    {
	int n;

	TwpSkip (p);

	for (n = 0; p->bp[n] != ')' && p->bp[n] != 12 ; ++n);

	return TwpSetToken (p, n);
    }

static int TwpIsEmpty (Ctrl p)
    {
	TwpSkip (p);

	return (p->bp[0] == '\0');
    }

static int TwpIsId (Ctrl p)
    {
	int n;

	TwpSkip (p);
	if (! isalpha(p->bp[0])) return 0;
	for(n = 0; isalnum(p->bp[n]) || '_' ==p->bp[n] || '$' ==p->bp[n]; ++n);
	return TwpSetToken (p, n);
    }

			/* --- End of ps_utl_hotkey.c --- */

