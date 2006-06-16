/* 
 * Proview   $Id: FlowVector.java,v 1.4 2006-06-16 05:09:38 claes Exp $
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


public class FlowVector {
  public FlowVector() {
  }

  //public static void open( BufferedReader reader, FlowCmn cmn, Vector<Object> a) {
  public static void open( BufferedReader reader, FlowCmn cmn, Vector a) {
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
	case Flow.eSave_NodeClass:
	  FlowNodeClass nc = new FlowNodeClass( cmn);
	  nc.open( reader);
	  a.add( (Object) nc);
	  break;
	case Flow.eSave_ConClass:
	  FlowConClass cc = new FlowConClass( cmn);
	  cc.open( reader);
	  a.add( (Object) cc);
	  break;
	case Flow.eSave_Node:
	  FlowNode n = new FlowNode( cmn);
	  n.open( reader);
	  a.add( n);
	  break;
	case Flow.eSave_Con:
	  FlowCon c = new FlowCon( cmn);
	  c.open( reader);
	  a.add( c);
	  break;
	case Flow.eSave_End:
	  end = true;
	  break;
	default:	  
	  System.out.println( "Syntax error in FlowVector");
	  break;
	}
	if ( end)
	  break;
      }
    } catch ( Exception e) {
      System.out.println( "IOExeption FlowVector");
    }
  }

}





