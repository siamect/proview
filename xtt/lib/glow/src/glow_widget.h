/* 
 * Proview   $Id: glow_widget.h,v 1.2 2005-09-01 14:57:54 claes Exp $
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

#ifndef glow_widget_h
#define glow_widget_h

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
	} GlowClassPart;

typedef struct {
	CoreClassPart	core_class;
	CompositeClassPart composite_class;
	GlowClassPart	glow_class;
	} GlowClassRec, *GlowWidgetClass;

typedef struct {
	void		*glow_ctx;
        int		(*init_proc)(GlowCtx *ctx, void *client_data);
	int		is_navigator;
	void		*client_data;
	int		scrollbars;
	Widget		main_glow_widget;
	Widget		scroll_h;
	Widget		scroll_v;
	Widget		form;
	} GlowPart;

typedef struct {
	CorePart	core;
	CompositePart	composite;
	GlowPart	glow;
	} GlowRec, *GlowWidget;

Widget GlowCreate( 
	Widget parent, 
	char *name, 
	ArgList args, 
	int argCount,
        int	(*init_proc)(GlowCtx *ctx, void *client_data),
	void *client_data
	);

Widget GlowCreateNav( Widget parent, char *name, ArgList args, int argCount, 
	Widget main_glow);

Widget ScrolledGlowCreate( 
	Widget parent, 
	char *name, 
	ArgList args, 
	int argCount,
        int (*init_proc)(GlowCtx *ctx, void *client_data),
	void *client_data, 
	Widget *glow_w
);	

#if defined OS_VMS
#pragma member_alignment restore
#endif

#if defined __cplusplus
  }
#endif
#endif
