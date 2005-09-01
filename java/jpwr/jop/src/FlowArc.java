/* 
 * Proview   $Id: FlowArc.java,v 1.2 2005-09-01 14:57:50 claes Exp $
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


public class FlowArc implements FlowArrayElem {
  FlowPoint ll;
  FlowPoint ur;
  double angel1;
  double angel2;
  int draw_type;
  int line_width;
  FlowCmn cmn;

  public FlowArc( FlowCmn cmn) {
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
	case Flow.eSave_Arc:
	  break;
	case Flow.eSave_Arc_draw_type:
	  draw_type = new Integer(token.nextToken()).intValue();
	  break;
	case Flow.eSave_Arc_line_width:
	  line_width = new Integer(token.nextToken()).intValue();
	  break;
	case Flow.eSave_Arc_angel1:
	  angel1 = new Double(token.nextToken()).doubleValue();
	  break;
	case Flow.eSave_Arc_angel2:
	  angel2 = new Double(token.nextToken()).doubleValue();
	  break;
	case Flow.eSave_Arc_ll:
	  ll.open( reader);
	  break;
	case Flow.eSave_Arc_ur:
	  ur.open( reader);
	  break;
	case Flow.eSave_End:
	  end = true;
	  break;
	default:
	  System.out.println( "Syntax error in FlowArc");
	  break;
	}
	if ( end)
	  break;
      }
    } catch ( Exception e) {
      System.out.println( "IOExeption FlowArc");
    }
  }

  public void draw( Graphics2D g, FlowPoint p, String[] annotv, boolean highlight) {
      Arc2D.Double rect = new Arc2D.Double( (ll.x + p.x) * cmn.zoom_factor,
						   (ll.y + p.y) * cmn.zoom_factor,
						   (ur.x - ll.x) * cmn.zoom_factor,
						   (ur.y - ll.y) * cmn.zoom_factor,
						  angel1, angel2, Arc2D.OPEN);
      g.setStroke( new BasicStroke( (float)(cmn.zoom_factor / cmn.base_zoom_factor * line_width)));
      g.setColor( Color.black);
      if ( highlight)
        g.setColor( Color.red);
      g.draw( rect);
  }

}





