import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;

public class JopPanel extends JPanel {
  public JopPanel() {}
  int original_width = 44;
  int original_height = 44;
  Shape[] shapes = new Shape[] {
    new Polygon( new int[] { 22, 2, 22, 42, 22}, new int[] {2, 22, 42, 22, 2}, 5),
    new Line2D.Float( 42F, 22F, 2F, 22F),
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
System.out.println("Component painted");
    }
  }

  public void paintComponent(Graphics g1) {
    Graphics2D g = (Graphics2D) g1;
    double scaleWidth = ((double) getWidth()) / original_width;
    double scaleHeight = ((double) getHeight()) / original_height;
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
    if ( rotate != 0)
      g.transform( AffineTransform.getRotateInstance(
        Math.PI * rotate/180,((double)original_width)/2, ((double)original_height)/2));
//    g.transform( AffineTransform.getScaleInstance( scaleWidth, scaleHeight));
    g.setColor(GeColor.getColor(21, fillColor));
    g.fill( shapes[0]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, borderColor));
    g.draw( shapes[0]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(0, borderColor));
    g.draw( shapes[1]);
  }


}
