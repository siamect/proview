package jpwr.jop;
import jpwr.rt.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
public class GeFrameThin extends GeComponent {
  Dimension size;
  public GeFrameThin()
  {
    try {
      jbInit();
    }
    catch(Exception e) {
      e.printStackTrace();
    }
  }

  private void jbInit() throws Exception {
    size = new Dimension( 144, 84);
  }
  Shape[] shapes = new Shape[] { null, null, null, null};  
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

    if ( shapes[0] == null) {
      shapes[0] = new Line2D.Float(width - 2F, 2F, 2F, 2F);
      shapes[1] = new Line2D.Float(2F, height - 2F, 2F, 2F);
      shapes[2] = new Line2D.Float(width - 2F, height - 2F, 2F, height - 2F);
      shapes[3] = new Line2D.Float(width - 2F, height - 2F, width - 2F, 2F);
    }

    if ( 45.0 <= rotate && rotate < 135.0) {
      g.translate( width, 0.0); 
      g.rotate( Math.PI * rotate/180, 0.0, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/width,
      		width/height));
    }
    else if ( 135.0 <= rotate && rotate < 225.0)
    {
      g.rotate( Math.PI * rotate/180, width/2, height/2);
    }
    else if ( 225.0 <= rotate && rotate < 315.0)
    {
      g.translate( -height, 0.0);
      g.rotate( Math.PI * rotate/180, height, 0.0);
      g.transform( AffineTransform.getScaleInstance( height/width,
      		width/height));
    }

    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(24, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));
    g.draw( shapes[0]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(24, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));
    g.draw( shapes[1]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(10, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));
    g.draw( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(10, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor));
    g.draw( shapes[3]);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}








