package jpwr.beans;
import jpwr.rt.*;
import jpwr.jop.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import javax.swing.Timer;
import java.awt.event.*;

public class JopPump extends JComponent implements JopDynamic, ActionListener{
  Dimension size;
  JopFrame frame;
  Timer timer = new Timer(500, this);
  public JopPump()
  {
    try {
      jbInit();
    }
    catch(Exception e) {
      e.printStackTrace();
    }
  }

  private void jbInit() throws Exception {
    size = new Dimension( 44, 44);
    timer.start();
  }
  public void actionPerformed(ActionEvent e) {
    timer.stop();
    timer = null;
    Container parent = getParent();
    while ( parent != null) {
      if ( parent instanceof JopFrame) {
        frame = (JopFrame)parent;
        frame.engine.add(this);
        break;
      }
      parent = parent.getParent();
    }
  }
  int original_width = 44;
  int original_height = 44;
  Shape[] shapes = new Shape[] { 
    new Arc2D.Float(2F, 2.39058F, 40F, 40F, 0F, 360F, Arc2D.PIE),
    new Polygon( new int[] { 23, 15, 31, 23}, new int[] {2, 18, 18, 2}, 4),
  };
  int fillColor = 9999;
  int originalFillColor = 9999;
  int borderColor = 9999;
  public void setFillColor( int fillColor) {
    this.fillColor = fillColor;
    this.originalFillColor = fillColor;
  }
  public void resetFillColor() {
    fillColor = originalFillColor;
  }
  public int getFillColor() {
    return fillColor;
  }
  public void setBorderColor( int borderColor) {
    this.borderColor = borderColor;
  }
  public int getBorderColor() {
    return borderColor;
  }
  double rotate;
  public void setRotate( double rotate) { this.rotate = rotate;}
  public double getRotate() { return rotate;}
  public void paint(Graphics g1) {
    Graphics2D g = (Graphics2D) g1;
    Component c;
    Point p;
    paintComponent(g);
    for ( int i = 0; i < getComponentCount(); i++) {
      AffineTransform save = g.getTransform();
      c = getComponent(i);
      p = c.getLocation();
      g.translate((int)p.getX(), (int)p.getY());
      c.paint(g);
      g.setTransform(save);
    }
  }
  public void paintComponent(Graphics g1) {
    Graphics2D g = (Graphics2D) g1;
    double scaleWidth = ((double) getWidth()) / original_width;
    double scaleHeight = ((double) getHeight()) / original_height;
    AffineTransform save = g.getTransform();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
    if ( rotate != 0)
      g.transform( AffineTransform.getRotateInstance(
        Math.PI * rotate/180,((double)original_width)/2, ((double)original_height)/2));
    g.transform( AffineTransform.getScaleInstance( scaleWidth, scaleHeight));
    ((Arc2D)shapes[0]).setArcType(Arc2D.PIE);
    g.setColor(GeColor.getColor(21, fillColor));
    g.fill( shapes[0]);
    ((Arc2D)shapes[0]).setArcType(Arc2D.OPEN);
    g.setStroke( new BasicStroke(2F));
    g.setColor(GeColor.getColor(0, borderColor));
    g.draw( shapes[0]);
    g.setColor(GeColor.getColor(0, borderColor));
    g.fill( shapes[1]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, borderColor));
    g.draw( shapes[1]);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
  int lowColor = 3;
  public void setLowColor( int lowColor) { this.lowColor = lowColor;}
  public int getLowColor() { return lowColor;}
  String pwrAttribute = new String();
  public void setPwrAttribute( String pwrAttribute) { this.pwrAttribute = pwrAttribute;}
  public String getPwrAttribute() { return pwrAttribute;}
  String pwrAttrError = new String();
  public void setPwrAttrError( String pwrAttrError) { this.pwrAttrError = pwrAttrError;}
  public String getPwrAttrError() { return pwrAttrError;}
  boolean valueColor;
  boolean valueError;
  boolean firstScan = true;
  GdhrRefObjectInfo retColor = null;
  GdhrRefObjectInfo retError = null;
  public void dynamicOpen() {
    retColor = Gdh.refObjectInfo( pwrAttribute);
    if ( retColor.evenSts())
      System.out.println( "refObjectInfoError retColor");
    retError = Gdh.refObjectInfo( pwrAttrError);
    if ( retError.evenSts())
      System.out.println( "refObjectInfoError retError");
  }
  public void dynamicClose() {
    Gdh.unrefObjectInfo( retColor.refid);
    Gdh.unrefObjectInfo( retError.refid);
  }
  public void dynamicUpdate( boolean animationOnly) {
    if ( animationOnly)
      return;
    if ( firstScan) {
      firstScan = false;
    }
    valueColor = Gdh.getObjectRefInfoBoolean( retColor.id);
    valueError = Gdh.getObjectRefInfoBoolean( retError.id);
    if ( valueError) {
      fillColor = 56;
      repaint();
    }
    else if ( valueColor) {
      fillColor = originalFillColor;
      repaint();
    }
    else {
      fillColor = lowColor;
      repaint();
    }
  }
  public Object dynamicGetRoot() {
    return null;
  }
}
