#ifndef rt_xtt_h
#define rt_xtt_h

/* rt_xtt.h -- Simple navigator

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */


#ifndef pwr_h
extern "C" {
# include "pwr.h"
}
#endif
#ifndef flow_h
#include "flow.h"
#endif

#ifndef flow_browctx_h
#include "flow_browapi.h"
#endif

#ifndef rt_trace_h
extern "C" {
#include "rt_trace.h"
}
#endif

#ifndef xtt_xnav_h
#include "xtt_xnav.h"
#endif

class Xtt {
  public:
    Xtt( int argc, char *argv[], int *sts);
    void 	*parent_ctx;
    Widget	parent_wid;
    char 	name[80];
    Widget	brow_widget;
    Widget	form_widget;
    Widget	toplevel;
    XNav	*xnav;
    void	*root_item;
    Widget	msg_label;
    Widget	cmd_prompt;
    Widget	cmd_input;
    Widget	xnav_form;
    int		input_open;
    int		command_open;
    char	cmd_recall[30][160];
    int		cmd_current_recall;
    char        value_recall[30][160];
    int		value_current_recall;
    Widget	india_widget;
    Widget	india_label;
    Widget	india_text;
    void	(*india_ok_cb)( Xtt *, char *);
};

#endif




















