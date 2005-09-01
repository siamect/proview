/* 
 * Proview   $Id: FlowConPoint.java,v 1.2 2005-09-01 14:57:50 claes Exp $
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


public class FlowConPoint implements FlowArrayElem {
  FlowPoint p;
  int number;
  int direction;
  String trace_attribute;
  int trace_attr_type;
  FlowCmn cmn;

  public FlowConPoint( FlowCmn cmn) {
    this.cmn = cmn;
    p = new FlowPoint(cmn);
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
	case Flow.eSave_ConPoint:
	  break;
	case Flow.eSave_ConPoint_number:
	  number = new Integer(token.nextToken()).intValue();
	  break;
	case Flow.eSave_ConPoint_direction:
	  direction = new Integer(token.nextToken()).intValue();
	  break;
	case Flow.eSave_ConPoint_p:
	  p.open( reader);
	  break;
	case Flow.eSave_ConPoint_trace_attribute:
	  trace_attribute = reader.readLine();
	  break;
	case Flow.eSave_ConPoint_trace_attr_type:
	  trace_attr_type = new Integer(token.nextToken()).intValue();
	  break;
	case Flow.eSave_End:
	  end = true;
	  break;
	default:
	  System.out.println( "Syntax error in FlowConPoint");
	  break;
	}
	if ( end)
	  break;
      }
    } catch ( Exception e) {
      System.out.println( "IOExeption FlowConPoint");
    }
  }

  public void draw( Graphics2D g, FlowPoint p, String[] annotv, boolean highlight) {
  }

}

