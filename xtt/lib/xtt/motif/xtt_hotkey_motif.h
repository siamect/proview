/* 
 * Proview   $Id: xtt_hotkey_motif.h,v 1.1 2007-01-04 08:30:03 claes Exp $
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
 */

#ifndef	xtt_hotkey_motif_h
#define	xtt_hotkey_motif_h

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
#endif
