/* 
 * Proview   $Id: flow_browwidget_motif.h,v 1.1 2007-01-04 07:57:00 claes Exp $
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

#ifndef brow_widget_h
#define brow_widget_h

#if defined __cplusplus
  extern "C" {
#endif

/* X Toolkit is compiled with member alignment */

#if defined OS_VMS
#pragma member_alignment save
#pragma member_alignment
#endif

#include <X11/IntrinsicP.h>
#include <X11/CoreP.h>
#include <X11/CompositeP.h>

typedef struct {
	XmOffsetPtr	*offset;
	int		reserved;
	} browClassPart;

typedef struct {
	CoreClassPart	core_class;
	CompositeClassPart composite_class;
	browClassPart	brow_class;
	} BrowClassRec, *BrowWidgetClass;

typedef struct {
	void		*brow_ctx;
	void	       	*draw_ctx;
        int		(*init_proc)(FlowCtx *ctx, void *clien_data);
	int		is_navigator;
	void		*client_data;
	Widget		main_brow_widget;
	Widget		scroll_h;
	Widget		scroll_v;
	Widget		form;
	} BrowPart;

typedef struct {
	CorePart	core;
	CompositePart	composite;
	BrowPart	brow;
	} BrowRec, *BrowWidget;




Widget BrowCreate( 
	Widget parent, 
	char *name, 
	ArgList args, 
	int argCount,
        int (*init_proc)(FlowCtx *ctx, void *client_data),
	void *client_data
	);

Widget BrowCreateNav( Widget parent, char *name, ArgList args, int argCount, 
	Widget main_brow);

Widget ScrolledBrowCreate( 
	Widget parent, 
	char *name, 
	ArgList args, 
	int argCount,
        int (*init_proc)(FlowCtx *ctx, void *client_data),
	void *client_data, 
	Widget *brow_w
);	

void BrowCtxFromWidget( Widget w, void **ctx);

#if defined OS_VMS
#pragma member_alignment restore
#endif

#if defined __cplusplus
  }
#endif
#endif
