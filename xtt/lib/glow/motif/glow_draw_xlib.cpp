/* 
 * Proview   $Id: glow_draw_xlib.cpp,v 1.2 2007-01-11 11:40:31 claes Exp $
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

using namespace std;

#include <iostream.h>
#include <fstream.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
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
#include "glow_draw_xlib.h"
#include "glow_growwidget_motif.h"
#include "glow_colpalwidget_motif.h"
#include "glow_curvewidget_motif.h"

#include "glow_msg.h"

#if defined IMLIB
# if defined OS_LYNX
#  define __NO_INCLUDE_WARN__ 1
# endif
# include <X11/extensions/shape.h>
# include <Imlib.h>
# if defined OS_LYNX
#  undef __NO_INCLUDE_WARN__
# endif
#else
typedef void *ImlibData;
typedef void *ImlibImage;
#endif


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

static int glow_allocate_named_color( GlowDrawXLib *draw_ctx, char *named_color);
static int glow_allocate_color( GlowDrawXLib *draw_ctx, int rgb_red,
	int rgb_green, int rgb_blue);
static void event_timer( GlowCtx *ctx, int time_ms);
static void cancel_event_timer(GlowCtx *ctx);
static void event_timer_cb( GlowCtx *ctx);
static int glow_read_color_file( char *filename, draw_sColor **color_array, 
	int *size);

static GC get_gc( GlowDrawXLib *draw_ctx, int i, int j)
{
  if ( !draw_ctx->gcs[i][j]) {
    XGCValues 		xgcv;
    double r, g, b;

    GlowColor::rgb_color( i, &r, &g, &b);
    xgcv.foreground = glow_allocate_color( draw_ctx, int(r * 65535), 
		int(g * 65535), int(b * 65535));
    xgcv.background = draw_ctx->background;
    xgcv.cap_style = CapButt;
    xgcv.fill_rule = WindingRule;
    xgcv.line_width = j + 1;

    draw_ctx->gcs[i][j] = XCreateGC( draw_ctx->display, 
		draw_ctx->m_wind.window, GCForeground | GCBackground | GCLineWidth | GCCapStyle
		| GCFillRule, &xgcv);
  }
  return draw_ctx->gcs[i][j];
}

static int glow_create_cursor( GlowDrawXLib *draw_ctx)
{
  /* Create some cursors */

  /* Cross cursor */
  draw_ctx->cursors[glow_eDrawCursor_CrossHair] = 
		XCreateFontCursor( draw_ctx->display, XC_crosshair);

  draw_ctx->cursors[glow_eDrawCursor_DiamondCross] = 
		XCreateFontCursor( draw_ctx->display, XC_diamond_cross);

  draw_ctx->cursors[glow_eDrawCursor_Hand] = 
		XCreateFontCursor( draw_ctx->display, XC_hand2);

  draw_ctx->cursors[glow_eDrawCursor_BottomLeftCorner] = 
		XCreateFontCursor( draw_ctx->display, XC_bottom_left_corner);

  draw_ctx->cursors[glow_eDrawCursor_BottomRightCorner] = 
		XCreateFontCursor( draw_ctx->display, XC_bottom_right_corner);

  draw_ctx->cursors[glow_eDrawCursor_BottomSide] = 
		XCreateFontCursor( draw_ctx->display, XC_bottom_side);

  draw_ctx->cursors[glow_eDrawCursor_TopLeftCorner] = 
		XCreateFontCursor( draw_ctx->display, XC_top_left_corner);

  draw_ctx->cursors[glow_eDrawCursor_TopRightCorner] = 
		XCreateFontCursor( draw_ctx->display, XC_top_right_corner);

  draw_ctx->cursors[glow_eDrawCursor_TopSide] = 
		XCreateFontCursor( draw_ctx->display, XC_top_side);

  draw_ctx->cursors[glow_eDrawCursor_RightSide] = 
		XCreateFontCursor( draw_ctx->display, XC_right_side);

  draw_ctx->cursors[glow_eDrawCursor_LeftSide] = 
		XCreateFontCursor( draw_ctx->display, XC_left_side);

  return 1;
}

