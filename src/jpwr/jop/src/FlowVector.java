
package jpwr.jop;
import java.io.*;
import java.util.*;


public class FlowVector {
  public FlowVector() {
  }

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
	  a.add( nc);
	  break;
	case Flow.eSave_ConClass:
	  FlowConClass cc = new FlowConClass( cmn);
	  cc.open( reader);
	  a.add( cc);
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





