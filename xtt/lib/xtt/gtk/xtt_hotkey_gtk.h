/* 
 * Proview   $Id: xtt_hotkey_gtk.h,v 1.2 2008-10-31 12:51:36 claes Exp $
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

#ifndef	xtt_hotkey_gtk_h
#define	xtt_hotkey_gtk_h

#if defined OS_LINUX

#include <vector>
#include "pwr.h"

using namespace std;

class HotkeyAction {
 public:
  char m_name[80];
  void (* m_action)(char *,void *);
  HotkeyAction( const char *name, void (* action)(char *,void *)) : m_action(action) {
    strcpy( m_name, name);
  }
  HotkeyAction( const HotkeyAction& x) : m_action(x.m_action) {
    strcpy( m_name, x.m_name);
  }
};

class HotkeyKey {
 public:
  int m_mod;
  int m_keysym;
  char m_action_name[80];
  char m_action_arg[200];
  void (* m_action)(char *,void *);
  void *m_userdata;

  HotkeyKey( int mod, int keysym, char *action_name, char *action_arg) : 
    m_mod(mod), m_keysym(keysym), m_action(0), m_userdata(0) {
    strcpy( m_action_name, action_name);
    strcpy( m_action_arg, action_arg);
  }
  HotkeyKey( const HotkeyKey& x) : m_mod(x.m_mod), m_keysym(x.m_keysym), 
    m_action(x.m_action), m_userdata(x.m_userdata) {
    strcpy( m_action_name, x.m_action_name);
    strcpy( m_action_arg, x.m_action_arg);
  }
  void set_action( HotkeyAction *action, void *userdata) {
    if ( strcmp( m_action_name, action->m_name) == 0) {
      m_action = action->m_action;
      m_userdata = userdata;
    }
  }
};

class XttHotkey {
 public:
  pwr_tFileName m_filename;
  vector<HotkeyAction> m_actions;
  vector<HotkeyKey> m_keys;
  XttHotkey( const char *filename);
  ~XttHotkey();
  void register_action( const char *name, void (* action)(char *,void *), void *userdata);
  int read_file();
  int grab_key( int keysym, int modifier);
  static int event_handler( GdkXEvent *xevent, gpointer data);
};

#else
// Dummy for other platforms than OS_LINUX

class XttHotkey {
 public:
  // pwr_tFileName m_filename;
  // vector<HotkeyAction> m_actions;
  // vector<HotkeyKey> m_keys;
  XttHotkey( const char *filename) {}
  ~XttHotkey() {}
  void register_action( const char *name, void (* action)(char *,void *), void *userdata) {}
  //  int read_file() 
  //  int grab_key( int keysym, int modifier);
  static int event_handler( GdkXEvent *xevent, gpointer data) { return 1;}
};
#endif

#endif
