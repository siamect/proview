#include "glow_std.h"


#include <stdlib.h>
#include <iostream.h>
#include "glow.h"
#include "glow_ctx.h"
#include "glow_draw.h"
#include "glow_array.h"
#include "glow_pushbutton.h"
#include "glow_msg.h"

GlowPushButton::GlowPushButton( GlowCtx *glow_ctx, char *p_name,
	double x1, double y1, double width, double height) : 
	ctx(glow_ctx), pos(glow_ctx, x1,y1), 
	r(glow_ctx, x1, y1, width, height, glow_eDrawType_Line, 1), 
	t(glow_ctx, p_name, x1, y1+height/2, glow_eDrawType_TextHelvetica, 1)
{
  strcpy( name, p_name);
  zoom_factor = ctx->zoom_factor_x;
  draw(0,0,0,0);
}

void GlowPushButton::draw( int ll_x, int ll_y, int ur_x, int ur_y) 
{ 
  GlowPoint korr_pos(ctx);

//  pos.z_x = pos.x * zoom_factor + ctx->offset_x;
//  pos.z_y = pos.y * zoom_factor + ctx->offset_y;
//  korr_pos.z_x = pos.z_x + ctx->offset_x;
//  korr_pos.z_y = pos.z_y + ctx->offset_y;
//  r.draw( &pos, 0);
//  t.draw( &pos, 0);

  glow_draw_rect( ctx, pos.z_x + r.ll.z_x, pos.z_y + r.ll.z_y, 
	  r.ur.z_x - r.ll.z_x, r.ur.z_y - r.ll.z_y, r.draw_type, 
	  r.line_width-1, 0);
  glow_draw_text( ctx, pos.z_x + t.p.z_x, pos.z_y + t.p.z_y, t.text,
	  strlen(t.text), t.draw_type, t.text_size, 0, 0);
}

int GlowPushButton::event_handler( glow_eEvent event, int x, int y)
{
  int sts;

  sts = 0;
  switch ( event)
  {
    case glow_eEvent_MB1Click:

      if ( r.ll.z_x + pos.z_x <= x && 
         x <= r.ur.z_x  + pos.z_x  &&
         r.ll.z_y  + pos.z_y <= y && 
         y <= r.ur.z_y + pos.z_y)
      {
        sts = GLOW__NO_PROPAGATE;
        if ( strcmp( name, "Zoom in") == 0)
          ctx->zoom( 1.25);
        else if ( strcmp( name, "Zoom out") == 0)
          ctx->zoom( 0.8);
        else if ( strcmp( name, "Right") == 0)
          ctx->traverse( 50, 0);
        else if ( strcmp( name, "Left") == 0)
          ctx->traverse( -50, 0);
        else if ( strcmp( name, "Up") == 0)
          ctx->traverse( 0, -50);
        else if ( strcmp( name, "Down") == 0)
          ctx->traverse( 0, 50);
        else if ( strcmp( name, "Copy") == 0)
          ctx->copy();
        else if ( strcmp( name, "Cut") == 0)
          ctx->cut();
        else if ( strcmp( name, "Paste") == 0)
          ctx->paste();
        else if ( strcmp( name, "ConType") == 0)
        {
          int i;
          for ( i = 0; i < ctx->a_cc.size(); i++)
          {
             if ( ctx->default_conclass == ctx->a_cc[i])
               break;
          }
          if ( ++i >= ctx->a_cc.size())
            i = 0;
          ctx->set_default_conclass( ctx->a_cc[i]);
        }
        else
          cout << "Unknown pushbutton" << endl;
        break; 
     default:
       ;
   }
  }
  return sts;
}

ostream& operator<< ( ostream& o, const GlowPushButton p)
{
  o << "PushButton: " << p.name << endl;
  return o;
}

