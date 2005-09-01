/* 
 * Proview   $Id: rt_xtt.h,v 1.4 2005-09-01 14:57:48 claes Exp $
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

#ifndef rt_xtt_h
#define rt_xtt_h

/* rt_xtt.h -- Simple navigator */


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

#ifndef rt_qcom_h
extern "C" {
#include "rt_qcom.h"
}
#endif

#ifndef xtt_xnav_h
#include "xtt_xnav.h"
#endif

#ifndef xtt_focustimer_h
#include "xtt_focustimer.h"
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
    qcom_sQid	queid;
    XtIntervalId  timerid;
    FocusTimer	focustimer;
};

#endif




















