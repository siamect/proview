
package jpwr.jop;
import java.io.*;
import java.util.*;
import java.awt.*;
import java.awt.geom.*;
import javax.swing.*;


public class FlowArrow implements FlowArrayElem {
  FlowPoint p1;
  FlowPoint p2;
  FlowPoint p_dest;
  double arrow_width;
  double arrow_length;
  int draw_type;
  int line_width;
  FlowCmn cmn;

  public FlowArrow( FlowCmn cmn) {
    this.cmn = cmn;
    p1 = new FlowPoint(cmn);
    p2 = new FlowPoint(cmn);
    p_dest = new FlowPoint(cmn);
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
	case Flow.eSave_Arrow:
	  break;
	case Flow.eSave_Arrow_arrow_width:
	  arrow_width = new Double(token.nextToken()).doubleValue();
	  break;
	case Flow.eSave_Arrow_arrow_length:
	  arrow_length = new Double(token.nextToken()).doubleValue();
	  break;
	case Flow.eSave_Arrow_draw_type:
	  draw_type = new Integer(token.nextToken()).intValue();
	  break;
	case Flow.eSave_Arrow_line_width:
	  line_width = new Integer(token.nextToken()).intValue();
	  break;
	case Flow.eSave_Arrow_p_dest:
	  p_dest.open( reader);
	  break;
	case Flow.eSave_Arrow_p1:
	  p1.open( reader);
	  break;
	case Flow.eSave_Arrow_p2:
	  p2.open( reader);
	  break;
	case Flow.eSave_End:
	  end = true;
	  break;
	default:
	  System.out.println( "Syntax error in FlowArrow");
	  break;
	}
	if ( end)
	  break;
      }
    } catch ( Exception e) {
      System.out.println( "IOExeption FlowArrow");
    }
  }
  public void draw( Graphics2D g, FlowPoint p, String[] annotv, boolean highlight) {
      Polygon pol = new Polygon( new int[] {
	                           (int) ((p1.x + p.x) * cmn.zoom_factor),
	                           (int) ((p2.x + p.x) * cmn.zoom_factor),
	                           (int) ((p_dest.x + p.x) * cmn.zoom_factor),
	                           (int) ((p1.x + p.x) * cmn.zoom_factor)},
			       new int[] {
				   (int) ((p1.y + p.y) * cmn.zoom_factor),
				   (int) ((p2.y + p.y) * cmn.zoom_factor),
				   (int) ((p_dest.y + p.y) * cmn.zoom_factor),
				   (int) ((p1.y + p.y) * cmn.zoom_factor)},
			       4);

      switch ( draw_type) {
      case Flow.eDrawType_LineGray:
        g.setColor( Color.lightGray);
        break;
      case Flow.eDrawType_LineRed: 
      case Flow.eDrawType_LineDashedRed:
        g.setColor( Color.red);
        break;
      case Flow.eDrawType_LineErase:
	return;
      default:
        g.setColor( Color.black);
      }
      if ( highlight)
        g.setColor( Color.red);
      g.fill( pol);
  }

}







