/* 
 * Proview   $Id: flow_browwidget_motif.cpp,v 1.1 2007-01-04 07:57:00 claes Exp $
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

#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef __OS_VMS
#pragma member_alignment
#endif

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

#include "flow.h"
#include "flow_browctx.h"
#include "flow_draw_xlib.h"
#include "flow_browwidget_motif.h"

typedef struct {
	Widget  brow;
	Widget	form;
	Widget	scroll_h;
	Widget	scroll_v;
	int	scroll_h_managed;
	int	scroll_v_managed;
	} browwidget_sScroll;

static XtGeometryResult GeometryManager( 
		Widget w, 
		XtWidgetGeometry *request,
		XtWidgetGeometry *reply);
static void Notify( Widget w, XEvent *event);
static void Destroy( Widget w);
static Boolean AcceptFocus( Widget w, Time *t);
static void Realize( Widget w, unsigned long *dum, XSetWindowAttributes *swa);
static void Initialize( Widget rec, Widget new_widget, ArgList arg, int *args);
static void Redisplay( Widget w, XEvent *event, Region region);
static Boolean SetValues( Widget old, Widget request, Widget new_widget);

static char defaultTranslations[] = "#replace \n\
<Btn1Up>: notify()\n\
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
<FocusIn>: notify()\n\
<Map>: notify()\n\
<Unmap>: notify()\n\
<Key>Up: notify()\n\
<Key>Down: notify()\n\
<KeyDown>: notify()";


static XtActionsRec actionsList[] = { {"notify", (XtActionProc) Notify}};

BrowClassRec browClassRec = {
  { /* Core class part */
    (WidgetClass) &compositeClassRec,	/* superclass */
    "Brow",				/* class name */
    sizeof(BrowRec),			/* widget size */
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
    AcceptFocus, 			/* accept focus */
    XtVersionDontCheck,			/* version */
    NULL, 				/* callback offsets */
    defaultTranslations,		/* tm_table */
    NULL,				/* geometry */
    NULL,				/* disp accelerators */
    NULL				/* extension */
  },
  { /* composite class record */
    (XtGeometryHandler) GeometryManager, /* geometry manager */
    NULL,				/* change managed */
    XtInheritInsertChild,		/* insert child */
    XtInheritDeleteChild,		/* delete child */
    NULL				/* extension */
  },
  { /* brow class record */
    NULL,
    0
  }
};

WidgetClass browWidgetClass = (WidgetClass) &browClassRec;

static void scroll_h_action( 	Widget 		w,
				XtPointer 	client_data,
				XtPointer 	call_data);
static void scroll_v_action( 	Widget 		w,
				XtPointer 	client_data,
				XtPointer 	call_data);
static void scroll_callback( flow_sScroll *data);

static int brow_init_proc( Widget w, FlowCtx *fctx, void *client_data)
{
  browwidget_sScroll *scroll_data;
  BrowCtx	*ctx;

  scroll_data = (browwidget_sScroll *) malloc( sizeof( browwidget_sScroll));
  scroll_data->brow = w;
  scroll_data->scroll_h = ((BrowWidget) w)->brow.scroll_h;
  scroll_data->scroll_v = ((BrowWidget) w)->brow.scroll_v;
  scroll_data->form = ((BrowWidget) w)->brow.form;
  scroll_data->scroll_h_managed = 1;
  scroll_data->scroll_v_managed = 1;

  ctx = (BrowCtx *) ((BrowWidget) w)->brow.brow_ctx;
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

  return (((BrowWidget) w)->brow.init_proc)( ctx, client_data);
}

static void scroll_h_action( 	Widget 		w,
				XtPointer 	client_data,
				XtPointer 	call_data)
{
  XmScrollBarCallbackStruct *cbs = (XmScrollBarCallbackStruct *) call_data;
  BrowCtx *ctx = (BrowCtx *) ((BrowWidget) client_data)->brow.brow_ctx;

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
      brow_scroll_horizontal( ctx, cbs->value, 0);
      break;
  }
}

static void scroll_v_action( 	Widget 		w,
				XtPointer 	client_data,
				XtPointer 	call_data)
{

  XmScrollBarCallbackStruct *cbs = (XmScrollBarCallbackStruct *) call_data;
  BrowCtx *ctx = (BrowCtx *) ((BrowWidget) client_data)->brow.brow_ctx;
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
      brow_scroll_vertical( ctx, cbs->value, bottom);
      break;
  }
}

