package jpwr.beans;
import jpwr.rt.*;
import jpwr.jop.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
public class JopRelief extends JComponent {
  Dimension size;
  public JopRelief()
  {
    try {
      jbInit();
    }
    catch(Exception e) {
      e.printStackTrace();
    }
  }

  private void jbInit() throws Exception {
    size = new Dimension( 388, 228);
  }
  int original_width = 388;
  int original_height = 228;
  Shape[] shapes = new Shape[] { 
    new Arc2D.Float(306F, 2F, 80F, 80F, 45F, 45F, Arc2D.PIE),
    new Arc2D.Float(306F, 2F, 80F, 80F, 0F, 45F, Arc2D.PIE),
    new Arc2D.Float(306F, 146F, 80F, 80F, 315F, 45F, Arc2D.PIE),
    new Arc2D.Float(306F, 146F, 80F, 80F, 270F, 45F, Arc2D.PIE),
    new Arc2D.Float(2F, 146F, 80F, 80F, 225F, 45F, Arc2D.PIE),
    new Arc2D.Float(2F, 146F, 80F, 80F, 180F, 45F, Arc2D.PIE),
    new Arc2D.Float(2F, 2F, 80F, 80F, 135F, 45F, Arc2D.PIE),
    new Arc2D.Float(2F, 2F, 80F, 80F, 90F, 45F, Arc2D.PIE),
    new Arc2D.Float(8.4F, 8.4F, 67.2F, 67.2F, 90F, 90F, Arc2D.PIE),
    new Arc2D.Float(8.4F, 152.4F, 67.2F, 67.2F, 180F, 90F, Arc2D.PIE),
    new Arc2D.Float(312.4F, 152.4F, 67.2F, 67.2F, 270F, 90F, Arc2D.PIE),
    new Arc2D.Float(312.4F, 8.4F, 67.2F, 67.2F, 0F, 90F, Arc2D.PIE),
    new Rectangle2D.Float(40.4F, 2F, 307.2F, 6.4F),
    new Rectangle2D.Float(40.4F, 219.6F, 307.2F, 6.4F),
    new Rectangle2D.Float(2F, 40.4F, 6.4F, 147.2F),
    new Rectangle2D.Float(379.6F, 40.4F, 6.4F, 147.2F),
    new Polygon( new int[] { 40, 347, 347, 40, 40}, new int[] {8, 8, 219, 219, 11}, 5),
    new Polygon( new int[] { 8, 40, 40, 8, 8}, new int[] {40, 40, 187, 187, 40}, 5),
    new Polygon( new int[] { 347, 379, 379, 347, 347}, new int[] {40, 40, 187, 187, 40}, 5),
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
  public void paintComponent(Graphics g1) {
    Graphics2D g = (Graphics2D) g1;
    double scaleWidth = ((double) getWidth()) / original_width;
    double scaleHeight = ((double) getHeight()) / original_height;
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
    g.transform( AffineTransform.getScaleInstance( scaleWidth, scaleHeight));
    g.setColor(GeColor.getColor(20, fillColor));
    g.fill( shapes[0]);
    g.setColor(GeColor.getColor(32, fillColor));
    g.fill( shapes[1]);
    g.setColor(GeColor.getColor(32, fillColor));
    g.fill( shapes[2]);
    g.setColor(GeColor.getColor(33, fillColor));
    g.fill( shapes[3]);
    g.setColor(GeColor.getColor(33, fillColor));
    g.fill( shapes[4]);
    g.setColor(GeColor.getColor(32, fillColor));
    g.fill( shapes[5]);
    g.setColor(GeColor.getColor(32, fillColor));
    g.fill( shapes[6]);
    g.setColor(GeColor.getColor(30, fillColor));
    g.fill( shapes[7]);
    g.setColor(GeColor.getColor(31, fillColor));
    g.fill( shapes[8]);
    g.setColor(GeColor.getColor(31, fillColor));
    g.fill( shapes[9]);
    g.setColor(GeColor.getColor(31, fillColor));
    g.fill( shapes[10]);
    g.setColor(GeColor.getColor(31, fillColor));
    g.fill( shapes[11]);
    g.setColor(GeColor.getColor(30, fillColor));
    g.fill( shapes[12]);
    g.setColor(GeColor.getColor(33, fillColor));
    g.fill( shapes[13]);
    g.setColor(GeColor.getColor(32, fillColor));
    g.fill( shapes[14]);
    g.setColor(GeColor.getColor(32, fillColor));
    g.fill( shapes[15]);
    g.setColor(GeColor.getColor(31, fillColor));
    g.fill( shapes[16]);
    g.setColor(GeColor.getColor(31, fillColor));
    g.fill( shapes[17]);
    g.setColor(GeColor.getColor(31, fillColor));
    g.fill( shapes[18]);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
