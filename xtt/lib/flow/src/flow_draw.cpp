#include "flow_std.h"

#include <stdio.h>
#include <stdlib.h>


#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Mrm/MrmPublic.h>
#include <Xm/Text.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#define XK_MISCELLANY
#include <X11/keysymdef.h>

#include "flow.h"
#include "flow_ctx.h"
#include "flow_browctx.h"
#include "flow_draw.h"
#include "flow_widget.h"
#include "flow_browwidget.h"
#include "flow_msg.h"

#define DRAW_PRESS_PIX 9

typedef struct {
        Widget  toplevel;
        Widget	nav_shell;
	Widget	nav_toplevel;
        XtAppContext  app_ctx;
	Display	*display;
	Window	window;
	Window	nav_window;
	Screen	*screen;
	GC	gc;
	GC	gc_erase;
	GC	gc_inverse;
	GC	gc_yellow;
	GC	gc_green;
	GC	gcs[flow_eDrawType__][DRAW_TYPE_SIZE];
	XFontStruct	*font_struct[draw_eFont__][DRAW_FONT_SIZE];
	Font 	font[draw_eFont__][DRAW_FONT_SIZE];
        int	cursors[draw_eCursor__];
        int	ef;
        int	(*event_handler)(flow_eEvent event, int x, int y, int w, int h);
        int 	(*event_handler_nav)(flow_eEvent event, int x, int y);
	unsigned long background;
	XtIntervalId	timer_id;
        int     click_sensitivity;
} draw_sCtx, *draw_tCtx;

typedef struct {
	Widget		w;
	int		x;
	int		y;
	int		width;
	int		height;
	FlowArrayElem	*node;
	int		number;
        FlowCtx 	*ctx;
} draw_sAnnotData;

typedef struct {
	FlowCtx 	*ctx;
	void 		(*callback_func)( FlowCtx *ctx);
	XtIntervalId	timer_id;
} draw_sTimerCb;

typedef struct {
	Pixmap pixmap[DRAW_PIXMAP_SIZE];
	} draw_sPixmap;

static char	font_names[draw_eFont__][DRAW_FONT_SIZE][80] = { {	
	"-*-Helvetica-Bold-R-Normal--8-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Bold-R-Normal--10-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Bold-R-Normal--12-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Bold-R-Normal--14-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Bold-R-Normal--14-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Bold-R-Normal--18-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Bold-R-Normal--18-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Bold-R-Normal--18-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Bold-R-Normal--24-*-*-*-P-*-ISO8859-1"
	},{ 
	"-*-Helvetica-Medium-R-Normal--8-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Medium-R-Normal--10-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Medium-R-Normal--12-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Medium-R-Normal--14-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Medium-R-Normal--14-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Medium-R-Normal--18-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Medium-R-Normal--18-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Medium-R-Normal--18-*-*-*-P-*-ISO8859-1", 
	"-*-Helvetica-Medium-R-Normal--24-*-*-*-P-*-ISO8859-1"
	} };


static  XEvent	last_event;

static int flow_allocate_color( draw_tCtx draw_ctx, char *named_color);
static void event_timer( FlowCtx *ctx, int time_ms);
static void cancel_event_timer(FlowCtx *ctx);
static void event_timer_cb( FlowCtx *ctx);
static void draw_input_cb( Widget w, XtPointer client_data, XtPointer call_data);


static int flow_create_cursor( draw_tCtx draw_ctx)
{
  /* Create some cursors */

  /* Cross cursor */
  draw_ctx->cursors[draw_eCursor_CrossHair] = 
		XCreateFontCursor( draw_ctx->display, XC_crosshair);

  draw_ctx->cursors[draw_eCursor_DiamondCross] = 
		XCreateFontCursor( draw_ctx->display, XC_diamond_cross);

  draw_ctx->cursors[draw_eCursor_Hand] = 
		XCreateFontCursor( draw_ctx->display, XC_hand2);

  return 1;
}

static int draw_free_gc( draw_tCtx draw_ctx)
{
  int i, j;

  for ( i = 1; i < draw_eCursor__ ; i++)
    XFreeCursor( draw_ctx->display, draw_ctx->cursors[i]);

  XFreeGC( draw_ctx->display, draw_ctx->gc_yellow);
  XFreeGC( draw_ctx->display, draw_ctx->gc_green);
  XFreeGC( draw_ctx->display, draw_ctx->gc_inverse);
  for ( i = 0; i < flow_eDrawType__; i++)
  {
    for ( j = 0; j < DRAW_TYPE_SIZE; j++)
      XFreeGC( draw_ctx->display, draw_ctx->gcs[i][j]);
  }  

  for ( i = 0; i < draw_eFont__; i++)
  {
    for ( j = 0; j < DRAW_FONT_SIZE; j++)
    {
//      XUnloadFont( draw_ctx->display, draw_ctx->font[i][j]);
      XFreeFont( draw_ctx->display, draw_ctx->font_struct[i][j]);
    }
  }
  return 1;
}

static int flow_create_gc( draw_tCtx draw_ctx, Window window)
{
  Font 			font;
  XGCValues 		xgcv;
  int			i;

  /* Inverse gc */
  xgcv.background = XBlackPixelOfScreen(draw_ctx->screen);
  xgcv.foreground = draw_ctx->background;
  draw_ctx->gc_inverse = XCreateGC( draw_ctx->display, 
	window, GCForeground | GCBackground, &xgcv);

  /* Yellow gc */
  xgcv.foreground = flow_allocate_color( draw_ctx, "yellow");
  xgcv.background = draw_ctx->background;
  draw_ctx->gc_yellow = XCreateGC( draw_ctx->display, 
	window, GCForeground | GCBackground, &xgcv);

  /* Green gc */
  xgcv.foreground = flow_allocate_color( draw_ctx, "green");
  xgcv.background = draw_ctx->background;
  draw_ctx->gc_green = XCreateGC( draw_ctx->display, 
	window, GCForeground | GCBackground, &xgcv);

  /* Black line gc */
  xgcv.foreground = XBlackPixelOfScreen(draw_ctx->screen);
  xgcv.background = draw_ctx->background;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++)
  {
    xgcv.line_width = i + 1;

    draw_ctx->gcs[flow_eDrawType_Line][i] = XCreateGC( draw_ctx->display, 
	window, GCForeground | GCBackground | GCLineWidth, &xgcv);
  }

  /* Erase line gc */
  xgcv.foreground = draw_ctx->background;
  xgcv.background = draw_ctx->background;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++)
  {
    xgcv.line_width = i + 1;

    draw_ctx->gcs[flow_eDrawType_LineErase][i] = XCreateGC( draw_ctx->display, 
	window, GCForeground | GCBackground | GCLineWidth, &xgcv);
  }

  /* Red line gc */
  xgcv.foreground = flow_allocate_color( draw_ctx, "red");
  xgcv.background = draw_ctx->background;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++)
  {
    xgcv.line_width = i + 1;

    draw_ctx->gcs[flow_eDrawType_LineRed][i] = XCreateGC( draw_ctx->display, 
	window, GCForeground | GCBackground | GCLineWidth, &xgcv);
  }

  /* Gray line gc */
  xgcv.foreground = flow_allocate_color( draw_ctx, "gray");
  xgcv.background = draw_ctx->background;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++)
  {
    xgcv.line_width = i + 1;

    draw_ctx->gcs[flow_eDrawType_LineGray][i] = XCreateGC( draw_ctx->display, 
	window, GCForeground | GCBackground | GCLineWidth, &xgcv);
  }

  /* Dashed line gc */
  xgcv.foreground = XBlackPixelOfScreen(draw_ctx->screen);
  xgcv.background = draw_ctx->background;
  xgcv.line_style = LineOnOffDash;
  xgcv.dash_offset = 0;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++)
  {
    xgcv.line_width = i + 1;
    xgcv.dashes = 7 + i;

    draw_ctx->gcs[flow_eDrawType_LineDashed][i] = XCreateGC( draw_ctx->display, 
	window, 
	GCForeground | GCBackground | GCLineWidth| GCLineStyle | GCDashOffset |
	GCDashList, &xgcv);
  }

  /* Red dashed line gc */
  xgcv.foreground = flow_allocate_color( draw_ctx, "red");
  xgcv.background = draw_ctx->background;
  xgcv.line_style = LineOnOffDash;
  xgcv.dash_offset = 0;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++)
  {
    xgcv.line_width = i + 1;
    xgcv.dashes = 7 + i;

    draw_ctx->gcs[flow_eDrawType_LineDashedRed][i] = XCreateGC( draw_ctx->display, 
	window, 
	GCForeground | GCBackground | GCLineWidth| GCLineStyle | GCDashOffset |
	GCDashList, &xgcv);
  }

  /* Text */
  xgcv.foreground = XBlackPixelOfScreen(draw_ctx->screen);
  xgcv.background = draw_ctx->background;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++)
  {
    draw_ctx->gcs[flow_eDrawType_TextHelvetica][i] = XCreateGC( draw_ctx->display, 
	window, GCForeground | GCBackground, &xgcv);
  }

  for ( i = 0; i < DRAW_TYPE_SIZE; i++)
  {
    draw_ctx->gcs[flow_eDrawType_TextHelveticaBold][i] = XCreateGC( draw_ctx->display, 
	window, GCForeground | GCBackground, &xgcv);
  }

  xgcv.foreground = draw_ctx->background;
  xgcv.background = draw_ctx->background;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++)
  {
    draw_ctx->gcs[flow_eDrawType_TextHelveticaErase][i] = XCreateGC( draw_ctx->display, 
	window, GCForeground | GCBackground, &xgcv);
  }

  for ( i = 0; i < DRAW_TYPE_SIZE; i++)
  {
    draw_ctx->gcs[flow_eDrawType_TextHelveticaEraseBold][i] = XCreateGC( draw_ctx->display, 
	window, GCForeground | GCBackground, &xgcv);
  }

  for ( i = 0; i < DRAW_FONT_SIZE; i++)
  {
    font = XLoadFont( draw_ctx->display, 
	font_names[draw_eFont_HelveticaBold][i]);
    XSetFont( draw_ctx->display, 
	draw_ctx->gcs[flow_eDrawType_TextHelveticaBold][i], font);
    XSetFont( draw_ctx->display, 
	draw_ctx->gcs[flow_eDrawType_TextHelveticaEraseBold][i], font);
    draw_ctx->font[draw_eFont_HelveticaBold][i] = font;
    draw_ctx->font_struct[draw_eFont_HelveticaBold][i] =
		XQueryFont( draw_ctx->display, font);
  }
  for ( i = 0; i < DRAW_FONT_SIZE; i++)
  {
    font = XLoadFont( draw_ctx->display, font_names[draw_eFont_Helvetica][i]);
    XSetFont( draw_ctx->display, 
	draw_ctx->gcs[flow_eDrawType_TextHelvetica][i], font);
    XSetFont( draw_ctx->display, 
	draw_ctx->gcs[flow_eDrawType_TextHelveticaErase][i], font);
    draw_ctx->font[draw_eFont_Helvetica][i] = font;
    draw_ctx->font_struct[draw_eFont_Helvetica][i] =
		XQueryFont( draw_ctx->display, font);
  }

  return 1;
}


