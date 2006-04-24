/* 
 * Proview   $Id: FlowRect.java,v 1.3 2006-04-24 13:21:46 claes Exp $
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
import java.awt.*;
import java.awt.geom.*;
import javax.swing.*;

public class FlowRect implements FlowArrayElem {
  FlowPoint ll;
  FlowPoint ur;
  int draw_type;
  int line_width;
  int display_level;
  FlowCmn cmn;

  public FlowRect( FlowCmn cmn) {
    this.cmn = cmn;
    ll = new FlowPoint(cmn);
    ur = new FlowPoint(cmn);
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
	case Flow.eSave_Rect_draw_type:
	  draw_type  = new Integer(token.nextToken()).intValue();
	  break;
	case Flow.eSave_Rect_line_width:
	  line_width  = new Integer(token.nextToken()).intValue();
	  break;
	case Flow.eSave_Rect_display_level:
	  display_level = new Integer( token.nextToken()).intValue();
	  break;
	case Flow.eSave_Rect_ll:
	  ll.open( reader);
	  break;
	case Flow.eSave_Rect_ur:
	  ur.open( reader);
	  break;
	case Flow.eSave_End:
	  end = true;
	  break;
	default:
	  System.out.println( "Syntax error in FlowRect");
	  break;
	}
	if ( end)
	  break;
      }
    } catch ( Exception e) {
      System.out.println( "IOExeption FlowRect");
    }
  }

  public void draw( Graphics2D g, FlowPoint p, String[] annotv, boolean highlight) {
      if ( (display_level & FlowCmn.display_level) == 0)
	return;

      Rectangle2D.Double rect = new Rectangle2D.Double( (ll.x + p.x) * cmn.zoom_factor,
						   (ll.y + p.y) * cmn.zoom_factor,
						   (ur.x - ll.x) * cmn.zoom_factor,
						   (ur.y - ll.y) * cmn.zoom_factor);
      g.setStroke( new BasicStroke( (float)(cmn.zoom_factor / cmn.base_zoom_factor * line_width)));
      switch ( draw_type) {
      case Flow.eDrawType_LineGray:
        g.setColor( Color.lightGray);
        break;
      case Flow.eDrawType_LineRed: 
      case Flow.eDrawType_LineDashedRed:
        g.setColor( Color.red);
        break;
      case Flow.eDrawType_LineErase:
	return;
      default:
        g.setColor( Color.black);
      }
      if ( highlight)
        g.setColor( Color.red);
      g.draw( rect);
  }
}













