package jpwr.beans;
import jpwr.rt.*;
import jpwr.jop.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
public class JopSlider1 extends JopSlider {
  Dimension size;
  public JopSlider1()
  {
    try {
      jbInit();
    }
    catch(Exception e) {
      e.printStackTrace();
    }
  }

  private void jbInit() throws Exception {
    size = new Dimension( 34, 32);
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
  int original_width = 34;
  int original_height = 32;
  double rotate;
  public void setRotate( double rotate) { this.rotate = rotate;}
  public double getRotate() { return rotate;}
  Shape[] shapes = new Shape[] { 
    new Polygon( new int[] { 24, 32, 32, 12, 2, 24, 24}, new int[] {2, 24, 30, 30, 22, 22, 2}, 7),
    new Rectangle2D.Float(2F, 2F, 22F, 20F),
    new Rectangle2D.Float(2F, 2F, 22F, 6F),
    new Rectangle2D.Float(2F, 16F, 22F, 6F),
    new Line2D.Float( 24F, 11.3482F, 2F, 11.3482F),
    new Line2D.Float( 24F, 12.391F, 2F, 12.391F),
    new Rectangle2D.Float(2F, 2F, 22F, 20F),
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
    double scaleWidth = ((double) getWidth()) / original_width;
    double scaleHeight = ((double) getHeight()) / original_height;
    AffineTransform save = g.getTransform();
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
    if ( rotate != 0)
      g.transform( AffineTransform.getRotateInstance(
        Math.PI * rotate/180,((double)original_width)/2, ((double)original_height)/2));
    g.transform( AffineTransform.getScaleInstance( scaleWidth, scaleHeight));
    g.setColor(GeColor.getColor(8, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor));
    g.fill( shapes[0]);
    g.setColor(GeColor.getColor(22, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor));
    g.fill( shapes[1]);
    g.setColor(GeColor.getColor(20, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor));
    g.fill( shapes[2]);
    g.setColor(GeColor.getColor(24, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor));
    g.fill( shapes[3]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(24, borderColor));
    g.draw( shapes[4]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(20, borderColor));
    g.draw( shapes[5]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(9, borderColor));
    g.draw( shapes[6]);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
