/* 
 * Proview   $Id: glow_widget.cpp,v 1.2 2005-09-01 14:57:54 claes Exp $
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

#include "glow_std.h"

#include <stdio.h>
#include <stdlib.h>
#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Xm/ScrollBar.h>
#include <Xm/Form.h>
#include <Mrm/MrmPublic.h>
#ifndef _XtIntrinsic_h
#include <X11/Intrinsic.h>
#endif
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "glow.h"
#include "glow_ctx.h"
#include "glow_draw.h"
#include "glow_widget.h"


typedef struct {
	Widget  glow;
	Widget	form;
	Widget	scroll_h;
	Widget	scroll_v;
	int	scroll_h_managed;
	int	scroll_v_managed;
	} glowwidget_sScroll;

static void Notify( Widget w, XEvent *event);
static void Destroy( Widget w);
static void Realize( Widget w, unsigned long *dum, XSetWindowAttributes *swa);
static void Initialize( Widget rec, Widget new_widget, ArgList arg, int *args);
static void Redisplay( Widget w, XEvent *event, Region region);
static Boolean SetValues( Widget old, Widget request, Widget new_widget);

static char defaultTranslations[] = "<Btn1Up>: notify()\n\
<Btn2Up>: notify()\n\
<Btn3Up>: notify()\n\
<Btn1Down>: notify()\n\
<Btn2Down>: notify()\n\
<Btn3Down>: notify()\n\
<BtnMotion>: notify()\n\
<EnterWindow>: notify()\n\
<LeaveWindow>: notify()\n\
<VisibilityNotify>: notify()\n\
<MotionNotify>: notify()\n\
<Key>Up: notify()\n\
<Key>Down: notify()\n\
<Key>Return: notify()";

static XtActionsRec actionsList[] = { {"notify", (XtActionProc) Notify}};

GlowClassRec glowClassRec = {
  { /* Core class part */
    (WidgetClass) &compositeClassRec,	/* superclass */
    "Glow",				/* class name */
    sizeof(GlowRec),			/* widget size */
    NULL,				/* class initialize */
    NULL, 				/* class part initialize */
    FALSE, 				/* class inited */
    (XtInitProc) Initialize,		/* initialize */
    NULL, 				/* initialize hook */
    Realize,				/* realize */
    actionsList, 			/* actions */
    XtNumber( actionsList),		/* num actions */
    NULL, 				/* resourses */
    0,					/* num resourses */
    NULLQUARK,				/* xrm class */
    TRUE,				/* compress motion */
    TRUE,				/* compress expsure */
    TRUE,				/* compress enterleave */
    FALSE,				/* visible interest */
    Destroy,				/* destroy */
    XtInheritResize,			/* resize */
    Redisplay,				/* expose */
    (XtSetValuesFunc)SetValues,		/* set values */
    NULL,				/* set values hook */
    XtInheritSetValuesAlmost,		/* set values almost */
    NULL,				/* get values hook */
    NULL, 				/* accept focus */
    XtVersionDontCheck,			/* version */
    NULL, 				/* callback offsets */
    defaultTranslations,		/* tm_table */
    NULL,				/* geometry */
    NULL,				/* disp accelerators */
    NULL				/* extension */
  },
  { /* composite class record */
    NULL,				/* geometry manager */
    NULL,				/* change managed */
    NULL,				/* insert child */
    NULL,				/* delete child */
    NULL				/* extension */
  },
  { /* glow class record */
    NULL,
    0
  }
};

WidgetClass glowWidgetClass = (WidgetClass) &glowClassRec;

static void scroll_h_action( 	Widget 		w,
				XtPointer 	client_data,
				XtPointer 	call_data);
static void scroll_v_action( 	Widget 		w,
				XtPointer 	client_data,
				XtPointer 	call_data);
static void scroll_callback( glow_sScroll *data);

