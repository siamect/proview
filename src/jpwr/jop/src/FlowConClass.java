/* 
 * Proview   $Id: FlowConClass.java,v 1.2 2005-09-01 14:57:50 claes Exp $
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


public class FlowConClass {
  String cc_name;
  int con_type;
  int corner;
  int draw_type;
  int line_width;
  double arrow_width;
  double arrow_length;
  double round_corner_amount;
  int group;
  FlowCmn cmn;

  public FlowConClass( FlowCmn cmn) {
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
	case Flow.eSave_ConClass:
          break;
	case Flow.eSave_ConClass_cc_name:
	  cc_name = token.nextToken();
	  break;
	case Flow.eSave_ConClass_con_type:
	  con_type = new Integer(token.nextToken()).intValue();
	  break;
	case Flow.eSave_ConClass_corner:
	  corner = new Integer(token.nextToken()).intValue();
	  break;
	case Flow.eSave_ConClass_draw_type:
	  draw_type = new Integer(token.nextToken()).intValue();
	  break;
	case Flow.eSave_ConClass_line_width:
	  line_width = new Integer(token.nextToken()).intValue();
	  break;
	case Flow.eSave_ConClass_arrow_width:
	  arrow_width = new Double(token.nextToken()).doubleValue();
	  break;
	case Flow.eSave_ConClass_arrow_length:
	  arrow_length = new Double(token.nextToken()).doubleValue();
	  break;
	case Flow.eSave_ConClass_round_corner_amount:
	  round_corner_amount = new Double(token.nextToken()).doubleValue();
	  break;
	case Flow.eSave_ConClass_group:
	  group  = new Integer(token.nextToken()).intValue();
	  break;
	case Flow.eSave_End:
	  end = true;
	  break;
	default:	  
	  System.out.println( "Syntax error in FlowConClass");
	  break;
	}
	if ( end)
	  break;
      }
    } catch ( Exception e) {
      System.out.println( "IOExeption FlowConClass");
    }
  }

}

