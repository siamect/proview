
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

