/* 
 * Proview   $Id: FlowArray.java,v 1.3 2006-06-14 10:41:53 claes Exp $
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


public class FlowArray {
  Vector<Object> a = new Vector<Object>();
  FlowCmn cmn;

  public FlowArray( FlowCmn cmn) {
    this.cmn = cmn;
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
	case Flow.eSave_Array:
	  break;
	case Flow.eSave_Rect:
	  FlowRect rect = new FlowRect( cmn);
	  rect.open( reader);
	  a.add( rect);
	  break;
	case Flow.eSave_Line:
	  FlowLine l = new FlowLine( cmn);
	  l.open( reader);
	  a.add( l);
	  break;
	case Flow.eSave_Arc:
	  FlowArc arc = new FlowArc( cmn);
	  arc.open( reader);
	  a.add( arc);
	  break;
	case Flow.eSave_Text:
	  FlowText text = new FlowText( cmn);
	  text.open( reader);
	  a.add( text);
	  break;
	case Flow.eSave_ConPoint:
	  FlowConPoint cp = new FlowConPoint( cmn);
	  cp.open( reader);
	  a.add( cp);
	  break;
	case Flow.eSave_Annot:
	  FlowAnnot annot = new FlowAnnot( cmn);
	  annot.open( reader);
	  a.add( annot);
	  break;
	case Flow.eSave_Arrow:
	  FlowArrow arrow = new FlowArrow( cmn);
	  arrow.open( reader);
	  a.add( arrow);
	  break;
	case Flow.eSave_Point:
	  FlowPoint point = new FlowPoint( cmn);
	  point.open( reader);
	  a.add( point);
	  break;
	case Flow.eSave_End:
	  end = true;
	  break;
	default:	  
	  System.out.println( "Syntax error in FlowArray");
	  break;
	}
	if ( end)
	  break;
      }
    } catch ( Exception e) {
      System.out.println( "IOExeption FlowArray");
    }
  }

  public void draw( Graphics2D g, FlowPoint p, String[] annotv, boolean highlight) {
    for ( int i = 0; i < a.size(); i++) {
      ((FlowArrayElem)a.get(i)).draw( g, p, annotv, highlight);
    }
  }

  public void draw( Graphics2D g, FlowPoint p, int size, boolean highlight) {
    for ( int i = 0; i < size; i++) {
      ((FlowArrayElem)a.get(i)).draw( g, p, null, highlight);
    }
  }

}









