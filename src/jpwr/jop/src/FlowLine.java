/* 
 * Proview   $Id: FlowLine.java,v 1.2 2005-09-01 14:57:50 claes Exp $
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


public class FlowLine implements FlowArrayElem {
  FlowPoint p1;
  FlowPoint p2;
  int draw_type;
  int line_width;
  FlowCmn cmn;

  public FlowLine( FlowCmn cmn) {
    this.cmn = cmn;
    p1 = new FlowPoint(cmn);
    p2 = new FlowPoint(cmn);
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
	case Flow.eSave_Line:
	  break;
	case Flow.eSave_Line_draw_type:
	  draw_type  = new Integer(token.nextToken()).intValue();
	  break;
	case Flow.eSave_Line_line_width:
	  line_width  = new Integer(token.nextToken()).intValue();
	  break;
	case Flow.eSave_Line_p1:
	  p1.open( reader);
	  break;
	case Flow.eSave_Line_p2:
	  p2.open( reader);
	  break;
	case Flow.eSave_End:
	  end = true;
	  break;
	default:
	  System.out.println( "Syntax error in FlowLine");
	  break;
	}
	if ( end)
	  break;
      }
    } catch ( Exception e) {
      System.out.println( "IOExeption FlowLine");
    }
  }

  public void draw( Graphics2D g, FlowPoint p, String[] annotv, boolean highlight) {
      Line2D.Double line = new Line2D.Double( (p1.x + p.x) * cmn.zoom_factor,
						   (p1.y + p.y) * cmn.zoom_factor,
						   (p2.x + p.x) * cmn.zoom_factor,
						   (p2.y + p.y) * cmn.zoom_factor);
      switch ( draw_type) {
      case Flow.eDrawType_LineDashed:
      case Flow.eDrawType_LineDashedRed:
	g.setStroke( new BasicStroke( (float)(cmn.zoom_factor / cmn.base_zoom_factor * line_width),
		     BasicStroke.CAP_BUTT,
		     BasicStroke.JOIN_BEVEL, 0, 
		     new float[] { 4F * line_width, 2F * line_width},
		     0));
	break;
      default:
        g.setStroke( new BasicStroke( (float)(cmn.zoom_factor / cmn.base_zoom_factor * line_width)));
      }
      switch ( draw_type) {
      case Flow.eDrawType_LineGray:
        g.setColor( Color.lightGray);
        break;
      case Flow.eDrawType_LineRed:
      case Flow.eDrawType_LineDashedRed:
        g.setColor( Color.red);
        break;
      default:
        g.setColor( Color.black);
      }
      if ( highlight)
        g.setColor( Color.red);
      g.draw( line);
  }

}




