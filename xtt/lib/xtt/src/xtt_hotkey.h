#ifndef	op_hotkey_h
#define	op_hotkey_h

/* op_hotkey.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

typedef int		hotkeySTATUS;
#define	TkSUCCESS	1
#define	TkNOOP		2
#define	TkERROR		3
#define	TkPARSE_ERROR	4
#define TkNO_BINDINGS	5
#define	TkINV_ARGS	6

typedef	void*		hotkeyHandle;

hotkeySTATUS hotkey_Initialize (
    Widget w,			/*@ARG:RO,VAL,Id of a widget in the	*/
				/* application. This widget is passed	*/
				/* to the `TkHotkeyActionProc' called	*/
				/* when a hotkey is pressed by the user.*/
    hotkeyHandle* handle )	/*@ARG:WR,REF,Returned handle. This	*/
				/* handle must be used when calling the	*/
				/* tkhk_process() routine.		*/
;
hotkeySTATUS hotkey_Process (
    hotkeyHandle handle,	/*@ARG:RO,VAL,Hotkey handle retured by	*/
				/* tkhk_initialize() routine.		*/
    XEvent *event )		/*@ARG:RO,REF,Address of XEvent record.	*/
;
#endif/*TkHotkey_DEFS*/
