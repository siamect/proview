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

public class JopMotor extends JComponent implements JopDynamic, ActionListener{
  Dimension size;
  Object root;
  JopEngine en;
  Timer timer = new Timer(500, this);
  public JopMotor()
  {
    try {
      jbInit();
    }
    catch(Exception e) {
      e.printStackTrace();
    }
  }

  private void jbInit() throws Exception {
    size = new Dimension( 28, 28);
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
  int original_width = 28;
  int original_height = 28;
  double rotate;
  public void setRotate( double rotate) {
    if ( rotate < 0)
      this.rotate = rotate % 360 + 360;
    else
      this.rotate = rotate % 360;
  }
  public double getRotate() { return rotate;}
  Shape[] shapes = new Shape[] { 
    new Arc2D.Float(2F, 2.7508F, 24F, 24F, 0F, 360F, Arc2D.PIE),
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
    g.setColor(GeColor.getColor(23, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, false));
    g.fill( shapes[0]);
    ((Arc2D)shapes[0]).setArcType(Arc2D.OPEN);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, false));
    g.draw( shapes[0]);
    g.setColor(Color.black);
    g.setFont(new Font("Helvetica", Font.PLAIN, 14));
    g.drawString( "M",8, 18);
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
  boolean valueColorOld;
  boolean valueError;
  boolean valueErrorOld;
  boolean firstScan = true;
  GdhrRefObjectInfo retColor = null;
  GdhrRefObjectInfo retError = null;
  boolean colorAttrFound = false;
  boolean errorAttrFound = false;
  public void dynamicOpen() {
    if ( pwrAttribute.compareTo("") != 0) {
      retColor = en.gdh.refObjectInfo( pwrAttribute);
      if ( retColor.evenSts())
        System.out.println( "refObjectInfoError retColor");
      else
        colorAttrFound = true;
    }
    if ( pwrAttrError.compareTo("") != 0) {
      retError = en.gdh.refObjectInfo( pwrAttrError);
      if ( retError.evenSts())
        System.out.println( "refObjectInfoError retError");
      else
        errorAttrFound = true;
    }
  }
  public void dynamicClose() {
    if ( colorAttrFound)
      en.gdh.unrefObjectInfo( retColor.refid);
    if ( errorAttrFound)
      en.gdh.unrefObjectInfo( retError.refid);
  }
  public void dynamicUpdate( boolean animationOnly) {
    if ( animationOnly)
      return;
    if ( errorAttrFound) {
      valueColor = en.gdh.getObjectRefInfoBoolean( retColor.id);
      valueError = en.gdh.getObjectRefInfoBoolean( retError.id);
      if ( valueError != valueErrorOld || valueColor != valueColorOld || firstScan) {
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
        valueErrorOld = valueError;
        valueColorOld = valueColor;
      }
    }
    else if ( colorAttrFound) {
      valueColor = en.gdh.getObjectRefInfoBoolean( retColor.id);
      if ( valueColorOld != valueColor || firstScan) {
        if ( valueColor) {
          fillColor = originalFillColor;
          repaint();
        }
        else {
          fillColor = lowColor;
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
