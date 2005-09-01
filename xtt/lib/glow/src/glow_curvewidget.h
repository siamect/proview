/* 
 * Proview   $Id: glow_curvewidget.h,v 1.2 2005-09-01 14:57:53 claes Exp $
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

#ifndef curve_widget_h
#define curve_widget_h

#if defined __cplusplus
  extern "C" {
#endif

/* X Toolkit is compiled with member alignment */

#if defined OS_VMS
#pragma member_alignment save
#pragma member_alignment
#endif

#include <X11/CoreP.h>
#include <X11/CompositeP.h>

typedef struct {
	XmOffsetPtr	*offset;
	int		reserved;
	} curveClassPart;

typedef struct {
	CoreClassPart	core_class;
	CompositeClassPart composite_class;
	curveClassPart	curve_class;
	} CurveClassRec, *CurveWidgetClass;

typedef struct {
	void		*curve_ctx;
        int		(*init_proc)(GlowCtx *ctx, void *clien_data);
	int		is_navigator;
	void		*client_data;
	Widget		main_curve_widget;
	Widget		scroll_h;
	Widget		scroll_v;
	Widget		form;
	} CurvePart;

typedef struct {
	CorePart	core;
	CompositePart	composite;
	CurvePart	curve;
	} CurveRec, *CurveWidget;




Widget CurveCreate( 
	Widget parent, 
	char *name, 
	ArgList args, 
	int argCount,
        int (*init_proc)(GlowCtx *ctx, void *client_data),
	void *client_data
	);

Widget CurveCreateNav( Widget parent, char *name, ArgList args, int argCount, 
	Widget main_curve);

Widget ScrolledCurveCreate( 
	Widget parent, 
	char *name, 
	ArgList args, 
	int argCount,
        int (*init_proc)(GlowCtx *ctx, void *client_data),
	void *client_data, 
	Widget *curve_w
);	

void CurveCtxFromWidget( Widget w, void **ctx);

#if defined OS_VMS
#pragma member_alignment restore
#endif

#if defined __cplusplus
  }
#endif
#endif
