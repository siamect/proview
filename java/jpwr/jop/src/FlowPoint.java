
package jpwr.jop;
import java.io.*;
import java.util.*;


public class FlowPoint {
  double x;
  double y;
  FlowCmn cmn;
  
  public FlowPoint( FlowCmn cmn) {
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
	case Flow.eSave_Point:
	  break;
	case Flow.eSave_Point_x:
	  x  = new Double(token.nextToken()).doubleValue();
	  break;
	case Flow.eSave_Point_y:
	  y  = new Double(token.nextToken()).doubleValue();
	  break;
	case Flow.eSave_End:
	  end = true;
	  break;
	default:
	  System.out.println("Syntax error in FlowPoint");
	  break;
	}
	if ( end)
	  break;
      }
    } catch ( Exception e) {
      System.out.println( "IOExeption FlowRect");
    }
  }
}












