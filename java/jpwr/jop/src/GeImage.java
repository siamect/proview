
package jpwr.jop;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import javax.swing.Timer;
import java.awt.event.*;

public class GeImage extends JComponent implements ActionListener {
  Dimension size;
  Object root;
  JopSession session;
  Timer timer = new Timer(500, this);
  public GeImage()
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

  public void setSession( JopSession session) {
    this.session = session;
  }

  public void actionPerformed(ActionEvent e) {
    boolean root_found = false;
    Container parent = getParent();
    while ( parent != null) {
      if ( parent instanceof JopFrame) {
        root = parent;
        root_found = true;
	session = ((JopFrame)root).session;
        repaint(); 
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
  String image;
  Image originalImage = null;
  public BufferedImage mImage = null;
  BufferedImage mImageSrc = null;
  int original_width = 60;
  int original_height = 30;
  int transparent;
  double rotate;
  public int colorTone = 0;
  public int colorShift = 0;
  public int colorBrightness = 0;
  public int colorIntensity = 0;
  public int colorInverse = 0;

  public void setColorTone( int colorTone) {
    this.colorTone = colorTone;
  }
  public int getColorTone() {
    return colorTone;
  }
  public void setColorShift( int colorShift) {
    this.colorShift = colorShift;
  }
  public int getColorShift() {
    return colorShift;
  }
  public void setColorBrightness( int colorBrightness) {
    this.colorBrightness = colorBrightness;
  }
  public int getColorBrightness() {
    return colorBrightness;
  }
  public void setColorIntensity( int colorIntensity) {
    this.colorIntensity = colorIntensity;
  }
  public int getColorIntensity() {
    return colorIntensity;
  }

  public String getImage() {
    return image;
  }
  public void setImage( String image) {
   this.image = image;
  }
  public void setRotate( double rotate) {
    if ( rotate % 360 == 0)
      this.rotate = 0; 
    else if ( rotate < 0)
      this.rotate = rotate % 360 + 360;
    else
      this.rotate = rotate % 360;
  }
  public double getRotate() { return rotate;}

  public void setTransparent( int transparent) {
    this.transparent = transparent;
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
    if ( session == null) {
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
    if ( session == null)
      return;

    Graphics2D g = (Graphics2D) g1;
    AffineTransform gTransform = g.getTransform();

    float width = getWidth();
    float height = getHeight();

    if ( mImage == null && image != null) {

      if ( session == null)
        return; 
      originalImage = JopSpider.getImage( session, image);
      if ( originalImage == null) return;
    }
    {
      if ( transparent == 1)
        mImageSrc = JopUtilities.makeTranspBufferedImage( originalImage);
      else
        mImageSrc = JopUtilities.makeBufferedImage( originalImage);

      if ( mImageSrc == null) return;

      if ( rotate != 0) {
        RenderingHints rh = new RenderingHints( RenderingHints.KEY_INTERPOLATION,
			  RenderingHints.VALUE_INTERPOLATION_BILINEAR);

        if ( 45.0 <= rotate && rotate < 135.0) {
          AffineTransform at = AffineTransform.getTranslateInstance( 	  
		     getWidth() ,0);
	  at.rotate( rotate / 180 * Math.PI, 0, 0);
	  at.scale(
	       1.0 * getHeight() / mImageSrc.getWidth(this),
               1.0 * getWidth() / mImageSrc.getHeight(this));
          AffineTransformOp op = new AffineTransformOp( at, rh);
          mImage = op.filter( mImageSrc, null);      
        }
        else if ( 135.0 <= rotate && rotate < 225.0) {
          AffineTransform at = AffineTransform.getTranslateInstance( 	  
		     getWidth() , getHeight());
	  at.rotate( rotate / 180 * Math.PI, 0, 0);
	  at.scale(
	       1.0 * getWidth() / mImageSrc.getWidth(this),
               1.0 * getHeight() / mImageSrc.getHeight(this));
           AffineTransformOp op = new AffineTransformOp( at, rh);
          mImage = op.filter( mImageSrc, null);
        }
        else if ( 225.0 <= rotate && rotate < 315.0) {
          AffineTransform at = AffineTransform.getTranslateInstance( 	  
		     0, getHeight());
	  at.rotate( rotate / 180 * Math.PI, 0, 0);
	  at.scale(
	       1.0 * getHeight() / mImageSrc.getWidth(this),
               1.0 * getWidth() / mImageSrc.getHeight(this));
          AffineTransformOp op = new AffineTransformOp( at, rh);
          mImage = op.filter( mImageSrc, null);      

        }
      }
      else {
        // Scale only

        AffineTransform at = AffineTransform.getScaleInstance( 
	       1.0 * getWidth() / mImageSrc.getWidth(this), 
               1.0 * getHeight() / mImageSrc.getHeight(this));

        AffineTransformOp op = new AffineTransformOp( at, null);
        mImage = op.filter( mImageSrc, null);
      }
    }

    setImageColor();
    g.drawImage( mImage, null, 0, 0);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}

  void setImageColor()
  {
    int           i, j;
    int[]         p;
    float         factor_intens;
    float         factor_light;
    int           value;

    if ( colorIntensity > 0)
      factor_intens = 1f + 0.1f * colorIntensity;
    else
      factor_intens = 1f + 0.1f * colorIntensity;

    if ( colorBrightness > 0)
      factor_light = 1f - 0.1f * colorBrightness;
    else
      factor_light = 1f + 0.1f * colorBrightness;


    if ( !(colorTone == GeColor.COLOR_TONE_NO ||
           colorTone > GeColor.COLOR_TONE_MAX) ||
         colorShift != 0 || colorIntensity != 0 || colorBrightness != 0 ||
         colorInverse != 0) {
      WritableRaster raster = mImage.getRaster();
      int[] pixel = new int[4];
      for ( int y = 0; y < mImage.getHeight(); y++) {
        for ( int x = 0; x < mImage.getWidth(); x++) {
	  pixel = raster.getPixel( x, y, pixel);
          if ( !(colorTone == GeColor.COLOR_TONE_NO ||
                 colorTone > GeColor.COLOR_TONE_MAX)) {
	    p = rgbTone( pixel[0], pixel[1], pixel[2], colorTone);         
            pixel[0] = p[0];
            pixel[1] = p[1];
            pixel[2] = p[2];
          }
          if ( colorShift != 0) {
 	    p = rgbTone( pixel[0], pixel[1], pixel[2], colorTone);         
            pixel[0] = p[0];
            pixel[1] = p[1];
            pixel[2] = p[2];
          }

          if ( colorIntensity != 0) {
            for ( j = 0; j < 3; j++) {
              value = (int)(factor_intens * pixel[j]) - colorIntensity * 25;
              if ( value > 255)
                pixel[j] = 255;
              else if ( value < 0)
                pixel[j] = 0;
              else
                pixel[j] = value;
            }
          }

          if ( colorBrightness != 0) {
            for ( j = 0; j < 3; j++) {
              if ( colorBrightness > 0)
                value = (int)( factor_light * pixel[j]) + colorBrightness * 25;
              else
                value = (int)( factor_light * pixel[j]);
              if ( value > 255)
                pixel[j] = 255;
              else if ( value < 0)
                pixel[j] = 0;
              else
                pixel[j] = value;
            }
          }

          if ( colorInverse == 1) {
            int m = (pixel[0] + pixel[1] + pixel[2]) / 3;
	    for ( j = 0; j < 3; j++) {
	      value = 255 - m + (pixel[j] - m);
              if ( value < 0)
                value = 0;
              if ( value > 255)
                value = 255;
              pixel[j] = value;
            }
	  }

          raster.setPixel( x, y, pixel);
        }
      }
    }
  }

  int[] rgbTone( int x0, int y0, int z0, int tone)
  {
    int a1, b2;
    int tmp, m;
    float ka1, kb1;
    int ka2, kb2;

    m = (x0 + y0 + z0) / 3;

    ka1 = 0.6f; /* 0.6 */
    ka2 = 120; /* 130 */
    kb1 = 0.4f;
    kb2 = 0;  
    if ( m > 75)
      a1 = (int)(m * ka1) + ka2;
    else
      a1 = (int)(m * 1.5);
    if ( a1 > 255)
      a1 = 255;
    b2 = (int)(kb1 * m) + kb2;
    if ( b2 < 0)
      b2 = 0;

    int[] p = new int[3];
    switch ( tone) {
      case GeColor.COLOR_TONE_GRAY:
        p[0] = m;
        p[1] = m;
        p[2] = m;
        break;      
      case GeColor.COLOR_TONE_YELLOW:
        p[0] = a1;
        p[1] = a1;
        p[2] = b2;
        break;
      case GeColor.COLOR_TONE_GOLD:
        tmp = (int)( 1.2 * a1);
        if  (tmp <= 255)
          p[0] = tmp;
        else
          p[0] = 255;
        p[1] = (int)(0.9 * a1);
        p[2] = b2;
        break;
      case GeColor.COLOR_TONE_ORANGE:
        tmp = (int)(1.2 * a1);
        if  (tmp <= 255)
          p[0] = (int)tmp;
        else
          p[0] = 255;
        p[1] = (int)(0.6 * a1);
        p[2] = b2;
        break;
      case GeColor.COLOR_TONE_RED:
        p[0] = a1;
        p[1] = b2;
        p[2]  = b2;
        break;
      case GeColor.COLOR_TONE_MAGENTA:
        p[0] = a1;
        p[1] = b2;
        p[2] = a1;
        break;
      case GeColor.COLOR_TONE_BLUE:
        p[0] = b2;
        p[1] = b2;
        p[2] = a1;
        break;
      case GeColor.COLOR_TONE_SEABLUE:
        p[0] = b2;
        p[1] = a1;
        p[2] = a1;
        break;
      case GeColor.COLOR_TONE_GREEN:
        p[0] = b2;
        p[1] = a1;
        p[2] = b2;
        break;
      case GeColor.COLOR_TONE_DARKGRAY:
        p[0] = (int)(m / 1.3);
        p[1] = (int)(m / 1.3);
        p[2] = (int)(m / 1.3);
        break;
    }
    return p;
  }


  int[] rgbShift( int x0, int y0, int z0, int shift) {
    int x;
    int y;
    int z;
    int d;
    int step;

    shift = -shift;
    shift %= 8;
    if ( shift < 0)
      shift += 8;

    x = x0;
    y = y0;
    z = z0;

    for( ;;) {
      if ( x == y && y == z)
        break;
      if ( x > y && y >= z && x > z) {
        d = x - z;
        step = 6 * d / 10 * shift;

        if ( step <= z + d - y) {
          y += step;
          break;
        }
        step -= z + d - y;
        y = z + d;
        if ( step <= d) {
          x -= step;
	  break;
        }
        x -= d;
        step -= d;
        if ( step <= d) {
          z += step;
	  break;
        }
        z += d;
        step -= d;
        if ( step <= d) {
          y -= step; 
	  break;
        }
        y -= d;
        step -= d;
        if ( step <= d) {
          x += step;
	  break;
        }
        x += d;
        step -= d;
        if ( step <= d) {
          z -= step;
	  break;
        }
        z -= d;
        step -= d;
        if ( step <= d) {
          y += step;
	  break;
        }
        y += d;
        // printf( "Error, shift larger than one lap\n");
        break;
      }
      else if ( x > z && y >= x && y > z) {
        d = y - z;
        step = 6 * d / 10 * shift;

        if ( step <= x - z) {
          x -= step;
          break;
        }
        step -= x - z;
        x = z;
        if ( step <= d) {
          z += step;
	  break;
        }
        z += d;
        step -= d;
        if ( step <= d) {
          y -= step; 
	  break;
        }
        y -= d;
        step -= d;
        if ( step <= d) {
          x += step; 
	  break;
        }
        x += d;
        step -= d;
        if ( step <= d) {
          z -= step;
	  break;
        }
        z -= d;
        step -= d;
        if ( step <= d) {
          y += step;
	  break;
        }
        y += d;
        step -= d;
        if ( step <= d) {
          x -= step;
	  break;
        }
        x -= d;
        // printf( "Error, shift larger than one lap\n");
        break;
      }
      else if ( y > z && z >= x && y > x) {
        d = y - x;
        step = 6 * d / 10 * shift;

        if ( step <= x + d - z) {
          z += step;
          break;
        }
        step -= x + d - z;
        z = x  + d;
        if ( step <= d) {
          y -= step;
	  break;
        }
        y -= d;
        step -= d;
        if ( step <= d) {
          x += step; 
	  break;
        }
        x += d;
        step -= d;
        if ( step <= d) {
          z -= step; 
	  break;
        }
        z -= d;
        step -= d;
        if ( step <= d) {
          y += step;
	  break;
        }
        y += d;
        step -= d;
        if ( step <= d) {
          x -= step;
	  break;
        }
        x -= d;
        step -= d;
        if ( step <= d) {
          z += step;
	  break;
        }
        z += d;
        // printf( "Error, shift larger than one lap\n");
        break;
      }
      else if ( z >= y && y > x && z >= y) {
        d = z - x;
        step = 6 * d / 10 * shift;

        if ( step <= y - x) {
          y -= step;
	  break;
        }
        step -= y - x;
        y = x;
        if ( step <= d) {
          x += step;
	  break;
        }
        x += d;
        step -= d;
        if ( step <= d) {
          z -= step; 
	  break;
        }
        z -= d;
        step -= d;
        if ( step <= d) {
          y += step; 
	  break;
        }
        y += d;
        step -= d;
        if ( step <= d) {
          x -= step;
	  break;
        }
        x -= d;
        step -= d;
        if ( step <= d) {
          z += step;
	  break;
        }
        z += d;
        step -= d;
        if ( step <= d) {
          y -= step;
	  break;
        }
        y -= d;
        // printf( "Error, shift larger than one lap\n");
        break;
      }
      else if ( z > x && x >= y && z > y) {
        d = z - y;
        step = 6 * d / 10 * shift;

        if ( step <= y + d - x) {
          x += step;
          break;
        }
        step -= y + d - x;
        x = y + d;
        if ( step <= d) {
          z -= step;
	  break;
        }
        z -= d;
        step -= d;
        if ( step <= d) {
          y += step; 
	  break;
        }
        y += d;
        step -= d;
        if ( step <= d) {
          x -= step; 
	  break;
        }
        x -= d;
        step -= d;
        if ( step <= d) {
          z += step;
	  break;
        }
        z += d;
        step -= d;
        if ( step <= d) {
          y -= step;
	  break;
        }
        y -= d;
        step -= d;
        if ( step <= d) {
          x += step;
	  break;
        }
        x += d;
        // printf( "Error, shift larger than one lap\n");
        break;
      }
      else /* if ( x >= z && z > y && x > y) */ {
        d = x - y;
        step = 6 * d / 8 * shift;

        if ( step <= z - y) {
          z -= step;
          break;
        }
        step -= z - y;
        z = y;
        if ( step <= d) {
          y += step;
	  break;
        }
        y += d;
        step -= d;
        if ( step <= d) {
          x -= step; 
	  break;
        }
        x -= d;
        step -= d;
        if ( step <= d) {
          z += step; 
  	  break;
        }
        z += d;
        step -= d;
        if ( step <= d) {
          y -= step;
	  break;
        }
        y -= d;
        step -= d;
        if ( step <= d) {
          x += step;
	  break;
        }
        x += d;
        step -= d;
        if ( step <= d) {
          z -= step;
	  break;
        }
        z -= d;
        // printf( "Error, shift larger than one lap\n");
        break;
      }   
    }

    int[] p = new int[3];
    p[0] = x;
    p[1] = y;
    p[2] = z;
    return p;
  }

}