static void scroll_callback( flow_sScroll *data)
{
  browwidget_sScroll *scroll_data;
  Arg 		arg[20];
  int		i;

  scroll_data = (browwidget_sScroll *) data->scroll_data;

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

static XtGeometryResult GeometryManager( 
		Widget w, 
		XtWidgetGeometry *request,
		XtWidgetGeometry *reply)
{
  if ( request->request_mode & CWX)
    w->core.x = request->x;
  if ( request->request_mode & CWY)
    w->core.y = request->y;
  if ( request->request_mode & CWWidth)
    w->core.width = request->width;
  if ( request->request_mode & CWHeight)
    w->core.height = request->height;
  if ( request->request_mode & CWBorderWidth)
    w->core.border_width = request->border_width;
  return XtGeometryYes;
}

static void Initialize( Widget rec, Widget new_widget, ArgList arg, int *args)
{
/*
  BrowWidget w;

  XtManageChild( new_widget);
  w = (BrowWidget) new_widget;
*/
}  

static void Redisplay( Widget w, XEvent *event, Region region)
{

  ((FlowDrawXLib *)((BrowCtx *)((BrowWidget)w)->brow.brow_ctx)->fdraw)->event_handler( (BrowCtx *)((BrowWidget)w)->brow.brow_ctx, *event);
}

static void Notify( Widget w, XEvent *event)
{
  
  ((FlowDrawXLib *)((BrowCtx *)((BrowWidget)w)->brow.brow_ctx)->fdraw)->event_handler( (BrowCtx *)((BrowWidget)w)->brow.brow_ctx, *event);
}

static Boolean SetValues( Widget old, Widget request, Widget new_widget)
{
  return 0;
}

static void Destroy( Widget w)
{
  if ( ((BrowWidget) w)->brow.is_navigator)
    return;
  delete ((FlowCtx *)((BrowWidget)w)->brow.brow_ctx)->fdraw;
}

static Boolean AcceptFocus( Widget w, Time *t)
{
  if ( ((BrowWidget) w)->brow.is_navigator)
    return 0;
  ((FlowCtx *)((BrowWidget)w)->brow.brow_ctx)->fdraw->set_inputfocus( (FlowCtx *)((BrowWidget)w)->brow.brow_ctx);
  return 1;
}

static void Realize( Widget w, unsigned long *mask, XSetWindowAttributes *swa)
{

  (* browWidgetClass->core_class.superclass->core_class.realize)
	(w, mask, swa);

  if ( ((BrowWidget) w)->brow.is_navigator)
  {
    if ( !((BrowWidget) w)->brow.brow_ctx)
    {
      BrowWidget main_brow = (BrowWidget) ((BrowWidget) w)->brow.main_brow_widget;

      ((BrowWidget) w)->brow.brow_ctx = main_brow->brow.brow_ctx;
      ((FlowDrawXLib *)((BrowCtx *)((BrowWidget) w)->brow.brow_ctx)->fdraw)->init_nav( w, main_brow->brow.brow_ctx);
    }
  }
  else
  {
    if ( !((BrowWidget) w)->brow.brow_ctx)
    {
      FlowDrawXLib *fdraw = new FlowDrawXLib( w, 
			  &((BrowWidget) w)->brow.brow_ctx, 
			  brow_init_proc, 
			  ((BrowWidget) w)->brow.client_data,
			  flow_eCtxType_Brow);
      ((BrowCtx *)((BrowWidget) w)->brow.brow_ctx)->fdraw = fdraw;
    }
  }
}

extern "C" Widget BrowCreate( 
	Widget parent, 
	char *name, 
	ArgList args, 
	int argCount,
        int (*init_proc)(FlowCtx *ctx, void *client_data),
	void *client_data
	)
{
  BrowWidget w;

  w = (BrowWidget) XtCreateWidget( name, browWidgetClass, parent, args, 
		argCount);
  w->brow.init_proc = init_proc;
  w->brow.brow_ctx = 0;
  w->brow.is_navigator = 0;
  w->brow.client_data = client_data;
  return (Widget) w;
}

extern "C" Widget ScrolledBrowCreate( 
	Widget parent, 
	char *name, 
	ArgList args, 
	int argCount,
        int	(*init_proc)(FlowCtx *ctx, void *client_data),
	void *client_data, 
	Widget *brow_w
)
{
  Widget 	form, scroll_h, scroll_v;
  BrowWidget 	brow;
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
  brow = (BrowWidget) BrowCreate( form, "brow", arg, i, init_proc, client_data);
  XtManageChild( (Widget) brow);

  brow->brow.scroll_h = scroll_h;
  brow->brow.scroll_v = scroll_v;
  brow->brow.form = form;

  *brow_w = (Widget) brow;
  return (Widget) form;
}

Widget BrowCreateNav( Widget parent, char *name, ArgList args, int argCount,
	Widget main_brow)
{
  BrowWidget w;

  w = (BrowWidget) XtCreateWidget( name, browWidgetClass, parent, args, argCount);
  w->brow.is_navigator = 1;
  w->brow.brow_ctx = 0;
  w->brow.main_brow_widget = main_brow;
  return (Widget) w;
}

void BrowCtxFromWidget( Widget w, void **ctx)
{
  *ctx = ((BrowWidget) w)->brow.brow_ctx;
}

