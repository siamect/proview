#include "glow_std.h"

#include <stdlib.h>
#include <fstream.h>
#include <math.h>
#include <float.h>
#include "glow_ctx.h"
#include "glow_colpalctx.h"
#include "glow_node.h"
#include "glow_point.h"
#include "glow_rect.h"
#include "glow_line.h"
#include "glow_arc.h"
#include "glow_text.h"
#include "glow_conpoint.h"
#include "glow_node.h"
#include "glow_nodeclass.h"
#include "glow_con.h"
#include "glow_conclass.h"
#include "glow_pushbutton.h"
#include "glow_growrect.h"
#include "glow_growtext.h"
#include "glow_draw.h"
#include "glow_msg.h"


void ColPalCtx::configure()
{
  GrowRect *rect;
  GrowText *text;
  double x, y;
  int i;

  if ( nodraw) 
    return;
  
  display_fill = new GrowRect( this, "FillRect", 0, 0, display_entry_width, 
	entry_height, 
	glow_eDrawType_Line, 1, 0, glow_mDisplayLevel_1, 1, 1, 0, current_fill);
  insert( (GlowArrayElem *)display_fill);
  text = new GrowText( this, "FillText", "Fill", 0.3, 
		       entry_height / 2 + 0.3, 
	glow_eDrawType_TextHelvetica, glow_eDrawType_Line, 2);
  insert( (GlowArrayElem *)text);

  display_border = new GrowRect( this, "BorderRect", display_entry_width, 0, 
	display_entry_width, entry_height,
	glow_eDrawType_Line, 1, 0, glow_mDisplayLevel_1, 1, 1, 0, current_border);
  insert( (GlowArrayElem *)display_border);
  text = new GrowText( this, "BorderText", "Border", display_entry_width + 0.3, 
		       entry_height / 2 + 0.3, 
	glow_eDrawType_TextHelvetica, glow_eDrawType_Color4, 2);
  insert( (GlowArrayElem *)text);

  display_text = new GrowRect( this, "TextRect", display_entry_width * 2, 0, 
	display_entry_width, entry_height,
	glow_eDrawType_Line, 1, 0, glow_mDisplayLevel_1, 1, 1, 0, current_border);
  insert( (GlowArrayElem *)display_text);
  text = new GrowText( this, "TextText", "Text", display_entry_width * 2 + 0.3, 
		       entry_height / 2 + 0.3, 
	glow_eDrawType_TextHelvetica, glow_eDrawType_Color4, 2);
  insert( (GlowArrayElem *)text);

  for ( i = glow_eDrawType_Line; i <= glow_eDrawType_Color300; i++)
  {
    x = entry_width * ( double(i)/columns - floor( double(i)/columns)) * columns;
    y = entry_height * ( 1 + floor( i / columns + DBL_EPSILON));
    rect = new GrowRect( this, "ColorEntry", x, y, entry_width, entry_height, 
	glow_eDrawType_Line, 1, 0, glow_mDisplayLevel_1, 1, 1, 0, (glow_eDrawType) i);
    insert( rect);
  }

  double tone_entry_width = 3 * entry_width;
  y += entry_height;
  x = 0;
  text = new GrowText( this, "ToneText", "Tone", x + 0.15, 
		       y + entry_height / 2 + 0.25, 
	glow_eDrawType_TextHelveticaBold, glow_eDrawType_Line, 2);
  insert( (GlowArrayElem *)text);
  x += 2 * tone_entry_width;
  sprintf( name, "ToneEntry%d", 0);
  rect = new GrowRect( this, name, x, y, 
	tone_entry_width * 2, entry_height, glow_eDrawType_Line, 1, 0, 
        glow_mDisplayLevel_1, 1, 1, 0, glow_eDrawType_Color34);
  insert( rect);
  text = new GrowText( this, name, "Reset", x + 0.15, 
		       y + entry_height / 2 + 0.3, 
		       glow_eDrawType_TextHelvetica, glow_eDrawType_Line, 2);
  insert( (GlowArrayElem *)text);


  x = 0;
  for ( i = glow_eDrawTone_No + 1; i < glow_eDrawTone_GrayHighSaturation; i++) {
    char name[16];
    glow_eDrawType drawtype;

    switch( i) {
      case glow_eDrawTone_No:       	drawtype = glow_eDrawType_Color32; break;
      case glow_eDrawTone_Gray:     	drawtype = glow_eDrawType_Color35; break;
      case glow_eDrawTone_YellowGreen:  drawtype = glow_eDrawType_Color85; break;
      case glow_eDrawTone_Yellow:     	drawtype = glow_eDrawType_Color115; break;
      case glow_eDrawTone_Orange:   	drawtype = glow_eDrawType_Color145; break;
      case glow_eDrawTone_Red:      	drawtype = glow_eDrawType_Color175; break;
      case glow_eDrawTone_Magenta:  	drawtype = glow_eDrawType_Color205; break;
      case glow_eDrawTone_Blue:     	drawtype = glow_eDrawType_Color235; break;
      case glow_eDrawTone_Seablue:  	drawtype = glow_eDrawType_Color265; break;
      case glow_eDrawTone_Green:    	drawtype = glow_eDrawType_Color295; break;
      case glow_eDrawTone_DarkGray: 	drawtype = glow_eDrawType_Color38; break;
      case glow_eDrawTone_DarkYellowGreen:   drawtype = glow_eDrawType_Color88; break;
      case glow_eDrawTone_DarkYellow:     drawtype = glow_eDrawType_Color118; break;
      case glow_eDrawTone_DarkOrange:   drawtype = glow_eDrawType_Color148; break;
      case glow_eDrawTone_DarkRed:      drawtype = glow_eDrawType_Color178; break;
      case glow_eDrawTone_DarkMagenta:  drawtype = glow_eDrawType_Color208; break;
      case glow_eDrawTone_DarkBlue:     drawtype = glow_eDrawType_Color238; break;
      case glow_eDrawTone_DarkSeablue:  drawtype = glow_eDrawType_Color268; break;
      case glow_eDrawTone_DarkGreen:    drawtype = glow_eDrawType_Color298; break;
      case glow_eDrawTone_LightGray: 	drawtype = glow_eDrawType_Color32; break;
      case glow_eDrawTone_LightYellowGreen:  drawtype = glow_eDrawType_Color82; break;
      case glow_eDrawTone_LightYellow:    drawtype = glow_eDrawType_Color112; break;
      case glow_eDrawTone_LightOrange:  drawtype = glow_eDrawType_Color142; break;
      case glow_eDrawTone_LightRed:     drawtype = glow_eDrawType_Color172; break;
      case glow_eDrawTone_LightMagenta: drawtype = glow_eDrawType_Color202; break;
      case glow_eDrawTone_LightBlue:    drawtype = glow_eDrawType_Color232; break;
      case glow_eDrawTone_LightSeablue: drawtype = glow_eDrawType_Color262; break;
      case glow_eDrawTone_LightGreen:   drawtype = glow_eDrawType_Color292; break;
    }
    if ( i == glow_eDrawTone_Gray || 
	 i == glow_eDrawTone_LightGray || 
	 i == glow_eDrawTone_DarkGray) {
      // New row
      y += entry_height;
      x = 0;
    }
    else
      x += tone_entry_width;

    sprintf( name, "ToneEntry%d", i);
    rect = new GrowRect( this, name, x, y, tone_entry_width, entry_height, 
	glow_eDrawType_Line, 1, 0, glow_mDisplayLevel_1, 1, 1, 0, drawtype);
    insert( rect);

  }
  get_borders();
  a.zoom();
  redraw();
  change_scrollbar();
}

