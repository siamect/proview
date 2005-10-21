/* 
 * Proview   $Id: rt_trace.h,v 1.8 2005-10-21 16:11:22 claes Exp $
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

#ifndef rt_trace_h
#define rt_trace_h

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef flow_h
#include "flow.h"
#endif

#ifndef flow_api_h
#include "flow_api.h"
#endif


typedef struct
{
  void          *parent_ctx;
  Widget        parent_wid;
  char          *name;
} trace_t_commonpart;

typedef struct tra_sCtx *tra_tCtx;
typedef struct trace_s_node trace_t_node;

struct trace_s_node {
  trace_t_node  *Next;
  tra_tCtx    tractx;
};

struct tra_sCtx{
  trace_t_commonpart	cp;
  Widget		flow_widget;
  Widget		toplevel;
  Widget		form;
  Widget		menu;
  Widget		nav_shell;
  Widget		nav_widget;
  flow_tNodeClass	trace_analyse_nc;
  flow_tConClass	trace_con_cc;
  int			trace_started;
  XtIntervalId		trace_timerid;
  flow_tCtx		flow_ctx;
  flow_tNode		trace_changenode;
  pwr_tObjid		objid;
  double		scan_time;
  void			(*close_cb) (tra_tCtx);
  void			(*help_cb) (tra_tCtx, char *);
  void			(*subwindow_cb) (void *parent_ctx, pwr_tObjid objid);
  void			(*display_object_cb) (void *parent_ctx, pwr_tObjid objid);
  void			(*collect_insert_cb) (void *parent_ctx, pwr_tObjid objid);
  int                   (*is_authorized_cb)(void *parent_ctx, unsigned int access); 
  void                  (*popup_menu_cb)(void *parent_ctx, 
					 pwr_sAttrRef attrref,
					 unsigned long item_type,
					 unsigned long utility, 
					 char *arg, Widget *popup); 
  int                   (*call_method_cb)(void *parent_ctx, char *method,
					  char *filter,
					  pwr_sAttrRef attrref,
					  unsigned long item_type,
					  unsigned long utility, 
					  char *arg);
  trace_t_node		*trace_list;
  pwr_tFileName	       	filename;
  int			version;
  int			has_host;
  pwr_tOName   		hostname;
  pwr_tAName   		plcconnect;
};


tra_tCtx trace_new( 	void 		*parent_ctx, 
			Widget 		parent_wid, 
			pwr_tObjid	objid,
			void 		(*close_cb)(tra_tCtx),
			void 		(*help_cb)(tra_tCtx, char *),
			void 		(*subwindow_cb)(void *, pwr_tObjid),
			void 		(*display_object_cb)(void *, pwr_tObjid),
			void 		(*collect_insert_cb)(void *, pwr_tObjid),
			int             (*is_authorized_cb)(void *, unsigned int) 
);
void	trace_del( tra_tCtx	tractx);

int trace_search_object( 	tra_tCtx	tractx,
				char		*object_str);
void trace_pop( tra_tCtx	tractx);

void trace_swap( tra_tCtx tractx, int mode);

#endif