static int draw_free_gc( GlowDrawXLib *draw_ctx)
{
  int i, j;

  for ( i = 1; i < glow_eDrawCursor__ ; i++)
    XFreeCursor( draw_ctx->display, draw_ctx->cursors[i]);

  XFreeGC( draw_ctx->display, draw_ctx->gc_inverse);
  for ( i = 0; i < glow_eDrawType__; i++)
  {
    for ( j = 0; j < DRAW_TYPE_SIZE; j++) {
      if ( draw_ctx->gcs[i][j])
	XFreeGC( draw_ctx->display, draw_ctx->gcs[i][j]);
    }
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

static int glow_create_gc( GlowDrawXLib *draw_ctx, Window window)
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

  sts = glow_read_color_file( "/home/claes/test/ge_colors.dat", &color_array, &size);
  if ( ODD(sts))
  {
    color_p = color_array;
    for ( int j = glow_eDrawType_Color4; j <= glow_eDrawType_Color300; j++)
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


static int glow_allocate_named_color( GlowDrawXLib *draw_ctx, char *named_color)
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

static int glow_allocate_color( GlowDrawXLib *draw_ctx, int rgb_red,
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

GlowDrawXLib::~GlowDrawXLib()
{
  ctx->set_nodraw();
  if ( ctx->type() == glow_eCtxType_Grow)
    delete (GrowCtx *)ctx;
  else
    delete ctx;
  draw_free_gc( this);

  if ( m_wind.buffer)
    XFreePixmap( display, m_wind.buffer);
  if ( m_wind.background_pixmap)
    XFreePixmap( display, m_wind.background_pixmap);

  if ( nav_wind.buffer)
    XFreePixmap( display, nav_wind.buffer);
  if ( nav_wind.background_pixmap)
    XFreePixmap( display, nav_wind.background_pixmap);
}

int GlowDrawXLib::init_nav( Widget nav_widget)
{
  nav_wind.toplevel = nav_widget;
  nav_wind.window = XtWindow( nav_wind.toplevel);

//  glow_create_gc( this, nav_wind.window);

  ctx->no_nav = 0;
  return 1;
}

GlowDrawXLib::GlowDrawXLib(
	Widget toplevel, 
	void **glow_ctx,
        int (*init_proc)(Widget w, GlowCtx *ctx, void *client_data),
	void  *client_data, 
	glow_eCtxType type) 
  : ef(0), background(0), original_background(0), timer_id(0), click_sensitivity(0)
{
  int		i;
  Arg 		args[20];

  memset( gcs, 0, sizeof(gcs));
  memset( font, 0, sizeof(font));
  memset( cursors, 0, sizeof(cursors));

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
  
  ctx->gdraw = this;
  m_wind.toplevel = toplevel;

  display = XtDisplay( m_wind.toplevel);
  m_wind.window = XtWindow( m_wind.toplevel);
  screen = XtScreen( m_wind.toplevel);

  ctx->mw.window = &m_wind;
  ctx->navw.window = &nav_wind;

  i = 0;
  XtSetArg(args[i],XmNbackground, &background); i++;
  XtGetValues( m_wind.toplevel ,args,i);
  original_background = background;

  glow_create_gc( this, m_wind.window);

  glow_create_cursor( this);

#if defined IMLIB
  imlib = Imlib_init( display);
#endif

  get_window_size( &ctx->mw, &ctx->mw.window_width, &ctx->mw.window_height);
  create_buffer( &ctx->mw);
  init_proc( toplevel, ctx, client_data);
}

int GlowDrawXLib::event_handler( XEvent event)
{
  static int	button_pressed = 0;
  static int	button_clicked = 0;
  static int	button_clicked_and_pressed = 0;
  static int 	button1_pressed = 0;
  static int 	button2_pressed = 0;
  static int 	button3_pressed = 0;
  static int	last_press_x = 0;
  static int	last_press_y = 0;

//  cout << "Event : button_pressed " << button_pressed << " clicked " << 
//	button_clicked << " c&p " << button_clicked_and_pressed << endl;

  if ( event.xany.window == m_wind.window) {
    switch ( event.type) {
    case KeyPress : {
      KeySym keysym;
      Modifiers mod;

      XtTranslateKeycode( display, event.xkey.keycode, 0, &mod,
			  &keysym);
      keysym &= 0xFFFF;
      if ( (keysym >= 0x020 && keysym <= 0x20ac) ||
	   (keysym >= 0xFF80 && keysym <= 0xFFB9 && keysym != XK_KP_Enter && keysym != 0xFF44)) {
	char buff;
	XLookupString( &event.xkey, &buff, sizeof(buff), NULL, NULL);

	if ( buff >= 0x020)
	  ctx->event_handler( glow_eEvent_Key_Ascii, 0, 0, (int)buff, 0);
	else
	  ctx->event_handler( glow_eEvent_Key_CtrlAscii, 0, 0, (int)buff, 0);
      }
      else {
	switch ( keysym) {
	case XK_Return:
	case XK_KP_Enter:
	case 0xFF44:			// XK_KP_Enter sometimes...
	  ctx->event_handler( glow_eEvent_Key_Return, 0, 0, 0, 0);
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
	case XK_Delete:
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
	case XK_Cancel:
	  ctx->event_handler( glow_eEvent_Key_Escape, 0, 0, 0, 0);
	  break;
	case XK_Tab:
	  if ( event.xkey.state & ShiftMask)
	    ctx->event_handler( glow_eEvent_Key_ShiftTab, 0, 0, 0, 0);
	  else
	    ctx->event_handler( glow_eEvent_Key_Tab, 0, 0, 0, 0);
	  break;
	default:
	  ;
	}
      }
      break;
    }
    case ButtonPress : 
// cout << "Button press event" << endl;
//        printf( "-- Button event: (%d,%d) button: %d time:%d\n", event.xbutton.x, 
//		event.xbutton.y, event.xbutton.button, event.xbutton.time);

//        XSetInputFocus( display, m_wind.window, 
//		RevertToNone, CurrentTime);

      switch ( event.xbutton.button) {
      case Button1:
	ctx->event_handler( glow_eEvent_MB1Down, event.xbutton.x, event.xbutton.y, 0, 0);
	if ( click_sensitivity & glow_mSensitivity_MB1Click && 
	     !(click_sensitivity & glow_mSensitivity_MB1DoubleClick) && 
	     !(click_sensitivity & glow_mSensitivity_MB1Press)) {
	  memcpy( &last_event, &event, sizeof(event));
	  button_pressed = 0;
	  button_clicked = 1;
	  last_press_x = event.xbutton.x;
	  last_press_y = event.xbutton.y;
	  return 1;
	}
	else if ( !(click_sensitivity & glow_mSensitivity_MB1Click) && 
		  !(click_sensitivity & glow_mSensitivity_MB1DoubleClick) && 
		  click_sensitivity & glow_mSensitivity_MB1Press) {
	  memcpy( &last_event, &event, sizeof(event));
	  button_pressed = 1;
	  button_clicked = 0;
	  last_press_x = event.xbutton.x;
	  last_press_y = event.xbutton.y;
	}
	break;
      case Button3:
	ctx->event_handler( glow_eEvent_MB3Down, event.xbutton.x, event.xbutton.y, 0, 0);
	if ( click_sensitivity & glow_mSensitivity_MB3Press && 
	     !(click_sensitivity & glow_mSensitivity_MB3DoubleClick) && 
	     !(click_sensitivity & glow_mSensitivity_MB3Click)) {
	  ctx->event_handler( glow_eEvent_MB3Press, 
			      event.xbutton.x, event.xbutton.y, 0, 0);
	  click_sensitivity = 0;
	  return 1;
	}
	else if ( click_sensitivity & glow_mSensitivity_MB3Click && 
		  !(click_sensitivity & glow_mSensitivity_MB3DoubleClick) && 
		  !(click_sensitivity & glow_mSensitivity_MB3Press))
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
      if ( button_clicked) {
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
      if ( !button_pressed ) {
	memcpy( &last_event, &event, sizeof(event));
	button_pressed = event.xbutton.button;
	last_press_x = event.xbutton.x;
	last_press_y = event.xbutton.y;
	event_timer(ctx, 200);
	return 1;
      }
      else {    
// cout << "Button press detected" << endl;
	/* Press event, callback from timer */
	button_pressed = 0;
	button_clicked_and_pressed = 0;
	switch ( event.xbutton.button) {
	case Button1:

	  button1_pressed = 1;
	  if (       (event.xbutton.state & ShiftMask) && 
		     !(event.xbutton.state & ControlMask)) {
	    ctx->event_handler( glow_eEvent_MB1PressShift, event.xbutton.x, event.xbutton.y, 0, 0);
	    click_sensitivity = 0;
	  }
	  else if ( !(event.xbutton.state & ShiftMask) && 
		    (event.xbutton.state & ControlMask)) {
	    ctx->event_handler( glow_eEvent_MB1PressCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	    click_sensitivity = 0;
	  }
	  else if (  (event.xbutton.state & ShiftMask) && 
		     (event.xbutton.state & ControlMask)) {
	    ctx->event_handler( glow_eEvent_MB1PressShiftCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	    click_sensitivity = 0;
	  }
	  else {
	    ctx->event_handler( glow_eEvent_MB1Press, event.xbutton.x, event.xbutton.y, 0, 0);
	    click_sensitivity = 0;
	  }
	  break;
	case Button2:
	  button2_pressed = 1;
	  if (       (event.xbutton.state & ShiftMask) && 
		     !(event.xbutton.state & ControlMask)) {
	    ctx->event_handler( glow_eEvent_MB2PressShift, event.xbutton.x, event.xbutton.y, 0, 0);
	    click_sensitivity = 0;
	  }
	  else if ( !(event.xbutton.state & ShiftMask) && 
		    (event.xbutton.state & ControlMask)) {
	    ctx->event_handler( glow_eEvent_MB2PressShift, event.xbutton.x, event.xbutton.y, 0, 0);
	    click_sensitivity = 0;
	  }
	  else if (  (event.xbutton.state & ShiftMask) && 
		     (event.xbutton.state & ControlMask)) {
	    ctx->event_handler( glow_eEvent_MB2PressShiftCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	    click_sensitivity = 0;
	  }
	  else {
	    ctx->event_handler( glow_eEvent_MB2Press, event.xbutton.x, event.xbutton.y, 0, 0);
	    click_sensitivity = 0;
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
	  click_sensitivity = 0;
	  break;
	}
      }
      break;

    case ButtonRelease:
      button1_pressed = 0;
      button2_pressed = 0;
      button3_pressed = 0;

// cout << "Button release event" << endl;

      switch ( event.xbutton.button) {
      case Button1:
	ctx->event_handler( glow_eEvent_MB1Up, event.xbutton.x, event.xbutton.y, 0, 0);
	break;
      }

      if ( ! button_pressed ) {
	if ( button_clicked) {
	  /* Button click, callback from timer */
// cout << "Button click detected state " << event.xbutton.state << endl;
            button_clicked = 0;
          switch ( event.xbutton.button) {
	  case Button1:
	    if (       (event.xbutton.state & ShiftMask) && 
		       !(event.xbutton.state & ControlMask)) {
	      ctx->event_handler( glow_eEvent_MB1ClickShift, event.xbutton.x, event.xbutton.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    else if ( !(event.xbutton.state & ShiftMask) && 
		      (event.xbutton.state & ControlMask)) {
	      ctx->event_handler( glow_eEvent_MB1ClickCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    else if (  (event.xbutton.state & ShiftMask) && 
		       (event.xbutton.state & ControlMask)) {
	      ctx->event_handler( glow_eEvent_MB1ClickShiftCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    else {
	      ctx->event_handler( glow_eEvent_MB1Click, event.xbutton.x, event.xbutton.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    break;
	  case Button2:
	    if (       (event.xbutton.state & ShiftMask) && 
		       !(event.xbutton.state & ControlMask)) {
	      ctx->event_handler( glow_eEvent_MB2ClickShift, event.xbutton.x, event.xbutton.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    else if ( !(event.xbutton.state & ShiftMask) && 
		      (event.xbutton.state & ControlMask)) {
	      ctx->event_handler( glow_eEvent_MB2ClickCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    else if (  (event.xbutton.state & ShiftMask) && 
		       (event.xbutton.state & ControlMask)) {
	      ctx->event_handler( glow_eEvent_MB2ClickShiftCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    else {
	      ctx->event_handler( glow_eEvent_MB2Click, event.xbutton.x, event.xbutton.y, 0, 0);
	      click_sensitivity = 0;
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
	    click_sensitivity = 0;
	    break;
	  }
	}
	else {
	  /* Button release */
// cout << "Button release detected" << endl;
	  ctx->event_handler( glow_eEvent_ButtonRelease, event.xbutton.x, event.xbutton.y, 0, 0);
	}        
      }
      else {
	/* Button click */
	cancel_event_timer( ctx);
	if ( ! button_clicked_and_pressed) {
//  cout << "Button first click detected" << endl;
	  /* wait for button double click */
	  memcpy( &last_event, &event, sizeof(event));
	  button_clicked = 1;
	  event_timer( ctx, 200);
	  button_pressed = 0;
	  return 1;
	}
	else {
	  /* Button double click */ 
// cout << "Button double click detected" << endl;
	  cancel_event_timer( ctx);
	  button_clicked = 0;
	  button_pressed = 0;
	  button_clicked_and_pressed = 0;
	  switch ( event.xbutton.button) {
	  case Button1:
	    if ( (event.xbutton.state & ShiftMask) && 
		 !(event.xbutton.state & ControlMask)) {
	      ctx->event_handler( glow_eEvent_MB1DoubleClickShift, event.xbutton.x, event.xbutton.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    else if ( !(event.xbutton.state & ShiftMask) && 
		      (event.xbutton.state & ControlMask)) {
	      ctx->event_handler( glow_eEvent_MB1DoubleClickCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    else if (  (event.xbutton.state & ShiftMask) && 
		       (event.xbutton.state & ControlMask)) {
	      ctx->event_handler( glow_eEvent_MB1DoubleClickShiftCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    else {
	      ctx->event_handler( glow_eEvent_MB1DoubleClick, event.xbutton.x, event.xbutton.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    break;
	  case Button2:
	    if ( (event.xbutton.state & ShiftMask) && 
		 !(event.xbutton.state & ControlMask)) {
	      ctx->event_handler( glow_eEvent_MB2DoubleClickShift, event.xbutton.x, event.xbutton.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    else if ( !(event.xbutton.state & ShiftMask) && 
		      (event.xbutton.state & ControlMask)) {
	      ctx->event_handler( glow_eEvent_MB2DoubleClickCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    else if (  (event.xbutton.state & ShiftMask) && 
		       (event.xbutton.state & ControlMask)) {
	      ctx->event_handler( glow_eEvent_MB2DoubleClickShiftCtrl, event.xbutton.x, event.xbutton.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    else {
	      ctx->event_handler( glow_eEvent_MB2DoubleClick, event.xbutton.x, event.xbutton.y, 0, 0);
	      click_sensitivity = 0;
	    }
	    break;
	  case Button3:
#if 0
	    if ( (event.xbutton.state & ShiftMask) && 
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
      switch ( event.xvisibility.state) {
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
	    abs( event.xbutton.y - last_press_y) > DRAW_PRESS_PIX)) {
//          printf( "Press: x %d last_x %d\n", event.xbutton.x, last_press_x);
//          printf( "       y %d last_y %d\n", event.xbutton.y, last_press_y);

	event.xbutton.x = last_press_x;
	event.xbutton.y = last_press_y;

	/* Button press */
	cancel_event_timer( ctx);
	switch ( button_pressed) {
	case Button1:
	  button1_pressed = 1;
	  if ( (event.xbutton.state & ShiftMask) && 
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
	  if ( (event.xbutton.state & ShiftMask) && 
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
  else if ( event.xany.window == nav_wind.window) {
    switch ( event.type) {
    case ButtonPress : 
//            printf( "-- Button event nav: (%d,%d) button: %d\n", event.xbutton.x, 
//		event.xbutton.y, event.xbutton.button);
      switch ( event.xbutton.button) {
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
      switch ( event.xbutton.button) {
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
  XFlush( display);
  return 1;
}

void GlowDrawXLib::enable_event( glow_eEvent event, 
		glow_eEventType event_type, 
		int (*event_cb)(GlowCtx *ctx, glow_tEvent event))
{
  ctx->enable_event( event, event_type, event_cb);
}

int GlowDrawXLib::rect( GlowWind *wind, int x, int y, int width, int height, 
	glow_eDrawType gc_type, int idx, int highlight)
{
  if ( ctx->nodraw) return 1;
  DrawWindXLib *w = (DrawWindXLib *) wind->window;

  if ( gc_type == glow_eDrawType_LineGray && highlight)
    gc_type = glow_eDrawType_Line;

  if ( w->clip_on) 
    set_clip( w, get_gc( this, gc_type+highlight, idx));

  if ( !w->draw_buffer_only)
    XDrawRectangle( display, w->window, 
	get_gc( this, gc_type+highlight, idx), 
	x, y, width, height);
  if ( w->double_buffer_on)
    XDrawRectangle( display, w->buffer, 
        get_gc( this, gc_type+highlight, idx), 
	x, y, width, height);

  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type+highlight, idx));

  return 1;
}

int GlowDrawXLib::rect_erase( GlowWind *wind, int x, int y, int width, int height,
			      int idx)
{
  if ( ctx->nodraw) return 1;
  DrawWindXLib *w = (DrawWindXLib *) wind->window;

  if ( w->clip_on)
    set_clip( w, get_gc( this, glow_eDrawType_LineErase, idx));

  if ( !w->draw_buffer_only)
    XDrawRectangle( display, w->window, 
		    get_gc( this, glow_eDrawType_LineErase, idx), 
		    x, y, width, height);
  if ( w->double_buffer_on)
    XDrawRectangle( display, w->buffer, 
		    get_gc( this, glow_eDrawType_LineErase, idx), 
		    x, y, width, height);
  
  if ( w->clip_on)
    reset_clip( w, get_gc( this, glow_eDrawType_LineErase, idx));

  return 1;
}

int GlowDrawXLib::arrow( GlowWind *wind, int x1, int y1, int x2, int y2, 
			 int x3, int y3,
			 glow_eDrawType gc_type, int idx, int highlight)
{
  if ( ctx->nodraw) return 1;

  XPoint p[4] = {{x1,y1},{x2,y2},{x3,y3},{x1,y1}};
  DrawWindXLib *w = (DrawWindXLib *) wind->window;

  if ( gc_type == glow_eDrawType_LineGray && highlight)
    gc_type = glow_eDrawType_Line;

  if ( w->clip_on)
    set_clip( w, get_gc( this, gc_type+highlight, idx));

  if ( !w->draw_buffer_only)
    XFillPolygon( display, w->window,
	get_gc( this, gc_type+highlight, idx), p, 4, Convex, CoordModeOrigin);
  if ( w->double_buffer_on)
    XFillPolygon( display, w->buffer,
	get_gc( this, gc_type+highlight, idx), p, 4, Convex, CoordModeOrigin);

  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type+highlight, idx));

  return 1;
}

int GlowDrawXLib::arrow_erase( GlowWind *wind, int x1, int y1, int x2, int y2, 
			       int x3, int y3,
			       int idx)
{
  if ( ctx->nodraw) return 1;

  XPoint p[4] = {{x1,y1},{x2,y2},{x3,y3},{x1,y1}};
  DrawWindXLib *w = (DrawWindXLib *) wind->window;

  if ( w->clip_on)
    set_clip( w, get_gc( this, glow_eDrawType_LineErase, idx));

  if ( !w->draw_buffer_only)
    XFillPolygon( display, w->window,
	get_gc( this, glow_eDrawType_LineErase, idx),
	p, 4, Convex, CoordModeOrigin);
  if ( w->double_buffer_on)
    XFillPolygon( display, w->buffer,
	get_gc( this, glow_eDrawType_LineErase, idx),
	p, 4, Convex, CoordModeOrigin);

  if ( w->clip_on)
    reset_clip( w, get_gc( this, glow_eDrawType_LineErase, idx));

  return 1;
}

int GlowDrawXLib::arc( GlowWind *wind, int x, int y, int width, int height, 
		       int angel1, int angel2,
		       glow_eDrawType gc_type, int idx, int highlight)
{
  if ( ctx->nodraw) return 1;
  DrawWindXLib *w = (DrawWindXLib *) wind->window;

  if ( gc_type == glow_eDrawType_LineGray && highlight)
    gc_type = glow_eDrawType_Line;

  // Fix for highlight for connections in grow
  if ( highlight && ctx->type() == glow_eCtxType_Grow)
    gc_type = glow_eDrawType_LineHighlight;

  // if ( width < 35 && height < 35) {width++; height++;} // This looks good in Reflexion X ...

  if ( angel1 >= 360)
    angel1 = angel1 - angel1 / 360 * 360;
  else if ( angel1 < 0)
    angel1 = angel1 + ( -angel1 / 360 + 1) * 360;

  if ( w->clip_on)
    set_clip( w, get_gc( this, gc_type+highlight, idx));

  if ( !w->draw_buffer_only)
    XDrawArc( display, w->window,
	get_gc( this, gc_type+highlight, idx), 
	x, y, width, height, angel1*64, angel2*64);

  if ( w->double_buffer_on)
    XDrawArc( display, w->buffer,
	get_gc( this, gc_type+highlight, idx), 
	x, y, width, height, angel1*64, angel2*64);

  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type+highlight, idx));

  return 1;
}

int GlowDrawXLib::fill_arc( GlowWind *wind, int x, int y, int width, int height, 
	int angel1, int angel2, glow_eDrawType gc_type, int highlight)
{
  if ( ctx->nodraw) return 1;
  DrawWindXLib *w = (DrawWindXLib *) wind->window;

  if ( gc_type == glow_eDrawType_LineGray && highlight)
    gc_type = glow_eDrawType_LineRed;

  if ( angel1 >= 360)
    angel1 = angel1 - angel1 / 360 * 360;
  else if ( angel1 < 0)
    angel1 = angel1 + ( -angel1 / 360 + 1) * 360;

  if ( w->clip_on)
    set_clip( w, get_gc( this, gc_type+highlight, 0));

  if ( !w->draw_buffer_only)
    XFillArc( display, w->window,
	get_gc( this, gc_type+highlight, 0), 
	x, y, width, height, angel1*64, angel2*64);
  if ( w->double_buffer_on)
    XFillArc( display, w->buffer,
	get_gc( this, gc_type+highlight, 0), 
	x, y, width, height, angel1*64, angel2*64);

  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type+highlight, 0));

  return 1;
}

int GlowDrawXLib::arc_erase( GlowWind *wind, int x, int y, int width, int height,
	int angel1, int angel2,
	int idx)
{
  if ( ctx->nodraw) return 1;
  DrawWindXLib *w = (DrawWindXLib *) wind->window;

  // if ( width < 35 && height < 35) {width++; height++;} // This looks good in Reflexion X ...

  if ( angel1 >= 360)
    angel1 = angel1 - angel1 / 360 * 360;
  else if ( angel1 < 0)
    angel1 = angel1 + ( -angel1 / 360 + 1) * 360;

  if ( w->clip_on)
    set_clip( w, get_gc( this, glow_eDrawType_LineErase, idx));

  if ( !w->draw_buffer_only)
    XDrawArc( display, w->window,
	get_gc( this, glow_eDrawType_LineErase, idx), 
	x, y, width, height, angel1*64, angel2*64);
  if ( w->double_buffer_on)
    XDrawArc( display, w->buffer,
	get_gc( this, glow_eDrawType_LineErase, idx), 
	x, y, width, height, angel1*64, angel2*64);

  if ( w->clip_on)
    reset_clip( w, get_gc( this, glow_eDrawType_LineErase, idx));
  return 1;
}

int GlowDrawXLib::line( GlowWind *wind, int x1, int y1, int x2, int y2,
	glow_eDrawType gc_type, int idx, int highlight)
{
  if ( ctx->nodraw) return 1;
  DrawWindXLib *w = (DrawWindXLib *) wind->window;

  // Fix for highlight for connections in grow
  if ( highlight && ctx->type() == glow_eCtxType_Grow)
    gc_type = glow_eDrawType_LineHighlight;

  if ( gc_type == glow_eDrawType_LineGray && highlight)
    gc_type = glow_eDrawType_Line;

  if ( w->clip_on)
    set_clip( w, get_gc( this, gc_type+highlight, idx));

  if ( !w->draw_buffer_only)
    XDrawLine( display, w->window,
	get_gc( this, gc_type+highlight, idx), 
	x1, y1, x2, y2);
  if ( w->double_buffer_on)
    XDrawLine( display, w->buffer,
	get_gc( this, gc_type+highlight, idx), 
	x1, y1, x2, y2);

  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type+highlight, idx));
  return 1;
}

int GlowDrawXLib::line_dashed( GlowWind *wind, int x1, int y1, int x2, int y2,
	glow_eDrawType gc_type, int idx, int highlight, glow_eLineType line_type)
{
  XGCValues xgcv;
  if ( ctx->nodraw) return 1;
  DrawWindXLib *w = (DrawWindXLib *) wind->window;

  // Fix for highlight for connections in grow
  if ( highlight && ctx->type() == glow_eCtxType_Grow)
    gc_type = glow_eDrawType_LineHighlight;

  if ( gc_type == glow_eDrawType_LineGray && highlight)
    gc_type = glow_eDrawType_Line;

  xgcv.line_style = LineOnOffDash;
  xgcv.dash_offset = 0;
  switch ( line_type) {
  case glow_eLineType_Dashed1:
    xgcv.dashes = 1 + idx;
    XChangeGC( display, get_gc( this, gc_type+highlight, idx),
	GCLineStyle | GCDashOffset | GCDashList, &xgcv);
    break;
  case glow_eLineType_Dashed2:
    xgcv.dashes = 1 + 2 * idx;
    XChangeGC( display, get_gc( this, gc_type+highlight, idx),
	GCLineStyle | GCDashOffset | GCDashList, &xgcv);
    break;
  case glow_eLineType_Dashed3:
    xgcv.dashes = 1 + 3 * idx;
    XChangeGC( display, get_gc( this, gc_type+highlight, idx),
	GCLineStyle | GCDashOffset | GCDashList, &xgcv);
    break;
  case glow_eLineType_Dotted: {
    static char list[4];
    list[0] = 1 + idx;
    list[1] = 1 + 4 * idx;
    XSetDashes( display, get_gc( this, gc_type+highlight, idx),
		0, list, 2);
    XChangeGC( display, get_gc( this, gc_type+highlight, idx),
	GCLineStyle | GCDashOffset, &xgcv);
    break;
    }
  case glow_eLineType_DotDashed1: {
    static char list[4];
    list[0] = 1 + 3 * idx;
    list[1] = 1 + 2 * idx;
    list[2] = 1 + idx;
    list[3] = 1 + 2 * idx;
    XSetDashes( display, get_gc( this, gc_type+highlight, idx),
		0, list, 4);
    XChangeGC( display, get_gc( this, gc_type+highlight, idx),
	GCLineStyle | GCDashOffset, &xgcv);
    break;
    }
  case glow_eLineType_DotDashed2: {
    static char list[4];
    list[0] = 1 + 6 * idx;
    list[1] = 1 + 3 * idx;
    list[2] = 1 + idx;
    list[3] = 1 + 3 * idx;
    XSetDashes( display, get_gc( this, gc_type+highlight, idx),
		0, list, 4);
    XChangeGC( display, get_gc( this, gc_type+highlight, idx),
	GCLineStyle | GCDashOffset, &xgcv);
    break;
    }
  default: ;
  }

  if ( w->clip_on)
    set_clip( w, get_gc( this, gc_type+highlight, idx));

  if ( !w->draw_buffer_only)
    XDrawLine( display, w->window,
	get_gc( this, gc_type+highlight, idx), 
	x1, y1, x2, y2);
  if ( w->double_buffer_on)
    XDrawLine( display, w->buffer,
	get_gc( this, gc_type+highlight, idx), 
	x1, y1, x2, y2);

  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type+highlight, idx));

  xgcv.line_style = LineSolid;
  XChangeGC( display, get_gc( this, gc_type+highlight, idx),
	GCLineStyle, &xgcv);
  return 1;
}

int GlowDrawXLib::line_erase( GlowWind *wind, int x1, int y1, int x2, int y2,
	int idx)
{
  if ( ctx->nodraw) return 1;
  DrawWindXLib *w = (DrawWindXLib *) wind->window;

  if ( w->clip_on)
    set_clip( w, get_gc( this, glow_eDrawType_LineErase, idx));

  if ( !w->draw_buffer_only)
    XDrawLine( display, w->window,
	get_gc( this, glow_eDrawType_LineErase, idx), 
	x1, y1, x2, y2);
  if ( w->double_buffer_on)
    XDrawLine( display, w->buffer,
	get_gc( this, glow_eDrawType_LineErase, idx), 
	x1, y1, x2, y2);

  if ( w->clip_on)
    reset_clip( w, get_gc( this, glow_eDrawType_LineErase, idx));

  return 1;
}

int GlowDrawXLib::polyline( GlowWind *wind, glow_sPointX *points, int point_cnt,
	glow_eDrawType gc_type, int idx, int highlight)
{
  if ( ctx->nodraw) return 1;
  DrawWindXLib *w = (DrawWindXLib *) wind->window;

  if ( gc_type == glow_eDrawType_LineGray && highlight)
    gc_type = glow_eDrawType_Line;

  if ( w->clip_on)
    set_clip( w, get_gc( this, gc_type+highlight, idx));

  if ( !w->draw_buffer_only)
    XDrawLines( display, w->window,
	get_gc( this, gc_type+highlight, idx), 
	(XPoint *) points, point_cnt, CoordModeOrigin);
  if ( w->double_buffer_on)
    XDrawLines( display, w->buffer,
	get_gc( this, gc_type+highlight, idx), 
	(XPoint *) points, point_cnt, CoordModeOrigin);

  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type+highlight, idx));
  return 1;
}

int GlowDrawXLib::fill_polyline( GlowWind *wind, glow_sPointX *points, int point_cnt,
	glow_eDrawType gc_type, int highlight)
{
  if ( ctx->nodraw) return 1;
  DrawWindXLib *w = (DrawWindXLib *) wind->window;

  if ( gc_type == glow_eDrawType_LineGray && highlight)
    gc_type = glow_eDrawType_Line;

  if ( w->clip_on)
    set_clip( w, get_gc( this, gc_type+highlight, 0));

  if ( !w->draw_buffer_only)
    XFillPolygon( display, w->window,
	get_gc( this, gc_type+highlight, 0), (XPoint *) points, point_cnt, 
	Nonconvex, CoordModeOrigin);
  if ( w->double_buffer_on)
    XFillPolygon( display, w->buffer,
	get_gc( this, gc_type+highlight, 0), (XPoint *) points, point_cnt, 
	Nonconvex, CoordModeOrigin);

  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type+highlight, 0));
  return 1;
}

int GlowDrawXLib::polyline_erase( GlowWind *wind, glow_sPointX *points, int point_cnt,
	int idx)
{
  if ( ctx->nodraw) return 1;
  DrawWindXLib *w = (DrawWindXLib *) wind->window;

  if ( w->clip_on)
    set_clip( w, get_gc( this, glow_eDrawType_LineErase, idx));

  if ( !w->draw_buffer_only)
    XDrawLines( display, w->window,
	get_gc( this, glow_eDrawType_LineErase, idx), 
	(XPoint *)points, point_cnt, CoordModeOrigin);
  if ( w->double_buffer_on)
    XDrawLines( display, w->buffer,
	get_gc( this, glow_eDrawType_LineErase, idx), 
	(XPoint *)points, point_cnt, CoordModeOrigin);

  if ( w->clip_on)
    reset_clip( w, get_gc( this, glow_eDrawType_LineErase, idx));
  return 1;
}

int GlowDrawXLib::text( GlowWind *wind, int x, int y, char *text, int len,
	glow_eDrawType gc_type, glow_eDrawType color, int idx, int highlight, int line)
{
  if ( ctx->nodraw) return 1;
  DrawWindXLib *w = (DrawWindXLib *) wind->window;

  if ( w->clip_on)
    set_clip( w, get_gc( this, gc_type, idx));

  if ( color != glow_eDrawType_Line) {
    XGCValues 		xgcv;

    XGetGCValues( display, get_gc( this, color, 0), 
	GCForeground, &xgcv);

    XChangeGC( display, get_gc( this, gc_type, idx),
	GCForeground, &xgcv);
  }

  if ( !w->draw_buffer_only)
    XDrawString( display, w->window,
	get_gc( this, gc_type, idx), 
	x, y, text, len);
  if ( w->double_buffer_on)
    XDrawString( display, w->buffer,
	get_gc( this, gc_type, idx), 
	x, y, text, len);
  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type, idx));

  if ( color != glow_eDrawType_Line) {
    XGCValues 		xgcv;

    XGetGCValues( display, get_gc( this, glow_eDrawType_Line, 0), 
	GCForeground, &xgcv);

    XChangeGC( display, get_gc( this, gc_type, idx),
	GCForeground, &xgcv);
  }
  return 1;
}

int GlowDrawXLib::text_cursor( GlowWind *wind, int x, int y, char *text, int len,
	glow_eDrawType gc_type, glow_eDrawType color, int idx, int highlight, int pos)
{
  if ( ctx->nodraw) return 1;
  DrawWindXLib *w = (DrawWindXLib *) wind->window;

  int width, height, descent;
  get_text_extent( text, pos, gc_type, idx,
			&width, &height, &descent);

  if ( w->clip_on)
    set_clip( w, get_gc( this, gc_type, idx));

  if ( !w->draw_buffer_only) {
    XDrawLine( display, w->window,
	get_gc( this, color, 1), 
	x + width, y + descent, x + width, y - height + descent);  
  }
  if ( w->double_buffer_on) {
    XDrawLine( display, w->buffer,
	get_gc( this, color, 1), 
	x + width, y + descent, x + width, y - height + descent);
  }
  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type, idx));

  return 1;
}

int GlowDrawXLib::text_erase( GlowWind *wind, int x, int y, char *text, int len,
	glow_eDrawType gc_type, int idx, int line)
{
  if ( ctx->nodraw) return 1;
  DrawWindXLib *w = (DrawWindXLib *) wind->window;

  if ( gc_type == glow_eDrawType_TextHelvetica)
    gc_type = glow_eDrawType_TextHelveticaErase;
  else if ( gc_type == glow_eDrawType_TextHelveticaBold)
    gc_type = glow_eDrawType_TextHelveticaEraseBold;

  if ( w->clip_on)
    set_clip( w, get_gc( this, gc_type, idx));
  if ( !w->draw_buffer_only)
    XDrawString( display, w->window,
	get_gc( this, gc_type, idx), 
	x, y, text, len);
  if ( w->double_buffer_on)
    XDrawString( display, w->buffer,
	get_gc( this, gc_type, idx), 
	x, y, text, len);
  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type, idx));
  return 1;
}

int GlowDrawXLib::pixmaps_create( GlowWind *wind, glow_sPixmapData *pixmap_data,
	void **pixmaps)
{
  DrawWindXLib *w = (DrawWindXLib *) wind->window;
  draw_sPixmap *pms;
  glow_sPixmapDataElem *prev_pdata, *pdata = (glow_sPixmapDataElem *)pixmap_data;
  int	i;

  pms = (draw_sPixmap *) calloc( 1, sizeof( *pms));
  for ( i = 0; i < DRAW_PIXMAP_SIZE; i++)
  {
    if ( i == 0 || 
         (i > 0 && pdata->bits != prev_pdata->bits))
      pms->pixmap[i] = XCreateBitmapFromData( display,
	w->window, pdata->bits, pdata->width,
	pdata->height);
    else
      pms->pixmap[i] = pms->pixmap[i-1];
    prev_pdata = pdata;
    pdata++;
  }
  *pixmaps = (void *) pms;
  return 1;
}

void GlowDrawXLib::pixmaps_delete( GlowWind *wind, void *pixmaps)
{
  draw_sPixmap *pms;
  int i;

  pms = (draw_sPixmap *) pixmaps;
  for ( i = 0; i < DRAW_PIXMAP_SIZE; i++)
  {
    if ( i == 0 || 
         (i > 0 && pms->pixmap[i] != pms->pixmap[i-1]))
      XFreePixmap( display, pms->pixmap[i]);
  }
  free ( pixmaps);
}

int GlowDrawXLib::pixmap( GlowWind *wind, int x, int y, glow_sPixmapData *pixmap_data,
	void *pixmaps, glow_eDrawType gc_type, int idx, int highlight, int line)
{
  DrawWindXLib *w = (DrawWindXLib *) wind->window;
  draw_sPixmap *pms;
  glow_sPixmapDataElem *pdata = (glow_sPixmapDataElem *)pixmap_data + idx;

  if ( ctx->nodraw) return 1;
  
  pms = (draw_sPixmap *) pixmaps;

  if ( w->clip_on)
    set_clip( w, get_gc( this, gc_type, idx));

  if ( !w->draw_buffer_only)
    XCopyPlane( display, pms->pixmap[idx], w->window,
	get_gc( this, gc_type, idx), 
	0, 0, pdata->width, pdata->height, x, y, 1);
  if ( w->double_buffer_on)
    XCopyPlane( display, pms->pixmap[idx], w->buffer,
	get_gc( this, gc_type, idx), 
	0, 0, pdata->width, pdata->height, x, y, 1);

  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type, idx));
  return 1;
}

int GlowDrawXLib::pixmap_inverse( GlowWind *wind, int x, int y, glow_sPixmapData *pixmap_data,
	void *pixmaps, glow_eDrawType gc_type, int idx, int line)
{
  DrawWindXLib *w = (DrawWindXLib *) wind->window;
  draw_sPixmap *pms;
  glow_sPixmapDataElem *pdata = (glow_sPixmapDataElem *)pixmap_data + idx;

  pms = (draw_sPixmap *) pixmaps;

  if ( w->clip_on)
    set_clip( w, gc_inverse);
  if ( !w->draw_buffer_only)
    XCopyPlane( display, pms->pixmap[idx], w->window,
	gc_inverse,
	0, 0, pdata->width, pdata->height, x, y, 1);
  if ( w->double_buffer_on)
    XCopyPlane( display, pms->pixmap[idx], w->buffer,
	gc_inverse,
	0, 0, pdata->width, pdata->height, x, y, 1);
  if ( w->clip_on)
    reset_clip( w, gc_inverse);
  return 1;
}

int GlowDrawXLib::pixmap_erase( GlowWind *wind, int x, int y, glow_sPixmapData *pixmap_data,
	void *pixmaps, glow_eDrawType gc_type, int idx, int line)
{
  DrawWindXLib *w = (DrawWindXLib *) wind->window;
  glow_sPixmapDataElem *pdata = (glow_sPixmapDataElem *)pixmap_data + idx;
  if ( ctx->nodraw) return 1;

  // Intersection with clipmask should be calculated here...

  if ( !w->draw_buffer_only)
    XClearArea( display, w->window, 
	x, y, pdata->width, pdata->height, False);
  if ( w->double_buffer_on)
    XClearArea( display, w->buffer, 
	x, y, pdata->width, pdata->height, False);
  return 1;
}

int GlowDrawXLib::image( GlowWind *wind, int x, int y, int width, int height,
		     glow_tImImage image, glow_tPixmap pixmap, glow_tPixmap clip_mask)
{
  DrawWindXLib *w = (DrawWindXLib *) wind->window;
  if ( ctx->nodraw) return 1;

  if ( width == 0 || height == 0)
    return 1;

  if ( clip_mask)
    set_image_clip_mask( clip_mask, x, y);
  else if ( w->clip_on)
    set_clip( w, get_gc( this, glow_eDrawType_Line, 0));

  if ( !w->draw_buffer_only)
    XCopyArea( display, (Pixmap)pixmap, w->window,
	get_gc( this, glow_eDrawType_Line, 0), 
	0, 0, width, height, x, y);
  if ( w->double_buffer_on)
    XCopyArea( display, (Pixmap)pixmap, w->buffer,
	get_gc( this, glow_eDrawType_Line, 0), 
	0, 0, width, height, x, y);

  if ( clip_mask)
    reset_image_clip_mask();
  else if ( w->clip_on)
    reset_clip( w, get_gc( this, glow_eDrawType_Line, 0));
  return 1;
}


int GlowDrawXLib::fill_rect( GlowWind *wind, int x, int y, int w, int h, 
	glow_eDrawType gc_type)
{
  DrawWindXLib *ww = (DrawWindXLib *) wind->window;
  XPoint p[5] = {{x,y},{x+w,y},{x+w,y+h},{x,y+h},{x,y}};
  if ( ctx->nodraw) return 1;

  if ( ww->clip_on)
    set_clip( ww, get_gc( this, gc_type, 0));
  if ( !ww->draw_buffer_only)
    XFillPolygon( display, ww->window,
	get_gc( this, gc_type, 0), p, 5, Convex, CoordModeOrigin);
  if ( ww->double_buffer_on)
    XFillPolygon( display, ww->buffer,
	get_gc( this, gc_type, 0), p, 5, Convex, CoordModeOrigin);
  if ( ww->clip_on)
    reset_clip( ww, get_gc( this, gc_type, 0));
  return 1;
}

void GlowDrawXLib::clear( GlowWind *wind)
{
  if ( ctx->nodraw) return;
  DrawWindXLib *w = (DrawWindXLib *) wind->window;

  if ( !w->double_buffer_on)
    XClearWindow( display, w->window);
  else
    buffer_background( w);
}

void GlowDrawXLib::copy_buffer( GlowWind *wind,
		int ll_x, int ll_y, int ur_x, int ur_y)
{
  if ( ctx->nodraw) return;
  DrawWindXLib *w = (DrawWindXLib *) wind->window;

  int x0 = min( ll_x, ur_x);
  int x1 = max( ll_x, ur_x);
  int y0 = min( ll_y, ur_y);
  int y1 = max( ll_y, ur_y);

  XCopyArea( display, w->buffer, w->window,
	get_gc( this, glow_eDrawType_Line, 0), 
	x0, y0, x1 - x0, y1 - y0, x0, y0);
}

void GlowDrawXLib::get_window_size( GlowWind *wind, int *width, int *height)
{
  DrawWindXLib *w = (DrawWindXLib *) wind->window;
  XWindowAttributes attr;

  XGetWindowAttributes( display, w->window, &attr);
  *width = attr.width;
  *height = attr.height;
}

void GlowDrawXLib::set_window_size( GlowWind *wind, int width, int height)
{
  DrawWindXLib *w = (DrawWindXLib *) wind->window;
  XWindowChanges	xwc;

  if ( !w->window) return;

  xwc.width = width;
  xwc.height = height;

  XConfigureWindow( display, w->window, CWWidth | CWHeight,
	&xwc);
}

static void draw_timer_cb( draw_sTimerCb *timer_cb)
{
  GlowDrawXLib *draw_ctx = (GlowDrawXLib *) timer_cb->ctx->gdraw;

  (timer_cb->callback_func)(timer_cb->ctx);
  XFlush( draw_ctx->display);
  free( timer_cb);
}

static void event_timer_cb( GlowCtx *ctx)
{
//  printf( "Timer callback\n");
  GlowDrawXLib *draw_ctx = (GlowDrawXLib *) ctx->gdraw;
  draw_ctx->timer_id = 0;

  draw_ctx->event_handler( last_event);
}

static void cancel_event_timer( GlowCtx *ctx)
{
  GlowDrawXLib *draw_ctx  = (GlowDrawXLib *) ctx->gdraw;
  if ( draw_ctx->timer_id) {
    XtRemoveTimeOut( draw_ctx->timer_id);
    draw_ctx->timer_id = 0;
  }
//  printf( "Timer removed\n");
//  sys$cantim( ctx, 0);
}

static void event_timer( GlowCtx *ctx, int time_ms)
{
//  $DESCRIPTOR (timeunitdsc, "0 0:0:0.1");       /* 0.1 second units */
//  int	tmo[2], tmptime[2];
//  int	sts;
  GlowDrawXLib *draw_ctx = (GlowDrawXLib *) ctx->gdraw;

//  sts = sys$bintim (&timeunitdsc, &tmo);
//  lib$mult_delta_time (
//		&time_ms,		/* multiplier */
//		&tmo);			/* delta_time (modified) */
//  sys$gettim (&tmptime);
//  lib$add_times (&tmo, &tmptime, &tmo);

//  sts = sys$setimr( draw_ctx->ef, tmo, event_timer_cb, ctx, 0);


//  printf( "Add timer\n");
  draw_ctx->timer_id = XtAppAddTimeOut(
	XtWidgetToApplicationContext(draw_ctx->m_wind.toplevel) ,time_ms,
	(XtTimerCallbackProc)event_timer_cb, ctx);

}

void GlowDrawXLib::set_timer( GlowCtx *gctx, int time_ms,
  	void (*callback_func)( GlowCtx *ctx), void **id)
{
  draw_sTimerCb *timer_cb;
//  $DESCRIPTOR (timeunitdsc, "0 0:0:0.3");       /* 0.1 second units */
//  int	tmo[2], tmptime[2];
//  int	sts;

//  sts = sys$bintim (&timeunitdsc, &tmo);
//  sys$gettim (&tmptime);
//  lib$add_times (&tmo, &tmptime, &tmo);

  timer_cb = (draw_sTimerCb *) calloc( 1, sizeof(draw_sTimerCb));
  timer_cb->ctx = gctx;
  timer_cb->callback_func = callback_func;

  timer_cb->timer_id = XtAppAddTimeOut(
	XtWidgetToApplicationContext( m_wind.toplevel) ,time_ms,
	(XtTimerCallbackProc)draw_timer_cb, timer_cb);

//  sts = sys$setimr( 1, tmo, draw_timer_cb, timer_cb, 0);
  *id = (void *)timer_cb;
}

void GlowDrawXLib::remove_timer( void *id)
{
  XtRemoveTimeOut( ((draw_sTimerCb *)id)->timer_id);

//  sys$cantim( id, 0);
  free( (char *) id);
}

void GlowDrawXLib::set_cursor( GlowWind *wind, glow_eDrawCursor cursor)
{
  DrawWindXLib *w = (DrawWindXLib *) wind->window;
  if ( !w->window) return;

  if ( cursor == glow_eDrawCursor_Normal)
    XUndefineCursor( display, w->window);
  else
    XDefineCursor( display, w->window, cursors[cursor]);
  XFlush( display);
}

int GlowDrawXLib::get_text_extent( char *text, int len,
				   glow_eDrawType gc_type, int idx,
				   int *width, int *height, int *descent)
{
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
  
  XTextExtents( font_struct[font_idx][idx], text, len,
	&text_direction, &text_ascent, &text_descent, &char_struct);
  *height = font_struct[font_idx][idx]->ascent + 
  	font_struct[font_idx][idx]->descent;
  *descent = font_struct[font_idx][idx]->descent;
  *width = char_struct.width;
  return 1;
}

void GlowDrawXLib::copy_area( GlowWind *wind, int x, int y)
{
  DrawWindXLib *w = (DrawWindXLib *) wind->window;
  GC gc;
  if ( ctx->nodraw) return;
  int window_width, window_height;

  if ( &ctx->mw == wind) {
    window_width = ctx->mw.window_width;
    window_height = ctx->mw.window_height;
  }
  else {
    window_width = ctx->navw.window_width;
    window_height = ctx->navw.window_height;
  }

  gc = get_gc( this, glow_eDrawType_Line, 3);
  if ( x >= 0 && y >= 0) {
    XCopyArea( display, w->window, w->window, gc, 
	0, 0, window_width-x, window_height-y, x, y);
    if ( !w->double_buffer_on) {
      if ( x)
        XClearArea( display, w->window, 0, 0, 
		x, window_height, 0);
      if ( y)
        XClearArea( display, w->window, x, 0, 
		window_width, y, 0);
    }
    else {
      XCopyArea( display, w->buffer, w->buffer, gc, 
	0, 0, window_width-x, window_height-y, x, y);
      if ( x)
        XFillRectangle( display, w->buffer, 
			get_gc( this, glow_eDrawType_LineErase, 0),
			0, 0, x, window_height);
      if ( y)
        XFillRectangle( display, w->buffer, 
			get_gc( this, glow_eDrawType_LineErase, 0),
			x, 0, window_width, y);
    }
  }
  else if ( x <= 0 && y <= 0)
  {
    XCopyArea( display, w->window, w->window, gc,
	-x, -y, window_width+x, window_height+y, 0, 0);
    if ( !w->double_buffer_on) {
      if ( x)
        XClearArea( display, w->window, 
	  window_width+x, 0, window_width, window_height, 0);
      if ( y)
        XClearArea( display, w->window, 
	  0, window_height+y, window_width+x, window_height, 0);
    }
    else {
      XCopyArea( display, w->buffer, w->buffer, gc,
	-x, -y, window_width+x, window_height+y, 0, 0);
      if ( x)
        XFillRectangle( display, w->buffer, 
	  get_gc( this, glow_eDrawType_LineErase, 0),
	  window_width+x, 0, window_width, window_height);
      if ( y)
        XFillRectangle( display, w->buffer, 
	  get_gc( this, glow_eDrawType_LineErase, 0),
	  0, window_height+y, window_width+x, window_height);
    }
  }
  else if ( x <= 0 && y >= 0)
  {
    XCopyArea( display, w->window, w->window, gc,
	-x, 0, window_width+x, window_height-y, 0, y);
    if ( !w->double_buffer_on) {
      if ( x)
        XClearArea( display, w->window, 
	  window_width+x, 0, window_width, window_height, 0);
      if ( y)
        XClearArea( display, w->window, 
	  0, 0, window_width+x, y, 0);
    }
    else {
      XCopyArea( display, w->buffer, w->buffer, gc,
	-x, 0, window_width+x, window_height-y, 0, y);
      if ( x)
        XFillRectangle( display, w->buffer, 
	  get_gc( this, glow_eDrawType_LineErase, 0),
	  window_width+x, 0, window_width, window_height);
      if ( y)
        XFillRectangle( display, w->buffer, 
	  get_gc( this, glow_eDrawType_LineErase, 0),
	  0, 0, window_width+x, y);
    }
  }
  else
  {
    XCopyArea( display, w->window, w->window, gc,
	0, -y, window_width-x, window_height+y, x, 0);
    if ( !w->double_buffer_on) {
      if ( x)
        XClearArea( display, w->window, 
	  0, 0, x, window_height, 0);
      if ( y)
        XClearArea( display, w->window, 
	  x, window_height+y, window_width, window_height, 0);
    }
    else {
      XCopyArea( display, w->buffer, w->buffer, gc,
	0, -y, window_width-x, window_height+y, x, 0);
      if ( x)
        XFillRectangle( display, w->buffer, 
	  get_gc( this, glow_eDrawType_LineErase, 0),
	  0, 0, x, window_height);
      if ( y)
        XFillRectangle( display, w->buffer, 
	  get_gc( this, glow_eDrawType_LineErase, 0),
	  x, window_height+y, window_width, window_height);
    }
  }
}

void GlowDrawXLib::clear_area( GlowWind *wind, int ll_x, int ur_x, int ll_y, int ur_y)
{

  if ( ctx->nodraw) return;
  DrawWindXLib *w = (DrawWindXLib *) wind->window;

  XClearArea( display, w->window, ll_x, ll_y, ur_x - ll_x, 
		ur_y - ll_y, 0);
}

void GlowDrawXLib::set_inputfocus( GlowWind *wind)
{
  DrawWindXLib *w = (DrawWindXLib *) wind->window;
  XSetInputFocus( display, w->window, 
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
    return 0;
#if 0
    // No color-file exist, use default values
    double r, g, b;

    printf( "** Using default color palette\n");
    *color_array = (draw_sColor *) calloc( 300, sizeof( draw_sColor));
    *size = 0;
    color_p = *color_array;
    for ( int i = 3; i < 300; i++)
    {
      GlowColor::rgb_color( i, &r, &g, &b);
      color_p->red = int( r * 65535);
      color_p->green = int( g * 65535);
      color_p->blue = int( b * 65535);

      color_p++;
      (*size)++;
    }
#endif
  }
  else
  {
    printf( "** Opening color file %s\n", filename);

    fp.open( filename);
#ifndef OS_VMS
    if ( !fp)
      return GLOW__FILEOPEN;
#endif

    *color_array = (draw_sColor *) calloc( 300, sizeof( draw_sColor));
    *size = 0;
    line_cnt = 0;
    color_p = *color_array;
    while ( *size < 300)
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

void GlowDrawXLib::set_background( GlowWind *wind, glow_eDrawType drawtype, glow_tPixmap pixmap, 
				   glow_tImImage image, int pixmap_width, int pixmap_height)
{
  DrawWindXLib *w = (DrawWindXLib *) wind->window;
  XGCValues 		xgcv;
  Arg 			args[20];
  int			i;
  int			sts;

  sts = XGetGCValues( display, get_gc( this, drawtype, 0), 
	GCForeground, &xgcv);
  if ( !sts)
  {
    cout << "** Error getting background gc" << endl;
    return;
  }
  background = xgcv.foreground;

  // Change erase gcs
  xgcv.foreground =   background;
  xgcv.background =   background;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++) {
    XChangeGC( display, get_gc( this, glow_eDrawType_LineErase, i),
	GCForeground | GCBackground, &xgcv);
    XChangeGC( display, get_gc( this, glow_eDrawType_TextHelveticaErase, i),
	GCForeground | GCBackground, &xgcv);
    XChangeGC( display, get_gc( this, glow_eDrawType_TextHelveticaEraseBold, i),
      GCForeground | GCBackground, &xgcv);
  }

  if ( ! pixmap) {
    i = 0;
    XtSetArg(args[i],XmNbackground,  background); i++;
    XtSetValues( w->toplevel, args, i);

    if ( w->buffer)
      buffer_background( w);
  }
  else {
    if ( w->background_pixmap)
      XFreePixmap( display, (Pixmap)w->background_pixmap);

    w->background_pixmap = (Pixmap)pixmap;
    w->background_pixmap_width = pixmap_width;
    w->background_pixmap_height = pixmap_height;
    XSetWindowBackgroundPixmap( display, w->window, (Pixmap)pixmap);
    if ( w->buffer)
      buffer_background( w);
  }
}

void GlowDrawXLib::reset_background( GlowWind *wind)
{
  DrawWindXLib *w = (DrawWindXLib *) wind->window;
  XGCValues 		xgcv;
  Arg 			args[20];
  int			i;

  i = 0;
  background = original_background;
  XtSetArg(args[i],XmNbackground, background); i++;
  XtSetValues( w->toplevel, args, i);

  // Change erase gcs
  xgcv.foreground =  background;
  xgcv.background =  background;
  for ( i = 0; i < DRAW_TYPE_SIZE; i++) {
    XChangeGC( display, get_gc( this, glow_eDrawType_LineErase, i),
	GCForeground | GCBackground, &xgcv);
    XChangeGC( display, get_gc( this, glow_eDrawType_TextHelveticaErase, i),
	GCForeground | GCBackground, &xgcv);
    XChangeGC( display, get_gc( this, glow_eDrawType_TextHelveticaEraseBold, i),
	GCForeground | GCBackground, &xgcv);
  }
}

void GlowDrawXLib::set_clip( DrawWind *wind, GC gc)
{
  DrawWindXLib *w = (DrawWindXLib *) wind;
  XSetClipRectangles( display, gc, 0, 0, 
		      &w->clip_rectangle[w->clip_cnt-1], 1, Unsorted);
}

void GlowDrawXLib::reset_clip( DrawWind *w, GC gc)
{
  XSetClipMask( display, gc, None);
}

void GlowDrawXLib::set_image_clip_mask( glow_tPixmap pixmap, int x, int y)
{
  XSetClipMask( display, get_gc( this, glow_eDrawType_Line, 0), 
		(Pixmap)pixmap);
  XSetClipOrigin( display, get_gc( this, glow_eDrawType_Line, 0), x, y);
}

void GlowDrawXLib::reset_image_clip_mask()
{
  XSetClipMask( display, get_gc( this, glow_eDrawType_Line, 0), 
		None);
  XSetClipOrigin( display, get_gc( this, glow_eDrawType_Line, 0), 0, 0);
}

int GlowDrawXLib::set_clip_rectangle( GlowWind *wind,
		int ll_x, int ll_y, int ur_x, int ur_y)
{
  DrawWindXLib *w = (DrawWindXLib *) wind->window;
  if ( w->clip_cnt >= DRAW_CLIP_SIZE) {
    printf("** Draw clip refuced\n");
    return 0;
  }
  int x0, x1, y0, y1;
  if ( w->clip_cnt == 0) {
    x0 = min( ll_x, ur_x);
    x1 = max( ll_x, ur_x);
    y0 = min( ll_y, ur_y);
    y1 = max( ll_y, ur_y);
  }
  else {
    x0 = min( ll_x, ur_x);
    x1 = max( ll_x, ur_x);
    y0 = min( ll_y, ur_y);
    y1 = max( ll_y, ur_y);

    x0 = max( x0, w->clip_rectangle[w->clip_cnt-1].x);
    x1 = min( x1, w->clip_rectangle[w->clip_cnt-1].x +
	      w->clip_rectangle[w->clip_cnt-1].width);
    y0 = max( y0, w->clip_rectangle[w->clip_cnt-1].y);
    y1 = min( y1, w->clip_rectangle[w->clip_cnt-1].y +
	      w->clip_rectangle[w->clip_cnt-1].height);
    if ( x0 > x1)
      x0 = x1;
    if ( y0 > y1)
      y0 = y1;
  }
  w->clip_rectangle[w->clip_cnt].x = x0;
  w->clip_rectangle[w->clip_cnt].y = y0;
  w->clip_rectangle[w->clip_cnt].width = x1 - x0;
  w->clip_rectangle[w->clip_cnt].height = y1 - y0;
  w->clip_cnt++;
  w->clip_on = 1;
  return 1;
}

void GlowDrawXLib::reset_clip_rectangle( GlowWind *wind)
{
  DrawWindXLib *w = (DrawWindXLib *) wind->window;
  if ( w->clip_cnt == 0) {
    printf( "** Draw clip mismatch\n");
    return;
  }
  w->clip_cnt--;
  if ( w->clip_cnt == 0)
    w->clip_on = 0;
}

int GlowDrawXLib::clip_level( GlowWind *wind)
{
  DrawWindXLib *w = (DrawWindXLib *) wind->window;
  return w->clip_cnt;
}

int GlowDrawXLib::draw_point( GlowWind *wind, int x1, int y1, glow_eDrawType gc_type)
{
  DrawWindXLib *w = (DrawWindXLib *) wind->window;
  if ( ctx->nodraw) return 1;

  if ( w->clip_on)
    set_clip( w, get_gc( this, gc_type, 0));

  XDrawPoint( display, w->window,
	get_gc( this, gc_type, 0), 
	x1, y1);
  if ( w->double_buffer_on)
    XDrawPoint( display, w->buffer,
	get_gc( this, gc_type, 0), 
	x1, y1);

  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type, 0));
  return 1;
}

int GlowDrawXLib::draw_points( GlowWind *wind, glow_sPointX *points, int point_num, 
	glow_eDrawType gc_type)
{
  if ( ctx->nodraw) return 1;
  DrawWindXLib *w = (DrawWindXLib *) wind->window;

  if ( w->clip_on)
    set_clip( w, get_gc( this, gc_type, 0));

  XDrawPoints( display, w->window,
	get_gc( this, gc_type, 0), 
	(XPoint *) points, point_num, CoordModeOrigin);

  if ( w->clip_on)
    reset_clip( w, get_gc( this, gc_type, 0));
  return 1;
}


void GlowDrawXLib::set_click_sensitivity( GlowWind *wind, int value)
{
  click_sensitivity = value;
}

void GlowDrawXLib::draw_background( GlowWind *wind, int x, int y, int w, int h)
{
  DrawWindXLib *ww = (DrawWindXLib *) wind->window;
  XClearArea( display, ww->window, x, y, w, h, False);
}

int GlowDrawXLib::create_buffer( GlowWind *wind)
{
  DrawWindXLib *w = (DrawWindXLib *) wind->window;
  int depth;
  int window_width, window_height;

  if ( &ctx->mw == wind) {
    window_width = ctx->mw.window_width;
    window_height = ctx->mw.window_height;
  }
  else {
    window_width = ctx->navw.window_width;
    window_height = ctx->navw.window_height;
  }

  if ( !w->double_buffer_on || !window_width)
    return 0;

  if ( window_width == w->buffer_width &&
       window_height == w->buffer_height)
    return 0;


  if ( w->buffer)
    XFreePixmap( display, w->buffer);

  depth = XDefaultDepthOfScreen( screen);
  w->buffer = XCreatePixmap( display, w->window,
	      window_width, window_height, depth);
  w->buffer_width = window_width;
  w->buffer_height = window_height;

  buffer_background( w);
  return 1;
}

void GlowDrawXLib::buffer_background( DrawWind *wind)
{
  DrawWindXLib *w = (DrawWindXLib *) wind;
  int window_width, window_height, subwindow_x, subwindow_y;

  if ( ctx->mw.window == wind) {
    window_width = ctx->mw.window_width;
    window_height = ctx->mw.window_height;
    subwindow_x = ctx->mw.subwindow_x;
    subwindow_y = ctx->mw.subwindow_y;
  }
  else {
    window_width = ctx->navw.window_width;
    window_height = ctx->navw.window_height;
    subwindow_x = ctx->navw.subwindow_x;
    subwindow_y = ctx->navw.subwindow_y;
  }

  if ( !w->double_buffer_on || !window_width)
    return;

  if ( w->background_pixmap) {

    if ( w->clip_on)
      set_clip( w, get_gc( this, glow_eDrawType_Line, 0));

    if ( !((GrowCtx *)ctx)->background_tiled)
      XCopyArea( display, w->background_pixmap,
        w->buffer, get_gc( this, glow_eDrawType_Line, 0), 
	0, 0, w->buffer_width, w->buffer_height, 0, 0);
    else {
      int i, j;
      for ( i = 0; 
            i <= w->buffer_width / w->background_pixmap_width;
            i++)
	for ( j = 0;
              j <= w->buffer_height / w->background_pixmap_height;
              j++)
          XCopyArea( display, w->background_pixmap,
             w->buffer, get_gc( this, glow_eDrawType_Line, 0), 
	     0, 0, w->background_pixmap_width, 
             w->background_pixmap_height, 
             i * w->background_pixmap_width, 
             j * w->background_pixmap_height);
    }  
    if ( w->clip_on)
      reset_clip( w, get_gc( this, glow_eDrawType_Line, 0));
  }
  else {
    if ( w->clip_on)
      set_clip( w, get_gc( this, glow_eDrawType_LineErase, 0));

    XFillRectangle( display, w->buffer,
		    get_gc( this, glow_eDrawType_LineErase, 0),
		    subwindow_x, subwindow_y, window_width, window_height);
    if ( w->clip_on)
      reset_clip( w, get_gc( this, glow_eDrawType_LineErase, 0));

  }
}

int GlowDrawXLib::print( char *filename, double x0, double x1, int end)
{
#if defined IMLIB

#define ps_cPageHeight 820
#define ps_cPageWidth 535
#define ps_cLeftMargin 100
#define ps_cTopMargin 100

  DrawWindXLib *w = &m_wind;
  int width, height;
  unsigned char *rgb;
  unsigned char transp[3] = {255,0,255};
  int i, j;
  int grey;
  int red, blue, green;
  double scalex = 0.71;
  double scaley = 0.78;
  double x, y;
  bool colorimage = true;
  static DrawPs *ps = 0;
  bool new_file = false;

  int window_width = ctx->mw.window_width;
  int window_height = ctx->mw.window_height;
	
  x = ps_cLeftMargin;
  y = ps_cPageHeight - ps_cTopMargin;

  // imlib = Imlib_init( display);

  ImlibImage *image;

  if ( w->double_buffer_on)
    image = Imlib_create_image_from_drawable( (ImlibData *)imlib, w->buffer,
					    0, 0, 0, w->buffer_width,
					    w->buffer_height);
  else
    image = Imlib_create_image_from_drawable( (ImlibData *)imlib, w->window,
					    0, 0, 0, window_width,
					    window_height);
  if ( !image)
    return 0;

  if ( !ps) {
    ps = new DrawPs( filename);
    new_file = true;
    ps->y = y;
  }
  else
    y = ps->y;

  width = image->rgb_width;
  height = image->rgb_height;

  if ( x0 != 0 || x1 != 0) {
    double total_width = width / (x1 - x0);

    if ( total_width * scalex  > ps_cPageWidth - ps_cLeftMargin) {
      x = ps_cPageWidth - total_width * scalex;
      if ( x < 50) {
	double scale_factor = (ps_cPageWidth - 50) / (total_width * scalex);
	x = 50;
	scalex = scalex * scale_factor;
	scaley = scaley * scale_factor;
      }
    }
    x += scalex * total_width * x0;
  }
  else if ( width * scalex  > ps_cPageWidth - ps_cLeftMargin) {
    x = ps_cPageWidth - width * scalex;
    if ( x < 50) {
      double scale_factor = (ps_cPageWidth - 50) / (width * scalex);
      x = 50;
      scalex = scalex * scale_factor;
      scaley = scaley * scale_factor;
    }
  }
  if ( (x0 == 0 && x1 == 0) || x1 == 1.0)
    ps->y -= scaley * height;

  if ( new_file) {
    ps->fp <<
"%!PS-Adobe-2.0 EPSF-1.2" << endl <<
"%%Creator: Proview   $Id: glow_draw_xlib.cpp,v 1.2 2007-01-11 11:40:31 claes Exp $ Glow" << endl <<
"%%EndComments" << endl << endl;
  }
  else
    ps->fp <<
"restore" << endl;

  ps->fp <<
"1.000000 1.000000 scale" << endl <<
"save" << endl <<
scalex * width << " " << scaley * height << " scale" << endl <<
"/oneline " << width << " string def" << endl <<
"/drawimage {" << endl <<
" " << width << " " << height << " 8 [" << width << " 0 0 -" << height << " 0 " << height << "]" << endl <<
" { currentfile oneline readhexstring pop }" << endl;
  if ( colorimage) {
    ps->fp <<
"false 3" << endl <<
"colorimage" << endl;
  }
  else
    ps->fp <<
"image" << endl;

  ps->fp <<
"} def" << endl <<
x/scalex/width << " " << (y - height*scaley)/scaley/height << " translate" << endl <<
"drawimage" << endl;

  ps->fp.flags( (ps->fp.flags() & ~ios_base::dec) | ios_base::hex | ios_base::uppercase);
  ps->fp.fill('0');
  rgb = image->rgb_data;
  j = 0;
  for ( i = 0; i < image->rgb_height * image->rgb_width * 3; i+=3) {
    if ( !colorimage) {
      if ( *rgb == transp[0] && *(rgb+1) == transp[1] && *(rgb+2) == transp[2]) {
	grey = 255;
      }
      else {
	grey = (int) ((0.0 + *rgb + *(rgb+1) + *(rgb+2)) / 3 + 0.5);
      }
      rgb += 3;
      ps->fp.width(2);
      ps->fp << grey;
      if ( ++j >= 40) {
	j = 0;
	ps->fp << endl;
      }
    }
    else {
      if ( *rgb == transp[0] && *(rgb+1) == transp[1] && *(rgb+2) == transp[2]) {
	red = blue = green = 255;
      }
      else {
	red = *rgb;
	green = *(rgb+1);
	blue = *(rgb+2);
      }
      rgb += 3;
      ps->fp.width(2);
      ps->fp << blue;
      ps->fp.width(2);
      ps->fp << green;
      ps->fp.width(2);
      ps->fp << red;
      if ( ++j >= 20) {
	j = 0;
	ps->fp << endl;
      }
    }
  }

  if ( end) {
    ps->fp << endl <<
"restore" << endl << 
"showpage" << endl;
    delete ps;
    ps = 0;
  }
  else {
    ps->fp.flags( ((ps->fp.flags() & ~ios_base::hex) & ~ios_base::uppercase) | ios_base::dec);
  }
  Imlib_destroy_image( (ImlibData *)imlib, image);

#endif
  return 1;
}


// Image functions
int GlowDrawXLib::image_get_width( glow_tImImage image)
{
  return ((ImlibImage *)image)->rgb_width;
}

int GlowDrawXLib::image_get_height( glow_tImImage image)
{
  return ((ImlibImage *)image)->rgb_height;
}

int GlowDrawXLib::image_get_rowstride( glow_tImImage image)
{
  return ((ImlibImage *)image)->rgb_width;
}

unsigned char *GlowDrawXLib::image_get_data( glow_tImImage image)
{
  return ((ImlibImage *)image)->rgb_data;
}

void GlowDrawXLib::image_rotate( glow_tImImage *image, int to_rotation, int from_rotation) 
{
  int drot = to_rotation - from_rotation;
  drot = int( (float(drot) / 360 - floor( float(drot) / 360)) * 360);
  printf( "Drot: %d\n", drot);

  if ( drot) {
    if ( drot == 90 || drot == 270)
      Imlib_rotate_image( (ImlibData *)imlib, (ImlibImage *)*image, 1);
    if ( drot == 180 || drot == 270)
      Imlib_flip_image_vertical( (ImlibData *)imlib, (ImlibImage *)*image);
    if ( drot == 90 || drot == 180)
      Imlib_flip_image_horizontal( (ImlibData *)imlib, (ImlibImage *)*image);
  }
}

void GlowDrawXLib::image_flip_vertical( glow_tImImage *image) 
{
  Imlib_flip_image_horizontal( (ImlibData *)imlib, (ImlibImage *)*image);
}

void GlowDrawXLib::image_flip_horizontal( glow_tImImage *image) 
{
  Imlib_flip_image_vertical( (ImlibData *)imlib, (ImlibImage *)*image);
}

void GlowDrawXLib::image_scale( int width, int height, glow_tImImage orig_im, glow_tImImage *im, 
				glow_tPixmap *im_pixmap, glow_tPixmap *im_mask)
{
#if 0
#if defined IMLIB
  if ( width == ((ImlibImage *)*im)->rgb_width && height == ((ImlibImage *)*im)->rgb_height)
    return;

  Imlib_render( (ImlibData *)imlib, (ImlibImage *)*im, width, height);

  if ( *im_pixmap)
    Imlib_free_pixmap( (ImlibData *)imlib, (Pixmap)*im_pixmap);
  *im_pixmap = (glow_tPixmap) Imlib_move_image( (ImlibData *)imlib, (ImlibImage *)*im);
  *im_mask = (glow_tPixmap) Imlib_move_mask( (ImlibData *)imlib, (ImlibImage *)*im);
#endif
#endif
}

int GlowDrawXLib::image_load( char *imagefile, 
			      glow_tImImage *orig_im, glow_tImImage *im)
{
#if defined IMLIB
  if ( *orig_im)
    Imlib_kill_image( (ImlibData *)imlib, (ImlibImage *)*orig_im);

  if ( im && *im)
    Imlib_kill_image( (ImlibData *)imlib, (ImlibImage *)*im);

  *orig_im = Imlib_load_image( (ImlibData *)imlib, imagefile);
  if ( !*orig_im) 
    return 0;

  // Make a copy
  if ( im)
    *im = Imlib_clone_image( (ImlibData *)imlib, (ImlibImage *)*orig_im);
#endif
  return 1;
}

int GlowDrawXLib::image_render( int width, int height, 
				glow_tImImage orig_im, glow_tImImage *im, 
				glow_tPixmap *im_pixmap, glow_tPixmap *im_mask)
{
#if defined IMLIB

  if ( *im_pixmap) {
    Imlib_free_pixmap( (ImlibData *)imlib, (Pixmap)*im_pixmap);
    *im_pixmap = 0;
  }

  Imlib_render( (ImlibData *)imlib, (ImlibImage *)*im, width, height);
  *im_pixmap = (glow_tPixmap) Imlib_move_image( (ImlibData *)imlib, (ImlibImage *)*im);
  if ( im_mask)
    *im_mask = (glow_tPixmap) Imlib_move_mask( (ImlibData *)imlib, (ImlibImage *)*im);

#endif
  return 1;
}

void GlowDrawXLib::image_free( glow_tImImage image) 
{
  Imlib_destroy_image( (ImlibData *)imlib, (ImlibImage *)image);
}

void GlowDrawXLib::pixmap_free( glow_tPixmap pixmap) 
{
  Imlib_free_pixmap( (ImlibData *)imlib, (Pixmap)pixmap);
}

void GlowDrawXLib::image_pixel_iter( glow_tImImage orig_image, glow_tImImage *image, 
				     void (* pixel_cb)(void *, unsigned char *), void *userdata)
{
  unsigned char *rgb;
  int 		rgb_height;
  int 		rgb_width;
  unsigned char transp[3] = {255,0,255};

  if ( orig_image) {
    if ( image)
      Imlib_destroy_image( (ImlibData *)imlib, (ImlibImage *)*image);
    *image = Imlib_clone_image( (ImlibData *)imlib, (ImlibImage *)orig_image);
  }
  else if ( !image)
    return;

  rgb = ctx->gdraw->image_get_data( (ImlibImage *)*image);
  rgb_height = image_get_height( (ImlibImage *)*image);
  rgb_width = image_get_width( (ImlibImage *)*image);

  for ( int i = 0; i < rgb_height * rgb_width * 3; i+=3) {
    if ( *rgb == transp[0] && *(rgb+1) == transp[1] && *(rgb+2) == transp[2]) {
      rgb += 3;
      continue;
    }

    (pixel_cb) ( userdata, rgb);
    rgb += 3;
  }

  Imlib_changed_image( (ImlibData *)imlib, (ImlibImage *)*image);
}
