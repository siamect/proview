#include "glow_std.h"

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

#include "glow.h"
#include "glow_ctx.h"
#include "glow_browctx.h"
#include "glow_growctx.h"
#include "glow_colpalctx.h"
#include "glow_curvectx.h"
#include "glow_draw.h"
#include "glow_widget.h"
#include "glow_growwidget.h"
#include "glow_colpalwidget.h"
#include "glow_curvewidget.h"

#include "glow_msg.h"

#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))

#define DRAW_PRESS_PIX 9


typedef struct {
	Widget		w;
	int		x;
	int		y;
	int		width;
	int		height;
	GlowArrayElem	*node;
	int		number;
        GlowCtx 	*ctx;
} draw_sAnnotData;

typedef struct {
	GlowCtx 	*ctx;
	void 		(*callback_func)( GlowCtx *ctx);
	XtIntervalId	timer_id;
} draw_sTimerCb;

typedef struct {
	Pixmap pixmap[DRAW_PIXMAP_SIZE];
	} draw_sPixmap;

typedef struct {
	int	red;
	int	green;
	int	blue;
	} draw_sColor;

static char	font_names[glow_eDrawFont__][DRAW_FONT_SIZE][80] = { {	
	"-*-Helvetica-Medium-R-Normal--8-*-*-*-P-*-ISO8859-1", 
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

static int glow_allocate_named_color( draw_tCtx draw_ctx, char *named_color);
static int glow_allocate_color( draw_tCtx draw_ctx, int rgb_red,
	int rgb_green, int rgb_blue);
static void event_timer( GlowCtx *ctx, int time_ms);
static void cancel_event_timer(GlowCtx *ctx);
static void event_timer_cb( GlowCtx *ctx);
static void draw_input_cb( Widget w, XtPointer client_data, XtPointer call_data);
static int glow_read_color_file( char *filename, draw_sColor **color_array, 
	int *size);
static void glow_set_clip( draw_tCtx draw_ctx, GC gc);
static void glow_reset_clip( draw_tCtx draw_ctx, GC gc);


static int glow_create_cursor( draw_tCtx draw_ctx)
{
  /* Create some cursors */

  /* Cross cursor */
  draw_ctx->cursors[glow_eDrawCursor_CrossHair] = 
		XCreateFontCursor( draw_ctx->display, XC_crosshair);

  draw_ctx->cursors[glow_eDrawCursor_DiamondCross] = 
		XCreateFontCursor( draw_ctx->display, XC_diamond_cross);

  draw_ctx->cursors[glow_eDrawCursor_Hand] = 
		XCreateFontCursor( draw_ctx->display, XC_hand2);

  return 1;
}

static int draw_free_gc( draw_tCtx draw_ctx)
{
  int i, j;

  for ( i = 1; i < glow_eDrawCursor__ ; i++)
    XFreeCursor( draw_ctx->display, draw_ctx->cursors[i]);

  XFreeGC( draw_ctx->display, draw_ctx->gc_inverse);
  for ( i = 0; i < glow_eDrawType__; i++)
  {
    for ( j = 0; j < DRAW_TYPE_SIZE; j++)
      XFreeGC( draw_ctx->display, draw_ctx->gcs[i][j]);
  }  

  for ( i = 0; i < glow_eDrawFont__; i++)
  {
    for ( j = 0; j < DRAW_FONT_SIZE; j++)
    {
//      XUnloadFont( draw_ctx->display, draw_ctx->font[i][j]);
      XFreeFont( draw_ctx->display, draw_ctx->font_struct[i][j]);
    }
  }
  return 1;
}

static int glow_create_gc( draw_tCtx draw_ctx, Window window)
{
  Font 			font;
  XGCValues 		xgcv;
  int			i;
  draw_sColor 		*color_array, *color_p;
  int 			size, sts;

  /* Inverse gc */
  xgcv.background = XBlackPixelOfScreen(draw_ctx->screen);
  xgcv.foreground = draw_ctx->background;
  xgcv.cap_style = CapButt;
  draw_ctx->gc_inverse = XCreateGC( draw_ctx->display, 
	window, GCForeground | GCBackground | GCCapStyle, &xgcv);

  /* Black line gc */
  xgcv.foreground = XBlackPixelOfScreen(draw_ctx->screen);
  xgcv.background = draw_ctx->background;
  xgcv.cap_style = CapButt;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++)
  {
    xgcv.line_width = i + 1;

    draw_ctx->gcs[glow_eDrawType_Line][i] = XCreateGC( draw_ctx->display, 
	window, GCForeground | GCBackground | GCLineWidth | GCCapStyle, &xgcv);
  }

  /* Erase line gc */
  xgcv.foreground = draw_ctx->background;
  xgcv.background = draw_ctx->background;
  xgcv.cap_style = CapButt;
  xgcv.fill_rule = WindingRule;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++)
  {
    xgcv.line_width = i + 1;

    draw_ctx->gcs[glow_eDrawType_LineErase][i] = XCreateGC( draw_ctx->display, 
	window, GCForeground | GCBackground | GCLineWidth | GCCapStyle
	| GCFillRule, &xgcv);
  }

  /* Red line gc */
  xgcv.foreground = glow_allocate_named_color( draw_ctx, "red");
  xgcv.background = draw_ctx->background;
  xgcv.cap_style = CapButt;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++)
  {
    xgcv.line_width = i + 1;

    draw_ctx->gcs[glow_eDrawType_LineRed][i] = XCreateGC( draw_ctx->display, 
	window, GCForeground | GCBackground | GCLineWidth | GCCapStyle, &xgcv);
  }

  /* Gray line gc */
  xgcv.foreground = glow_allocate_named_color( draw_ctx, "gray");
  xgcv.background = draw_ctx->background;
  xgcv.cap_style = CapButt;
  xgcv.fill_rule = WindingRule;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++)
  {
    xgcv.line_width = i + 1;

    draw_ctx->gcs[glow_eDrawType_LineGray][i] = XCreateGC( draw_ctx->display, 
	window, GCForeground | GCBackground | GCLineWidth | GCCapStyle
	| GCFillRule, &xgcv);
  }

#if 0
  for ( int j = glow_eDrawType_Color4; j <= glow_eDrawType_Color100; j++)
  {
    char color[40];

    switch ( j)
    {
      case glow_eDrawType_Color4: strcpy( color, "aquamarine"); break;
      case glow_eDrawType_Color5: strcpy( color, "blue"); break;
      case glow_eDrawType_Color6: strcpy( color, "blue violet"); break;
      case glow_eDrawType_Color7: strcpy( color, "brown"); break;
      case glow_eDrawType_Color8: strcpy( color, "cadet blue"); break;
      case glow_eDrawType_Color9: strcpy( color, "cyan"); break;
      case glow_eDrawType_Color10: strcpy( color, "light gray"); break;
      case glow_eDrawType_Color11: strcpy( color, "light steel blue"); break;
      case glow_eDrawType_Color12: strcpy( color, "lime green"); break;
      case glow_eDrawType_Color13: strcpy( color, "magenta"); break;
      case glow_eDrawType_Color14: strcpy( color, "maroon"); break;
      case glow_eDrawType_Color15: strcpy( color, "medium aquamarine"); break;
      case glow_eDrawType_Color16: strcpy( color, "coral"); break;
      case glow_eDrawType_Color17: strcpy( color, "cornglower blue"); break;
      case glow_eDrawType_Color18: strcpy( color, "gray5"); break;
      case glow_eDrawType_Color19: strcpy( color, "gray25"); break;
      case glow_eDrawType_Color20: strcpy( color, "gray75"); break;
      default: ;
    }
    xgcv.foreground = glow_allocate_named_color( draw_ctx, color);
    xgcv.background = draw_ctx->background;
    for ( i = 0; i < DRAW_TYPE_SIZE; i++)
    {
      xgcv.line_width = i + 1;

      draw_ctx->gcs[j][i] = XCreateGC( draw_ctx->display, 
	window, GCForeground | GCBackground | GCLineWidth, &xgcv);
    }
  }
#endif

  sts = glow_read_color_file( "ge_colors.dat", &color_array, &size);
  if ( ODD(sts))
  {
    color_p = color_array;
    for ( int j = glow_eDrawType_Color4; j <= glow_eDrawType_Color100; j++)
    {
      if ( j - glow_eDrawType_Color4 >= size)
        break;

      xgcv.foreground = glow_allocate_color( draw_ctx, color_p->red, 
		color_p->green, color_p->blue);
      xgcv.background = draw_ctx->background;
      xgcv.cap_style = CapButt;
      xgcv.fill_rule = WindingRule;
      for ( i = 0; i < DRAW_TYPE_SIZE; i++)
      {
        xgcv.line_width = i + 1;

        draw_ctx->gcs[j][i] = XCreateGC( draw_ctx->display, 
		window, GCForeground | GCBackground | GCLineWidth | GCCapStyle
		| GCFillRule, &xgcv);
      }
      color_p++;
    }
    free( (char *) color_array);
  }

  /* Dashed line gc */
  xgcv.foreground = XBlackPixelOfScreen(draw_ctx->screen);
  xgcv.background = draw_ctx->background;
  xgcv.line_style = LineOnOffDash;
  xgcv.cap_style = CapButt;
  xgcv.dash_offset = 0;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++)
  {
    xgcv.line_width = i + 1;
    xgcv.dashes = 7 + i;

    draw_ctx->gcs[glow_eDrawType_LineDashed][i] = XCreateGC( draw_ctx->display, 
	window, 
	GCForeground | GCBackground | GCLineWidth| GCLineStyle | GCDashOffset |
	GCDashList | GCCapStyle, &xgcv);
  }

  /* Red dashed line gc */
  xgcv.foreground = glow_allocate_named_color( draw_ctx, "red");
  xgcv.background = draw_ctx->background;
  xgcv.line_style = LineOnOffDash;
  xgcv.cap_style = CapButt;
  xgcv.dash_offset = 0;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++)
  {
    xgcv.line_width = i + 1;
    xgcv.dashes = 7 + i;

    draw_ctx->gcs[glow_eDrawType_LineDashedRed][i] = XCreateGC( draw_ctx->display, 
	window, 
	GCForeground | GCBackground | GCLineWidth| GCLineStyle | GCDashOffset |
	GCDashList | GCCapStyle, &xgcv);
  }

  /* Text */
  xgcv.foreground = XBlackPixelOfScreen(draw_ctx->screen);
  xgcv.background = draw_ctx->background;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++)
  {
    draw_ctx->gcs[glow_eDrawType_TextHelvetica][i] = XCreateGC( draw_ctx->display, 
	window, GCForeground | GCBackground, &xgcv);
  }

  for ( i = 0; i < DRAW_TYPE_SIZE; i++)
  {
    draw_ctx->gcs[glow_eDrawType_TextHelveticaBold][i] = XCreateGC( draw_ctx->display, 
	window, GCForeground | GCBackground, &xgcv);
  }

  xgcv.foreground = draw_ctx->background;
  xgcv.background = draw_ctx->background;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++)
  {
    draw_ctx->gcs[glow_eDrawType_TextHelveticaErase][i] = XCreateGC( draw_ctx->display, 
	window, GCForeground | GCBackground, &xgcv);
  }

  for ( i = 0; i < DRAW_TYPE_SIZE; i++)
  {
    draw_ctx->gcs[glow_eDrawType_TextHelveticaEraseBold][i] = XCreateGC( draw_ctx->display, 
	window, GCForeground | GCBackground, &xgcv);
  }

  for ( i = 0; i < DRAW_FONT_SIZE; i++)
  {
    font = XLoadFont( draw_ctx->display, 
	font_names[glow_eDrawFont_HelveticaBold][i]);
    XSetFont( draw_ctx->display, 
	draw_ctx->gcs[glow_eDrawType_TextHelveticaBold][i], font);
    XSetFont( draw_ctx->display, 
	draw_ctx->gcs[glow_eDrawType_TextHelveticaEraseBold][i], font);
    draw_ctx->font[glow_eDrawFont_HelveticaBold][i] = font;
    draw_ctx->font_struct[glow_eDrawFont_HelveticaBold][i] =
		XQueryFont( draw_ctx->display, font);
  }
  for ( i = 0; i < DRAW_FONT_SIZE; i++)
  {
    font = XLoadFont( draw_ctx->display, font_names[glow_eDrawFont_Helvetica][i]);
    XSetFont( draw_ctx->display, 
	draw_ctx->gcs[glow_eDrawType_TextHelvetica][i], font);
    XSetFont( draw_ctx->display, 
	draw_ctx->gcs[glow_eDrawType_TextHelveticaErase][i], font);
    draw_ctx->font[glow_eDrawFont_Helvetica][i] = font;
    draw_ctx->font_struct[glow_eDrawFont_Helvetica][i] =
		XQueryFont( draw_ctx->display, font);
  }

  return 1;
}


static int glow_allocate_named_color( draw_tCtx draw_ctx, char *named_color)
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