void ColPalCtx::change_scrollbar()
{
  glow_sScroll data;

  if ( !scroll_size)
  {
    scroll_size = entry_height + 1.0 / zoom_factor_y;
  }

  data.scroll_data = scroll_data;
  data.total_width = int( (x_right - x_left) / scroll_size);
  data.total_height = int( (y_high - y_low) / scroll_size);
  data.window_width = int( window_width / scroll_size / zoom_factor_x);
  data.window_height = int( window_height / scroll_size / zoom_factor_y + 1);
  data.offset_x = int( offset_x / scroll_size / zoom_factor_x - x_left / 
	scroll_size);
  data.offset_y = int( offset_y /scroll_size / zoom_factor_y - y_low / 
	scroll_size);

  (scroll_callback)( &data);
}


void ColPalCtx::redraw()
{
  clear();
  draw( 0, 0, window_width, window_height);
  nav_zoom();
}

void ColPalCtx::zoom( double factor)
{ 
  if ( fabs(factor) < DBL_EPSILON)
    return;

  zoom_factor_x *= factor;
  zoom_factor_y *= factor;
  if ( offset_x != 0)
    offset_x = int( (offset_x - window_width / 2.0 * ( 1.0/factor - 1)) 
		* factor);
  if ( offset_y != 0)
    offset_y = int( (offset_y  - window_height / 2.0 * ( 1.0/factor - 1)) 
		* factor);
  offset_x = max( offset_x, 0);
  offset_y = max( offset_y, 0);
  if ( (x_right - x_left) * zoom_factor_x <= window_width)
    offset_x = 0;
  if ( (y_high - y_low) * zoom_factor_y <= window_height)
    offset_y = 0;
  a.zoom();
  clear();
  draw( 0, 0, window_width, window_height);
  nav_zoom();
  change_scrollbar();
}

