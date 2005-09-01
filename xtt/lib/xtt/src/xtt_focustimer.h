/* 
 * Proview   $Id: xtt_focustimer.h,v 1.4 2005-09-01 14:57:48 claes Exp $
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

#ifndef xtt_focustimer_h
#define xtt_focustimer_h


class FocusTimer {
 public:
  FocusTimer() : set_focus_disabled(0), request_cnt(0), focus_timerid(0) {
  }

  void disable( Widget w, int time) {
    set_focus_disabled++;
    if ( request_cnt > 1)
      request_cnt = 0;
    focus_timerid = XtAppAddTimeOut( XtWidgetToApplicationContext(w), time,
				     enable_set_focus, this);
  }
  int disabled() {
    // Return false on first request after disable
    request_cnt++;
    return (request_cnt > 1);
  }
    
 private:
  int		set_focus_disabled;
  int		request_cnt;
  XtIntervalId 	focus_timerid;

  static void enable_set_focus( void *ft, XtIntervalId *id) {
    ((FocusTimer *)ft)->set_focus_disabled--;
    ((FocusTimer *)ft)->request_cnt = 0;
  }
 public:
  ~FocusTimer() {
    if ( set_focus_disabled)
      XtRemoveTimeOut( focus_timerid);
  }
};

#endif