int glow_init_proc( Widget w, GlowCtx *fctx, void *client_data)
{
  glowwidget_sScroll *scroll_data;
  GlowCtx	*ctx;

  if ( ((GlowWidget) w)->glow.scrollbars)
  {
    scroll_data = (glowwidget_sScroll *) malloc( sizeof( glowwidget_sScroll));
    scroll_data->glow = w;
    scroll_data->scroll_h = ((GlowWidget) w)->glow.scroll_h;
    scroll_data->scroll_v = ((GlowWidget) w)->glow.scroll_v;
    scroll_data->form = ((GlowWidget) w)->glow.form;
    scroll_data->scroll_h_managed = 1;
    scroll_data->scroll_v_managed = 1;

    ctx = (GlowCtx *) ((GlowWidget) w)->glow.glow_ctx;
    ctx->register_scroll_callback( (void *) scroll_data, scroll_callback);

    XtAddCallback( scroll_data->scroll_h, XmNvalueChangedCallback, scroll_h_action, w);
    XtAddCallback( scroll_data->scroll_h, XmNdragCallback, scroll_h_action, w);
    XtAddCallback( scroll_data->scroll_h, XmNincrementCallback, scroll_h_action, w);
    XtAddCallback( scroll_data->scroll_h, XmNdecrementCallback, scroll_h_action, w);
    XtAddCallback( scroll_data->scroll_h, XmNpageIncrementCallback, scroll_h_action, w);
    XtAddCallback( scroll_data->scroll_h, XmNpageDecrementCallback, scroll_h_action, w);
    XtAddCallback( scroll_data->scroll_h, XmNtoTopCallback, scroll_h_action, w);
    XtAddCallback( scroll_data->scroll_h, XmNtoBottomCallback, scroll_h_action, w);

    XtAddCallback( scroll_data->scroll_v, XmNvalueChangedCallback, scroll_v_action, w);
    XtAddCallback( scroll_data->scroll_v, XmNdragCallback, scroll_v_action, w);
    XtAddCallback( scroll_data->scroll_v, XmNincrementCallback, scroll_v_action, w);
    XtAddCallback( scroll_data->scroll_v, XmNdecrementCallback, scroll_v_action, w);
    XtAddCallback( scroll_data->scroll_v, XmNpageIncrementCallback, scroll_v_action, w);
    XtAddCallback( scroll_data->scroll_v, XmNpageDecrementCallback, scroll_v_action, w);
    XtAddCallback( scroll_data->scroll_v, XmNtoTopCallback, scroll_v_action, w);
    XtAddCallback( scroll_data->scroll_v, XmNtoBottomCallback, scroll_v_action, w);
  }
  return (((GlowWidget) w)->glow.init_proc)( ctx, client_data);
}

static void scroll_h_action( 	Widget 		w,
				XtPointer 	client_data,
				XtPointer 	call_data)
{
  XmScrollBarCallbackStruct *cbs = (XmScrollBarCallbackStruct *) call_data;
  GlowCtx *ctx = (GlowCtx *) ((GlowWidget) client_data)->glow.glow_ctx;

  switch( cbs->reason)
  {
    case XmCR_DRAG:
    case XmCR_VALUE_CHANGED:
    case XmCR_INCREMENT:
    case XmCR_DECREMENT:
    case XmCR_PAGE_INCREMENT:
    case XmCR_PAGE_DECREMENT:
    case XmCR_TO_TOP:
    case XmCR_TO_BOTTOM:
      glow_scroll_horizontal( ctx, cbs->value, 0);
      break;
  }
}

static void scroll_v_action( 	Widget 		w,
				XtPointer 	client_data,
				XtPointer 	call_data)
{

  XmScrollBarCallbackStruct *cbs = (XmScrollBarCallbackStruct *) call_data;
  GlowCtx *ctx = (GlowCtx *) ((GlowWidget) client_data)->glow.glow_ctx;
  Arg 		arg[20];
  int		i;
  int		maximum, slider, value, bottom;


  // Calculate if position is bottom
  i = 0;
  XtSetArg( arg[i], XmNmaximum, &maximum);i++;
  XtSetArg( arg[i], XmNsliderSize, &slider);i++;
  XtSetArg( arg[i], XmNvalue, &value);i++;
  XtGetValues( w, arg, i);
  if ( slider + value == maximum)
    bottom = 1;
  else
    bottom = 0;

  switch( cbs->reason)
  {
    case XmCR_DRAG:
    case XmCR_VALUE_CHANGED:
    case XmCR_INCREMENT:
    case XmCR_DECREMENT:
    case XmCR_PAGE_INCREMENT:
    case XmCR_PAGE_DECREMENT:
    case XmCR_TO_TOP:
    case XmCR_TO_BOTTOM:
      glow_scroll_vertical( ctx, cbs->value, bottom);
      break;
  }
}

