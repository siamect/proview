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

public class JopIndroundlarge extends JComponent implements JopDynamic, ActionListener{
  Dimension size;
  Object root;
  JopEngine en;
  Timer timer = new Timer(500, this);
  public JopIndroundlarge()
  {
    try {
      jbInit();
    }
    catch(Exception e) {
      e.printStackTrace();
    }
  }

  private void jbInit() throws Exception {
    size = new Dimension( 61, 62);
    timer.start();
  }
  public void actionPerformed(ActionEvent e) {
    boolean engine_found = false;
    Container parent = getParent();
    while ( parent != null) {
      if ( parent instanceof JopFrame) {
        en = ((JopFrame)parent).engine;
        if ( !en.isReady())
          break;
        en.add(this);
        root = parent;
        engine_found = true;
        break;
      }
      parent = parent.getParent();
    }
    if ( !engine_found) {
      parent = getParent();
      while ( parent != null) {
        if ( parent instanceof JopApplet) {
          en = ((JopApplet)parent).engine;
          if ( !en.isReady())
            break;
          en.add(this);
          root = parent;
          engine_found = true;
          break;
        }
        parent = parent.getParent();
      }
    }
    if ( engine_found) {
      timer.stop();
      timer = null;    
    }
  }
  int fillColor = 9999;
  int originalFillColor = 9999;
  int borderColor = 9999;
  int colorTone = 0;
  int originalColorTone = 0;
  int colorShift = 0;
  int originalColorShift = 0;
  int colorBrightness = 0;
  int originalColorBrightness = 0;
  int colorIntensity = 0;
  int originalColorIntensity = 0;
  int colorInverse = 0;
  int originalColorInverse = 0;
  public void setColorTone( int colorTone) {
    this.colorTone = colorTone;
    originalColorTone = colorTone;
  }
  public int getColorTone() {
    return colorTone;
  }
  public void setColorShift( int colorShift) {
    this.colorShift = colorShift;
    originalColorShift = colorShift;
  }
  public int getColorShift() {
    return colorShift;
  }
  public void setColorBrightness( int colorBrightness) {
    this.colorBrightness = colorBrightness;
    originalColorBrightness = colorBrightness;
  }
  public int getColorBrightness() {
    return colorBrightness;
  }
  public void setColorIntensity( int colorIntensity) {
    this.colorIntensity = colorIntensity;
    originalColorIntensity = colorIntensity;
  }
  public int getColorIntensity() {
    return colorIntensity;
  }
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
  int original_width = 61;
  int original_height = 62;
  double rotate;
  public void setRotate( double rotate) {
    if ( rotate < 0)
      this.rotate = rotate % 360 + 360;
    else
      this.rotate = rotate % 360;
  }
  public double getRotate() { return rotate;}
  Shape[] shapes = new Shape[] { 
    new Arc2D.Float(2F, 2F, 57F, 58F, 0F, 360F, Arc2D.PIE),
    new Polygon( new int[] { 8, 6, 12, 20, 33, 40, 26, 15, 12, 11}, new int[] {25, 33, 44, 52, 54, 50, 47, 37, 26, 18}, 10),
    new Polygon( new int[] { 36, 34, 40, 47, 39}, new int[] {14, 19, 23, 20, 12}, 5),
  };
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
    float width = getWidth();
    float height = getHeight();
    AffineTransform save = g.getTransform();
    AffineTransform save_tmp;
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
    if ( 45.0 <= rotate && rotate < 135.0) {
      g.translate( width, 0.0); 
      g.rotate( Math.PI * rotate/180, 0.0, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else if ( 135.0 <= rotate && rotate < 225.0)
    {
      g.rotate( Math.PI * rotate/180, width/2, height/2);
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    }
    else if ( 225.0 <= rotate && rotate < 315.0)
    {
      g.translate( -height, 0.0);
      g.rotate( Math.PI * rotate/180, height, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/original_width,
      		width/original_height));
    }
    else
      g.transform( AffineTransform.getScaleInstance( width/original_width,
      		height/original_height));
    ((Arc2D)shapes[0]).setArcType(Arc2D.PIE);
    g.setColor(GeColor.getColor(96, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor));
    g.fill( shapes[0]);
    ((Arc2D)shapes[0]).setArcType(Arc2D.OPEN);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));
    g.draw( shapes[0]);
    g.setColor(GeColor.getColor(97, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor));
    g.fill( shapes[1]);
    g.setColor(GeColor.getColor(90, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor));
    g.fill( shapes[2]);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
  int lowTone = 1;
  public void setLowTone( int lowTone) { this.lowTone = lowTone;}
  public int getLowTone() { return lowTone;}
  String pwrAttribute = new String();
  public void setPwrAttribute( String pwrAttribute) { this.pwrAttribute = pwrAttribute;}
  public String getPwrAttribute() { return pwrAttribute;}
  boolean valueColor;
  boolean valueColorOld;
  boolean firstScan = true;
  GdhrRefObjectInfo retColor = null;
  boolean colorAttrFound = false;
  public void dynamicOpen() {
    if ( pwrAttribute.compareTo("") != 0) {
      retColor = en.gdh.refObjectInfo( pwrAttribute);
      if ( retColor.evenSts())
        System.out.println( "refObjectInfoError retColor");
      else
        colorAttrFound = true;
    }
  }
  public void dynamicClose() {
    if ( colorAttrFound)
      en.gdh.unrefObjectInfo( retColor.refid);
  }
  public void dynamicUpdate( boolean animationOnly) {
    if ( animationOnly)
      return;
    if ( colorAttrFound) {
      valueColor = en.gdh.getObjectRefInfoBoolean( retColor.id);
      if ( valueColorOld != valueColor || firstScan) {
        if ( valueColor) {
          colorTone = originalColorTone;
          fillColor = originalFillColor;
          repaint();
        }
        else {
          if ( lowTone <= GeColor.COLOR_TONE_MAX)
            colorTone = lowTone;
          else
            fillColor = lowTone;
          repaint();
        }
        valueColorOld = valueColor;
      }
    }
    if ( firstScan)
      firstScan = false;
  }
  public Object dynamicGetRoot() {
    return null;
  }
}
