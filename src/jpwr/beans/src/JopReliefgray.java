package jpwr.beans;
import jpwr.rt.*;
import jpwr.jop.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
public class JopReliefgray extends JComponent {
  Dimension size;
  public JopReliefgray()
  {
    try {
      jbInit();
    }
    catch(Exception e) {
      e.printStackTrace();
    }
  }

  private void jbInit() throws Exception {
    size = new Dimension( 244, 144);
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
  int original_width = 244;
  int original_height = 144;
  double rotate;
  public void setRotate( double rotate) {
    if ( rotate < 0)
      this.rotate = rotate % 360 + 360;
    else
      this.rotate = rotate % 360;
  }
  public double getRotate() { return rotate;}
  Shape[] shapes = new Shape[] { 
    new Rectangle2D.Float(6F, 6F, 232F, 132F),
    new Polygon( new int[] { 202, 238, 238, 210, 146, 202}, new int[] {6, 6, 70, 138, 138, 6}, 6),
    new Polygon( new int[] { 50, 110, 170, 110, 50}, new int[] {138, 6, 6, 138, 138}, 5),
    new Polygon( new int[] { 106, 166, 202, 146, 106}, new int[] {138, 6, 6, 138, 138}, 5),
    new Polygon( new int[] { 4, 37, 113, 53, 5, 4}, new int[] {75, 5, 5, 138, 138, 74}, 6),
    new Polygon( new int[] { 2, 6, 238, 242, 2}, new int[] {2, 6, 6, 2, 2}, 5),
    new Polygon( new int[] { 2, 6, 6, 2, 2}, new int[] {2, 6, 138, 142, 2}, 5),
    new Polygon( new int[] { 238, 238, 242, 242, 238}, new int[] {6, 138, 142, 2, 6}, 5),
    new Polygon( new int[] { 2, 6, 238, 242, 2}, new int[] {142, 138, 138, 142, 142}, 5),
    new Polygon( new int[] { 42, 102}, new int[] {142, 2}, 2),
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
    g.setColor(GeColor.getColor(15, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, false));
    g.fill( shapes[0]);
    g.setColor(GeColor.getColor(11, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, false));
    g.fill( shapes[1]);
    g.setColor(GeColor.getColor(16, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, false));
    g.fill( shapes[2]);
    g.setColor(GeColor.getColor(12, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, false));
    g.fill( shapes[3]);
    g.setColor(GeColor.getColor(11, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, false));
    g.fill( shapes[4]);
    g.setColor(GeColor.getColor(10, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, false));
    g.fill( shapes[5]);
    g.setColor(GeColor.getColor(17, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, false));
    g.fill( shapes[6]);
    g.setColor(GeColor.getColor(17, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, false));
    g.fill( shapes[7]);
    g.setColor(GeColor.getColor(14, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, false));
    g.fill( shapes[8]);
    g.setColor(GeColor.getColor(12, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, fillColor, false));
    g.fill( shapes[9]);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