static int glow_allocate_color( draw_tCtx draw_ctx, int rgb_red,
	int rgb_green, int rgb_blue)
{
  XColor exact_color;
  Screen *screen;
  Visual *visual;

  screen = draw_ctx->screen;
  visual = XDefaultVisualOfScreen( screen);
  if ( visual->c_class == TrueColor ||
       visual->c_class == PseudoColor ||
       visual->c_class == DirectColor ||
       visual->c_class == StaticColor)
  {
    exact_color.red = rgb_red;
    exact_color.green = rgb_green;
    exact_color.blue = rgb_blue;
    if ( XAllocColor( draw_ctx->display, 
		XDefaultColormapOfScreen( draw_ctx->screen),
	    	&exact_color))
      return exact_color.pixel;
    else
      printf( "** Color not allocated !\n");
    return XBlackPixelOfScreen( draw_ctx->screen);
  }
  else
  {
    return XBlackPixelOfScreen( draw_ctx->screen);
  }
}

void glow_draw_delete( GlowCtx *ctx)
{
  draw_tCtx	draw_ctx = (draw_tCtx) ctx->draw_ctx;

  ctx->set_nodraw();
  if ( ctx->type() == glow_eCtxType_Grow)
    delete (GrowCtx *)ctx;
  else
    delete ctx;
  draw_free_gc( draw_ctx);
  if ( draw_ctx->buffer)
    XFreePixmap( draw_ctx->display, draw_ctx->buffer);
  if ( draw_ctx->background_pixmap)
    XFreePixmap( draw_ctx->display, draw_ctx->background_pixmap);

  free( (char *) draw_ctx);
}

int glow_draw_create_secondary_ctx( 
	GlowCtx *glow_ctx,
	void **secondary_glow_ctx,
        int (*init_proc)(GlowCtx *ctx, void *client_data),
	void  *client_data, 
	glow_eCtxType type
)
{
  GlowCtx	*ctx;

  if ( type == glow_eCtxType_Brow)
    ctx = (GlowCtx *) new BrowCtx("Claes context", 20);
  else if ( type == glow_eCtxType_Grow)
    ctx = (GlowCtx *) new GrowCtx("Claes context", 20);
  else if ( type == glow_eCtxType_ColPal)
    ctx = (GlowCtx *) new ColPalCtx("Claes context", 20);
  else if ( type == glow_eCtxType_Curve)
    ctx = (GlowCtx *) new CurveCtx("Claes context", 20);
  else
    ctx = new GlowCtx("Claes context", 20);
  
  ctx->draw_ctx = glow_ctx->draw_ctx;
  ctx->set_nodraw();
  ctx->trace_connect_func = glow_ctx->trace_connect_func;
  ctx->trace_disconnect_func = glow_ctx->trace_disconnect_func;
  ctx->trace_scan_func = glow_ctx->trace_scan_func;
  ctx->trace_started = glow_ctx->trace_started;
  ctx->zoom_factor_x = glow_ctx->zoom_factor_x;
  ctx->zoom_factor_y = glow_ctx->zoom_factor_y;
  if ( type == glow_eCtxType_Brow)
  {
    ((BrowCtx *)ctx)->scroll_callback = ((BrowCtx *)glow_ctx)->scroll_callback;
    ((BrowCtx *)ctx)->scroll_data = ((BrowCtx *)glow_ctx)->scroll_data;
  }

  init_proc( ctx, client_data);
  *secondary_glow_ctx = (void *) ctx;
  return 1;
}

int glow_draw_change_ctx( 
	Widget w,
	GlowCtx *from_ctx,
	GlowCtx *to_ctx
)
{
//  if ( from_ctx->type() == glow_eCtxType_Brow)
//    ((BrowWidget)w)->brow.brow_ctx = to_ctx;
  if ( from_ctx->type() == glow_eCtxType_Grow)
    ((GrowWidget)w)->grow.grow_ctx = to_ctx;
  else
    ((GlowWidget)w)->glow.glow_ctx = to_ctx;

  to_ctx->window_width = from_ctx->window_width;
  to_ctx->window_height = from_ctx->window_height;
  from_ctx->set_nodraw();
  to_ctx->reset_nodraw();
  if ( from_ctx->type() == glow_eCtxType_Brow)
  {
    to_ctx->zoom_factor_x = from_ctx->zoom_factor_x;
    to_ctx->zoom_factor_y = from_ctx->zoom_factor_y;
    ((BrowCtx *)to_ctx)->configure(0);
  }
  else
    to_ctx->redraw();
  return 1;
}

void glow_draw_delete_secondary_ctx( GlowCtx *ctx)
{
  ctx->set_nodraw();
  delete ctx;
}

int glow_draw_init( 
	Widget toplevel, 
	void **glow_ctx,
        int (*init_proc)(Widget w, GlowCtx *ctx, void *client_data),
	void  *client_data, 
	glow_eCtxType type
)
{
  int		i;
  draw_tCtx	draw_ctx;
  Arg 		args[20];
  GlowCtx	*ctx;

  if ( type == glow_eCtxType_Brow)
    ctx = (GlowCtx *) new BrowCtx("Claes context", 20);
  else if ( type == glow_eCtxType_Grow)
    ctx = (GlowCtx *) new GrowCtx("Claes context", 20);
  else if ( type == glow_eCtxType_ColPal)
    ctx = (GlowCtx *) new ColPalCtx("Claes context", 20);
  else if ( type == glow_eCtxType_Curve)
    ctx = (GlowCtx *) new CurveCtx("Claes context", 20);
  else
    ctx = new GlowCtx("Claes context", 20);
  *glow_ctx = (void *) ctx;
  
  draw_ctx = (draw_tCtx) calloc( 1, sizeof(*draw_ctx));
  ctx->draw_ctx = draw_ctx;
  draw_ctx->toplevel = toplevel;

  draw_ctx->display = XtDisplay( draw_ctx->toplevel);
  draw_ctx->window = XtWindow( draw_ctx->toplevel);
  draw_ctx->screen = XtScreen( draw_ctx->toplevel);

  i = 0;
  XtSetArg(args[i],XmNbackground, &draw_ctx->background); i++;
  XtGetValues( draw_ctx->toplevel ,args,i);
  draw_ctx->original_background = draw_ctx->background;

  glow_create_gc( draw_ctx, draw_ctx->window);

  glow_create_cursor( draw_ctx);

#if defined IMLIB
  draw_ctx->imlib = Imlib_init( draw_ctx->display);
#endif

  glow_draw_get_window_size( ctx, &ctx->window_width, &ctx->window_height);
  glow_draw_create_buffer( ctx);
  init_proc( toplevel, ctx, client_data);

  return 1;
}


int glow_draw_init_nav( Widget nav_widget, void *glow_ctx)
{
  draw_tCtx	draw_ctx;

  draw_ctx = (draw_tCtx)((GlowCtx *)glow_ctx)->draw_ctx;
  draw_ctx->nav_toplevel = nav_widget;
  draw_ctx->nav_window = XtWindow( draw_ctx->nav_toplevel);

//  glow_create_gc( draw_ctx, draw_ctx->nav_window);

  ((GlowCtx *)glow_ctx)->no_nav = 0;
  return 1;
}


int draw_event_handler( GlowCtx *ctx, XEvent event)
{
  static int	button_pressed = 0;
  static int	button_clicked = 0;
  static int	button_clicked_and_pressed = 0;
  static int 	button1_pressed = 0;
  static int 	button2_pressed = 0;
  static int 	button3_pressed = 0;
  static int	last_press_x = 0;
  static int	last_press_y = 0;
  draw_tCtx draw_ctx;

//  cout << "Event : button_pressed " << button_pressed << " clicked " << 
//	button_clicked << " c&p " << button_clicked_and_pressed << endl;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

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
            ctx->event_handler( glow_eEvent_Key_Return, 0, 0, 0, 0);
//            printf( "-- Return key event\n");
	    break;
          case XK_Up:
            ctx->event_handler( glow_eEvent_Key_Up, 0, 0, 0, 0);
	    break;
          case XK_Down:
            ctx->event_handler( glow_eEvent_Key_Down, 0, 0, 0, 0);
	    break;
          case XK_Right:
            ctx->event_handler( glow_eEvent_Key_Right, 0, 0, 0, 0);
	    break;
          case XK_Left:
            ctx->event_handler( glow_eEvent_Key_Left, 0, 0, 0, 0);
	    break;
          case XK_Page_Up:
            ctx->event_handler( glow_eEvent_Key_PageUp, 0, 0, 0, 0);
	    break;
          case XK_Page_Down:
            ctx->event_handler( glow_eEvent_Key_PageDown, 0, 0, 0, 0);
	    break;
          case XK_BackSpace:
            ctx->event_handler( glow_eEvent_Key_BackSpace, 0, 0, 0, 0);
	    break;
          case XK_KP_F1:
            ctx->event_handler( glow_eEvent_Key_PF1, 0, 0, 0, 0);
	    break;
          case XK_KP_F2:
            ctx->event_handler( glow_eEvent_Key_PF2, 0, 0, 0, 0);
	    break;
          case XK_KP_F3:
            ctx->event_handler( glow_eEvent_Key_PF3, 0, 0, 0, 0);
	    break;
          case XK_KP_F4:
            ctx->event_handler( glow_eEvent_Key_PF4, 0, 0, 0, 0);
	    break;
          case XK_Tab:
            ctx->event_handler( glow_eEvent_Key_Tab, 0, 0, 0, 0);
	    break;
	  default:
            ;
        }
        break;
      }
      case ButtonPress : 
// cout << "Button press event" << endl;
//        printf( "-- Button event: (%d,%d) button: %d time:%d\n", event.xbutton.x, 
//		event.xbutton.y, event.xbutton.button, event.xbutton.time);

