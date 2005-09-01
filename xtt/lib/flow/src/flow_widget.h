/* 
 * Proview   $Id: flow_widget.h,v 1.2 2005-09-01 14:56:12 claes Exp $
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
 **/

#ifndef flow_widget_h
#define flow_widget_h

#if defined __cplusplus
  extern "C" {
#endif

#if defined OS_VMS
#pragma member_alignment save
#pragma member_alignment
#endif

#include <X11/CoreP.h>
#include <X11/CompositeP.h>


typedef struct {
	XmOffsetPtr	*offset;
	int		reserved;
	} FlowClassPart;

typedef struct {
	CoreClassPart	core_class;
	CompositeClassPart composite_class;
	FlowClassPart	flow_class;
	} FlowClassRec, *FlowWidgetClass;

typedef struct {
	void		*flow_ctx;
        int		(*init_proc)(FlowCtx *ctx, void *client_data);
	int		is_navigator;
	void		*client_data;
	int		scrollbars;
	Widget		main_flow_widget;
	Widget		scroll_h;
	Widget		scroll_v;
	Widget		form;
	} FlowPart;

typedef struct {
	CorePart	core;
	CompositePart	composite;
	FlowPart	flow;
	} FlowRec, *FlowWidget;

Widget FlowCreate( 
	Widget parent, 
	char *name, 
	ArgList args, 
	int argCount,
        int	(*init_proc)(FlowCtx *ctx, void *client_data),
	void *client_data
	);

Widget FlowCreateNav( Widget parent, char *name, ArgList args, int argCount, 
	Widget main_flow);

Widget ScrolledFlowCreate( 
	Widget parent, 
	char *name, 
	ArgList args, 
	int argCount,
        int (*init_proc)(FlowCtx *ctx, void *client_data),
	void *client_data, 
	Widget *flow_w
);	

#if defined OS_VMS
#pragma member_alignment restore
#endif

#if defined __cplusplus
  }
#endif
#endif
