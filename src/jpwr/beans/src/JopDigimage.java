/* 
 * Proview   $Id: JopDigimage.java,v 1.2 2005-09-01 14:57:49 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

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

public class JopDigimage extends JComponent implements JopDynamic, ActionListener{
  Dimension size;
  Object root;
  JopEngine en;
  JopSession session;
  Timer timer = new Timer(500, this);
  private Image normalImage;
  private Image lowImage;
  private Image currentImage;
  public JopDigimage()
  {
    try {
      jbInit();
    }
    catch(Exception e) {
      e.printStackTrace();
    }
  }

  private void jbInit() throws Exception {
    size = new Dimension( 44, 44);
    timer.start();
  }
  public void actionPerformed(ActionEvent e) {
    boolean engine_found = false;
    Container parent = getParent();
    while ( parent != null) {
      if ( parent instanceof JopFrame) {
        root = parent;
	session = ((JopFrame)root).session;
        en = ((JopFrame)parent).engine;
        if ( !en.isReady())
          break;
        en.add(this);
        engine_found = true;
        break;
      }
      parent = parent.getParent();
    }
    if ( !engine_found) {
      parent = getParent();
      while ( parent != null) {
        if ( parent instanceof JopApplet) {
          root = parent;
	  session = ((JopApplet)root).session;
          en = ((JopApplet)parent).engine;
          if ( !en.isReady())
            break;
          en.add(this);
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
    if ( (normalImage == null && image != null) || 
	 (image != null && !image.equals(imageOld))) {
      normalImage = JopSpider.getImage( session, image);
      imageOld = new String(image);
      int imageWidth = normalImage.getWidth(this);
      int imageHeight = normalImage.getHeight(this);
      if ( valueImage)
        currentImage = normalImage;
    }
    if (lowImage == null && imageLow != null) {
      lowImage = JopSpider.getImage( session, imageLow);
      int imageWidth = lowImage.getWidth(this);
      int imageHeight = lowImage.getHeight(this);
      if ( !valueImage)
        currentImage = lowImage;
    }
    if ( currentImage != null)
      g.drawImage( currentImage, 0, 0, this);
  }
  String image = "test.gif";
  String imageLow = "test.gif";
  public String getImage() {
    return image;
  }
  public void setImage( String image) {
   this.image = image;
  }
  public String getImageLow() {
    return imageLow;
  }
  public void setImageLow( String imageLow) {
   this.imageLow = imageLow;
  }
  private String imageOld;
  private float widthOld = 0;
  private float heightOld = 0;
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
  private String pwrAttribute = new String();
  public void setPwrAttribute( String pwrAttribute) { this.pwrAttribute = pwrAttribute;}
  public String getPwrAttribute() { return pwrAttribute;}
  private boolean valueImage = true;
  private boolean valueImageOld;
  private boolean firstScan = true;
  private GdhrRefObjectInfo retImage = null;
  private boolean imageAttrFound = false;
  public void dynamicOpen() {
    if ( pwrAttribute.compareTo("") != 0) {
      retImage = en.gdh.refObjectInfo( pwrAttribute);
      if ( retImage.evenSts())
        System.out.println( "refObjectInfoError retImage");
      else
        imageAttrFound = true;
    }
  }
  public void dynamicClose() {
    if ( imageAttrFound)
      en.gdh.unrefObjectInfo( retImage.refid);
  }
  public void dynamicUpdate( boolean animationOnly) {
    if ( animationOnly)
      return;
    if ( normalImage == null)
      return;
    if ( imageAttrFound) {
      valueImage = en.gdh.getObjectRefInfoBoolean( retImage.id);
      if ( valueImageOld != valueImage || firstScan) {
        if ( valueImage) {
          currentImage = normalImage;
          repaint();
        }
        else {
          currentImage = lowImage;
          repaint();
        }
      }
      valueImageOld = valueImage;
    }
    if ( firstScan)
      firstScan = false;
  }

  public boolean imageUpdate( Image img, int infoflags, int x, int y,
  	int width, int height) {
    if (( infoflags & ImageObserver.ERROR) != 0) {
      System.out.println("Error loading image");
    }
    if ((infoflags & ImageObserver.WIDTH) != 0 &&
        (infoflags & ImageObserver.HEIGHT) != 0) {
      int imageWidth = img.getWidth(this);
      int imageHeight = img.getHeight(this);
    }
    if ((infoflags & ImageObserver.SOMEBITS) != 0) {
      if ( img == currentImage)
        repaint();
    }
    if ((infoflags & ImageObserver.ALLBITS) != 0) {
      int imageWidth = img.getWidth(this);
      int imageHeight = img.getHeight(this);
      if ( img == currentImage)
        repaint();
      return false;
    }
    return true;
  }
  public Object dynamicGetRoot() {
    return null;
  }
}
