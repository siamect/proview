package jpwr.beans;
import jpwr.rt.*;
import jpwr.jop.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
public class JopFrameroundcorners extends JComponent {
  Dimension size;
  public JopFrameroundcorners()
  {
    try {
      jbInit();
    }
    catch(Exception e) {
      e.printStackTrace();
    }
  }

  private void jbInit() throws Exception {
    size = new Dimension( 264, 184);
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
  int original_width = 264;
  int original_height = 184;
  double rotate;
  public void setRotate( double rotate) {
    if ( rotate < 0)
      this.rotate = rotate % 360 + 360;
    else
      this.rotate = rotate % 360;
  }
  public double getRotate() { return rotate;}
  Shape[] shapes = new Shape[] { 
    new Arc2D.Float(2F, 142F, 40F, 40F, 225F, 45F, Arc2D.OPEN),
    new Arc2D.Float(222F, 142F, 40F, 40F, 315F, 45F, Arc2D.OPEN),
    new Arc2D.Float(222F, 2F, 40F, 40F, 0F, 45F, Arc2D.OPEN),
    new Arc2D.Float(2F, 2F, 40F, 40F, 90F, 45F, Arc2D.OPEN),
    new Arc2D.Float(222F, 2F, 40F, 40F, 45F, 45F, Arc2D.OPEN),
    new Arc2D.Float(2F, 142F, 40F, 40F, 180F, 45F, Arc2D.OPEN),
    new Arc2D.Float(222F, 142F, 40F, 40F, 270F, 45F, Arc2D.OPEN),
    new Line2D.Float( 242F, 2F, 22F, 2F),
    new Line2D.Float( 2F, 162F, 2F, 22F),
    new Line2D.Float( 242F, 182F, 22F, 182F),
    new Line2D.Float( 262F, 162F, 262F, 22F),
    new Arc2D.Float(2F, 2F, 40F, 40F, 135F, 45F, Arc2D.OPEN),
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
    g.setStroke( new BasicStroke(2F));
    g.setColor(GeColor.getColor(30, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));
    g.draw( shapes[0]);
    g.setStroke( new BasicStroke(2F));
    g.setColor(GeColor.getColor(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));
    g.draw( shapes[1]);
    g.setStroke( new BasicStroke(2F));
    g.setColor(GeColor.getColor(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));
    g.draw( shapes[2]);
    g.setStroke( new BasicStroke(2F));
    g.setColor(GeColor.getColor(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));
    g.draw( shapes[3]);
    g.setStroke( new BasicStroke(2F));
    g.setColor(GeColor.getColor(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));
    g.draw( shapes[4]);
    g.setStroke( new BasicStroke(2F));
    g.setColor(GeColor.getColor(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));
    g.draw( shapes[5]);
    g.setStroke( new BasicStroke(2F));
    g.setColor(GeColor.getColor(30, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));
    g.draw( shapes[6]);
    g.setStroke( new BasicStroke(2F));
    g.setColor(GeColor.getColor(33, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));
    g.draw( shapes[7]);
    g.setStroke( new BasicStroke(2F));
    g.setColor(GeColor.getColor(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));
    g.draw( shapes[8]);
    g.setStroke( new BasicStroke(2F));
    g.setColor(GeColor.getColor(30, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));
    g.draw( shapes[9]);
    g.setStroke( new BasicStroke(2F));
    g.setColor(GeColor.getColor(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));
    g.draw( shapes[10]);
    g.setStroke( new BasicStroke(2F));
    g.setColor(GeColor.getColor(32, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));
    g.draw( shapes[11]);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
