package jpwr.jop;
import java.awt.*;
import java.awt.event.*;
import java.net.URL;
import java.awt.geom.*;
import java.awt.image.*;
import javax.swing.*;

public class JopUtilities {
  private static final Component sComponent = new Component() {};
  private static final MediaTracker sTracker = new MediaTracker( sComponent);
  private static int sID = 0;

  public static boolean waitForImage( Image image) {
	int id;
	synchronized( sComponent) { 
          id = sID++; 
        }
	sTracker.addImage( image, id);
	try { 
          sTracker.waitForID( id);
        }
	catch ( InterruptedException ie) {
	    return false;
	}

	if ( sTracker.isErrorID( id)) return false;
	return true;
  }

  public static BufferedImage makeBufferedImage( Image image) {
	if ( waitForImage(image) == false)
	    return null;
	BufferedImage bufferedImage = new BufferedImage( image.getWidth(null),
		  image.getHeight(null), BufferedImage.TYPE_INT_RGB);
	Graphics2D g2 = bufferedImage.createGraphics();

	g2.drawImage( image, null, null);
	return bufferedImage;
  }
  public static BufferedImage makeTranspBufferedImage( Image image) {
	if ( waitForImage(image) == false)
	    return null;
	BufferedImage bufferedImage = new BufferedImage( image.getWidth(null),
		  image.getHeight(null), BufferedImage.TYPE_INT_ARGB);
	Graphics2D g2 = bufferedImage.createGraphics();

	g2.drawImage( image, null, null);

        // Make image transparent
        int[] TRANSPARENT_COLOR = {255,255,255,0};
        WritableRaster raster = bufferedImage.getRaster();
        int[] pixel = new int[4];
        for ( int y = 0; y < bufferedImage.getHeight(); y++)
          for ( int x = 0; x < bufferedImage.getWidth(); x++) {
            pixel = raster.getPixel( x, y, pixel);
            if ( pixel[0] == TRANSPARENT_COLOR[0] &&
                 pixel[1] == TRANSPARENT_COLOR[1] &&
                 pixel[2] == TRANSPARENT_COLOR[2]) {
              raster.setPixel( x, y, TRANSPARENT_COLOR);
            }
          }

	return bufferedImage;
  }

}