static void scroll_callback( glow_sScroll *data)
{
  glowwidget_sScroll *scroll_data;
  Arg 		arg[20];
  int		i;

  scroll_data = (glowwidget_sScroll *) data->scroll_data;

  if ( data->total_width <= data->window_width)
  {
    if ( data->offset_x == 0)
      data->total_width = data->window_width;
    if ( scroll_data->scroll_h_managed)
    {
      // Remove horizontal scrollbar
    }
  }
  else
  {
    if ( !scroll_data->scroll_h_managed)
    {
      // Insert horizontal scrollbar
    }
  }

  if ( data->total_height <= data->window_height)
  {
    if ( data->offset_y == 0)
      data->total_height = data->window_height;
    if ( scroll_data->scroll_v_managed)
    {
      // Remove vertical scrollbar
    }
  }
  else
  {
    if ( !scroll_data->scroll_v_managed)
    {
      // Insert vertical scrollbar
    }
  }
  if ( data->offset_x < 0)
    data->offset_x = 0;
  if ( data->offset_y < 0)
    data->offset_y = 0;
  if ( data->total_height < data->window_height + data->offset_y)
    data->total_height = data->window_height + data->offset_y;
  if ( data->total_width < data->window_width + data->offset_x)
    data->total_width = data->window_width + data->offset_x;
  if ( data->window_width < 1)
    data->window_width = 1;
  if ( data->window_height < 1)
    data->window_height = 1;

  if ( scroll_data->scroll_h_managed)
  {
    i = 0;
    XtSetArg( arg[i], XmNmaximum, data->total_width);i++;
    XtSetArg( arg[i], XmNsliderSize, data->window_width);i++;
    XtSetArg( arg[i], XmNvalue, data->offset_x);i++;
    XtSetValues( scroll_data->scroll_h, arg, i);
  }

  if ( scroll_data->scroll_v_managed)
  {
    i = 0;
    XtSetArg( arg[i], XmNmaximum, data->total_height);i++;
    XtSetArg( arg[i], XmNsliderSize, data->window_height);i++;
    XtSetArg( arg[i], XmNvalue, data->offset_y);i++;
    XtSetValues( scroll_data->scroll_v, arg, i);
  }
}

static void Initialize( Widget rec, Widget new_widget, ArgList arg, int *args)
{
/*
  GlowWidget w;

  XtManageChild( new_widget);
  w = (GlowWidget) new_widget;
*/
}  

static void Redisplay( Widget w, XEvent *event, Region region)
{

  draw_event_handler( (GlowCtx *)((GlowWidget)w)->glow.glow_ctx, *event);
}

static void Notify( Widget w, XEvent *event)
{
  
  draw_event_handler( (GlowCtx *)((GlowWidget)w)->glow.glow_ctx, *event);
}

static Boolean SetValues( Widget old, Widget request, Widget new_widget)
{
  return 0;
}

static void Destroy( Widget w)
{
  if ( ((GlowWidget) w)->glow.is_navigator)
    return;
  glow_draw_delete( (GlowCtx *)((GlowWidget)w)->glow.glow_ctx);
}

static void Realize( Widget w, unsigned long *mask, XSetWindowAttributes *swa)
{

  (* glowWidgetClass->core_class.superclass->core_class.realize)
	(w, mask, swa);

  if ( ((GlowWidget) w)->glow.is_navigator)
  {
    if ( !((GlowWidget) w)->glow.glow_ctx)
    {
      GlowWidget main_glow = (GlowWidget) ((GlowWidget) w)->glow.main_glow_widget;

      ((GlowWidget) w)->glow.glow_ctx = main_glow->glow.glow_ctx;
      glow_draw_init_nav( w, main_glow->glow.glow_ctx);
    }
  }
  else
  {
    if ( !((GlowWidget) w)->glow.glow_ctx)
    {
      glow_draw_init( w, &((GlowWidget) w)->glow.glow_ctx, 
		glow_init_proc, 
		((GlowWidget) w)->glow.client_data,
		glow_eCtxType_Glow);
    }
  }
}