void ColPalCtx::print( char *filename)
{
  int		i;
  double ll_x, ll_y, ur_x, ur_y;
  double width, height;

  if ( a.size() == 0)
    return;
  ((GlowNode *)a[0])->measure( &ll_x, &ll_y, &ur_x, &ur_y);
  height = 60 * (ur_y - ll_y);
  width = 0.70 * height;

  print_ps = new GlowPscript( filename, this, 1);

  for ( i = 0;; i++)
  {
    ll_y = i * height;
    ur_y = ll_y + height;
    ll_x = 0;
    ur_x = width;
    if (  ll_y > y_high)
      break;

    print_ps->print_page( ll_x, ll_y, ur_x, ur_y);
  }
  delete print_ps;
}

void colpal_scroll_horizontal( ColPalCtx *ctx, int value, int bottom)
{
  int x_pix;

  x_pix = int( - value * ctx->scroll_size * ctx->zoom_factor_x + 
	(ctx->offset_x - ctx->x_left * ctx->zoom_factor_x));
  ctx->scroll( x_pix, 0);
}

void colpal_scroll_vertical( ColPalCtx *ctx, int value, int bottom)
{
  int y_pix;

  y_pix = int( - value * ctx->scroll_size * ctx->zoom_factor_y + 
	(ctx->offset_y - ctx->y_low * ctx->zoom_factor_y));
  // Correction for the bottom position
  if ( bottom && ( y_pix >= 0 ||
	ctx->window_height + y_pix < ctx->y_high * ctx->zoom_factor_y - ctx->offset_y))
//        window_height >= (y_high - y_low) * zoom_factor_y)
    y_pix = int( ctx->window_height + ctx->offset_y - 
		ctx->y_high * ctx->zoom_factor_y);
  ctx->scroll( 0, y_pix);
}



