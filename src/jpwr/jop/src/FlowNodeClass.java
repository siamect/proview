
package jpwr.jop;
import java.io.*;
import java.util.*;
import java.awt.*;
import java.awt.geom.*;
import javax.swing.*;

public class FlowNodeClass {
  FlowArray a;
  String nc_name;
  int group;
  FlowCmn cmn;

  public FlowNodeClass( FlowCmn cmn) {
    this.cmn = cmn;
    a = new FlowArray( cmn);
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
	case Flow.eSave_NodeClass_nc_name:
	  nc_name = token.nextToken();
	  break;
	case Flow.eSave_NodeClass_a:
	  a.open( reader);
	  break;
	case Flow.eSave_NodeClass_group:
	  group  = new Integer(token.nextToken()).intValue();
	  break;
	case Flow.eSave_End:
	  end = true;
	  break;
	default:	  
	  System.out.println( "Syntax error in FlowNodeClass");
	  break;
	}
	if ( end)
	  break;
      }
    } catch ( Exception e) {
      System.out.println( "IOExeption FlowNodeClass");
    }
  }

  public void draw( Graphics2D g, FlowPoint p, String[] annotv, boolean highlight) {
    a.draw( g, p, annotv, highlight);
  }

}