static int flow_allocate_color( draw_tCtx draw_ctx, char *named_color)
{
  XColor exact_color, screen_color;
  Screen *screen;
  Visual *visual;

  screen = draw_ctx->screen;
  visual = XDefaultVisualOfScreen( screen);
  if ( visual->c_class == TrueColor ||
       visual->c_class == PseudoColor ||
       visual->c_class == DirectColor ||
       visual->c_class == StaticColor)
  {
    if ( XAllocNamedColor( draw_ctx->display, 
		XDefaultColormapOfScreen( draw_ctx->screen),
	    	named_color, &screen_color, &exact_color))
      return screen_color.pixel;
    else
      printf( "** Color not allocated !\n");
    return XBlackPixelOfScreen( draw_ctx->screen);
  }
  else
  {
    return XBlackPixelOfScreen( draw_ctx->screen);
  }
}

void flow_draw_delete( FlowCtx *ctx)
{
  draw_tCtx	draw_ctx = (draw_tCtx) ctx->draw_ctx;

  ctx->set_nodraw();
  delete ctx;
  draw_free_gc( draw_ctx);
  free( (char *) draw_ctx);
}

int flow_draw_create_secondary_ctx( 
	FlowCtx *flow_ctx,
	void **secondary_flow_ctx,
        int (*init_proc)(FlowCtx *ctx, void *client_data),
	void  *client_data, 
	flow_eCtxType type
)
{
  FlowCtx	*ctx;

  if ( type == flow_eCtxType_Brow)
    ctx = (FlowCtx *) new BrowCtx("Claes context", 20);
  else
    ctx = new FlowCtx("Claes context", 20);
  
  ctx->draw_ctx = flow_ctx->draw_ctx;
  ctx->set_nodraw();
  ctx->trace_connect_func = flow_ctx->trace_connect_func;
  ctx->trace_disconnect_func = flow_ctx->trace_disconnect_func;
  ctx->trace_scan_func = flow_ctx->trace_scan_func;
  ctx->trace_started = flow_ctx->trace_started;
  ctx->zoom_factor = flow_ctx->zoom_factor;
  if ( type == flow_eCtxType_Brow)
  {
    ((BrowCtx *)ctx)->scroll_callback = ((BrowCtx *)flow_ctx)->scroll_callback;
    ((BrowCtx *)ctx)->scroll_data = ((BrowCtx *)flow_ctx)->scroll_data;
  }

  init_proc( ctx, client_data);
  *secondary_flow_ctx = (void *) ctx;
  return 1;
}

int flow_draw_change_ctx( 
	Widget w,
	FlowCtx *from_ctx,
	FlowCtx *to_ctx
)
{
  if ( from_ctx->type() == flow_eCtxType_Brow)
    ((BrowWidget)w)->brow.brow_ctx = to_ctx;
  else
    ((FlowWidget)w)->flow.flow_ctx = to_ctx;

  to_ctx->window_width = from_ctx->window_width;
  to_ctx->window_height = from_ctx->window_height;
  from_ctx->set_nodraw();
  to_ctx->reset_nodraw();
  if ( from_ctx->type() == flow_eCtxType_Brow)
  {
    to_ctx->zoom_factor = from_ctx->zoom_factor;
    ((BrowCtx *)to_ctx)->configure(0);
  }
  else
    to_ctx->redraw();
  return 1;
}

void flow_draw_delete_secondary_ctx( FlowCtx *ctx)
{
  ctx->set_nodraw();
  delete ctx;
}

int flow_draw_init( 
	Widget toplevel, 
	void **flow_ctx,
        int (*init_proc)(Widget w, FlowCtx *ctx, void *client_data),
	void  *client_data, 
	flow_eCtxType type
)
{
  int		i;
  draw_tCtx	draw_ctx;
  Arg 		args[20];
  FlowCtx	*ctx;

  if ( type == flow_eCtxType_Brow)
    ctx = (FlowCtx *) new BrowCtx("Claes context", 20);
  else
    ctx = new FlowCtx("Claes context", 20);
  *flow_ctx = (void *) ctx;
  
  draw_ctx = (draw_tCtx) calloc( 1, sizeof(*draw_ctx));
  ctx->draw_ctx = draw_ctx;
  draw_ctx->toplevel = toplevel;

//  sts = lib$get_ef( &draw_ctx->ef);

  draw_ctx->display = XtDisplay( draw_ctx->toplevel);
  draw_ctx->window = XtWindow( draw_ctx->toplevel);
  draw_ctx->screen = XtScreen( draw_ctx->toplevel);

/*
  i = 0;
  XtSetArg(args[i],XmNbackground, XWhitePixelOfScreen(draw_ctx->screen)); i++;
  XtSetValues( draw_ctx->toplevel ,args,i);
*/
  i = 0;
  XtSetArg(args[i],XmNbackground, &draw_ctx->background); i++;
  XtGetValues( draw_ctx->toplevel ,args,i);

#if 0
  if ( navigator_popup )
  {
    /* Create shell for navigator */

    i = 0;
    XtSetArg(args[i],XmNallowShellResize, TRUE); i++;
    XtSetArg(args[i],XmNallowResize, TRUE); i++;
    XtSetArg(args[i],XmNwidth,200);i++;
    XtSetArg(args[i],XmNheight,200);i++;
    XtSetArg(args[i],XmNx,500);i++;
    XtSetArg(args[i],XmNy,500);i++;

/**
    draw_ctx->nav_shell = XtAppCreateShell( "Navigator", "navclass", 
	applicationShellWidgetClass, draw_ctx->display, args, i);
    XtRealizeWidget( draw_ctx->nav_shell);
***/
    draw_ctx->nav_shell = XmCreateDialogShell( draw_ctx->toplevel, "Navigator",
	args, i);
    XtManageChild( draw_ctx->nav_shell);


    i = 0;
    XtSetArg(args[i],XmNwidth,200);i++;
    XtSetArg(args[i],XmNheight,200);i++;
/*  XtSetArg(args[i],XmNbackground, XWhitePixelOfScreen(draw_ctx->screen)); i++;*/
    draw_ctx->nav_toplevel = FlowCreateNav( draw_ctx->nav_shell, "navigator", 
	args, i, draw_ctx->toplevel);  

/*
    XtRealizeWidget( draw_ctx->nav_shell);
*/
    draw_ctx->nav_window = XtWindow( draw_ctx->nav_toplevel);
  }
#endif
#if 0
  {



    XSizeHints xsh;
    XSetWindowAttributes xswa;

    xswa.event_mask = ExposureMask | ButtonPressMask | Button1MotionMask |
	Button2MotionMask | Button3MotionMask | ButtonReleaseMask |
	EnterWindowMask | LeaveWindowMask | PointerMotionMask |
	KeyPressMask | KeyReleaseMask;
    xswa.background_pixel = XWhitePixelOfScreen(draw_ctx->screen);

    /* Navigator window */
    draw_ctx->nav_window = XCreateWindow( draw_ctx->display, XRootWindowOfScreen(draw_ctx->screen), 
	0, 0, 200, 200, 4, 
	XDefaultDepthOfScreen(draw_ctx->screen), InputOutput, 
	XDefaultVisualOfScreen(draw_ctx->screen), CWEventMask | CWBackPixel, &xswa);

    xsh.x = 500;
    xsh.y = 500;
    xsh.width = 200;
    xsh.height = 200;
    xsh.flags = PPosition | PSize;
    XSetNormalHints( draw_ctx->display, draw_ctx->nav_window, &xsh);
    XStoreName( draw_ctx->display, draw_ctx->nav_window, "Navigator");

  }
#endif

  flow_create_gc( draw_ctx, draw_ctx->window);

  flow_create_cursor( draw_ctx);

  init_proc( toplevel, ctx, client_data);

  return 1;
}


