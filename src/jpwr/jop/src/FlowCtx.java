/* 
 * Proview   $Id: FlowCtx.java,v 1.2 2005-09-01 14:57:50 claes Exp $
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
 */


package jpwr.jop;
import java.io.*;
import java.util.*;
import jpwr.rt.*;


public class FlowCtx implements FlowCtxInterface {
  FlowCmn cmn;
  String name;

  public FlowCtx( Gdh gdh, JopSession session) {
    cmn = new FlowCmn( this, gdh, session);
  }

  public void unselect() {
    for ( int i = 0; i < cmn.a.size(); i++)
      ((FlowComponent)cmn.a.get(i)).setSelect( false);
  }

  public void open( BufferedReader reader) {
    String line;
    StringTokenizer token;
    boolean end = false;

    try {
      while( (line = reader.readLine()) != null) {
	token = new StringTokenizer(line);
	int key = new Integer(token.nextToken()).intValue();
	if ( cmn.debug) System.out.println( "line : " + key);

	switch ( key) {
	case Flow.eSave_Ctx:
	  break;
	case Flow.eSave_Ctx_zoom_factor:
	  cmn.zoom_factor = new Double(token.nextToken()).doubleValue();
	  break;
	case Flow.eSave_Ctx_base_zoom_factor:
	  cmn.base_zoom_factor = new Double(token.nextToken()).doubleValue();
	  break;
	case Flow.eSave_Ctx_offset_x:
	  cmn.offset_x = new Integer(token.nextToken()).intValue();
	  break;
	case Flow.eSave_Ctx_offset_y:
	  cmn.offset_y = new Integer(token.nextToken()).intValue();
	  break;
	case Flow.eSave_Ctx_nav_zoom_factor:
	case Flow.eSave_Ctx_print_zoom_factor:
	case Flow.eSave_Ctx_nav_offset_x:
	case Flow.eSave_Ctx_nav_offset_y:
	  break;
	case Flow.eSave_Ctx_x_right:
	  cmn.x_right = new Double(token.nextToken()).doubleValue();
	  break;
	case Flow.eSave_Ctx_x_left:
	  cmn.x_left = new Double(token.nextToken()).doubleValue();
	  break;
	case Flow.eSave_Ctx_y_high:
	  cmn.y_high = new Double(token.nextToken()).doubleValue();
	  break;
	case Flow.eSave_Ctx_y_low:
	  cmn.y_low = new Double(token.nextToken()).doubleValue();
	  break;
	case Flow.eSave_Ctx_nav_rect_ll_x:
	case Flow.eSave_Ctx_nav_rect_ll_y:
	case Flow.eSave_Ctx_nav_rect_ur_x:
	case Flow.eSave_Ctx_nav_rect_ur_y:
	case Flow.eSave_Ctx_nav_rect_hot:
	  break;
	case Flow.eSave_Ctx_name:
	  name = token.nextToken();
	  break;
	case Flow.eSave_Ctx_user_highlight:
	case Flow.eSave_Ctx_grid_size_x:
	case Flow.eSave_Ctx_grid_size_y:
	case Flow.eSave_Ctx_grid_on:
	case Flow.eSave_Ctx_draw_delta:
	case Flow.eSave_Ctx_refcon_width:
	case Flow.eSave_Ctx_refcon_height:
	case Flow.eSave_Ctx_refcon_textsize:
	case Flow.eSave_Ctx_refcon_linewidth:
	  break;
	case Flow.eSave_Ctx_a_nc:
	  FlowVector.open( reader, cmn, cmn.a_nc);
	  break;
	case Flow.eSave_Ctx_a_cc:
	  FlowVector.open( reader, cmn, cmn.a_cc);
	  break;
	case Flow.eSave_Ctx_a:
	  FlowVector.open( reader, cmn, cmn.a);
	  break;
	case Flow.eSave_End:
	  end = true;
	  break;
	default:
	  System.out.println( "Syntax error in FlowCtx");
	  break;
	}
	if ( end)
	  break;
      }
    } catch ( Exception e) {
      System.out.println( "IOExeption FlowCtx");
    }
    cmn.zoom_factor = cmn.base_zoom_factor;
  }

  Object getSelected() {
    for ( int i = 0; i < cmn.a.size(); i++) {
      if ( ((FlowComponent)cmn.a.get(i)).getSelect())
	return cmn.a.get(i);
    }
    return null;
  }

  Object getObject( String name) {
    for ( int i = 0; i < cmn.a.size(); i++) {
      if ( ((FlowComponent)cmn.a.get(i)).getName().equalsIgnoreCase( name))
	return cmn.a.get(i);
    }
    return null;
  }
}







