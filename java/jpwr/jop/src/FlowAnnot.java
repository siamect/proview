
package jpwr.jop;
import java.io.*;
import java.util.*;
import java.awt.*;
import java.awt.geom.*;
import javax.swing.*;


public class FlowAnnot implements FlowArrayElem {
  FlowPoint p;
  int draw_type;
  int text_size;
  int display_level;
  int annot_type;
  int number;
  FlowCmn cmn;

  public FlowAnnot( FlowCmn cmn) {
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
	case Flow.eSave_Annot:
	  break;
	case Flow.eSave_Annot_number:
	  number = new Integer(token.nextToken()).intValue();
	  break;
	case Flow.eSave_Annot_draw_type:
	  draw_type = new Integer(token.nextToken()).intValue();
	  break;
	case Flow.eSave_Annot_text_size:
	  text_size = new Integer(token.nextToken()).intValue();
	  break;
	case Flow.eSave_Annot_display_level:
	  display_level = new Integer(token.nextToken()).intValue();
	  break;
	case Flow.eSave_Annot_p:
	  p.open( reader);
	  break;
	case Flow.eSave_Annot_annot_type:
	  annot_type = new Integer(token.nextToken()).intValue();
	  break;
	case Flow.eSave_End:
	  end = true;
	  break;
	default:
	  System.out.println( "Syntax error in FlowAnnot");
	  break;
	}
	if ( end)
	  break;
      }
    } catch ( Exception e) {
      System.out.println( "IOExeption FlowAnnot");
    }
  }

  public void draw( Graphics2D g, FlowPoint p0, String[] annotv, boolean highlight) { 
    if ( annotv[number] == null)
      return;
    if ( (display_level & cmn.display_level) == 0)
      return;

    int tsize;
    int idx = (int)(cmn.zoom_factor / cmn.base_zoom_factor * (text_size +4) - 4);
    if ( idx < 0) return;
    // if ( idx > 8)
    //   idx = 8;

    switch( idx) {
    case 0: tsize = 8; break;
    case 1: tsize = 10; break;
    case 2: tsize = 12; break;
    case 3: tsize = 14; break;
    case 4: tsize = 14; break;
    case 5: tsize = 18; break;
    case 6: tsize = 18; break;
    case 7: tsize = 18; break;
    default: tsize = idx * 3;
    }
    tsize -= tsize/5;

    Font f;
    switch ( draw_type) {
    case Flow.eDrawType_TextHelveticaBold:
      f = new Font("Helvetica", Font.BOLD, tsize);
      break;
    default:
      f = new Font("Helvetica", Font.PLAIN, tsize);
    }
    char[] c = new char[] { 10 };
    StringTokenizer token = new StringTokenizer( annotv[number], new String(c));
    g.setColor( Color.black);
    if ( highlight)
      g.setColor( Color.red);
    g.setFont( f);

    float x = (float)((p.x + p0.x) * cmn.zoom_factor);
    float y = (float)((p.y + p0.y) * cmn.zoom_factor - tsize/4);
    while ( token.hasMoreTokens()) {
      g.drawString( token.nextToken(), x, y);
      y += f.getSize2D() * 1.4;
    }
  }

}