int flow_draw_init_nav( Widget nav_widget, void *flow_ctx)
{
  draw_tCtx	draw_ctx;

  draw_ctx = (draw_tCtx)((FlowCtx *)flow_ctx)->draw_ctx;
  draw_ctx->nav_toplevel = nav_widget;
  draw_ctx->nav_window = XtWindow( draw_ctx->nav_toplevel);

//  flow_create_gc( draw_ctx, draw_ctx->nav_window);

  ((FlowCtx *)flow_ctx)->no_nav = 0;
  return 1;
}


int draw_event_handler( FlowCtx *ctx, XEvent event)
{
  static int	button_pressed = 0;
  static int	button_clicked = 0;
  static int	button_clicked_and_pressed = 0;
  static int 	button1_pressed = 0;
  static int 	button2_pressed = 0;
  static int 	button3_pressed = 0;
  static int	last_press_x = 0;
  static int	last_press_y = 0;
  draw_tCtx     draw_ctx;
  int           sts = 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

//  cout << "Event : button_pressed " << button_pressed << " clicked " << 
//	button_clicked << " c&p " << button_clicked_and_pressed << endl;

  if ( event.xany.window == draw_ctx->window)
  {
    switch ( event.type)
    {
      case KeyPress : 
      {
	KeySym keysym;
	Modifiers mod;

	XtTranslateKeycode( draw_ctx->display, event.xkey.keycode, 0, &mod,
		&keysym);
	keysym &= 0xFFFF;
	switch ( keysym) 
        {
          case XK_Return:
            sts = ctx->event_handler( flow_eEvent_Key_Return, 0, 0, 0, 0);
//            printf( "-- Return key event\n");
	    break;
          case XK_Up:
            sts = ctx->event_handler( flow_eEvent_Key_Up, 0, 0, 0, 0);
	    break;
          case XK_Down:
            sts = ctx->event_handler( flow_eEvent_Key_Down, 0, 0, 0, 0);
	    break;
          case XK_Right:
	    if ( event.xkey.state & ShiftMask)
              sts = ctx->event_handler( flow_eEvent_Key_ShiftRight, 0, 0, 0, 0);
            else
              sts = ctx->event_handler( flow_eEvent_Key_Right, 0, 0, 0, 0);
	    break;
          case XK_Left:
	    if ( event.xkey.state & ShiftMask)
              sts = ctx->event_handler( flow_eEvent_Key_ShiftLeft, 0, 0, 0, 0);
            else
              sts = ctx->event_handler( flow_eEvent_Key_Left, 0, 0, 0, 0);
	    break;
          case XK_Page_Up:
            sts = ctx->event_handler( flow_eEvent_Key_PageUp, 0, 0, 0, 0);
	    break;
          case XK_Page_Down:
            sts = ctx->event_handler( flow_eEvent_Key_PageDown, 0, 0, 0, 0);
	    break;
          case XK_BackSpace:
            sts = ctx->event_handler( flow_eEvent_Key_BackSpace, 0, 0, 0, 0);
	    break;
          case XK_KP_F1:
            sts = ctx->event_handler( flow_eEvent_Key_PF1, 0, 0, 0, 0);
	    break;
          case XK_KP_F2:
            sts = ctx->event_handler( flow_eEvent_Key_PF2, 0, 0, 0, 0);
	    break;
          case XK_KP_F3:
            sts = ctx->event_handler( flow_eEvent_Key_PF3, 0, 0, 0, 0);
	    break;
          case XK_KP_F4:
            sts = ctx->event_handler( flow_eEvent_Key_PF4, 0, 0, 0, 0);
	    break;
          case XK_Tab:
            sts = ctx->event_handler( flow_eEvent_Key_Tab, 0, 0, 0, 0);
	    break;
	  default:
            ;
        }
        break;
      }
      case ButtonPress : 

//        printf( "-- Button event: (%d,%d) button: %d time:%d\n", event.xbutton.x, 
//		event.xbutton.y, event.xbutton.button, event.xbutton.time);

//        XSetInputFocus( draw_ctx->display, draw_ctx->window, 
//		RevertToNone, CurrentTime);

        switch ( event.xbutton.button)
        {
          case Button3:
            ctx->event_handler( flow_eEvent_MB3Down, event.xbutton.x, event.xbutton.y, 0, 0);
	    if ( draw_ctx->click_sensitivity & flow_mSensitivity_MB3Press && 
	         !(draw_ctx->click_sensitivity & flow_mSensitivity_MB3DoubleClick) && 
	         !(draw_ctx->click_sensitivity & flow_mSensitivity_MB3Click)) {
              ctx->event_handler( flow_eEvent_MB3Press, 
				  event.xbutton.x, event.xbutton.y, 0, 0);
	      draw_ctx->click_sensitivity = 0;
	      return 1;
            }
            break;
	  default:
	    ;
        }
	/* Detect press or click event */
	if ( button_clicked)
        {
	  /* Wait for release */
          button_clicked_and_pressed = 1;
	  cancel_event_timer( ctx);
          button_clicked = 0;
          memcpy( &last_event, &event, sizeof(event));
          button_pressed = event.xbutton.button;
	  last_press_x = event.xbutton.x;
	  last_press_y = event.xbutton.y;
	  event_timer(ctx, 200);
          return 1;
        }
        if ( !button_pressed )
        {
          memcpy( &last_event, &event, sizeof(event));
          button_pressed = event.xbutton.button;
	  last_press_x = event.xbutton.x;
	  last_press_y = event.xbutton.y;
	  event_timer(ctx, 200);
          return 1;
        }
	else
        {    
// cout << "Button press detected" << endl;
          /* Press event, callback from timer */
	  button_pressed = 0;
          button_clicked_and_pressed = 0;
          switch ( event.xbutton.button)
          {
            case Button1:
              button1_pressed = 1;
	      if (       (event.xbutton.state & ShiftMask) && 
	                !(event.xbutton.state & ControlMask))
                sts = ctx->event_handler( flow_eEvent_MB1PressShift, event.xbutton.x, event.xbutton.y, 0, 0);
	      else if ( !(event.xbutton.state & ShiftMask) && 
	                 (event.xbutton.state & ControlMask))
                sts = ctx->event_handler( flow_eEvent_MB1PressCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	      else if (  (event.xbutton.state & ShiftMask) && 
	                 (event.xbutton.state & ControlMask))
                sts = ctx->event_handler( flow_eEvent_MB1PressShiftCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	      else
                ctx->event_handler( flow_eEvent_MB1Press, event.xbutton.x, event.xbutton.y, 0, 0);
              break;
            case Button2:
              button2_pressed = 1;
	      if (       (event.xbutton.state & ShiftMask) && 
	                !(event.xbutton.state & ControlMask))
                sts = ctx->event_handler( flow_eEvent_MB2PressShift, event.xbutton.x, event.xbutton.y, 0, 0);
	      else if ( !(event.xbutton.state & ShiftMask) && 
	                 (event.xbutton.state & ControlMask))
                sts = ctx->event_handler( flow_eEvent_MB2PressShift, event.xbutton.x, event.xbutton.y, 0, 0);
	      else if (  (event.xbutton.state & ShiftMask) && 
	                 (event.xbutton.state & ControlMask))
                sts = ctx->event_handler( flow_eEvent_MB2PressShiftCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	      else
                sts = ctx->event_handler( flow_eEvent_MB2Press, event.xbutton.x, event.xbutton.y, 0, 0);
              break;
            case Button3:
              button3_pressed = 1;
#if 0
	      if (       (event.xbutton.state & ShiftMask) && 
	                !(event.xbutton.state & ControlMask))
                sts = ctx->event_handler( flow_eEvent_MB3PressShift, event.xbutton.x, event.xbutton.y, 0, 0);
	      else if ( !(event.xbutton.state & ShiftMask) && 
	                 (event.xbutton.state & ControlMask))
                sts = ctx->event_handler( flow_eEvent_MB3PressShift, event.xbutton.x, event.xbutton.y, 0, 0);
	      else if (  (event.xbutton.state & ShiftMask) && 
	                 (event.xbutton.state & ControlMask))
                sts = ctx->event_handler( flow_eEvent_MB3PressShiftCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	      else
#endif
                sts = ctx->event_handler( flow_eEvent_MB3Press, event.xbutton.x, event.xbutton.y, 0, 0);
              break;
          }
        }
        break;

      case ButtonRelease:
        button1_pressed = 0;
        button2_pressed = 0;
        button3_pressed = 0;

//	cout << "Button release event " << event.xbutton.x << " " << event.xbutton.y << endl;

        if ( ! button_pressed )
        {
	  if ( button_clicked)
          {
            /* Button click, callback from timer */
// cout << "Button click detected state " << event.xbutton.state << endl;
            button_clicked = 0;
            switch ( event.xbutton.button)
            {
              case Button1:
	        if (       (event.xbutton.state & ShiftMask) && 
	                  !(event.xbutton.state & ControlMask))
                  sts = ctx->event_handler( flow_eEvent_MB1ClickShift, event.xbutton.x, event.xbutton.y, 0, 0);
	        else if ( !(event.xbutton.state & ShiftMask) && 
	                   (event.xbutton.state & ControlMask))
                  sts = ctx->event_handler( flow_eEvent_MB1ClickCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	        else if (  (event.xbutton.state & ShiftMask) && 
	                   (event.xbutton.state & ControlMask))
                  sts = ctx->event_handler( flow_eEvent_MB1ClickShiftCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	        else
                  sts = ctx->event_handler( flow_eEvent_MB1Click, event.xbutton.x, event.xbutton.y, 0, 0);
                break;
              case Button2:
	        if (       (event.xbutton.state & ShiftMask) && 
	                  !(event.xbutton.state & ControlMask))
                  sts = ctx->event_handler( flow_eEvent_MB2ClickShift, event.xbutton.x, event.xbutton.y, 0, 0);
	        else if ( !(event.xbutton.state & ShiftMask) && 
	                   (event.xbutton.state & ControlMask))
                  sts = ctx->event_handler( flow_eEvent_MB2ClickCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	        else if (  (event.xbutton.state & ShiftMask) && 
	                   (event.xbutton.state & ControlMask))
                  sts = ctx->event_handler( flow_eEvent_MB2ClickShiftCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	        else
                  sts = ctx->event_handler( flow_eEvent_MB2Click, event.xbutton.x, event.xbutton.y, 0, 0);
                break;
              case Button3:
#if 0
	        if (       (event.xbutton.state & ShiftMask) && 
	                  !(event.xbutton.state & ControlMask))
                  sts = ctx->event_handler( flow_eEvent_MB3ClickShift, event.xbutton.x, event.xbutton.y, 0, 0);
	        else if ( !(event.xbutton.state & ShiftMask) && 
	                   (event.xbutton.state & ControlMask))
                  sts = ctx->event_handler( flow_eEvent_MB3ClickCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	        else if (  (event.xbutton.state & ShiftMask) && 
	                   (event.xbutton.state & ControlMask))
                  sts = ctx->event_handler( flow_eEvent_MB3ClickShiftCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	        else
#endif
                  sts = ctx->event_handler( flow_eEvent_MB3Click, event.xbutton.x, event.xbutton.y, 0, 0);
                break;
	    }
          }
          else
          {
            /* Button release */
// cout << "Button release detected" << endl;
            sts = ctx->event_handler( flow_eEvent_ButtonRelease, event.xbutton.x, event.xbutton.y, 0, 0);
          }        
        }
        else
        {
	  /* Button click */
	  cancel_event_timer( ctx);
	  if ( ! button_clicked_and_pressed)
          {
//  cout << "Button first click detected" << endl;
            /* wait for button double click */
            memcpy( &last_event, &event, sizeof(event));
	    button_clicked = 1;
	    event_timer( ctx, 200);
	    button_pressed = 0;
            return 1;
          }
          else
          {
	    /* Button double click */ 
// cout << "Button double click detected" << endl;
	    cancel_event_timer( ctx);
	    button_clicked = 0;
            button_pressed = 0;
            button_clicked_and_pressed = 0;
            switch ( event.xbutton.button)
            {
              case Button1:
	        if (       (event.xbutton.state & ShiftMask) && 
	                  !(event.xbutton.state & ControlMask))
                  sts = ctx->event_handler( flow_eEvent_MB1DoubleClickShift, event.xbutton.x, event.xbutton.y, 0, 0);
	        else if ( !(event.xbutton.state & ShiftMask) && 
	                   (event.xbutton.state & ControlMask))
                  sts = ctx->event_handler( flow_eEvent_MB1DoubleClickCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	        else if (  (event.xbutton.state & ShiftMask) && 
	                   (event.xbutton.state & ControlMask))
                  sts = ctx->event_handler( flow_eEvent_MB1DoubleClickShiftCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	        else
                  sts = ctx->event_handler( flow_eEvent_MB1DoubleClick, event.xbutton.x, event.xbutton.y, 0, 0);
                break;
              case Button2:
	        if (       (event.xbutton.state & ShiftMask) && 
	                  !(event.xbutton.state & ControlMask))
                  sts = ctx->event_handler( flow_eEvent_MB2DoubleClickShift, event.xbutton.x, event.xbutton.y, 0, 0);
	        else if ( !(event.xbutton.state & ShiftMask) && 
	                   (event.xbutton.state & ControlMask))
                  sts = ctx->event_handler( flow_eEvent_MB2DoubleClickCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	        else if (  (event.xbutton.state & ShiftMask) && 
	                   (event.xbutton.state & ControlMask))
                  sts = ctx->event_handler( flow_eEvent_MB2DoubleClickShiftCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	        else
                  sts = ctx->event_handler( flow_eEvent_MB2DoubleClick, event.xbutton.x, event.xbutton.y, 0, 0);
                break;
              case Button3:
#if 0
	        if (       (event.xbutton.state & ShiftMask) && 
	                  !(event.xbutton.state & ControlMask))
                  sts = ctx->event_handler( flow_eEvent_MB3DoubleClickShift, event.xbutton.x, event.xbutton.y, 0, 0);
	        else if ( !(event.xbutton.state & ShiftMask) && 
	                   (event.xbutton.state & ControlMask))
                  sts = ctx->event_handler( flow_eEvent_MB3DoubleClickCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	        else if (  (event.xbutton.state & ShiftMask) && 
	                   (event.xbutton.state & ControlMask))
                  sts = ctx->event_handler( flow_eEvent_MB3DoubleClickShiftCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	        else
                  sts = ctx->event_handler( flow_eEvent_MB3DoubleClick, event.xbutton.x, event.xbutton.y, 0, 0);
#endif
                break;
            }
          }
        }
        break;
      case Expose:
//        printf( "-- Expose event.. x: %d, y: %d, w: %d, h: %d\n",
//		event.xexpose.x, 
//		event.xexpose.y, event.xexpose.width, event.xexpose.height);
        sts = ctx->event_handler( flow_eEvent_Exposure, event.xexpose.x, 
		event.xexpose.y, event.xexpose.width, event.xexpose.height);
        break;
      case VisibilityNotify : 
	switch ( event.xvisibility.state)
        {
          case VisibilityUnobscured:
            sts = ctx->event_handler( flow_eEvent_VisibilityUnobscured, 0, 0, 0, 0);
            break;
          default: 
            sts = ctx->event_handler( flow_eEvent_VisibilityObscured, 0, 0, 0, 0);
            break;
        }
        break;        
      case MotionNotify:
//            printf( "-- Button1 motion event: (%d,%d)\n", event.xbutton.x, 
//		event.xbutton.y);
        if ( button3_pressed) {
          button3_pressed = 0;
	  button_pressed = 0;
          button_clicked_and_pressed = 0;
        }

        if ( button_pressed && 
	     (abs( event.xbutton.x - last_press_x) > DRAW_PRESS_PIX ||
	      abs( event.xbutton.y - last_press_y) > DRAW_PRESS_PIX))
        {
//          printf( "Press: x %d last_x %d\n", event.xbutton.x, last_press_x);
//          printf( "       y %d last_y %d\n", event.xbutton.y, last_press_y);

          event.xbutton.x = last_press_x;
          event.xbutton.y = last_press_y;

	  /* Button press */
	  cancel_event_timer( ctx);
          switch ( button_pressed)
          {
            case Button1:
              button1_pressed = 1;
	      if (       (event.xbutton.state & ShiftMask) && 
	                !(event.xbutton.state & ControlMask))
                sts = ctx->event_handler( flow_eEvent_MB1PressShift, event.xbutton.x, event.xbutton.y, 0, 0);
	      else if ( !(event.xbutton.state & ShiftMask) && 
	                 (event.xbutton.state & ControlMask))
                sts = ctx->event_handler( flow_eEvent_MB1PressCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	      else if (  (event.xbutton.state & ShiftMask) && 
	                 (event.xbutton.state & ControlMask))
                sts = ctx->event_handler( flow_eEvent_MB1PressShiftCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	      else
                ctx->event_handler( flow_eEvent_MB1Press, event.xbutton.x, event.xbutton.y, 0, 0);
              break;
            case Button2:
              button2_pressed = 1;
	      if (       (event.xbutton.state & ShiftMask) && 
	                !(event.xbutton.state & ControlMask))
                sts = ctx->event_handler( flow_eEvent_MB2PressShift, event.xbutton.x, event.xbutton.y, 0, 0);
	      else if ( !(event.xbutton.state & ShiftMask) && 
	               (event.xbutton.state & ControlMask))
                sts = ctx->event_handler( flow_eEvent_MB2PressShift, event.xbutton.x, event.xbutton.y, 0, 0);
	      else if (  (event.xbutton.state & ShiftMask) && 
	               (event.xbutton.state & ControlMask))
                sts = ctx->event_handler( flow_eEvent_MB2PressShiftCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	      else
                sts = ctx->event_handler( flow_eEvent_MB2Press, event.xbutton.x, event.xbutton.y, 0, 0);
              break;
            case Button3:
              button3_pressed = 1;
                sts = ctx->event_handler( flow_eEvent_MB3Press, event.xbutton.x, event.xbutton.y, 0, 0);
              break;
          }
	  button_pressed = 0;
          button_clicked_and_pressed = 0;

        }
        if ( button1_pressed || button2_pressed || button3_pressed)
          sts = ctx->event_handler( flow_eEvent_ButtonMotion, event.xbutton.x, event.xbutton.y, 0, 0);
        else
          sts = ctx->event_handler( flow_eEvent_CursorMotion, event.xbutton.x, event.xbutton.y, 0, 0);
        break;
      case EnterNotify:
//        printf( "-- Enter event: (%d,%d)\n", event.xbutton.x, 
//		event.xbutton.y);
        sts = ctx->event_handler( flow_eEvent_Enter, event.xbutton.x, event.xbutton.y, 0, 0);
        break;          
      case LeaveNotify:
//        printf( "-- Leave event: (%d,%d)\n", event.xbutton.x, 
//		event.xbutton.y);

        sts = ctx->event_handler( flow_eEvent_Leave, event.xbutton.x, event.xbutton.y, 0, 0);
        break;          
      case MapNotify:
//        printf( "-- Enter event: (%d,%d)\n", event.xbutton.x, 
//		event.xbutton.y);
        sts = ctx->event_handler( flow_eEvent_Map, 0, 0, 0, 0);
        break;          
      case UnmapNotify:
//        printf( "-- Enter event: (%d,%d)\n", event.xbutton.x, 
//		event.xbutton.y);
        sts = ctx->event_handler( flow_eEvent_Unmap, 0, 0, 0, 0);
        break;          
      case FocusIn:
//        printf( "-- Focus in event\n");
        break;          
      default:
//        printf("-- Other event: %d \n", event.type);
        break;
     }
  }
  else if ( event.xany.window == draw_ctx->nav_window)
  {
    switch ( event.type)
    {
      case ButtonPress : 
//            printf( "-- Button event nav: (%d,%d) button: %d\n", event.xbutton.x, 
//		event.xbutton.y, event.xbutton.button);
        switch ( event.xbutton.button)
        {
          case Button1:
            button1_pressed = 1;
            sts = ctx->event_handler_nav( flow_eEvent_MB1Press, event.xbutton.x, event.xbutton.y);
            break;
          case Button2:
            button2_pressed = 1;
            sts = ctx->event_handler_nav( flow_eEvent_MB2Press, event.xbutton.x, event.xbutton.y);
            break;
          case Button3:
            button3_pressed = 1;
            break;
        }
        break;          
      case ButtonRelease : 
//        printf( "-- Button release event nav: (%d,%d)\n", event.xbutton.x, 
//		event.xbutton.y);
        switch ( event.xbutton.button)
        {
          case Button1:
            button1_pressed = 0;
            break;
          case Button2:
            button2_pressed = 0;
            break;
          case Button3:
            button3_pressed = 0;
            break;
        }
        sts = ctx->event_handler_nav( flow_eEvent_ButtonRelease, event.xbutton.x, event.xbutton.y);
        break;          
      case Expose:
//        printf( "-- Navigator expose event..\n" );
        sts = ctx->event_handler_nav( flow_eEvent_Exposure, event.xbutton.x, event.xbutton.y);
        break; 
      case MotionNotify:
//            printf( "-- Button1 motion event nav: (%d,%d)\n", event.xbutton.x, 
//		event.xbutton.y);
        if ( button1_pressed || button2_pressed || button3_pressed)
          sts = ctx->event_handler_nav( flow_eEvent_ButtonMotion, event.xbutton.x, event.xbutton.y);
        else
          sts = ctx->event_handler_nav( flow_eEvent_CursorMotion, event.xbutton.x, event.xbutton.y);
        break;          
     }
  }
  if ( sts != FLOW__DESTROYED)
    XFlush( draw_ctx->display);
  return 1;
}

void	draw_enable_event( FlowCtx *ctx, flow_eEvent event, 
		flow_eEventType event_type, 
		int (*event_cb)(FlowCtx *ctx, flow_tEvent event))
{
  ctx->enable_event( event, event_type, event_cb);
}

int flow_draw_rect( FlowCtx *ctx, int x, int y, int width, int height, 
	flow_eDrawType gc_type, int idx, int highlight)
{
  draw_tCtx draw_ctx;
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( gc_type == flow_eDrawType_LineGray && highlight)
    gc_type = flow_eDrawType_Line;

  XDrawRectangle( draw_ctx->display, draw_ctx->window, 
	draw_ctx->gcs[gc_type+highlight][idx], 
	x, y, width, height);
  return 1;
}

int flow_draw_rect_erase( FlowCtx *ctx, int x, int y, int width, int height,
	int idx)
{
  draw_tCtx draw_ctx;
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XDrawRectangle( draw_ctx->display, draw_ctx->window, 
	draw_ctx->gcs[flow_eDrawType_LineErase][idx], 
	x, y, width, height);
  return 1;
}

int flow_draw_nav_rect( FlowCtx *ctx, int x, int y, int width, int height,
	flow_eDrawType gc_type, int idx, int highlight)
{
  draw_tCtx draw_ctx;
  if ( ctx->no_nav || ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( gc_type == flow_eDrawType_LineGray && highlight)
    gc_type = flow_eDrawType_Line;

  XDrawRectangle( draw_ctx->display, draw_ctx->nav_window,
	draw_ctx->gcs[gc_type+highlight][idx], 
	x, y, width, height);
  return 1;
}

int flow_draw_nav_rect_erase( FlowCtx *ctx, int x, int y, int width, int height,
	int idx)
{
  draw_tCtx draw_ctx;
  if ( ctx->no_nav || ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XDrawRectangle( draw_ctx->display, draw_ctx->nav_window,
	draw_ctx->gcs[flow_eDrawType_LineErase][idx], 
	x, y, width, height);
  return 1;
}

int flow_draw_arrow( FlowCtx *ctx, int x1, int y1, int x2, int y2, 
	int x3, int y3,
	flow_eDrawType gc_type, int idx, int highlight)
{
  draw_tCtx draw_ctx;
  XPoint p[4] = {{x1,y1},{x2,y2},{x3,y3},{x1,y1}};
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( gc_type == flow_eDrawType_LineGray && highlight)
    gc_type = flow_eDrawType_Line;

  XFillPolygon( draw_ctx->display, draw_ctx->window,
	draw_ctx->gcs[gc_type+highlight][idx], p, 4, Complex, CoordModeOrigin);
  return 1;
}

int flow_draw_arrow_erase( FlowCtx *ctx, int x1, int y1, int x2, int y2, 
	int x3, int y3,
	int idx)
{
  XPoint p[4] = {{x1,y1},{x2,y2},{x3,y3},{x1,y1}};
  draw_tCtx draw_ctx;
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XFillPolygon( draw_ctx->display, draw_ctx->window,
	draw_ctx->gcs[flow_eDrawType_LineErase][idx],
	p, 4, Complex, CoordModeOrigin);
  return 1;
}

int flow_draw_nav_arrow( FlowCtx *ctx, int x1, int y1, int x2, int y2, 
	int x3, int y3,
	flow_eDrawType gc_type, int idx, int highlight)
{
  draw_tCtx draw_ctx;
  XPoint p[4] = {{x1,y1},{x2,y2},{x3,y3},{x1,y1}};
  if ( ctx->no_nav || ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( gc_type == flow_eDrawType_LineGray && highlight)
    gc_type = flow_eDrawType_Line;

  XFillPolygon( draw_ctx->display, draw_ctx->nav_window,
	draw_ctx->gcs[gc_type+highlight][idx], p, 4, Complex, CoordModeOrigin);
  return 1;
}

int flow_draw_nav_arrow_erase( FlowCtx *ctx, int x1, int y1, int x2, int y2, 
	int x3, int y3,
	int idx)
{
  XPoint p[4] = {{x1,y1},{x2,y2},{x3,y3},{x1,y1}};
  draw_tCtx draw_ctx;
  if ( ctx->no_nav || ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XFillPolygon( draw_ctx->display, draw_ctx->nav_window,
	draw_ctx->gcs[flow_eDrawType_LineErase][idx],
	p, 4, Complex, CoordModeOrigin);
  return 1;
}

int flow_draw_arc( FlowCtx *ctx, int x, int y, int width, int height, 
	int angel1, int angel2,
	flow_eDrawType gc_type, int idx, int highlight)
{
  draw_tCtx draw_ctx;
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( gc_type == flow_eDrawType_LineGray && highlight)
    gc_type = flow_eDrawType_Line;

  XDrawArc( draw_ctx->display, draw_ctx->window,
	draw_ctx->gcs[gc_type+highlight][idx], 
	x, y, width, height, angel1*64, angel2*64);
  return 1;
}

int flow_draw_arc_erase( FlowCtx *ctx, int x, int y, int width, int height,
	int angel1, int angel2,
	int idx)
{
  draw_tCtx draw_ctx;
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XDrawArc( draw_ctx->display, draw_ctx->window,
	draw_ctx->gcs[flow_eDrawType_LineErase][idx], 
	x, y, width, height, angel1*64, angel2*64);
  return 1;
}

int flow_draw_nav_arc( FlowCtx *ctx, int x, int y, int width, int height,
	int angel1, int angel2,
	flow_eDrawType gc_type, int idx, int highlight)
{
  draw_tCtx draw_ctx;
  if ( ctx->no_nav || ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( gc_type == flow_eDrawType_LineGray && highlight)
    gc_type = flow_eDrawType_Line;

  XDrawArc( draw_ctx->display, draw_ctx->nav_window,
	draw_ctx->gcs[gc_type+highlight][idx], 
	x, y, width, height, angel1*64, angel2*64);
  return 1;
}

int flow_draw_nav_arc_erase( FlowCtx *ctx, int x, int y, int width, int height,
	int angel1, int angel2,
	int idx)
{
  draw_tCtx draw_ctx;
  if ( ctx->no_nav || ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XDrawArc( draw_ctx->display, draw_ctx->nav_window,
	draw_ctx->gcs[flow_eDrawType_LineErase][idx], 
	x, y, width, height, angel1*64, angel2*64);
  return 1;
}

int flow_draw_line( FlowCtx *ctx, int x1, int y1, int x2, int y2,
	flow_eDrawType gc_type, int idx, int highlight)
{
  draw_tCtx draw_ctx;
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( gc_type == flow_eDrawType_LineGray && highlight)
    gc_type = flow_eDrawType_Line;

  XDrawLine( draw_ctx->display, draw_ctx->window,
	draw_ctx->gcs[gc_type+highlight][idx], 
	x1, y1, x2, y2);
  return 1;
}

int flow_draw_line_erase( FlowCtx *ctx, int x1, int y1, int x2, int y2,
	int idx)
{
  draw_tCtx draw_ctx;
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XDrawLine( draw_ctx->display, draw_ctx->window,
	draw_ctx->gcs[flow_eDrawType_LineErase][idx], 
	x1, y1, x2, y2);
  return 1;
}

int flow_draw_nav_line( FlowCtx *ctx, int x1, int y1, int x2, int y2,
	flow_eDrawType gc_type, int idx, int highlight)
{
  draw_tCtx draw_ctx;
  if ( ctx->no_nav || ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( gc_type == flow_eDrawType_LineGray && highlight)
    gc_type = flow_eDrawType_Line;

  XDrawLine( draw_ctx->display, draw_ctx->nav_window,
	draw_ctx->gcs[gc_type+highlight][idx], 
	x1, y1, x2, y2);
  return 1;
}

int flow_draw_nav_line_erase( FlowCtx *ctx, int x1, int y1, int x2, int y2,
	int idx)
{
  draw_tCtx draw_ctx;
  if ( ctx->no_nav || ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XDrawLine( draw_ctx->display, draw_ctx->nav_window,
	draw_ctx->gcs[flow_eDrawType_LineErase][idx], 
	x1, y1, x2, y2);
  return 1;
}

int flow_draw_text( FlowCtx *ctx, int x, int y, char *text, int len,
	flow_eDrawType gc_type, int idx, int highlight, int line)
{
  draw_tCtx draw_ctx;
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XDrawString( draw_ctx->display, draw_ctx->window,
	draw_ctx->gcs[gc_type][idx], 
	x, y, text, len);
  return 1;
}

int flow_draw_text_erase( FlowCtx *ctx, int x, int y, char *text, int len,
	flow_eDrawType gc_type, int idx, int line)
{
  draw_tCtx draw_ctx;
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( gc_type == flow_eDrawType_TextHelvetica)
    gc_type = flow_eDrawType_TextHelveticaErase;
  else if ( gc_type == flow_eDrawType_TextHelveticaBold)
    gc_type = flow_eDrawType_TextHelveticaEraseBold;
  XDrawString( draw_ctx->display, draw_ctx->window,
	draw_ctx->gcs[gc_type][idx], 
	x, y, text, len);
  return 1;
}

int flow_draw_nav_text( FlowCtx *ctx, int x, int y, char *text, int len,
	flow_eDrawType gc_type, int idx, int highlight, int line)
{
  draw_tCtx draw_ctx;
  if ( ctx->no_nav || ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XDrawString( draw_ctx->display, draw_ctx->nav_window,
	draw_ctx->gcs[gc_type][idx], 
	x, y, text, len);
  return 1;
}

int flow_draw_nav_text_erase( FlowCtx *ctx, int x, int y, char *text, int len,
	flow_eDrawType gc_type, int idx, int line)
{
  draw_tCtx draw_ctx;
  if ( ctx->no_nav || ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( gc_type == flow_eDrawType_TextHelvetica)
    gc_type = flow_eDrawType_TextHelveticaErase;
  else if ( gc_type == flow_eDrawType_TextHelveticaBold)
    gc_type = flow_eDrawType_TextHelveticaEraseBold;
  XDrawString( draw_ctx->display, draw_ctx->nav_window,
	draw_ctx->gcs[gc_type][idx], 
	x, y, text, len);
  return 1;
}

int flow_draw_pixmaps_create( FlowCtx *ctx, flow_sPixmapData *pixmap_data,
	void **pixmaps)
{
  draw_tCtx draw_ctx;
  draw_sPixmap *pms;
  flow_sPixmapDataElem *prev_pdata, *pdata = (flow_sPixmapDataElem *)pixmap_data;
  int	i;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  pms = (draw_sPixmap *) calloc( 1, sizeof( *pms));
  for ( i = 0; i < DRAW_PIXMAP_SIZE; i++)
  {
    if ( i == 0 || 
         (i > 0 && pdata->bits != prev_pdata->bits))
      pms->pixmap[i] = XCreateBitmapFromData( draw_ctx->display,
	draw_ctx->window, pdata->bits, pdata->width,
	pdata->height);
    else
      pms->pixmap[i] = pms->pixmap[i-1];
    prev_pdata = pdata;
    pdata++;
  }
  *pixmaps = (void *) pms;
  return 1;
}

void flow_draw_pixmaps_delete( FlowCtx *ctx, void *pixmaps)
{
  draw_tCtx draw_ctx;
  draw_sPixmap *pms;
  int i;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;
  pms = (draw_sPixmap *) pixmaps;
  for ( i = 0; i < DRAW_PIXMAP_SIZE; i++)
  {
    if ( i == 0 || 
         (i > 0 && pms->pixmap[i] != pms->pixmap[i-1]))
      XFreePixmap( draw_ctx->display, pms->pixmap[i]);
  }
  free ( pixmaps);
}

int flow_draw_pixmap( FlowCtx *ctx, int x, int y, flow_sPixmapData *pixmap_data,
	void *pixmaps, flow_eDrawType gc_type, int idx, int highlight, int line)
{
  draw_tCtx draw_ctx;
  draw_sPixmap *pms;
  flow_sPixmapDataElem *pdata = (flow_sPixmapDataElem *)pixmap_data + idx;

  if ( ctx->nodraw) return 1;
  
  draw_ctx = (draw_tCtx) ctx->draw_ctx;
  pms = (draw_sPixmap *) pixmaps;

  XCopyPlane( draw_ctx->display, pms->pixmap[idx], draw_ctx->window,
	draw_ctx->gcs[gc_type][idx], 
	0, 0, pdata->width, pdata->height, x, y, 1);
  return 1;
}

int flow_draw_pixmap_inverse( FlowCtx *ctx, int x, int y, flow_sPixmapData *pixmap_data,
	void *pixmaps, flow_eDrawType gc_type, int idx, int line)
{
  draw_tCtx draw_ctx;
  draw_sPixmap *pms;
  flow_sPixmapDataElem *pdata = (flow_sPixmapDataElem *)pixmap_data + idx;

  if ( ctx->nodraw) return 1;
  
  draw_ctx = (draw_tCtx) ctx->draw_ctx;
  pms = (draw_sPixmap *) pixmaps;

  XCopyPlane( draw_ctx->display, pms->pixmap[idx], draw_ctx->window,
	draw_ctx->gc_inverse,
	0, 0, pdata->width, pdata->height, x, y, 1);
  return 1;
}

int flow_draw_pixmap_erase( FlowCtx *ctx, int x, int y, flow_sPixmapData *pixmap_data,
	void *pixmaps, flow_eDrawType gc_type, int idx, int line)
{
  draw_tCtx draw_ctx;
  flow_sPixmapDataElem *pdata = (flow_sPixmapDataElem *)pixmap_data + idx;
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XClearArea( draw_ctx->display, draw_ctx->window, 
	x, y, pdata->width, pdata->height, False);
  return 1;
}

int flow_draw_nav_pixmap( FlowCtx *ctx, int x, int y, flow_sPixmapData *pixmap_data,
	void *pixmaps, flow_eDrawType gc_type, int idx, int highlight, int line)
{
  draw_tCtx draw_ctx;
  draw_sPixmap *pms;
  flow_sPixmapDataElem *pdata = (flow_sPixmapDataElem *)pixmap_data + idx;

  if ( ctx->no_nav || ctx->nodraw) return 1;
  
  draw_ctx = (draw_tCtx) ctx->draw_ctx;
  pms = (draw_sPixmap *) pixmaps;

  XCopyPlane( draw_ctx->display, pms->pixmap[idx], draw_ctx->nav_window,
	draw_ctx->gcs[gc_type][idx], 
	0, 0, pdata->width, pdata->height, x, y, 1);
  return 1;
}

int flow_draw_nav_pixmap_erase( FlowCtx *ctx, int x, int y, flow_sPixmapData *pixmap_data,
	void *pixmaps, flow_eDrawType gc_type, int idx, int line)
{
  draw_tCtx draw_ctx;
  flow_sPixmapDataElem *pdata = (flow_sPixmapDataElem *)pixmap_data + idx;
  if ( ctx->no_nav || ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XClearArea( draw_ctx->display, draw_ctx->nav_window, 
	x, y, pdata->width, pdata->height, False);
  return 1;
}

int flow_draw_fill_rect( FlowCtx *ctx, int x, int y, int w, int h, 
	flow_eDrawType gc_type)
{
  draw_tCtx draw_ctx;
  XPoint p[5] = {{x,y},{x+w,y},{x+w,y+h},{x,y+h},{x,y}};
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( gc_type == flow_eDrawType_Green)
    XFillPolygon( draw_ctx->display, draw_ctx->window,
	draw_ctx->gc_green, p, 5, Convex, CoordModeOrigin);
  else if ( gc_type == flow_eDrawType_Yellow)
    XFillPolygon( draw_ctx->display, draw_ctx->window,
	draw_ctx->gc_yellow, p, 5, Convex, CoordModeOrigin);
  else
    XFillPolygon( draw_ctx->display, draw_ctx->window,
	draw_ctx->gcs[gc_type][0], p, 5, Convex, CoordModeOrigin);
  return 1;
}

void flow_draw_clear( FlowCtx *ctx)
{
  draw_tCtx draw_ctx;
  if ( ctx->nodraw) return;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XClearWindow( draw_ctx->display, draw_ctx->window);
}

void flow_draw_nav_clear( FlowCtx *ctx)
{
  draw_tCtx draw_ctx;
  if ( ctx->no_nav || ctx->nodraw) return;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XClearWindow( draw_ctx->display, draw_ctx->nav_window);
}


void flow_draw_get_window_size( FlowCtx *ctx, int *width, int *height)
{
  XWindowAttributes attr;
  draw_tCtx draw_ctx;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XGetWindowAttributes( draw_ctx->display, draw_ctx->window, &attr);
  *width = attr.width;
  *height = attr.height;
}

void flow_draw_get_nav_window_size( FlowCtx *ctx, int *width, int *height)
{
  XWindowAttributes attr;
  draw_tCtx draw_ctx;

  if ( ctx->no_nav) return;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XGetWindowAttributes( draw_ctx->display, draw_ctx->nav_window, &attr);
  *width = attr.width;
  *height = attr.height;
}

void flow_draw_set_nav_window_size( FlowCtx *ctx, int width, int height)
{
  draw_tCtx draw_ctx;
//  Arg 		args[20];
//  int		i;
  XWindowChanges	xwc;

  if ( ctx->no_nav) return;
  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  xwc.width = width;
  xwc.height = height;

//  i = 0;
//  XtSetArg(args[i],XmNwidth,width);i++;
//  XtSetArg(args[i],XmNheight,height);i++;
//  XtSetValues( draw_ctx->nav_shell ,args,i);
  XConfigureWindow( draw_ctx->display, draw_ctx->nav_window, CWWidth | CWHeight,
	&xwc);
}

void draw_timer_cb( draw_sTimerCb *timer_cb)
{
  draw_tCtx draw_ctx;

  draw_ctx = (draw_tCtx) timer_cb->ctx->draw_ctx;

  (timer_cb->callback_func)(timer_cb->ctx);
  XFlush( draw_ctx->display);
  free( timer_cb);
}

static void event_timer_cb( FlowCtx *ctx)
{
//  printf( "Timer callback\n");
  draw_event_handler( ctx, last_event);
}

static void cancel_event_timer( FlowCtx *ctx)
{
  draw_tCtx draw_ctx  = (draw_tCtx) ctx->draw_ctx;
  if ( draw_ctx->timer_id)
    XtRemoveTimeOut( draw_ctx->timer_id);
//  printf( "Timer removed\n");
//  sys$cantim( ctx, 0);
}

static void event_timer( FlowCtx *ctx, int time_ms)
{
//  $DESCRIPTOR (timeunitdsc, "0 0:0:0.1");       /* 0.1 second units */
//  int	tmo[2], tmptime[2];
//  int	sts;
  draw_tCtx draw_ctx;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

//  sts = sys$bintim (&timeunitdsc, &tmo);
//  lib$mult_delta_time (
//		&time_ms,		/* multiplier */
//		&tmo);			/* delta_time (modified) */
//  sys$gettim (&tmptime);
//  lib$add_times (&tmo, &tmptime, &tmo);

//  sts = sys$setimr( draw_ctx->ef, tmo, event_timer_cb, ctx, 0);


//  printf( "Add timer\n");
  draw_ctx->timer_id = XtAppAddTimeOut(
	XtWidgetToApplicationContext(draw_ctx->toplevel) ,time_ms,
	(XtTimerCallbackProc)event_timer_cb, ctx);

}

void draw_set_timer( FlowCtx *ctx, int time_ms,
  	void (*callback_func)( FlowCtx *ctx), void **id)
{
  draw_tCtx draw_ctx;
  draw_sTimerCb *timer_cb;
//  $DESCRIPTOR (timeunitdsc, "0 0:0:0.3");       /* 0.1 second units */
//  int	tmo[2], tmptime[2];
//  int	sts;

//  sts = sys$bintim (&timeunitdsc, &tmo);
//  sys$gettim (&tmptime);
//  lib$add_times (&tmo, &tmptime, &tmo);

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  timer_cb = (draw_sTimerCb *) calloc( 1, sizeof(draw_sTimerCb));
  timer_cb->ctx = ctx;
  timer_cb->callback_func = callback_func;

  timer_cb->timer_id = XtAppAddTimeOut(
	XtWidgetToApplicationContext(draw_ctx->toplevel) ,time_ms,
	(XtTimerCallbackProc)draw_timer_cb, timer_cb);

//  sts = sys$setimr( 1, tmo, draw_timer_cb, timer_cb, 0);
  *id = (void *)timer_cb;
}

void draw_cancel_timer( void *id)
{
  XtRemoveTimeOut( ((draw_sTimerCb *)id)->timer_id);

//  sys$cantim( id, 0);
  free( (char *) id);
}

void draw_set_cursor( FlowCtx *ctx, draw_eCursor cursor)
{
  draw_tCtx draw_ctx;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( cursor == draw_eCursor_Normal)
    XUndefineCursor( draw_ctx->display, draw_ctx->window);
  else
    XDefineCursor( draw_ctx->display, draw_ctx->window, 
		draw_ctx->cursors[cursor]);
  XFlush( draw_ctx->display);
}

void draw_set_nav_cursor( FlowCtx *ctx, draw_eCursor cursor)
{
  draw_tCtx draw_ctx;

  if ( ctx->no_nav) return;
  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( cursor == draw_eCursor_Normal)
    XUndefineCursor( draw_ctx->display, draw_ctx->nav_window);
  else
    XDefineCursor( draw_ctx->display, draw_ctx->nav_window, 
		draw_ctx->cursors[cursor]);
  XFlush( draw_ctx->display);
}

int draw_get_text_extent( FlowCtx *ctx, char *text, int len,
	flow_eDrawType gc_type, int idx,
	int *width, int *height)
{
  draw_tCtx draw_ctx;
  int	direction, ascent, descent;
  XCharStruct char_struct;
  draw_eFont font_idx;

  switch( gc_type) {
    case flow_eDrawType_TextHelvetica:
    case flow_eDrawType_TextHelveticaErase:
      font_idx = draw_eFont_Helvetica;
      break;
    case flow_eDrawType_TextHelveticaBold:
    case flow_eDrawType_TextHelveticaEraseBold:
      font_idx = draw_eFont_HelveticaBold;
      break;
    default:
      *width = 0;
      *height = 0;
      return 0;
  }
  draw_ctx = (draw_tCtx) ctx->draw_ctx;
  
  XTextExtents( draw_ctx->font_struct[font_idx][idx], text, len,
	&direction, &ascent, &descent, &char_struct);
  *height = draw_ctx->font_struct[font_idx][idx]->ascent + 
  	draw_ctx->font_struct[font_idx][idx]->descent;
  *width = char_struct.width;
  return 1;
}

void draw_copy_area( FlowCtx *ctx, int x, int y)
{
  draw_tCtx draw_ctx;
  GC gc;
  if ( ctx->nodraw) return;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;
  gc = draw_ctx->gcs[flow_eDrawType_Line][3];
  if ( x >= 0 && y >= 0)
  {
    XCopyArea( draw_ctx->display, draw_ctx->window, draw_ctx->window, gc, 
	0, 0, ctx->window_width-x, ctx->window_height-y, x, y);
    if ( x)
      XClearArea( draw_ctx->display, draw_ctx->window, 0, 0, 
		x, ctx->window_height, 0);
    if ( y)
      XClearArea( draw_ctx->display, draw_ctx->window, x, 0, 
		ctx->window_width, y, 0);
  }
  else if ( x <= 0 && y <= 0)
  {
    XCopyArea( draw_ctx->display, draw_ctx->window, draw_ctx->window, gc,
	-x, -y, ctx->window_width+x, ctx->window_height+y, 0, 0);
    if ( x)
      XClearArea( draw_ctx->display, draw_ctx->window, 
	ctx->window_width+x, 0, ctx->window_width, ctx->window_height, 0);
    if ( y)
      XClearArea( draw_ctx->display, draw_ctx->window, 
	0, ctx->window_height+y, ctx->window_width+x, ctx->window_height, 0);
  }
  else if ( x <= 0 && y >= 0)
  {
    XCopyArea( draw_ctx->display, draw_ctx->window, draw_ctx->window, gc,
	-x, 0, ctx->window_width+x, ctx->window_height-y, 0, y);
    if ( x)
      XClearArea( draw_ctx->display, draw_ctx->window, 
	ctx->window_width+x, 0, ctx->window_width, ctx->window_height, 0);
    if ( y)
      XClearArea( draw_ctx->display, draw_ctx->window, 
	0, 0, ctx->window_width+x, y, 0);
  }
  else
  {
    XCopyArea( draw_ctx->display, draw_ctx->window, draw_ctx->window, gc,
	0, -y, ctx->window_width-x, ctx->window_height+y, x, 0);
    if ( x)
      XClearArea( draw_ctx->display, draw_ctx->window, 
	0, 0, x, ctx->window_height, 0);
    if ( y)
      XClearArea( draw_ctx->display, draw_ctx->window, 
	x, ctx->window_height+y, ctx->window_width, ctx->window_height, 0);
  }
}

void draw_clear_area( FlowCtx *ctx, int ll_x, int ur_x, int ll_y, int ur_y)
{
  draw_tCtx draw_ctx;

  if ( ctx->nodraw) return;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;
  XClearArea( draw_ctx->display, draw_ctx->window, ll_x, ll_y, ur_x - ll_x, 
		ur_y - ll_y, 0);
}

int flow_draw_create_input( FlowCtx *ctx, int x, int y, char *text, int len,
	int idx, int width, int height, void *node, int number, void **data)
{
  XmFontList fontlist;
  XmFontListEntry entry;
  draw_tCtx draw_ctx;
  draw_sAnnotData *annot_data; 
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  annot_data = (draw_sAnnotData *) calloc(1, sizeof( draw_sAnnotData));
  entry = XmFontListEntryCreate("tag", XmFONT_IS_FONT, 
		draw_ctx->font_struct[0][idx]);
  fontlist = XmFontListAppendEntry( NULL, entry);

  annot_data->w = XtVaCreateWidget( "input", xmTextWidgetClass, draw_ctx->toplevel,
	XmNvalue, text, XmNx, x, XmNy, y - height, XmNfontList, fontlist,
	XmNwidth, width + 20, XmNheight, height + 20, NULL);
  XtAddCallback( annot_data->w, XmNactivateCallback, draw_input_cb, annot_data);
  XtManageChild( annot_data->w);
  annot_data->node = (FlowArrayElem *)node;
  annot_data->number = number;
  annot_data->x = x;
  annot_data->y = y;
  annot_data->width = width;
  annot_data->height = height;
  annot_data->ctx = ctx;
  *data = (void *) annot_data;
  XtFree( (char *)entry);
  XmFontListFree( fontlist);
  return 1;
}

int flow_draw_close_input( FlowCtx *ctx, void *data)
{
  draw_sAnnotData *annot_data = (draw_sAnnotData *)data;
  XtDestroyWidget( annot_data->w);
  free( (char *) annot_data);
  return 1;
}

int flow_draw_get_input( FlowCtx *ctx, void *data, char **text)
{
  draw_sAnnotData *annot_data = (draw_sAnnotData *)data;
  char *t;

  t = XmTextGetString( annot_data->w);

  *text = (char *) malloc( strlen(t) + 1);
  strcpy( *text, t);
  XtFree( t);
  return 1;
}

static void draw_input_cb( Widget w, XtPointer client_data, XtPointer call_data)
{
  draw_sAnnotData *data = (draw_sAnnotData *)client_data;
  char *text;

  /* Get input value */
  text = XmTextGetString( w);

  /* Call backcall function */
  data->ctx->annotation_input_cb( data->node, data->number, text);
  XtFree( text);
}

void flow_draw_move_input( FlowCtx *ctx, void *data, int x, int y,
	flow_ePosition pos_type)
{
  draw_sAnnotData *annot_data = (draw_sAnnotData *)data;

  if ( pos_type == flow_ePosition_Relative)
  {
    if ( !x && !y)
      return;

    annot_data->x += x;
    annot_data->y += y;
  }
  else
  {
    if ( x == annot_data->x && y == annot_data->y)
      return;

    annot_data->x = x;
    annot_data->y = y;
  }
  XtMoveWidget( annot_data->w, annot_data->x, annot_data->y - annot_data->height);
#if 0
  XtSetArg(args[i],XmNx, text_x + x); i++;
  XtSetArg(args[i],XmNy, text_y + y); i++;
  XtSetValues( annot_data->w, args, i);
#endif
}

void flow_set_inputfocus( FlowCtx *ctx)
{
  draw_tCtx draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XSetInputFocus( draw_ctx->display, draw_ctx->window, 
		RevertToParent, CurrentTime);
}

void flow_set_widget_inputfocus( Widget w)
{
  XSetInputFocus( XtDisplay(w), XtWindow(w), 
		RevertToParent, CurrentTime);
}

void flow_draw_set_click_sensitivity( FlowCtx *ctx, int value)
{
  draw_tCtx draw_ctx = (draw_tCtx) ctx->draw_ctx;

  draw_ctx->click_sensitivity = value;
}

