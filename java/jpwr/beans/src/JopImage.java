package jpwr.beans;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import jpwr.jop.*;
import javax.swing.Timer;
import java.awt.event.*;

public class JopImage extends JComponent implements ActionListener {
  Dimension size;
  Object root;
  JopSession session;
  Timer timer = new Timer(500, this);
  public JopImage()
  {
    try {
      jbInit();
    }
    catch(Exception e) {
      e.printStackTrace();
    }
  }

  private void jbInit() throws Exception {
    size = new Dimension( 60, 30);
    timer.start();
  }

  public void actionPerformed(ActionEvent e) {
    boolean root_found = false;
    Container parent = getParent();
    while ( parent != null) {
      if ( parent instanceof JopFrame) {
        root = parent;
        session = ((JopFrame)root).session;
        root_found = true;
        break;
      }
      parent = parent.getParent();
    }
    if ( !root_found) {
      parent = getParent();
      while ( parent != null) {
        if ( parent instanceof JopApplet) {
          root = parent;
	  session = ((JopApplet)root).session;
          root_found = true;
          break;
        }
        parent = parent.getParent();
      }
    }
    if ( root_found) {
      timer.stop();
      timer = null;    
    }
  }
  String image = "test.gif";
  String imageOld;
  Image mImage = null;
  int original_width = 60;
  int original_height = 30;
  float widthOld = 0;
  float heightOld = 0;

  public String getImage() {
    return image;
  }
  public void setImage( String image) {
   this.image = image;
  }

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

  public boolean imageUpdate( Image img, int infoflags, int x, int y,
  	int width, int height) {
    if (( infoflags & ImageObserver.ERROR) != 0) {
      System.out.println("Error loading image");
    }
    if ((infoflags & ImageObserver.WIDTH) != 0 &&
        (infoflags & ImageObserver.HEIGHT) != 0) {
      int imageWidth = mImage.getWidth(this);
      int imageHeight = mImage.getHeight(this);
    }
    if ((infoflags & ImageObserver.SOMEBITS) != 0) {
      repaint();
    }
    if ((infoflags & ImageObserver.ALLBITS) != 0) {
      int imageWidth = mImage.getWidth(this);
      int imageHeight = mImage.getHeight(this);
      repaint();
      return false;
    }
    return true;
  }
  public void paintComponent(Graphics g1) {    
   if ( root == null) {
      Container parent = getParent();
      while ( parent != null) {
	if ( parent instanceof JopFrame) {
	  root = parent;
	  session = ((JopFrame)root).session;  
	  break;
	}
        else if (  parent instanceof JopApplet) {
          root = parent;
	  session = ((JopApplet)root).session;
          break;
        }
        parent = parent.getParent();
      }
    }
    if ( root == null)
      return;
    Graphics2D g = (Graphics2D) g1;
    if ( (mImage == null && image != null) || 
	 (image != null && !image.equals(imageOld))) {
//      mImage = Toolkit.getDefaultToolkit().getImage( image);
      mImage = JopSpider.getImage( session, image);
      imageOld = new String(image);
    }
    g.drawImage( mImage, 0, 0, this);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