extern "C" Widget GlowCreate( 
	Widget parent, 
	char *name, 
	ArgList args, 
	int argCount,
        int	(*init_proc)(GlowCtx *ctx, void *client_data),
	void *client_data
	)
{
  GlowWidget w;

  w = (GlowWidget) XtCreateWidget( name, glowWidgetClass, parent, args, 
		argCount);
  w->glow.init_proc = init_proc;
  w->glow.glow_ctx = 0;
  w->glow.is_navigator = 0;
  w->glow.client_data = client_data;
  w->glow.scrollbars = 0;
  return (Widget) w;
}

extern "C" Widget ScrolledGlowCreate( 
	Widget parent, 
	char *name, 
	ArgList args, 
	int argCount,
        int	(*init_proc)(GlowCtx *ctx, void *client_data),
	void *client_data, 
	Widget *glow_w
)
{
  Widget 	form, scroll_h, scroll_v;
  GlowWidget 	glow;
  Arg 		arg[20];
  int		i;
  int		scroll_width = 15;

  form = XtCreateWidget( name, xmFormWidgetClass, parent, args,
		argCount);

  i = 0;
  XtSetArg( arg[i], XmNorientation, XmHORIZONTAL); i++;
  XtSetArg( arg[i], XmNrightAttachment, XmATTACH_FORM);i++;
  XtSetArg( arg[i], XmNleftAttachment, XmATTACH_FORM);i++;
  XtSetArg( arg[i], XmNbottomAttachment, XmATTACH_FORM);i++;
  XtSetArg( arg[i], XmNheight, scroll_width);i++;
  XtSetArg( arg[i], XmNrightOffset, scroll_width);i++;
  scroll_h = XtCreateWidget( "scroll_horizontal", xmScrollBarWidgetClass,
		form, arg, i);
  XtManageChild( scroll_h);

  i = 0;
  XtSetArg( arg[i], XmNorientation, XmVERTICAL); i++;
  XtSetArg( arg[i], XmNtopAttachment, XmATTACH_FORM);i++;
  XtSetArg( arg[i], XmNrightAttachment, XmATTACH_FORM);i++;
  XtSetArg( arg[i], XmNbottomAttachment, XmATTACH_FORM);i++;
  XtSetArg( arg[i], XmNwidth, scroll_width);i++;
  XtSetArg( arg[i], XmNbottomOffset, scroll_width);i++;
  scroll_v = XtCreateWidget( "scroll_vertical", xmScrollBarWidgetClass,
		form, arg, i);
  XtManageChild( scroll_v);

  i = 0;
  XtSetArg( arg[i], XmNtopAttachment, XmATTACH_FORM);i++;
  XtSetArg( arg[i], XmNleftAttachment, XmATTACH_FORM);i++;
  XtSetArg( arg[i], XmNbottomAttachment, XmATTACH_WIDGET);i++;
  XtSetArg( arg[i], XmNbottomWidget, scroll_h);i++;
  XtSetArg( arg[i], XmNrightAttachment, XmATTACH_WIDGET);i++;
  XtSetArg( arg[i], XmNrightWidget, scroll_v);i++;
/*
  XtSetArg( arg[i], XmNbottomAttachment, XmATTACH_FORM);i++;
  XtSetArg( arg[i], XmNrightAttachment, XmATTACH_FORM);i++;
*/
  glow = (GlowWidget) GlowCreate( form, "glow", arg, i, init_proc, client_data);
  XtManageChild( (Widget) glow);

  glow->glow.scroll_h = scroll_h;
  glow->glow.scroll_v = scroll_v;
  glow->glow.form = form;
  glow->glow.scrollbars = 1;

  *glow_w = (Widget) glow;
  return (Widget) form;
}

Widget GlowCreateNav( Widget parent, char *name, ArgList args, int argCount,
	Widget main_glow)
{
  GlowWidget w;

  w = (GlowWidget) XtCreateWidget( name, glowWidgetClass, parent, args, argCount);
  w->glow.is_navigator = 1;
  w->glow.glow_ctx = 0;
  w->glow.main_glow_widget = main_glow;
  return (Widget) w;
}

