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
  }
 public:
  ~FocusTimer() {
    if ( set_focus_disabled)
      XtRemoveTimeOut( focus_timerid);
  }
};

#endif
