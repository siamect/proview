
package jpwr.jop;
import java.io.*;
import java.util.*;
import java.awt.*;
import java.awt.geom.*;
import javax.swing.*;


public class FlowCon extends JComponent implements FlowComponent, JopDynamic {
  double x_right;
  double x_left;
  double y_high;
  double y_low;
  FlowCmn cmn;
  FlowConClass cc;
  int p_num;
  int l_num;
  int a_num;
  int arrow_num;
  int ref_num;
  double point_x[] = new double[8];
  double point_y[] = new double[8];
  FlowArray line_a;
  FlowArray arc_a;
  FlowArray arrow_a;
  FlowArray ref_a;
  String c_name;
  String trace_object;
  String trace_attribute;
  int trace_attr_type;
  int temporary_ref;
  boolean highlight; 

  FlowCon( FlowCmn cmn) {
    this.cmn = cmn;
    line_a = new FlowArray(cmn);
    arc_a = new FlowArray(cmn);
    arrow_a = new FlowArray(cmn);
    ref_a = new FlowArray(cmn);
  }

  public String getName() {
    return c_name;
  }
  public String getTraceObject() {
    return trace_object;
  }
  public void open( BufferedReader reader) {
    String line;
    StringTokenizer token;
    boolean end = false;
    int i;
    boolean found = false;

    try {
      while( (line = reader.readLine()) != null) {
	token = new StringTokenizer(line);
	int key = new Integer(token.nextToken()).intValue();
	if ( cmn.debug) System.out.println( "line : " + key);

	switch ( key) {
	case Flow.eSave_Con:
	  break;
	case Flow.eSave_Con_cc:
	  String cc_name = token.nextToken();
	  found = false;
          for ( i = 0; i < cmn.a_cc.size(); i++) {
	    if ( ((FlowConClass)cmn.a_cc.get(i)).cc_name.equals( cc_name)) {
	      cc = (FlowConClass) cmn.a_cc.get(i);
	      found = true;
	      break;
	    }
	  }
	  if ( !found)
	    System.out.println( "FlowCon: ConClass not found: " + cc_name);
	  break;
	case Flow.eSave_Con_c_name:
	  if ( token.hasMoreTokens())
	    c_name = token.nextToken();
          else
	    c_name = new String();
	  break;
	case Flow.eSave_Con_x_right:
	  x_right = new Double( token.nextToken()).doubleValue();
	  break;
	case Flow.eSave_Con_x_left:
	  x_left = new Double( token.nextToken()).doubleValue();
	  break;
	case Flow.eSave_Con_y_high:
	  y_high = new Double( token.nextToken()).doubleValue();
	  break;
	case Flow.eSave_Con_y_low:
	  y_low = new Double( token.nextToken()).doubleValue();
	  break;
	case Flow.eSave_Con_dest_node:
	  reader.readLine();
	  break;
	case Flow.eSave_Con_source_node:
	  reader.readLine();
	  break;
	case Flow.eSave_Con_dest_conpoint:
	case Flow.eSave_Con_source_conpoint:
	case Flow.eSave_Con_dest_direction:
	case Flow.eSave_Con_source_direction:
	  break;
	case Flow.eSave_Con_line_a:
	  line_a.open( reader);
	  break;
	case Flow.eSave_Con_arc_a:
	  arc_a.open( reader);
	  break;
	case Flow.eSave_Con_arrow_a:
	  arrow_a.open( reader);
	  break;
	case Flow.eSave_Con_ref_a:
	  ref_a.open( reader);
	  break;
	case Flow.eSave_Con_p_num:
	  p_num = new Integer( token.nextToken()).intValue();
	  break;
	case Flow.eSave_Con_l_num:
	  l_num = new Integer( token.nextToken()).intValue();
	  break;
	case Flow.eSave_Con_a_num:
	  a_num = new Integer( token.nextToken()).intValue();
	  break;
	case Flow.eSave_Con_arrow_num:
	  arrow_num = new Integer( token.nextToken()).intValue();
	  break;
	case Flow.eSave_Con_ref_num:
	  ref_num = new Integer( token.nextToken()).intValue();
	  break;
	case Flow.eSave_Con_point_x:
	  for ( i = 0; i < p_num; i++)
	    point_x[i] = new Double( reader.readLine()).doubleValue();
	  break;
	case Flow.eSave_Con_point_y:
	  for ( i = 0; i < p_num; i++)
	    point_y[i] = new Double( reader.readLine()).doubleValue();
	  break;

	case Flow.eSave_Con_source_ref_cnt:
	case Flow.eSave_Con_dest_ref_cnt:
	  break;
	case Flow.eSave_Con_trace_object:
	  if ( token.hasMoreTokens())
	    trace_object = token.nextToken();
	  break;
	case Flow.eSave_Con_trace_attribute:
	  if ( token.hasMoreTokens())
	    trace_attribute = token.nextToken();
	  break;
	case Flow.eSave_Con_trace_attr_type:
	  trace_attr_type = new Integer( token.nextToken()).intValue();
	  break;
	case Flow.eSave_Con_temporary_ref:
	  temporary_ref = new Integer( token.nextToken()).intValue();
	  break;
	case Flow.eSave_End:
	  end = true;
	  break;
	default:
	  System.out.println( "Syntax error in FlowCon");
	  break;
	}
	if ( end)
	  break;
      }
    } catch ( Exception e) {
      System.out.println( "IOExeption FlowCon");
    }
  }

  public void setBounds() {
      // Make extra space for arrows witch is not included in rectangel
      int x = (int)((x_left - cmn.x_left) * cmn.zoom_factor) - 3 * Flow.DRAWOFFSET;
      int y = (int)((y_low - cmn.y_low) * cmn.zoom_factor) - 3 * Flow.DRAWOFFSET;
      int width = (int)((x_right - x_left) * cmn.zoom_factor) + 6 * Flow.DRAWOFFSET;
      int height = (int)((y_high - y_low) * cmn.zoom_factor) + 6 * Flow.DRAWOFFSET;
      setBounds( x, y, width, height);
  }

  public boolean getSelect() {
    return false;
  }
  public void setSelect( boolean select) {
  }

  public void paintComponent( Graphics g1) {
    Graphics2D g = (Graphics2D) g1;
    if ( cmn.antiAliasing)
      g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);

    // Adjust pos to javabean koordinates
    FlowPoint p = new FlowPoint(cmn);
    p.x = - x_left + ((double) (3 * Flow.DRAWOFFSET)) / cmn.zoom_factor;
    p.y = - y_low + ((double) ( 3 * Flow.DRAWOFFSET)) / cmn.zoom_factor;
    if ( temporary_ref != 0 || cc.con_type == Flow.eConType_Reference)
      ref_a.draw( g, p, null, highlight);
    else {
      line_a.draw( g, p, l_num, highlight);
      arc_a.draw( g, p, a_num, highlight);
      arrow_a.draw( g, p, arrow_num, highlight);
    }
  }
  public Object dynamicGetRoot() {
    return null;
  }
  public void dynamicOpen() {
  }
  public void dynamicClose() {
  }
  public void dynamicUpdate( boolean animationOnly) {
  }
}