//        XSetInputFocus( draw_ctx->display, draw_ctx->window, 
//		RevertToNone, CurrentTime);

        switch ( event.xbutton.button)
        {
          case Button1:
            ctx->event_handler( glow_eEvent_MB1Down, event.xbutton.x, event.xbutton.y, 0, 0);
	    if ( draw_ctx->click_sensitivity & glow_mSensitivity_MB1Click && 
	         !(draw_ctx->click_sensitivity & glow_mSensitivity_MB1DoubleClick) && 
	         !(draw_ctx->click_sensitivity & glow_mSensitivity_MB1Press))
            {
              memcpy( &last_event, &event, sizeof(event));
              button_pressed = 0;
              button_clicked = 1;
	      last_press_x = event.xbutton.x;
	      last_press_y = event.xbutton.y;
              return 1;
	    }
            break;
          case Button3:
            ctx->event_handler( glow_eEvent_MB3Down, event.xbutton.x, event.xbutton.y, 0, 0);
	    if ( draw_ctx->click_sensitivity & glow_mSensitivity_MB3Press && 
	         !(draw_ctx->click_sensitivity & glow_mSensitivity_MB3DoubleClick) && 
	         !(draw_ctx->click_sensitivity & glow_mSensitivity_MB3Click)) {
              ctx->event_handler( glow_eEvent_MB3Press, 
				  event.xbutton.x, event.xbutton.y, 0, 0);
	      draw_ctx->click_sensitivity = 0;
	      return 1;
            }
	    else if ( draw_ctx->click_sensitivity & glow_mSensitivity_MB3Click && 
	         !(draw_ctx->click_sensitivity & glow_mSensitivity_MB3DoubleClick) && 
	         !(draw_ctx->click_sensitivity & glow_mSensitivity_MB3Press))
            {
              memcpy( &last_event, &event, sizeof(event));
              button_pressed = 0;
              button_clicked = 1;
	      last_press_x = event.xbutton.x;
	      last_press_y = event.xbutton.y;
              return 1;
	    }
            break;
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
	      {
                ctx->event_handler( glow_eEvent_MB1PressShift, event.xbutton.x, event.xbutton.y, 0, 0);
	        draw_ctx->click_sensitivity = 0;
	      }
	      else if ( !(event.xbutton.state & ShiftMask) && 
	                 (event.xbutton.state & ControlMask))
	      {
                ctx->event_handler( glow_eEvent_MB1PressCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	        draw_ctx->click_sensitivity = 0;
	      }
	      else if (  (event.xbutton.state & ShiftMask) && 
	                 (event.xbutton.state & ControlMask))
	      {
                ctx->event_handler( glow_eEvent_MB1PressShiftCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	        draw_ctx->click_sensitivity = 0;
	      }
	      else
	      {
                ctx->event_handler( glow_eEvent_MB1Press, event.xbutton.x, event.xbutton.y, 0, 0);
	        draw_ctx->click_sensitivity = 0;
	      }
              break;
            case Button2:
              button2_pressed = 1;
	      if (       (event.xbutton.state & ShiftMask) && 
	                !(event.xbutton.state & ControlMask))
	      {
                ctx->event_handler( glow_eEvent_MB2PressShift, event.xbutton.x, event.xbutton.y, 0, 0);
	        draw_ctx->click_sensitivity = 0;
	      }
	      else if ( !(event.xbutton.state & ShiftMask) && 
	                 (event.xbutton.state & ControlMask))
	      {
                ctx->event_handler( glow_eEvent_MB2PressShift, event.xbutton.x, event.xbutton.y, 0, 0);
	        draw_ctx->click_sensitivity = 0;
	      }
	      else if (  (event.xbutton.state & ShiftMask) && 
	                 (event.xbutton.state & ControlMask))
	      {
                ctx->event_handler( glow_eEvent_MB2PressShiftCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	        draw_ctx->click_sensitivity = 0;
	      }
	      else
	      {
                ctx->event_handler( glow_eEvent_MB2Press, event.xbutton.x, event.xbutton.y, 0, 0);
	        draw_ctx->click_sensitivity = 0;
	      }
              break;
            case Button3:
              button3_pressed = 1;
#if 0
	      if (       (event.xbutton.state & ShiftMask) && 
	                !(event.xbutton.state & ControlMask))
                ctx->event_handler( glow_eEvent_MB3PressShift, event.xbutton.x, event.xbutton.y, 0, 0);
	      else if ( !(event.xbutton.state & ShiftMask) && 
	                 (event.xbutton.state & ControlMask))
                ctx->event_handler( glow_eEvent_MB3PressShift, event.xbutton.x, event.xbutton.y, 0, 0);
	      else if (  (event.xbutton.state & ShiftMask) && 
	                 (event.xbutton.state & ControlMask))
                ctx->event_handler( glow_eEvent_MB3PressShiftCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	      else
#endif
                ctx->event_handler( glow_eEvent_MB3Press, event.xbutton.x, event.xbutton.y, 0, 0);
	        draw_ctx->click_sensitivity = 0;
              break;
          }
        }
        break;

      case ButtonRelease:
        button1_pressed = 0;
        button2_pressed = 0;
        button3_pressed = 0;

// cout << "Button release event" << endl;

        switch ( event.xbutton.button)
        {
          case Button1:
            ctx->event_handler( glow_eEvent_MB1Up, event.xbutton.x, event.xbutton.y, 0, 0);
            break;
        }

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
	        {
                  ctx->event_handler( glow_eEvent_MB1ClickShift, event.xbutton.x, event.xbutton.y, 0, 0);
	          draw_ctx->click_sensitivity = 0;
	        }
	        else if ( !(event.xbutton.state & ShiftMask) && 
	                   (event.xbutton.state & ControlMask))
	        {
                  ctx->event_handler( glow_eEvent_MB1ClickCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	          draw_ctx->click_sensitivity = 0;
	        }
	        else if (  (event.xbutton.state & ShiftMask) && 
	                   (event.xbutton.state & ControlMask))
	        {
                  ctx->event_handler( glow_eEvent_MB1ClickShiftCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	          draw_ctx->click_sensitivity = 0;
	        }
	        else
	        {
                  ctx->event_handler( glow_eEvent_MB1Click, event.xbutton.x, event.xbutton.y, 0, 0);
	          draw_ctx->click_sensitivity = 0;
	        }
                break;
              case Button2:
	        if (       (event.xbutton.state & ShiftMask) && 
	                  !(event.xbutton.state & ControlMask))
	        {
                  ctx->event_handler( glow_eEvent_MB2ClickShift, event.xbutton.x, event.xbutton.y, 0, 0);
	          draw_ctx->click_sensitivity = 0;
	        }
	        else if ( !(event.xbutton.state & ShiftMask) && 
	                   (event.xbutton.state & ControlMask))
	        {
                  ctx->event_handler( glow_eEvent_MB2ClickCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	          draw_ctx->click_sensitivity = 0;
	        }
	        else if (  (event.xbutton.state & ShiftMask) && 
	                   (event.xbutton.state & ControlMask))
	        {
                  ctx->event_handler( glow_eEvent_MB2ClickShiftCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	          draw_ctx->click_sensitivity = 0;
	        }
	        else
	        {
                  ctx->event_handler( glow_eEvent_MB2Click, event.xbutton.x, event.xbutton.y, 0, 0);
	          draw_ctx->click_sensitivity = 0;
	        }
                break;
              case Button3:
#if 0
	        if (       (event.xbutton.state & ShiftMask) && 
	                  !(event.xbutton.state & ControlMask))
                  ctx->event_handler( glow_eEvent_MB3ClickShift, event.xbutton.x, event.xbutton.y, 0, 0);
	        else if ( !(event.xbutton.state & ShiftMask) && 
	                   (event.xbutton.state & ControlMask))
                  ctx->event_handler( glow_eEvent_MB3ClickCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	        else if (  (event.xbutton.state & ShiftMask) && 
	                   (event.xbutton.state & ControlMask))
                  ctx->event_handler( glow_eEvent_MB3ClickShiftCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	        else
#endif
                  ctx->event_handler( glow_eEvent_MB3Click, event.xbutton.x, event.xbutton.y, 0, 0);
	          draw_ctx->click_sensitivity = 0;
                break;
	    }
          }
          else
          {
            /* Button release */
// cout << "Button release detected" << endl;
            ctx->event_handler( glow_eEvent_ButtonRelease, event.xbutton.x, event.xbutton.y, 0, 0);
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
	        {
                  ctx->event_handler( glow_eEvent_MB1DoubleClickShift, event.xbutton.x, event.xbutton.y, 0, 0);
	          draw_ctx->click_sensitivity = 0;
	        }
	        else if ( !(event.xbutton.state & ShiftMask) && 
	                   (event.xbutton.state & ControlMask))
	        {
                  ctx->event_handler( glow_eEvent_MB1DoubleClickCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	          draw_ctx->click_sensitivity = 0;
	        }
	        else if (  (event.xbutton.state & ShiftMask) && 
	                   (event.xbutton.state & ControlMask))
	        {
                  ctx->event_handler( glow_eEvent_MB1DoubleClickShiftCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	          draw_ctx->click_sensitivity = 0;
	        }
	        else
	        {
                  ctx->event_handler( glow_eEvent_MB1DoubleClick, event.xbutton.x, event.xbutton.y, 0, 0);
	          draw_ctx->click_sensitivity = 0;
	        }
                break;
              case Button2:
	        if (       (event.xbutton.state & ShiftMask) && 
	                  !(event.xbutton.state & ControlMask))
	        {
                  ctx->event_handler( glow_eEvent_MB2DoubleClickShift, event.xbutton.x, event.xbutton.y, 0, 0);
	          draw_ctx->click_sensitivity = 0;
	        }
	        else if ( !(event.xbutton.state & ShiftMask) && 
	                   (event.xbutton.state & ControlMask))
	        {
                  ctx->event_handler( glow_eEvent_MB2DoubleClickCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	          draw_ctx->click_sensitivity = 0;
	        }
	        else if (  (event.xbutton.state & ShiftMask) && 
	                   (event.xbutton.state & ControlMask))
	        {
                  ctx->event_handler( glow_eEvent_MB2DoubleClickShiftCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	          draw_ctx->click_sensitivity = 0;
	        }
	        else
	        {
                  ctx->event_handler( glow_eEvent_MB2DoubleClick, event.xbutton.x, event.xbutton.y, 0, 0);
	          draw_ctx->click_sensitivity = 0;
	        }
                break;
              case Button3:
#if 0
	        if (       (event.xbutton.state & ShiftMask) && 
	                  !(event.xbutton.state & ControlMask))
                  ctx->event_handler( glow_eEvent_MB3DoubleClickShift, event.xbutton.x, event.xbutton.y, 0, 0);
	        else if ( !(event.xbutton.state & ShiftMask) && 
	                   (event.xbutton.state & ControlMask))
                  ctx->event_handler( glow_eEvent_MB3DoubleClickCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	        else if (  (event.xbutton.state & ShiftMask) && 
	                   (event.xbutton.state & ControlMask))
                  ctx->event_handler( glow_eEvent_MB3DoubleClickShiftCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	        else
                  ctx->event_handler( glow_eEvent_MB3DoubleClick, event.xbutton.x, event.xbutton.y, 0, 0);
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
        ctx->event_handler( glow_eEvent_Exposure, event.xexpose.x, 
		event.xexpose.y, event.xexpose.width, event.xexpose.height);
        break;
      case VisibilityNotify : 
	switch ( event.xvisibility.state)
        {
          case VisibilityUnobscured:
            ctx->event_handler( glow_eEvent_VisibilityUnobscured, 0, 0, 0, 0);
            break;
          default: 
            ctx->event_handler( glow_eEvent_VisibilityObscured, 0, 0, 0, 0);
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
                ctx->event_handler( glow_eEvent_MB1PressShift, event.xbutton.x, event.xbutton.y, 0, 0);
	      else if ( !(event.xbutton.state & ShiftMask) && 
	                 (event.xbutton.state & ControlMask))
                ctx->event_handler( glow_eEvent_MB1PressCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	      else if (  (event.xbutton.state & ShiftMask) && 
	                 (event.xbutton.state & ControlMask))
                ctx->event_handler( glow_eEvent_MB1PressShiftCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	      else
                ctx->event_handler( glow_eEvent_MB1Press, event.xbutton.x, event.xbutton.y, 0, 0);
              break;
            case Button2:
              button2_pressed = 1;
	      if (       (event.xbutton.state & ShiftMask) && 
	                !(event.xbutton.state & ControlMask))
                ctx->event_handler( glow_eEvent_MB2PressShift, event.xbutton.x, event.xbutton.y, 0, 0);
	      else if ( !(event.xbutton.state & ShiftMask) && 
	               (event.xbutton.state & ControlMask))
                ctx->event_handler( glow_eEvent_MB2PressShift, event.xbutton.x, event.xbutton.y, 0, 0);
	      else if (  (event.xbutton.state & ShiftMask) && 
	               (event.xbutton.state & ControlMask))
                ctx->event_handler( glow_eEvent_MB2PressShiftCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	      else
                ctx->event_handler( glow_eEvent_MB2Press, event.xbutton.x, event.xbutton.y, 0, 0);
              break;
            case Button3:
              button3_pressed = 1;
                ctx->event_handler( glow_eEvent_MB3Press, event.xbutton.x, event.xbutton.y, 0, 0);
              break;
          }
	  button_pressed = 0;
          button_clicked_and_pressed = 0;

        }
        if ( button1_pressed || button2_pressed || button3_pressed)
          ctx->event_handler( glow_eEvent_ButtonMotion, event.xbutton.x, event.xbutton.y, 0, 0);
        else
          ctx->event_handler( glow_eEvent_CursorMotion, event.xbutton.x, event.xbutton.y, 0, 0);
        break;
      case EnterNotify:
//        printf( "-- Enter event: (%d,%d)\n", event.xbutton.x, 
//		event.xbutton.y);
        ctx->event_handler( glow_eEvent_Enter, event.xbutton.x, event.xbutton.y, 0, 0);
        break;          
      case LeaveNotify:
//        printf( "-- Leave event: (%d,%d)\n", event.xbutton.x, 
//		event.xbutton.y);

        ctx->event_handler( glow_eEvent_Leave, event.xbutton.x, event.xbutton.y, 0, 0);
        break;          
      case MapNotify:
//        printf( "-- Enter event: (%d,%d)\n", event.xbutton.x, 
//		event.xbutton.y);
        ctx->event_handler( glow_eEvent_Map, 0, 0, 0, 0);
        break;          
      case UnmapNotify:
//        printf( "-- Enter event: (%d,%d)\n", event.xbutton.x, 
//		event.xbutton.y);
        ctx->event_handler( glow_eEvent_Unmap, 0, 0, 0, 0);
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
            ctx->event_handler_nav( glow_eEvent_MB1Press, event.xbutton.x, event.xbutton.y);
            break;
          case Button2:
            button2_pressed = 1;
            ctx->event_handler_nav( glow_eEvent_MB2Press, event.xbutton.x, event.xbutton.y);
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
        ctx->event_handler_nav( glow_eEvent_ButtonRelease, event.xbutton.x, event.xbutton.y);
        break;          
      case Expose:
//        printf( "-- Navigator expose event..\n" );
        ctx->event_handler_nav( glow_eEvent_Exposure, event.xbutton.x, event.xbutton.y);
        break; 
      case MotionNotify:
//            printf( "-- Button1 motion event nav: (%d,%d)\n", event.xbutton.x, 
//		event.xbutton.y);
        if ( button1_pressed || button2_pressed || button3_pressed)
          ctx->event_handler_nav( glow_eEvent_ButtonMotion, event.xbutton.x, event.xbutton.y);
        else
          ctx->event_handler_nav( glow_eEvent_CursorMotion, event.xbutton.x, event.xbutton.y);
        break;          
     }
  }
  XFlush( draw_ctx->display);
  return 1;
}

void	glow_eDrawnable_event( GlowCtx *ctx, glow_eEvent event, 
		glow_eEventType event_type, 
		int (*event_cb)(GlowCtx *ctx, glow_tEvent event))
{
  ctx->enable_event( event, event_type, event_cb);
}

int glow_draw_rect( GlowCtx *ctx, int x, int y, int width, int height, 
	glow_eDrawType gc_type, int idx, int highlight)
{
  draw_tCtx draw_ctx;
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( gc_type == glow_eDrawType_LineGray && highlight)
    gc_type = glow_eDrawType_Line;

  if ( draw_ctx->clip_on)
    glow_set_clip( draw_ctx, draw_ctx->gcs[gc_type+highlight][idx]);

  if ( !ctx->draw_buffer_only)
    XDrawRectangle( draw_ctx->display, draw_ctx->window, 
	draw_ctx->gcs[gc_type+highlight][idx], 
	x, y, width, height);
  if ( ctx->double_buffer_on)
    XDrawRectangle( draw_ctx->display, draw_ctx->buffer, 
	draw_ctx->gcs[gc_type+highlight][idx], 
	x, y, width, height);

  if ( draw_ctx->clip_on)
    glow_reset_clip( draw_ctx, draw_ctx->gcs[gc_type+highlight][idx]);

  return 1;
}

int glow_draw_rect_erase( GlowCtx *ctx, int x, int y, int width, int height,
	int idx)
{
  draw_tCtx draw_ctx;
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( draw_ctx->clip_on)
    glow_set_clip( draw_ctx, draw_ctx->gcs[glow_eDrawType_LineErase][idx]);

  if ( !ctx->draw_buffer_only)
    XDrawRectangle( draw_ctx->display, draw_ctx->window, 
	draw_ctx->gcs[glow_eDrawType_LineErase][idx], 
	x, y, width, height);
  if ( ctx->double_buffer_on)
    XDrawRectangle( draw_ctx->display, draw_ctx->buffer, 
	draw_ctx->gcs[glow_eDrawType_LineErase][idx], 
	x, y, width, height);

  if ( draw_ctx->clip_on)
    glow_reset_clip( draw_ctx, draw_ctx->gcs[glow_eDrawType_LineErase][idx]);

  return 1;
}

int glow_draw_nav_rect( GlowCtx *ctx, int x, int y, int width, int height,
	glow_eDrawType gc_type, int idx, int highlight)
{
  draw_tCtx draw_ctx;
  if ( ctx->no_nav || ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( gc_type == glow_eDrawType_LineGray && highlight)
    gc_type = glow_eDrawType_Line;

  XDrawRectangle( draw_ctx->display, draw_ctx->nav_window,
	draw_ctx->gcs[gc_type+highlight][idx], 
	x, y, width, height);
  return 1;
}

int glow_draw_nav_rect_erase( GlowCtx *ctx, int x, int y, int width, int height,
	int idx)
{
  draw_tCtx draw_ctx;
  if ( ctx->no_nav || ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XDrawRectangle( draw_ctx->display, draw_ctx->nav_window,
	draw_ctx->gcs[glow_eDrawType_LineErase][idx], 
	x, y, width, height);
  return 1;
}

int glow_draw_arrow( GlowCtx *ctx, int x1, int y1, int x2, int y2, 
	int x3, int y3,
	glow_eDrawType gc_type, int idx, int highlight)
{
  draw_tCtx draw_ctx;
  XPoint p[4] = {{x1,y1},{x2,y2},{x3,y3},{x1,y1}};
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( gc_type == glow_eDrawType_LineGray && highlight)
    gc_type = glow_eDrawType_Line;

  if ( draw_ctx->clip_on)
    glow_set_clip( draw_ctx, draw_ctx->gcs[gc_type+highlight][idx]);

  if ( !ctx->draw_buffer_only)
    XFillPolygon( draw_ctx->display, draw_ctx->window,
	draw_ctx->gcs[gc_type+highlight][idx], p, 4, Convex, CoordModeOrigin);
  if ( ctx->double_buffer_on)
    XFillPolygon( draw_ctx->display, draw_ctx->buffer,
	draw_ctx->gcs[gc_type+highlight][idx], p, 4, Convex, CoordModeOrigin);

  if ( draw_ctx->clip_on)
    glow_reset_clip( draw_ctx, draw_ctx->gcs[gc_type+highlight][idx]);

  return 1;
}

int glow_draw_arrow_erase( GlowCtx *ctx, int x1, int y1, int x2, int y2, 
	int x3, int y3,
	int idx)
{
  XPoint p[4] = {{x1,y1},{x2,y2},{x3,y3},{x1,y1}};
  draw_tCtx draw_ctx;
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( draw_ctx->clip_on)
    glow_set_clip( draw_ctx, draw_ctx->gcs[glow_eDrawType_LineErase][idx]);

  if ( !ctx->draw_buffer_only)
    XFillPolygon( draw_ctx->display, draw_ctx->window,
	draw_ctx->gcs[glow_eDrawType_LineErase][idx],
	p, 4, Convex, CoordModeOrigin);
  if ( ctx->double_buffer_on)
    XFillPolygon( draw_ctx->display, draw_ctx->buffer,
	draw_ctx->gcs[glow_eDrawType_LineErase][idx],
	p, 4, Convex, CoordModeOrigin);

  if ( draw_ctx->clip_on)
    glow_reset_clip( draw_ctx, draw_ctx->gcs[glow_eDrawType_LineErase][idx]);

  return 1;
}

int glow_draw_nav_arrow( GlowCtx *ctx, int x1, int y1, int x2, int y2, 
	int x3, int y3,
	glow_eDrawType gc_type, int idx, int highlight)
{
  draw_tCtx draw_ctx;
  XPoint p[4] = {{x1,y1},{x2,y2},{x3,y3},{x1,y1}};
  if ( ctx->no_nav || ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( gc_type == glow_eDrawType_LineGray && highlight)
    gc_type = glow_eDrawType_Line;

  XFillPolygon( draw_ctx->display, draw_ctx->nav_window,
	draw_ctx->gcs[gc_type+highlight][idx], p, 4, Convex, CoordModeOrigin);
  return 1;
}

int glow_draw_nav_arrow_erase( GlowCtx *ctx, int x1, int y1, int x2, int y2, 
	int x3, int y3,
	int idx)
{
  XPoint p[4] = {{x1,y1},{x2,y2},{x3,y3},{x1,y1}};
  draw_tCtx draw_ctx;
  if ( ctx->no_nav || ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XFillPolygon( draw_ctx->display, draw_ctx->nav_window,
	draw_ctx->gcs[glow_eDrawType_LineErase][idx],
	p, 4, Convex, CoordModeOrigin);
  return 1;
}

int glow_draw_arc( GlowCtx *ctx, int x, int y, int width, int height, 
	int angel1, int angel2,
	glow_eDrawType gc_type, int idx, int highlight)
{
  draw_tCtx draw_ctx;
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( gc_type == glow_eDrawType_LineGray && highlight)
    gc_type = glow_eDrawType_Line;

  // Fix for highlight for connections in grow
  if ( highlight && ctx->type() == glow_eCtxType_Grow)
    gc_type = glow_eDrawType_Color58;

  if ( angel1 >= 360)
    angel1 = angel1 - angel1 / 360 * 360;
  else if ( angel1 < 0)
    angel1 = angel1 + ( -angel1 / 360 + 1) * 360;

  if ( draw_ctx->clip_on)
    glow_set_clip( draw_ctx, draw_ctx->gcs[gc_type+highlight][idx]);

//  printf( "arc: x: %d y: %d, w: %d, h: %d, a1: %d, a2: %d\n", 
//	x, y, width, height, angel1, angel2);
	
  if ( !ctx->draw_buffer_only)
    XDrawArc( draw_ctx->display, draw_ctx->window,
	draw_ctx->gcs[gc_type+highlight][idx], 
	x, y, width, height, angel1*64, angel2*64);
  if ( ctx->double_buffer_on)
    XDrawArc( draw_ctx->display, draw_ctx->buffer,
	draw_ctx->gcs[gc_type+highlight][idx], 
	x, y, width, height, angel1*64, angel2*64);

  if ( draw_ctx->clip_on)
    glow_reset_clip( draw_ctx, draw_ctx->gcs[gc_type+highlight][idx]);

  return 1;
}

int glow_draw_fill_arc( GlowCtx *ctx, int x, int y, int width, int height, 
	int angel1, int angel2, glow_eDrawType gc_type, int highlight)
{
  draw_tCtx draw_ctx;
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( gc_type == glow_eDrawType_LineGray && highlight)
    gc_type = glow_eDrawType_LineRed;

  if ( angel1 >= 360)
    angel1 = angel1 - angel1 / 360 * 360;
  else if ( angel1 < 0)
    angel1 = angel1 + ( -angel1 / 360 + 1) * 360;

  if ( draw_ctx->clip_on)
    glow_set_clip( draw_ctx, draw_ctx->gcs[gc_type+highlight][0]);

  if ( !ctx->draw_buffer_only)
    XFillArc( draw_ctx->display, draw_ctx->window,
	draw_ctx->gcs[gc_type+highlight][0], 
	x, y, width, height, angel1*64, angel2*64);
  if ( ctx->double_buffer_on)
    XFillArc( draw_ctx->display, draw_ctx->buffer,
	draw_ctx->gcs[gc_type+highlight][0], 
	x, y, width, height, angel1*64, angel2*64);

  if ( draw_ctx->clip_on)
    glow_reset_clip( draw_ctx, draw_ctx->gcs[gc_type+highlight][0]);

  return 1;
}

int glow_draw_nav_fill_arc( GlowCtx *ctx, int x, int y, int width, int height, 
	int angel1, int angel2, glow_eDrawType gc_type)
{
  draw_tCtx draw_ctx;
  if ( ctx->no_nav || ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( angel1 >= 360)
    angel1 = angel1 - angel1 / 360 * 360;
  else if ( angel1 < 0)
    angel1 = angel1 + ( -angel1 / 360 + 1) * 360;

  XFillArc( draw_ctx->display, draw_ctx->nav_window,
	draw_ctx->gcs[gc_type][0], 
	x, y, width, height, angel1*64, angel2*64);
  return 1;
}

int glow_draw_arc_erase( GlowCtx *ctx, int x, int y, int width, int height,
	int angel1, int angel2,
	int idx)
{
  draw_tCtx draw_ctx;
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( angel1 >= 360)
    angel1 = angel1 - angel1 / 360 * 360;
  else if ( angel1 < 0)
    angel1 = angel1 + ( -angel1 / 360 + 1) * 360;

  if ( draw_ctx->clip_on)
    glow_set_clip( draw_ctx, draw_ctx->gcs[glow_eDrawType_LineErase][idx]);

  if ( !ctx->draw_buffer_only)
    XDrawArc( draw_ctx->display, draw_ctx->window,
	draw_ctx->gcs[glow_eDrawType_LineErase][idx], 
	x, y, width, height, angel1*64, angel2*64);
  if ( ctx->double_buffer_on)
    XDrawArc( draw_ctx->display, draw_ctx->buffer,
	draw_ctx->gcs[glow_eDrawType_LineErase][idx], 
	x, y, width, height, angel1*64, angel2*64);

  if ( draw_ctx->clip_on)
    glow_reset_clip( draw_ctx, draw_ctx->gcs[glow_eDrawType_LineErase][idx]);
  return 1;
}

int glow_draw_nav_arc( GlowCtx *ctx, int x, int y, int width, int height,
	int angel1, int angel2,
	glow_eDrawType gc_type, int idx, int highlight)
{
  draw_tCtx draw_ctx;
  if ( ctx->no_nav || ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( gc_type == glow_eDrawType_LineGray && highlight)
    gc_type = glow_eDrawType_Line;

  if ( angel1 >= 360)
    angel1 = angel1 - angel1 / 360 * 360;
  else if ( angel1 < 0)
    angel1 = angel1 + ( -angel1 / 360 + 1) * 360;

  XDrawArc( draw_ctx->display, draw_ctx->nav_window,
	draw_ctx->gcs[gc_type+highlight][idx], 
	x, y, width, height, angel1*64, angel2*64);
  return 1;
}

int glow_draw_nav_arc_erase( GlowCtx *ctx, int x, int y, int width, int height,
	int angel1, int angel2,
	int idx)
{
  draw_tCtx draw_ctx;
  if ( ctx->no_nav || ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( angel1 >= 360)
    angel1 = angel1 - angel1 / 360 * 360;
  else if ( angel1 < 0)
    angel1 = angel1 + ( -angel1 / 360 + 1) * 360;

  XDrawArc( draw_ctx->display, draw_ctx->nav_window,
	draw_ctx->gcs[glow_eDrawType_LineErase][idx], 
	x, y, width, height, angel1*64, angel2*64);
  return 1;
}

int glow_draw_line( GlowCtx *ctx, int x1, int y1, int x2, int y2,
	glow_eDrawType gc_type, int idx, int highlight)
{
  draw_tCtx draw_ctx;
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  // Fix for highlight for connections in grow
  if ( highlight && ctx->type() == glow_eCtxType_Grow)
    gc_type = glow_eDrawType_Color58;

  if ( gc_type == glow_eDrawType_LineGray && highlight)
    gc_type = glow_eDrawType_Line;

  if ( draw_ctx->clip_on)
    glow_set_clip( draw_ctx, draw_ctx->gcs[gc_type+highlight][idx]);

  if ( !ctx->draw_buffer_only)
    XDrawLine( draw_ctx->display, draw_ctx->window,
	draw_ctx->gcs[gc_type+highlight][idx], 
	x1, y1, x2, y2);
  if ( ctx->double_buffer_on)
    XDrawLine( draw_ctx->display, draw_ctx->buffer,
	draw_ctx->gcs[gc_type+highlight][idx], 
	x1, y1, x2, y2);

  if ( draw_ctx->clip_on)
    glow_reset_clip( draw_ctx, draw_ctx->gcs[gc_type+highlight][idx]);
  return 1;
}

int glow_draw_line_erase( GlowCtx *ctx, int x1, int y1, int x2, int y2,
	int idx)
{
  draw_tCtx draw_ctx;
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( draw_ctx->clip_on)
    glow_set_clip( draw_ctx, draw_ctx->gcs[glow_eDrawType_LineErase][idx]);

  if ( !ctx->draw_buffer_only)
    XDrawLine( draw_ctx->display, draw_ctx->window,
	draw_ctx->gcs[glow_eDrawType_LineErase][idx], 
	x1, y1, x2, y2);
  if ( ctx->double_buffer_on)
    XDrawLine( draw_ctx->display, draw_ctx->buffer,
	draw_ctx->gcs[glow_eDrawType_LineErase][idx], 
	x1, y1, x2, y2);

  if ( draw_ctx->clip_on)
    glow_reset_clip( draw_ctx, draw_ctx->gcs[glow_eDrawType_LineErase][idx]);

  return 1;
}

int glow_draw_nav_line( GlowCtx *ctx, int x1, int y1, int x2, int y2,
	glow_eDrawType gc_type, int idx, int highlight)
{
  draw_tCtx draw_ctx;
  if ( ctx->no_nav || ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( gc_type == glow_eDrawType_LineGray && highlight)
    gc_type = glow_eDrawType_Line;

  XDrawLine( draw_ctx->display, draw_ctx->nav_window,
	draw_ctx->gcs[gc_type+highlight][idx], 
	x1, y1, x2, y2);
  return 1;
}

int glow_draw_nav_line_erase( GlowCtx *ctx, int x1, int y1, int x2, int y2,
	int idx)
{
  draw_tCtx draw_ctx;
  if ( ctx->no_nav || ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XDrawLine( draw_ctx->display, draw_ctx->nav_window,
	draw_ctx->gcs[glow_eDrawType_LineErase][idx], 
	x1, y1, x2, y2);
  return 1;
}

int glow_draw_polyline( GlowCtx *ctx, XPoint *points, int point_cnt,
	glow_eDrawType gc_type, int idx, int highlight)
{
  draw_tCtx draw_ctx;
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( gc_type == glow_eDrawType_LineGray && highlight)
    gc_type = glow_eDrawType_Line;

  if ( draw_ctx->clip_on)
    glow_set_clip( draw_ctx, draw_ctx->gcs[gc_type+highlight][idx]);

  if ( !ctx->draw_buffer_only)
    XDrawLines( draw_ctx->display, draw_ctx->window,
	draw_ctx->gcs[gc_type+highlight][idx], 
	points, point_cnt, CoordModeOrigin);
  if ( ctx->double_buffer_on)
    XDrawLines( draw_ctx->display, draw_ctx->buffer,
	draw_ctx->gcs[gc_type+highlight][idx], 
	points, point_cnt, CoordModeOrigin);

  if ( draw_ctx->clip_on)
    glow_reset_clip( draw_ctx, draw_ctx->gcs[gc_type+highlight][idx]);
  return 1;
}

int glow_draw_fill_polyline( GlowCtx *ctx, XPoint *points, int point_cnt,
	glow_eDrawType gc_type, int highlight)
{
  draw_tCtx draw_ctx;
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( gc_type == glow_eDrawType_LineGray && highlight)
    gc_type = glow_eDrawType_Line;

  if ( draw_ctx->clip_on)
    glow_set_clip( draw_ctx, draw_ctx->gcs[gc_type+highlight][0]);

  if ( !ctx->draw_buffer_only)
    XFillPolygon( draw_ctx->display, draw_ctx->window,
	draw_ctx->gcs[gc_type+highlight][0], points, point_cnt, 
	Nonconvex, CoordModeOrigin);
  if ( ctx->double_buffer_on)
    XFillPolygon( draw_ctx->display, draw_ctx->buffer,
	draw_ctx->gcs[gc_type+highlight][0], points, point_cnt, 
	Nonconvex, CoordModeOrigin);

  if ( draw_ctx->clip_on)
    glow_reset_clip( draw_ctx, draw_ctx->gcs[gc_type+highlight][0]);
  return 1;
}

int glow_draw_nav_fill_polyline( GlowCtx *ctx, XPoint *points, int point_cnt,
	glow_eDrawType gc_type)
{
  draw_tCtx draw_ctx;
  if ( ctx->no_nav || ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XFillPolygon( draw_ctx->display, draw_ctx->nav_window,
	draw_ctx->gcs[gc_type][0], points, point_cnt, 
	Nonconvex, CoordModeOrigin);
  return 1;
}

int glow_draw_polyline_erase( GlowCtx *ctx, XPoint *points, int point_cnt,
	int idx)
{
  draw_tCtx draw_ctx;
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( draw_ctx->clip_on)
    glow_set_clip( draw_ctx, draw_ctx->gcs[glow_eDrawType_LineErase][idx]);

  if ( !ctx->draw_buffer_only)
    XDrawLines( draw_ctx->display, draw_ctx->window,
	draw_ctx->gcs[glow_eDrawType_LineErase][idx], 
	points, point_cnt, CoordModeOrigin);
  if ( ctx->double_buffer_on)
    XDrawLines( draw_ctx->display, draw_ctx->buffer,
	draw_ctx->gcs[glow_eDrawType_LineErase][idx], 
	points, point_cnt, CoordModeOrigin);

  if ( draw_ctx->clip_on)
    glow_reset_clip( draw_ctx, draw_ctx->gcs[glow_eDrawType_LineErase][idx]);
  return 1;
}

int glow_draw_nav_polyline( GlowCtx *ctx, XPoint *points, int point_cnt,
	glow_eDrawType gc_type, int idx, int highlight)
{
  draw_tCtx draw_ctx;
  if ( ctx->no_nav || ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( gc_type == glow_eDrawType_LineGray && highlight)
    gc_type = glow_eDrawType_Line;

  XDrawLines( draw_ctx->display, draw_ctx->nav_window,
	draw_ctx->gcs[gc_type+highlight][idx], 
	points, point_cnt, CoordModeOrigin);
  return 1;
}

int glow_draw_nav_polyline_erase( GlowCtx *ctx, XPoint *points, int point_cnt,
	int idx)
{
  draw_tCtx draw_ctx;
  if ( ctx->no_nav || ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XDrawLines( draw_ctx->display, draw_ctx->nav_window,
	draw_ctx->gcs[glow_eDrawType_LineErase][idx], 
	points, point_cnt, CoordModeOrigin);
  return 1;
}

int glow_draw_text( GlowCtx *ctx, int x, int y, char *text, int len,
	glow_eDrawType gc_type, int idx, int highlight, int line)
{
  draw_tCtx draw_ctx;
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( draw_ctx->clip_on)
    glow_set_clip( draw_ctx, draw_ctx->gcs[gc_type][idx]);

  if ( !ctx->draw_buffer_only)
    XDrawString( draw_ctx->display, draw_ctx->window,
	draw_ctx->gcs[gc_type][idx], 
	x, y, text, len);
  if ( ctx->double_buffer_on)
    XDrawString( draw_ctx->display, draw_ctx->buffer,
	draw_ctx->gcs[gc_type][idx], 
	x, y, text, len);
  if ( draw_ctx->clip_on)
    glow_reset_clip( draw_ctx, draw_ctx->gcs[gc_type][idx]);
  return 1;
}

int glow_draw_text_erase( GlowCtx *ctx, int x, int y, char *text, int len,
	glow_eDrawType gc_type, int idx, int line)
{
  draw_tCtx draw_ctx;
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( gc_type == glow_eDrawType_TextHelvetica)
    gc_type = glow_eDrawType_TextHelveticaErase;
  else if ( gc_type == glow_eDrawType_TextHelveticaBold)
    gc_type = glow_eDrawType_TextHelveticaEraseBold;

  if ( draw_ctx->clip_on)
    glow_set_clip( draw_ctx, draw_ctx->gcs[gc_type][idx]);
  if ( !ctx->draw_buffer_only)
    XDrawString( draw_ctx->display, draw_ctx->window,
	draw_ctx->gcs[gc_type][idx], 
	x, y, text, len);
  if ( ctx->double_buffer_on)
    XDrawString( draw_ctx->display, draw_ctx->buffer,
	draw_ctx->gcs[gc_type][idx], 
	x, y, text, len);
  if ( draw_ctx->clip_on)
    glow_reset_clip( draw_ctx, draw_ctx->gcs[gc_type][idx]);
  return 1;
}

int glow_draw_nav_text( GlowCtx *ctx, int x, int y, char *text, int len,
	glow_eDrawType gc_type, int idx, int highlight, int line)
{
  draw_tCtx draw_ctx;
  if ( ctx->no_nav || ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XDrawString( draw_ctx->display, draw_ctx->nav_window,
	draw_ctx->gcs[gc_type][idx], 
	x, y, text, len);
  return 1;
}

int glow_draw_nav_text_erase( GlowCtx *ctx, int x, int y, char *text, int len,
	glow_eDrawType gc_type, int idx, int line)
{
  draw_tCtx draw_ctx;
  if ( ctx->no_nav || ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( gc_type == glow_eDrawType_TextHelvetica)
    gc_type = glow_eDrawType_TextHelveticaErase;
  else if ( gc_type == glow_eDrawType_TextHelveticaBold)
    gc_type = glow_eDrawType_TextHelveticaEraseBold;
  XDrawString( draw_ctx->display, draw_ctx->nav_window,
	draw_ctx->gcs[gc_type][idx], 
	x, y, text, len);
  return 1;
}

int glow_draw_pixmaps_create( GlowCtx *ctx, glow_sPixmapData *pixmap_data,
	void **pixmaps)
{
  draw_tCtx draw_ctx;
  draw_sPixmap *pms;
  glow_sPixmapDataElem *prev_pdata, *pdata = (glow_sPixmapDataElem *)pixmap_data;
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

void glow_draw_pixmaps_delete( GlowCtx *ctx, void *pixmaps)
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

int glow_draw_pixmap( GlowCtx *ctx, int x, int y, glow_sPixmapData *pixmap_data,
	void *pixmaps, glow_eDrawType gc_type, int idx, int highlight, int line)
{
  draw_tCtx draw_ctx;
  draw_sPixmap *pms;
  glow_sPixmapDataElem *pdata = (glow_sPixmapDataElem *)pixmap_data + idx;

  if ( ctx->nodraw) return 1;
  
  draw_ctx = (draw_tCtx) ctx->draw_ctx;
  pms = (draw_sPixmap *) pixmaps;

  if ( draw_ctx->clip_on)
    glow_set_clip( draw_ctx, draw_ctx->gcs[gc_type][idx]);

  if ( !ctx->draw_buffer_only)
    XCopyPlane( draw_ctx->display, pms->pixmap[idx], draw_ctx->window,
	draw_ctx->gcs[gc_type][idx], 
	0, 0, pdata->width, pdata->height, x, y, 1);
  if ( ctx->double_buffer_on)
    XCopyPlane( draw_ctx->display, pms->pixmap[idx], draw_ctx->buffer,
	draw_ctx->gcs[gc_type][idx], 
	0, 0, pdata->width, pdata->height, x, y, 1);

  if ( draw_ctx->clip_on)
    glow_reset_clip( draw_ctx, draw_ctx->gcs[gc_type][idx]);
  return 1;
}

int glow_draw_pixmap_inverse( GlowCtx *ctx, int x, int y, glow_sPixmapData *pixmap_data,
	void *pixmaps, glow_eDrawType gc_type, int idx, int line)
{
  draw_tCtx draw_ctx;
  draw_sPixmap *pms;
  glow_sPixmapDataElem *pdata = (glow_sPixmapDataElem *)pixmap_data + idx;

  if ( ctx->nodraw) return 1;
  
  draw_ctx = (draw_tCtx) ctx->draw_ctx;
  pms = (draw_sPixmap *) pixmaps;

  if ( draw_ctx->clip_on)
    glow_set_clip( draw_ctx, draw_ctx->gc_inverse);
  if ( !ctx->draw_buffer_only)
    XCopyPlane( draw_ctx->display, pms->pixmap[idx], draw_ctx->window,
	draw_ctx->gc_inverse,
	0, 0, pdata->width, pdata->height, x, y, 1);
  if ( ctx->double_buffer_on)
    XCopyPlane( draw_ctx->display, pms->pixmap[idx], draw_ctx->buffer,
	draw_ctx->gc_inverse,
	0, 0, pdata->width, pdata->height, x, y, 1);
  if ( draw_ctx->clip_on)
    glow_reset_clip( draw_ctx, draw_ctx->gc_inverse);
  return 1;
}

int glow_draw_pixmap_erase( GlowCtx *ctx, int x, int y, glow_sPixmapData *pixmap_data,
	void *pixmaps, glow_eDrawType gc_type, int idx, int line)
{
  draw_tCtx draw_ctx;
  glow_sPixmapDataElem *pdata = (glow_sPixmapDataElem *)pixmap_data + idx;
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  // Intersection with clipmask should be calculated here...

  if ( !ctx->draw_buffer_only)
    XClearArea( draw_ctx->display, draw_ctx->window, 
	x, y, pdata->width, pdata->height, False);
  if ( ctx->double_buffer_on)
    XClearArea( draw_ctx->display, draw_ctx->buffer, 
	x, y, pdata->width, pdata->height, False);
  return 1;
}

int glow_draw_nav_pixmap( GlowCtx *ctx, int x, int y, glow_sPixmapData *pixmap_data,
	void *pixmaps, glow_eDrawType gc_type, int idx, int highlight, int line)
{
  draw_tCtx draw_ctx;
  draw_sPixmap *pms;
  glow_sPixmapDataElem *pdata = (glow_sPixmapDataElem *)pixmap_data + idx;

  if ( ctx->no_nav || ctx->nodraw) return 1;
  
  draw_ctx = (draw_tCtx) ctx->draw_ctx;
  pms = (draw_sPixmap *) pixmaps;

  XCopyPlane( draw_ctx->display, pms->pixmap[idx], draw_ctx->nav_window,
	draw_ctx->gcs[gc_type][idx], 
	0, 0, pdata->width, pdata->height, x, y, 1);
  return 1;
}

int glow_draw_nav_pixmap_erase( GlowCtx *ctx, int x, int y, glow_sPixmapData *pixmap_data,
	void *pixmaps, glow_eDrawType gc_type, int idx, int line)
{
  draw_tCtx draw_ctx;
  glow_sPixmapDataElem *pdata = (glow_sPixmapDataElem *)pixmap_data + idx;
  if ( ctx->no_nav || ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XClearArea( draw_ctx->display, draw_ctx->nav_window, 
	x, y, pdata->width, pdata->height, False);
  return 1;
}

int glow_draw_image( GlowCtx *ctx, int x, int y, int width, int height,
		     Pixmap pixmap, Pixmap clip_mask)
{
  draw_tCtx draw_ctx;

  if ( ctx->nodraw) return 1;

  if ( width == 0 || height == 0)
    return 1;
  
  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( clip_mask)
    glow_set_image_clip_mask( ctx, clip_mask, x, y);
  else if ( draw_ctx->clip_on)
    glow_set_clip( draw_ctx, draw_ctx->gcs[glow_eDrawType_Line][0]);

  if ( !ctx->draw_buffer_only)
    XCopyArea( draw_ctx->display, pixmap, draw_ctx->window,
	draw_ctx->gcs[glow_eDrawType_Line][0], 
	0, 0, width, height, x, y);
  if ( ctx->double_buffer_on)
    XCopyArea( draw_ctx->display, pixmap, draw_ctx->buffer,
	draw_ctx->gcs[glow_eDrawType_Line][0], 
	0, 0, width, height, x, y);

  if ( clip_mask)
    glow_reset_image_clip_mask( ctx);
  else if ( draw_ctx->clip_on)
    glow_reset_clip( draw_ctx, draw_ctx->gcs[glow_eDrawType_Line][0]);
  return 1;
}


int glow_draw_nav_image( GlowCtx *ctx, int x, int y, int width, int height,
		     Pixmap pixmap, Pixmap clip_mask)
{
  draw_tCtx draw_ctx;

  if ( ctx->no_nav || ctx->nodraw) return 1;
  
  if ( width == 0 || height == 0)
    return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( clip_mask)
    glow_set_image_clip_mask( ctx, clip_mask, x, y);
  else if ( draw_ctx->clip_on)
    glow_set_clip( draw_ctx, draw_ctx->gcs[glow_eDrawType_Line][0]);

  XCopyArea( draw_ctx->display, pixmap, draw_ctx->nav_window,
	draw_ctx->gcs[glow_eDrawType_Line][0], 
	0, 0, width, height, x, y);

  if ( clip_mask)
    glow_reset_image_clip_mask( ctx);
  else if ( draw_ctx->clip_on)
    glow_reset_clip( draw_ctx, draw_ctx->gcs[glow_eDrawType_Line][0]);
  return 1;
}


int glow_draw_fill_rect( GlowCtx *ctx, int x, int y, int w, int h, 
	glow_eDrawType gc_type)
{
  draw_tCtx draw_ctx;
  XPoint p[5] = {{x,y},{x+w,y},{x+w,y+h},{x,y+h},{x,y}};
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( draw_ctx->clip_on)
    glow_set_clip( draw_ctx, draw_ctx->gcs[gc_type][0]);
  if ( !ctx->draw_buffer_only)
    XFillPolygon( draw_ctx->display, draw_ctx->window,
	draw_ctx->gcs[gc_type][0], p, 5, Convex, CoordModeOrigin);
  if ( ctx->double_buffer_on)
    XFillPolygon( draw_ctx->display, draw_ctx->buffer,
	draw_ctx->gcs[gc_type][0], p, 5, Convex, CoordModeOrigin);
  if ( draw_ctx->clip_on)
    glow_reset_clip( draw_ctx, draw_ctx->gcs[gc_type][0]);
  return 1;
}

int glow_draw_nav_fill_rect( GlowCtx *ctx, int x, int y, int w, int h, 
	glow_eDrawType gc_type)
{
  draw_tCtx draw_ctx;
  XPoint p[5] = {{x,y},{x+w,y},{x+w,y+h},{x,y+h},{x,y}};
  if ( ctx->no_nav || ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XFillPolygon( draw_ctx->display, draw_ctx->nav_window,
	draw_ctx->gcs[gc_type][0], p, 5, Convex, CoordModeOrigin);
  return 1;
}

void glow_draw_clear( GlowCtx *ctx)
{
  draw_tCtx draw_ctx;
  if ( ctx->nodraw) return;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( !ctx->double_buffer_on)
    XClearWindow( draw_ctx->display, draw_ctx->window);
  else
    glow_draw_buffer_background( ctx);
}

void glow_draw_nav_clear( GlowCtx *ctx)
{
  draw_tCtx draw_ctx;
  if ( ctx->no_nav || ctx->nodraw) return;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XClearWindow( draw_ctx->display, draw_ctx->nav_window);
}

void glow_draw_copy_buffer( GlowCtx *ctx,
		int ll_x, int ll_y, int ur_x, int ur_y)
{
  if ( ctx->nodraw) return;

  draw_tCtx draw_ctx = (draw_tCtx) ctx->draw_ctx;
  int x0 = min( ll_x, ur_x);
  int x1 = max( ll_x, ur_x);
  int y0 = min( ll_y, ur_y);
  int y1 = max( ll_y, ur_y);

  XCopyArea( draw_ctx->display, draw_ctx->buffer, draw_ctx->window,
	draw_ctx->gcs[glow_eDrawType_Line][0], 
	x0, y0, x1 - x0, y1 - y0, x0, y0);
}

void glow_draw_get_window_size( GlowCtx *ctx, int *width, int *height)
{
  XWindowAttributes attr;
  draw_tCtx draw_ctx;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XGetWindowAttributes( draw_ctx->display, draw_ctx->window, &attr);
  *width = attr.width;
  *height = attr.height;
}

void glow_draw_get_nav_window_size( GlowCtx *ctx, int *width, int *height)
{
  XWindowAttributes attr;
  draw_tCtx draw_ctx;

  if ( ctx->no_nav) return;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XGetWindowAttributes( draw_ctx->display, draw_ctx->nav_window, &attr);
  *width = attr.width;
  *height = attr.height;
}

void glow_draw_set_nav_window_size( GlowCtx *ctx, int width, int height)
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

static void draw_timer_cb( draw_sTimerCb *timer_cb)
{
  draw_tCtx draw_ctx;

  draw_ctx = (draw_tCtx) timer_cb->ctx->draw_ctx;

  (timer_cb->callback_func)(timer_cb->ctx);
  XFlush( draw_ctx->display);
  free( timer_cb);
}

static void event_timer_cb( GlowCtx *ctx)
{
//  printf( "Timer callback\n");
  draw_event_handler( ctx, last_event);
}

static void cancel_event_timer( GlowCtx *ctx)
{
  draw_tCtx draw_ctx  = (draw_tCtx) ctx->draw_ctx;
  if ( draw_ctx->timer_id)
    XtRemoveTimeOut( draw_ctx->timer_id);
//  printf( "Timer removed\n");
//  sys$cantim( ctx, 0);
}

static void event_timer( GlowCtx *ctx, int time_ms)
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

void draw_set_timer( GlowCtx *ctx, int time_ms,
  	void (*callback_func)( GlowCtx *ctx), void **id)
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

void draw_remove_timer( void *id)
{
  XtRemoveTimeOut( ((draw_sTimerCb *)id)->timer_id);

//  sys$cantim( id, 0);
  free( (char *) id);
}

void draw_set_cursor( GlowCtx *ctx, glow_eDrawCursor cursor)
{
  draw_tCtx draw_ctx;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( cursor == glow_eDrawCursor_Normal)
    XUndefineCursor( draw_ctx->display, draw_ctx->window);
  else
    XDefineCursor( draw_ctx->display, draw_ctx->window, 
		draw_ctx->cursors[cursor]);
  XFlush( draw_ctx->display);
}

void draw_set_nav_cursor( GlowCtx *ctx, glow_eDrawCursor cursor)
{
  draw_tCtx draw_ctx;

  if ( ctx->no_nav) return;
  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( cursor == glow_eDrawCursor_Normal)
    XUndefineCursor( draw_ctx->display, draw_ctx->nav_window);
  else
    XDefineCursor( draw_ctx->display, draw_ctx->nav_window, 
		draw_ctx->cursors[cursor]);
  XFlush( draw_ctx->display);
}

int draw_get_text_extent( GlowCtx *ctx, char *text, int len,
	glow_eDrawType gc_type, int idx,
	int *width, int *height, int *descent)
{
  draw_tCtx draw_ctx;
  int	text_direction, text_ascent, text_descent;
  XCharStruct char_struct;
  glow_eDrawFont font_idx;

  switch( gc_type) {
    case glow_eDrawType_TextHelvetica:
    case glow_eDrawType_TextHelveticaErase:
      font_idx = glow_eDrawFont_Helvetica;
      break;
    case glow_eDrawType_TextHelveticaBold:
    case glow_eDrawType_TextHelveticaEraseBold:
      font_idx = glow_eDrawFont_HelveticaBold;
      break;
    default:
      *width = 0;
      *height = 0;
      return 0;
  }
  draw_ctx = (draw_tCtx) ctx->draw_ctx;
  
  XTextExtents( draw_ctx->font_struct[font_idx][idx], text, len,
	&text_direction, &text_ascent, &text_descent, &char_struct);
  *height = draw_ctx->font_struct[font_idx][idx]->ascent + 
  	draw_ctx->font_struct[font_idx][idx]->descent;
  *descent = draw_ctx->font_struct[font_idx][idx]->descent;
  *width = char_struct.width;
  return 1;
}

void draw_copy_area( GlowCtx *ctx, int x, int y)
{
  draw_tCtx draw_ctx;
  GC gc;
  if ( ctx->nodraw) return;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;
  gc = draw_ctx->gcs[glow_eDrawType_Line][3];
  if ( x >= 0 && y >= 0)
  {
    XCopyArea( draw_ctx->display, draw_ctx->window, draw_ctx->window, gc, 
	0, 0, ctx->window_width-x, ctx->window_height-y, x, y);
    if ( !ctx->double_buffer_on) {
      if ( x)
        XClearArea( draw_ctx->display, draw_ctx->window, 0, 0, 
		x, ctx->window_height, 0);
      if ( y)
        XClearArea( draw_ctx->display, draw_ctx->window, x, 0, 
		ctx->window_width, y, 0);
    }
    else {
      XCopyArea( draw_ctx->display, draw_ctx->buffer, draw_ctx->buffer, gc, 
	0, 0, ctx->window_width-x, ctx->window_height-y, x, y);
      if ( x)
        XFillRectangle( draw_ctx->display, draw_ctx->buffer, 
			draw_ctx->gcs[glow_eDrawType_LineErase][0],
			0, 0, x, ctx->window_height);
      if ( y)
        XFillRectangle( draw_ctx->display, draw_ctx->buffer, 
			draw_ctx->gcs[glow_eDrawType_LineErase][0],
			x, 0, ctx->window_width, y);
    }
  }
  else if ( x <= 0 && y <= 0)
  {
    XCopyArea( draw_ctx->display, draw_ctx->window, draw_ctx->window, gc,
	-x, -y, ctx->window_width+x, ctx->window_height+y, 0, 0);
    if ( !ctx->double_buffer_on) {
      if ( x)
        XClearArea( draw_ctx->display, draw_ctx->window, 
	  ctx->window_width+x, 0, ctx->window_width, ctx->window_height, 0);
      if ( y)
        XClearArea( draw_ctx->display, draw_ctx->window, 
	  0, ctx->window_height+y, ctx->window_width+x, ctx->window_height, 0);
    }
    else {
      XCopyArea( draw_ctx->display, draw_ctx->buffer, draw_ctx->buffer, gc,
	-x, -y, ctx->window_width+x, ctx->window_height+y, 0, 0);
      if ( x)
        XFillRectangle( draw_ctx->display, draw_ctx->buffer, 
	  draw_ctx->gcs[glow_eDrawType_LineErase][0],
	  ctx->window_width+x, 0, ctx->window_width, ctx->window_height);
      if ( y)
        XFillRectangle( draw_ctx->display, draw_ctx->buffer, 
	  draw_ctx->gcs[glow_eDrawType_LineErase][0],
	  0, ctx->window_height+y, ctx->window_width+x, ctx->window_height);
    }
  }
  else if ( x <= 0 && y >= 0)
  {
    XCopyArea( draw_ctx->display, draw_ctx->window, draw_ctx->window, gc,
	-x, 0, ctx->window_width+x, ctx->window_height-y, 0, y);
    if ( !ctx->double_buffer_on) {
      if ( x)
        XClearArea( draw_ctx->display, draw_ctx->window, 
	  ctx->window_width+x, 0, ctx->window_width, ctx->window_height, 0);
      if ( y)
        XClearArea( draw_ctx->display, draw_ctx->window, 
	  0, 0, ctx->window_width+x, y, 0);
    }
    else {
      XCopyArea( draw_ctx->display, draw_ctx->buffer, draw_ctx->buffer, gc,
	-x, 0, ctx->window_width+x, ctx->window_height-y, 0, y);
      if ( x)
        XFillRectangle( draw_ctx->display, draw_ctx->buffer, 
	  draw_ctx->gcs[glow_eDrawType_LineErase][0],
	  ctx->window_width+x, 0, ctx->window_width, ctx->window_height);
      if ( y)
        XFillRectangle( draw_ctx->display, draw_ctx->buffer, 
	  draw_ctx->gcs[glow_eDrawType_LineErase][0],
	  0, 0, ctx->window_width+x, y);
    }
  }
  else
  {
    XCopyArea( draw_ctx->display, draw_ctx->window, draw_ctx->window, gc,
	0, -y, ctx->window_width-x, ctx->window_height+y, x, 0);
    if ( !ctx->double_buffer_on) {
      if ( x)
        XClearArea( draw_ctx->display, draw_ctx->window, 
	  0, 0, x, ctx->window_height, 0);
      if ( y)
        XClearArea( draw_ctx->display, draw_ctx->window, 
	  x, ctx->window_height+y, ctx->window_width, ctx->window_height, 0);
    }
    else {
      XCopyArea( draw_ctx->display, draw_ctx->buffer, draw_ctx->buffer, gc,
	0, -y, ctx->window_width-x, ctx->window_height+y, x, 0);
      if ( x)
        XFillRectangle( draw_ctx->display, draw_ctx->buffer, 
	  draw_ctx->gcs[glow_eDrawType_LineErase][0],
	  0, 0, x, ctx->window_height);
      if ( y)
        XFillRectangle( draw_ctx->display, draw_ctx->buffer, 
	  draw_ctx->gcs[glow_eDrawType_LineErase][0],
	  x, ctx->window_height+y, ctx->window_width, ctx->window_height);
    }
  }
}

void draw_clear_area( GlowCtx *ctx, int ll_x, int ur_x, int ll_y, int ur_y)
{
  draw_tCtx draw_ctx;

  if ( ctx->nodraw) return;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;
  XClearArea( draw_ctx->display, draw_ctx->window, ll_x, ll_y, ur_x - ll_x, 
		ur_y - ll_y, 0);
}

int glow_draw_create_input( GlowCtx *ctx, int x, int y, char *text, int len,
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
  Widget np, p;
  np = XtParent(draw_ctx->toplevel);
  while ( np)
  {
    p = np;
    np = XtParent( p);
  } 
  XtSetKeyboardFocus( p, annot_data->w);
//  XSetInputFocus( draw_ctx->display, draw_ctx->window, 
//		RevertToNone, CurrentTime);
//  XSetInputFocus( draw_ctx->display, XtWindow(annot_data->w), 
//		RevertToNone, CurrentTime);
  annot_data->node = (GlowArrayElem *)node;
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

int glow_draw_close_input( GlowCtx *ctx, void *data)
{
  draw_sAnnotData *annot_data = (draw_sAnnotData *)data;
  XtDestroyWidget( annot_data->w);
  free( (char *) annot_data);
  return 1;
}

int glow_draw_get_input( GlowCtx *ctx, void *data, char **text)
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

void glow_draw_move_input( GlowCtx *ctx, void *data, int x, int y,
	glow_ePosition pos_type)
{
  draw_sAnnotData *annot_data = (draw_sAnnotData *)data;

  if ( pos_type == glow_ePosition_Relative)
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

void glow_set_inputfocus( GlowCtx *ctx)
{
  draw_tCtx draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XSetInputFocus( draw_ctx->display, draw_ctx->window, 
		RevertToNone, CurrentTime);
}

static int glow_read_color_file( char *filename, draw_sColor **color_array, 
	int *size)
{
  char		line[80];
  draw_sColor	*color_p;
  ifstream	fp;
  int		nr;
  int		line_cnt;
  float		f_red, f_green, f_blue;

  if ( !check_file( filename))
  {
    // No color-file exist, use default values
    double default_color[] = {
1, 		1, 		1,		// 4 White 
0,		0.8714, 	0.2857,		// 5 SpringGreen3
1, 		1, 		0,		// 6 Yellow
0.2879, 	0.4945, 	0.9067,		// 7 SteelBlue3
0.593, 		0, 		0.6421,		// 8 Magenta3
0.270, 		0.270, 		0.270,		// 9 Grey
0.160, 		0.160,		0.160,		// 10 Grey
// Grey
0.950, 		0.950, 		0.950,		// 1 Grey
0.900, 		0.900, 		0.900,		// 2 Grey
0.850, 		0.850, 		0.850,		// 3 Grey
0.700, 		0.700, 		0.700,		// 4 Grey93
0.500, 		0.500, 		0.500,		// 5 Grey80
0.935, 		0.935, 		0.935,		// 6 Grey68
0.880, 		0.880, 		0.880,		// 7 Grey48
0.750, 		0.750, 		0.750,		// 8 Grey35
0.550, 		0.550, 		0.550,		// 9 Grey
0.350, 		0.350, 		0.350,		// 10 Grey
1,		1,		0.878,		// 1 LightYellow0
1,		1,		0.640,		// 2 LightYellow1
0.900,		0.900,		0.500,		// 3 LightYellow2
0.804,		0.804,		0.400,		// 4 LightYellow3
0.545,		0.545,		0.300,		// 5 LightYellow4
1,		1,		0.300,		// 6 yellow0
1,		1,		0,		// 7 yellow1
0.933,		0.933,		0,		// 8 yellow2
0.804,		0.804,		0,		// 9 yellow3
0.545,		0.545,		0,		// 10 yellow4
1,		0.970,		0.800,		// 1 Lightgoldenrod0
1,		0.925,		0.545,		// 2 Lightgoldenrod1
0.933,		0.862,		0.510,		// 3 Lightgoldenrod2
0.804,		0.745,		0.439,		// 4 Lightgoldenrod3
0.545,		0.506,		0.298,		// 5 Lightgoldenrod4
1,		0.930,		0.500,		// 6 gold00
1,		0.843,		0,		// 7 gold1
0.933,		0.788,		0,		// 8 gold2
0.804,		0.678,		0,		// 9 gold3
0.545,		0.459,		0,		// 10 gold4
1,		0.941,		0.800,		// 1 LightOrange (Modified)
0.950,		0.870,		0.678,		// 2 NavajoWhite
0.900,		0.628,		0.478,		// 3 LightSalmon1
0.804,		0.506,		0.384,		// 4 LightSalmon3
0.545,		0.341,		0.259,		// 5 LightSalmon4
1,		0.850,		0.550,		// 6 DarkOrange (Modified)
1,		0.650,		0.300,		// 7 DarkOrange (Modified)
1,		0.498,		0,		// 8 DarkOrange1
0.804,		0.400,		0,		// 9 DarkOrange3
0.545,		0.153,		0,		// 10 DarkOrange4
1,		0.894,		0.867,		// 1 MistyRose1
0.933,		0.835,		0.823,		// 2 MistyRose2
0.933,		0.682,		0.725,		// 3 LightPink (Modified)
0.804,		0.547,		0.584,		// 4 LightPink3
0.545,		0.357,		0.396,		// 5 LightPink4
1,		0.733,		0.733,		// 6 IndianRed0 (Modified)
1,		0.415,		0.415,		// 7 IndianRed1
1,		0.188,		0.188,		// 8 firebrick1
0.804,		0.149,		0.149,		// 9 firebrick3
0.545,		0.102,		0.102,		// 10 firebrick4
1,		0.890,		1,		// 1 Plum1 (Modified)
1,		0.733,		1,		// 2 Plum1
0.933,		0.682,		0.933,		// 3 Plum2
0.804,		0.588,		0.804,		// 4 Plum3
0.545,		0.400,		0.545,		// 5 Plum4
1,		0.800,		1,		// 6 Magenta00 (Mofified)
1,		0.500,		1,		// 7 Magenta0 (Modified) 
1,		0,		1,		// 8 Magenta1
0.804,		0,		0.804,		// 9 Magenta3
0.545,		0,		0.545,		// 10 Magenta4
0.829,		0.919,		1,		// 1 LightSteelBlue1 (Modified)
0.792,		0.882,		1,		// 2 LightSteelBlue1  
0.737,		0.823,		0.933,		// 3 LightSteelBlue2  
0.635,		0.710,		0.804,		// 4 LightSteelBlue3  
0.431,		0.482,		0.545,		// 5 LightSteelBlue4  
0.829,		0.919,		1,		// 6 LightCyan 
0.690,		0.878,		0.902,		// 7 Powder blue
0.388,		0.722,		1,		// 8 SteelBlue1  
0.275,		0.510,		0.706,		// 9 SteelBlue  
0,		0,		0.545,		// 10 blue4  
// Bluegreen
0.700,		0.960,		0.900,		// 1 aquamarine4 (Modified)
0.600,		0.930,		0.850,		// 2 aquamarine4  
0.463,		0.900,		0.700,		// 3 aquamarine4  
0.400,		0.804,		0.667,		// 4 aquamarine4  
0.271,		0.545,		0.455,		// 5 aquamarine4  
0.600,		1,		0.950,		// 6 turqoise 
0,		0.930,		0.930,		// 7 turqoise
0,		0.870,		0.720,		// 8 turqoise  
0,		0.700,		0.500,		// 9 turqoise  
0,		0.500,		0.300,		// 10 turqoise
0.829,		1,		0.7,		// 1 DarkOliveGreen1 (Modified)
0.737,		0.933,		0.408,		// 2 DarkOliveGreen2  
0.635,		0.804,		0.353,		// 3 DarkOliveGreen3  
// 0.431,		0.545,		0.239, DarkOliveGreen4 changed 020429  
0.520,		0.700,		0.275,		// 4 DarkOliveGreen4  
0.333,	  	0.420,		0.184,		// 5 DarkOliveGreen  
0.698,		1,		0.400,		// 6 chartreuse  	
0.463,		0.933,		0,		// 7 chartreuse2  	
0.4,		0.804,		0,		// 8 chartreuse3  	
0.27,		0.545,		0,		// 9 chartreuse4  	
0,		0.392,		0};		// 10 DarkGreen  	

    printf( "** Using default color palette\n");
    *color_array = (draw_sColor *) calloc( 100, sizeof( draw_sColor));
    *size = 0;
    color_p = *color_array;
    for ( int i = 0; i < int(sizeof(default_color)/sizeof(default_color[0]));)
    {
      color_p->red = int( default_color[i++] * 65535);
      color_p->green = int( default_color[i++] * 65535);
      color_p->blue = int( default_color[i++] * 65535);
      color_p++;
      (*size)++;
    }
  }
  else
  {
    printf( "** Opening color file %s\n", filename);

    fp.open( filename);
#ifndef OS_VMS
    if ( !fp)
      return GLOW__FILEOPEN;
#endif

    *color_array = (draw_sColor *) calloc( 100, sizeof( draw_sColor));
    *size = 0;
    line_cnt = 0;
    color_p = *color_array;
    while ( *size < 100)
    {
      fp.getline( line, sizeof( line));
      if ( line[0] == 0)
        break;
      line_cnt++;
      if ( line[0] == '!' || line[0] == '#')
        continue;
      nr = sscanf( line, "%f %f %f", &f_red, &f_green, &f_blue);
      if ( nr != 3) 
        printf( "** Syntax error in file %s, line %d", filename, line_cnt);
      else
      {
        color_p->red = int( f_red * 65535);
        color_p->green = int( f_green * 65535);
        color_p->blue = int( f_blue * 65535);
        color_p++;
        (*size)++;
      }
    }
    fp.close();
  }

  return 1;
}

char *glow_color_to_name( glow_eDrawType drawtype)
{
  static char color_name[20];
  char default_color_names[][20] = {
"Black","Red","Gray","White","Green","Yellow","Blue","Magenta","Grey","Grey",
"Grey1","Grey2","Grey3","Grey4","Grey5","Grey6","Grey7","Grey8","Grey9","Grey10",
"Yellow1","Yellow2","Yellow3","Yellow4","Yellow5","Yellow6","Yellow7","Yellow8","Yellow9","Yellow10",
"Gold1","Gold2","Gold3","Gold4","Gold5","Gold6","Gold7","Gold8","Gold9","Gold10",
"Orange1","Orange2","Orange3","Orange4","Orange5","Orange6","Orange7","Orange8","Orange9","Orange10",
"Red1","Red2","Red3","Red4","Red5","Red6","Red7","Red8","Red9","Red10",
"Magenta1","Magenta2","Magenta3","Magenta4","Magenta5","Magenta6","Magenta7","Magenta8","Magenta9","Magenta10",
"Blue1","Blue2","Blue3","Blue4","Blue5","Blue6","Blue7","Blue8","Blue9","Blue10",
"Turqoise1","Turqoise2","Turqoise3","Turqoise4","Turqoise5","Turqoise6","Turqoise7","Turqoise8","Turqoise9","Turqoise10",
"Green1","Green2","Green3","Green4","Green5","Green6","Green7","Green8","Green9","Green10"
  };
  if ( (unsigned int) drawtype > 
        sizeof(default_color_names)/sizeof(default_color_names[0])-1)
    strcpy( color_name, "");
  else
    strcpy( color_name, default_color_names[drawtype]);

  return color_name;
}

char *glow_colortone_to_name( glow_eDrawType drawtype)
{
  static char tone_name[20];
  char color_tone_names[][20] = {
	"NoTone", "ToneGray","ToneYellow","ToneGold","ToneOrange","ToneRed",
	"ToneMagenta", "ToneBlue","ToneTurqiose", "ToneGreen", "DarkGray"};

  if ( (unsigned int) drawtype > 
        sizeof(color_tone_names)/sizeof(color_tone_names[0])-1)
    strcpy( tone_name, "");
  else
    strcpy( tone_name, color_tone_names[drawtype]);
  return tone_name;
}

void glow_set_background( GlowCtx *ctx, glow_eDrawType drawtype, Pixmap pixmap,
      int pixmap_width, int pixmap_height)
{
  XGCValues 		xgcv;
  draw_tCtx draw_ctx = (draw_tCtx) ctx->draw_ctx;
  Arg 			args[20];
  int			i;
  int			sts;

  sts = XGetGCValues( draw_ctx->display, draw_ctx->gcs[drawtype][0], 
	GCForeground, &xgcv);
  if ( !sts)
  {
    cout << "** Error getting background gc" << endl;
    return;
  }
  draw_ctx->background = xgcv.foreground;

  // Change erase gcs
  xgcv.foreground =   draw_ctx->background;
  xgcv.background =   draw_ctx->background;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++)
  {
    XChangeGC( draw_ctx->display, draw_ctx->gcs[glow_eDrawType_LineErase][i],
	GCForeground | GCBackground, &xgcv);
    XChangeGC( draw_ctx->display, draw_ctx->gcs[glow_eDrawType_TextHelveticaErase][i],
	GCForeground | GCBackground, &xgcv);
    XChangeGC( draw_ctx->display, draw_ctx->gcs[glow_eDrawType_TextHelveticaEraseBold][i],
      GCForeground | GCBackground, &xgcv);
  }

  if ( ! pixmap)
  {
    i = 0;
    XtSetArg(args[i],XmNbackground, draw_ctx->background); i++;
    XtSetValues( draw_ctx->toplevel, args, i);
    if ( !ctx->no_nav)
      XtSetValues( draw_ctx->nav_toplevel, args, i);

    if ( draw_ctx->buffer)
      glow_draw_buffer_background( ctx);
  }
  else
  {
    if ( draw_ctx->background_pixmap)
      XFreePixmap( draw_ctx->display, draw_ctx->background_pixmap);

    draw_ctx->background_pixmap = pixmap;
    draw_ctx->background_pixmap_width = pixmap_width;
    draw_ctx->background_pixmap_height = pixmap_height;
    XSetWindowBackgroundPixmap( draw_ctx->display, draw_ctx->window, pixmap);
    if ( draw_ctx->buffer)
      glow_draw_buffer_background( ctx);
  }
}

void glow_reset_background( GlowCtx *ctx)
{
  XGCValues 		xgcv;
  draw_tCtx draw_ctx = (draw_tCtx) ctx->draw_ctx;
  Arg 			args[20];
  int			i;

  i = 0;
  draw_ctx->background = draw_ctx->original_background;
  XtSetArg(args[i],XmNbackground, draw_ctx->background); i++;
  XtSetValues( draw_ctx->toplevel, args, i);
  if ( !ctx->no_nav)
    XtSetValues( draw_ctx->nav_toplevel, args, i);

  // Change erase gcs
  xgcv.foreground =  draw_ctx->background;
  xgcv.background =  draw_ctx->background;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++)
  {
    XChangeGC( draw_ctx->display, draw_ctx->gcs[glow_eDrawType_LineErase][i],
	GCForeground | GCBackground, &xgcv);
    XChangeGC( draw_ctx->display, draw_ctx->gcs[glow_eDrawType_TextHelveticaErase][i],
	GCForeground | GCBackground, &xgcv);
    XChangeGC( draw_ctx->display, draw_ctx->gcs[glow_eDrawType_TextHelveticaEraseBold][i],
	GCForeground | GCBackground, &xgcv);
  }
}

static void glow_set_clip( draw_tCtx draw_ctx, GC gc)
{
  XSetClipRectangles( draw_ctx->display, gc, 0, 0, &draw_ctx->clip_rectangle,
	1, Unsorted);
}

static void glow_reset_clip( draw_tCtx draw_ctx, GC gc)
{
  XSetClipMask( draw_ctx->display, gc, None);
}

void glow_set_image_clip_mask( GlowCtx *ctx, Pixmap pixmap, int x, int y)
{
  draw_tCtx draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XSetClipMask( draw_ctx->display, draw_ctx->gcs[glow_eDrawType_Line][0], 
		pixmap);
  XSetClipOrigin( draw_ctx->display, draw_ctx->gcs[glow_eDrawType_Line][0], x, y);
}

void glow_reset_image_clip_mask( GlowCtx *ctx)
{
  draw_tCtx draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XSetClipMask( draw_ctx->display, draw_ctx->gcs[glow_eDrawType_Line][0], 
		None);
  XSetClipOrigin( draw_ctx->display, draw_ctx->gcs[glow_eDrawType_Line][0], 0, 0);
}

void glow_draw_set_clip_rectangle( GlowCtx *ctx,
		int ll_x, int ll_y, int ur_x, int ur_y)
{
  draw_tCtx draw_ctx = (draw_tCtx) ctx->draw_ctx;
  int x0 = min( ll_x, ur_x);
  int x1 = max( ll_x, ur_x);
  int y0 = min( ll_y, ur_y);
  int y1 = max( ll_y, ur_y);

  draw_ctx->clip_rectangle.x = x0;
  draw_ctx->clip_rectangle.y = y0;
  draw_ctx->clip_rectangle.width = x1 - x0;
  draw_ctx->clip_rectangle.height = y1 - y0;
  draw_ctx->clip_on = 1;
}

void glow_draw_reset_clip_rectangle( GlowCtx *ctx)
{
  ((draw_tCtx) ctx->draw_ctx)->clip_on = 0;
}

int glow_draw_point( GlowCtx *ctx, int x1, int y1, glow_eDrawType gc_type)
{
  draw_tCtx draw_ctx;
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( draw_ctx->clip_on)
    glow_set_clip( draw_ctx, draw_ctx->gcs[gc_type][0]);

  XDrawPoint( draw_ctx->display, draw_ctx->window,
	draw_ctx->gcs[gc_type][0], 
	x1, y1);
  if ( ctx->double_buffer_on)
    XDrawPoint( draw_ctx->display, draw_ctx->buffer,
	draw_ctx->gcs[gc_type][0], 
	x1, y1);

  if ( draw_ctx->clip_on)
    glow_reset_clip( draw_ctx, draw_ctx->gcs[gc_type][0]);
  return 1;
}

int glow_draw_points( GlowCtx *ctx, XPoint *points, int point_num, 
	glow_eDrawType gc_type)
{
  draw_tCtx draw_ctx;
  if ( ctx->nodraw) return 1;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( draw_ctx->clip_on)
    glow_set_clip( draw_ctx, draw_ctx->gcs[gc_type][0]);

  XDrawPoints( draw_ctx->display, draw_ctx->window,
	draw_ctx->gcs[gc_type][0], 
	points, point_num, CoordModeOrigin);

  if ( draw_ctx->clip_on)
    glow_reset_clip( draw_ctx, draw_ctx->gcs[gc_type][0]);
  return 1;
}


void glow_draw_set_click_sensitivity( GlowCtx *ctx, int value)
{
  draw_tCtx draw_ctx = (draw_tCtx) ctx->draw_ctx;

  draw_ctx->click_sensitivity = value;
}

void glow_draw_background( GlowCtx *ctx, int x, int y, int w, int h)
{
  draw_tCtx draw_ctx = (draw_tCtx) ctx->draw_ctx;

  XClearArea( draw_ctx->display, draw_ctx->window, x, y, w, h, False);
}

int glow_draw_create_buffer( GlowCtx *ctx)
{
  draw_tCtx draw_ctx;
  int depth;

  if ( !ctx->double_buffer_on || !ctx->window_width)
    return 0;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;

  if ( ctx->window_width == draw_ctx->buffer_width &&
       ctx->window_height == draw_ctx->buffer_height)
    return 0;


  if ( draw_ctx->buffer)
    XFreePixmap( draw_ctx->display, draw_ctx->buffer);

  depth = XDefaultDepthOfScreen( draw_ctx->screen);
  draw_ctx->buffer = XCreatePixmap( draw_ctx->display, draw_ctx->window,
	      ctx->window_width, ctx->window_height, depth);
  draw_ctx->buffer_width = ctx->window_width;
  draw_ctx->buffer_height = ctx->window_height;

  glow_draw_buffer_background( ctx);
  return 1;
}

void glow_draw_buffer_background( GlowCtx *ctx)
{
  draw_tCtx draw_ctx;

  if ( !ctx->double_buffer_on || !ctx->window_width)
    return;

  draw_ctx = (draw_tCtx) ctx->draw_ctx;
  if ( draw_ctx->background_pixmap) {

    if ( draw_ctx->clip_on)
      glow_set_clip( draw_ctx, draw_ctx->gcs[glow_eDrawType_Line][0]);

    if ( !((GrowCtx *)ctx)->background_tiled)
      XCopyArea( draw_ctx->display, draw_ctx->background_pixmap,
        draw_ctx->buffer, draw_ctx->gcs[glow_eDrawType_Line][0], 
	0, 0, draw_ctx->buffer_width, draw_ctx->buffer_height, 0, 0);
    else {
      int i, j;
      for ( i = 0; 
            i <= draw_ctx->buffer_width / draw_ctx->background_pixmap_width;
            i++)
	for ( j = 0;
              j <= draw_ctx->buffer_height / draw_ctx->background_pixmap_height;
              j++)
          XCopyArea( draw_ctx->display, draw_ctx->background_pixmap,
             draw_ctx->buffer, draw_ctx->gcs[glow_eDrawType_Line][0], 
	     0, 0, draw_ctx->background_pixmap_width, 
             draw_ctx->background_pixmap_height, 
             i * draw_ctx->background_pixmap_width, 
             j * draw_ctx->background_pixmap_height);
    }  
    if ( draw_ctx->clip_on)
      glow_reset_clip( draw_ctx, draw_ctx->gcs[glow_eDrawType_Line][0]);
  }
  else {
    if ( draw_ctx->clip_on)
      glow_set_clip( draw_ctx, draw_ctx->gcs[glow_eDrawType_LineErase][0]);

    XFillRectangle( draw_ctx->display, draw_ctx->buffer,
			draw_ctx->gcs[glow_eDrawType_LineErase][0],
			0, 0, ctx->window_width, ctx->window_height);
    if ( draw_ctx->clip_on)
      glow_reset_clip( draw_ctx, draw_ctx->gcs[glow_eDrawType_LineErase][0]);

  }
}