int ColPalCtx::event_handler( glow_eEvent event, int x, int y, int w, int h)
{
  int 		sts;
  int 		i;
  GlowCtx	*ctx;
  double 	fx, fy;
  int		callback = 0;

  ctx = this;
//  cout << "Event: " << event << endl;

  fx = double( x + ctx->offset_x) / ctx->zoom_factor_x;
  fy = double( y + ctx->offset_y) / ctx->zoom_factor_y;

  callback_object_type = glow_eObjectType_NoObject;
  callback_object = 0;

  switch ( event)
  {
    case glow_eEvent_MB1Click:
    case glow_eEvent_MB1ClickShift:
    case glow_eEvent_MB2Click:
      sts = 0;
      for ( i = 0; i < a.a_size; i++)
      {
        sts = a.a[i]->event_handler( event, x, y, fx, fy);
        if ( sts == GLOW__NO_PROPAGATE)
          break;
      }

      if ( callback_object_type != glow_eObjectType_NoObject) {
        if ( callback_object->type() == glow_eObjectType_GrowRect) {
          GrowRect *rect;
          char name[32];

          rect = (GrowRect *)callback_object;
          rect->get_object_name( name);
          if ( strncmp( name, "ToneEntry", 9) == 0 &&
                event == glow_eEvent_MB1Click)
          {
            glow_eDrawTone tone;

            sscanf( &name[9], "%d", (int *) &tone);
            if ( event_callback[event] &&
	         sts != GLOW__NO_PROPAGATE)
            {
              static glow_sEvent e;

              e.event = event;
              e.any.type = glow_eEventType_ColorTone;
              e.any.x_pixel = x;
              e.any.y_pixel = y;
              e.any.x = 1.0 * (x + offset_x) / zoom_factor_x;
              e.any.y = 1.0 * (y + offset_y) / zoom_factor_y;
              e.colortone.tone = tone;
              event_callback[event]( this, &e);
            }
            break;
          }
          if ( strcmp( name, "ColorEntry") != 0)
            break;
          if ( event == glow_eEvent_MB1Click) {
            current_fill = rect->fill_drawtype;
            ((GrowRect *)display_fill)->set_fill_color( current_fill);
          }
          else if ( event == glow_eEvent_MB1ClickShift) {
            current_text = rect->fill_drawtype;
            ((GrowRect *)display_text)->set_fill_color( current_text);
          }
          else {
            current_border = rect->fill_drawtype;
            ((GrowRect *)display_border)->set_fill_color( current_border);
          }
          callback = 1;
        }
        else if ( callback_object->type() == glow_eObjectType_GrowText) {
          GrowText *text;
          char name[32];

          text = (GrowText *)callback_object;
          text->get_object_name( name);
          if ( strncmp( name, "ToneEntry", 9) == 0 &&
                event == glow_eEvent_MB1Click)
          {
            glow_eDrawTone tone;

            sscanf( &name[9], "%d", (int *) &tone);
            if ( event_callback[event] &&
	         sts != GLOW__NO_PROPAGATE)
            {
              static glow_sEvent e;

              e.event = event;
              e.any.type = glow_eEventType_ColorTone;
              e.any.x_pixel = x;
              e.any.y_pixel = y;
              e.any.x = 1.0 * (x + offset_x) / zoom_factor_x;
              e.any.y = 1.0 * (y + offset_y) / zoom_factor_y;
              e.colortone.tone = tone;
              event_callback[event]( this, &e);
            }
            break;
          }
        }
      }
      break;

    case glow_eEvent_Exposure:
      int width, height;

      glow_draw_get_window_size( ctx, &width, &height);
      if ( window_width != width || window_height != height) {
        window_width = width;
        window_height = height;
        change_scrollbar();
      }

      draw( x, y, x + w, y + h);
      break;
    default:
      ;
  }

  if ( event_callback[event] &&
	sts != GLOW__NO_PROPAGATE && callback)
  {
    static glow_sEvent e;

    e.event = event;
    e.any.type = glow_eEventType_Object;
    e.any.x_pixel = x;
    e.any.y_pixel = y;
    e.any.x = 1.0 * (x + offset_x) / zoom_factor_x;
    e.any.y = 1.0 * (y + offset_y) / zoom_factor_y;
    e.object.object_type = callback_object_type;
    if ( callback_object_type != glow_eObjectType_NoObject)
      e.object.object = callback_object;
    event_callback[event]( this, &e);
  }
  return 1;
}

