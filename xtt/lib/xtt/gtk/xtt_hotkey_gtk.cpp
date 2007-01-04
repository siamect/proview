/* 
 * Proview   $Id: xtt_hotkey_gtk.cpp,v 1.1 2007-01-04 08:29:32 claes Exp $
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

/*
  Interface for global function keys.
  The XLib function XGrabKey is used because gdk doesn't have any corresponding function.
  The syntax of the definition file is

  ['Modifier1'] ['Modifier2'] ['Modifier3'] <Key>'keysym' : 'action'('argument') 

  where
    Modifier1,2 and 3 can be Control, Alt or Shift.
    keysym any key symbol, for example F10.
    action, the name of a registred action function: SetDig, ResetDig, ToggleDig or Command.
    argument, argument to the action function (an attribute, or a command)

  Example
    Control <Key>F11 		: ToggleDig( Test-Dv1.ActualValue)
    Shift <Key>F11     		: ToggleDig( Test-Dv2.ActualValue)
    Control Shift <Key>F11 	: ToggleDig( Test-Dv3.ActualValue)
    Alt <Key>F11 		: ToggleDig( Test-Dv4.ActualValue)
    Control Alt <Key>F11 	: ToggleDig( Test-Dv5.ActualValue)
    Control Shift Alt <Key>F11 	: ToggleDig( Test-Dv6.ActualValue)
    <Key>F10 			: SetDig( Test-Dv2.ActualValue)
    <Key>F12 			: Command ( open graph gtest)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gdk/gdk.h>
#include <gdk/gdkx.h>
#include <gdk/gdkkeysyms.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/keysym.h>
#include "xtt_hotkey_gtk.h"
#include "co_cdh.h"
#include "co_dcli.h"

void XttHotkey::register_action( char *name, void (* action)(char *,void *), void *userdata)
{
  HotkeyAction a( name, action);
  m_actions.push_back( a);

  for ( int i = 0; i < (int)m_keys.size(); i++) {
    m_keys[i].set_action( &m_actions[m_actions.size() - 1], userdata);
  }
}

XttHotkey::XttHotkey( char *filename)
{
  strcpy( m_filename, filename);
  read_file();
}

XttHotkey::~XttHotkey()
{
}

int XttHotkey::read_file()
{
  FILE *fp;
  char line[200];
  int row = 0;
  char p1[2][200];
  char p2[10][200];
  int i, n;
  char *s;

  dcli_translate_filename( m_filename, m_filename);
  fp = fopen( m_filename, "r");
  if ( !fp)
    return 0;

  while ( dcli_read_line( line, sizeof(line), fp)) {
    int mod = 0;
    int keysym;
    char keystr[20] = "";
    char action_arg[200];
    char action_name[200];

    row++;

    dcli_trim( line, line);
    if ( line[0] == 0 || line[0] == '#')
      continue;

    n = dcli_parse( line, ":", "", (char *)p1, 
		    sizeof( p1) / sizeof( p1[0]), sizeof( p1[0]), 0);
    if ( n != 2) {
      printf( "Syntax error, %s, row %d\n", m_filename, row);
      continue;
    }
    dcli_trim( p1[0], p1[0]);
    dcli_trim( p1[1], p1[1]);

    n = dcli_parse(  p1[0], " 	", "", (char *)p2, 
		    sizeof( p2) / sizeof( p2[0]), sizeof( p2[0]), 0);
    if ( n < 1) {
      printf( "Syntax error, %s, row %d\n", m_filename, row);
      continue;
    }
    for ( i = 0; i < n; i++) {
      if ( cdh_NoCaseStrcmp( p2[i], "Control") == 0)
	mod |= ControlMask;
      else if ( cdh_NoCaseStrcmp( p2[i], "Shift") == 0)
	mod |= ShiftMask; 
      else if ( cdh_NoCaseStrcmp( p2[i], "Alt") == 0)
	mod |= Mod1Mask; 
      else if ( cdh_NoCaseStrncmp( p2[i], "<key>", 5) == 0) {
	strcpy( keystr, &p2[i][5]);
      	dcli_trim( keystr, keystr);
      }
      else {
	printf( "Syntax error, %s, row %d\n", m_filename, row);
	break;
      }
    }

    n = dcli_parse(  p1[1], "(", "", (char *)p2, 
		     sizeof( p2) / sizeof( p2[0]), sizeof( p2[0]), 0);
    if ( n < 2) {
      printf( "Syntax error, %s, row %d\n", m_filename, row);
      continue;
    }
    strcpy( action_name, p2[0]);
    dcli_trim( action_name, action_name);
    strcpy( action_arg, p2[1]);
    if ( (s = strrchr( action_arg, ')')))
      *s = 0;
    else {
      printf( "Syntax error, %s, row %d\n", m_filename, row);
      continue;
    }
    keysym = XStringToKeysym( keystr);
    if ( !keysym) {
      printf( "Syntax error, %s, row %d\n", m_filename, row);
      continue;
    }

    HotkeyKey key( mod, keysym, action_name, action_arg);
    m_keys.push_back( key);
  }

  fclose( fp);

  for ( i = 0; i < (int)m_keys.size(); i++) {
    grab_key( m_keys[i].m_keysym, m_keys[i].m_mod);
  }

  return 1;
}

int XttHotkey::event_handler( GdkXEvent *xevent, gpointer data)
{
  XttHotkey *hotkey = (XttHotkey *)data;
  XKeyEvent *e = (XKeyEvent *)xevent;

  if ( e->type == KeyPress) {
    int key = e->keycode;
    int keysym = XKeycodeToKeysym( GDK_DISPLAY(), key, 0);

    for ( int i = 0; i < (int)hotkey->m_keys.size(); i++) {
      if ( hotkey->m_keys[i].m_keysym == keysym && 
	   hotkey->m_keys[i].m_mod == (int)(e->state & ~LockMask
				       & ~Mod2Mask & ~Mod3Mask & ~Mod4Mask & ~Mod5Mask)) {
	if ( hotkey->m_keys[i].m_action)
	  (hotkey->m_keys[i].m_action)( hotkey->m_keys[i].m_action_arg,
					hotkey->m_keys[i].m_userdata);
      }
    }
  }
  return GDK_FILTER_CONTINUE;
}

int XttHotkey::grab_key( int keysym, int modifier)
{
  GdkDisplay *display = gdk_display_get_default();
  int n_screens = gdk_display_get_n_screens( display);

  Display *dp = gdk_x11_display_get_xdisplay( display);
  int mode = GrabModeAsync;

  int keycode = XKeysymToKeycode( dp, keysym);
  if ( !keycode)
    return 0;

  gdk_error_trap_push();

  for ( int i = 0; i < n_screens; i++) {
    GdkWindow *root = gdk_screen_get_root_window( gdk_display_get_screen( display, i));
    Window w = gdk_x11_drawable_get_xid( root);

    XGrabKey( dp, keycode, modifier, w, True, mode, mode);
    XGrabKey( dp, keycode, modifier | LockMask, w, True, mode, mode);
    XGrabKey( dp, keycode, modifier | Mod2Mask, w, True, mode, mode);
    XGrabKey( dp, keycode, modifier | LockMask | Mod2Mask, w, True, mode, mode);
  }
  gdk_flush();
  gdk_error_trap_pop();
  return 1;
}

