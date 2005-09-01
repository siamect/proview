/* 
 * Proview   $Id: JopCon.java,v 1.2 2005-09-01 14:57:49 claes Exp $
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
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import jpwr.jop.*;

public class JopCon extends JComponent {
  Dimension size;
  public JopCon()
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
  }
  int borderColor = 9999;
  int lineWidth = 1;
  public void setBorderColor( int borderColor) {
    this.borderColor = borderColor;
  }
  public int getBorderColor() {
    return borderColor;
  }
  public void setLineWidth( int lineWidth) {
    this.lineWidth = lineWidth;
  }
  public int getLineWidth() {
    return lineWidth;
  }
  int original_width = 60;
  int original_height = 30;
  public double rotate;
  public void setRotate( double rotate) { this.rotate = rotate;}
  public double getRotate() { return rotate;}
  Shape[] shapes = new Shape[] { null, null, null,};
  float widthOld = 0;
  float heightOld = 0;
  double rotateOld = 0;
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
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);

    float width = getWidth();
    float height = getHeight();
    if ( shapes[0] == null) {
      shapes[0] = new Line2D.Float();
      shapes[1] = new Line2D.Float();
      shapes[2] = new Line2D.Float();
    }
    if ( width != widthOld || height != heightOld || rotate != rotateOld) {
      if ( 45.0 <= rotate && rotate < 135.0) {
        if ( width < 20) {
          ((Line2D.Float)shapes[0]).setLine(width/2, 0F, width/2, height);
          ((Line2D.Float)shapes[1]).setLine(width/2, 0F, width/2, 0F);
          ((Line2D.Float)shapes[2]).setLine(width/2, 0F, width/2, 0F);
        }
        else {
          ((Line2D.Float)shapes[0]).setLine(10F, 0F, 10F, height/2);
          ((Line2D.Float)shapes[1]).setLine(10F, height/2, width - 10F, height/2);
          ((Line2D.Float)shapes[2]).setLine(width - 10F, height/2, width - 10F, height);
        }
      }
      else if ( 135.0 <= rotate && rotate < 225.0) {
        if ( height < 20) {
          ((Line2D.Float)shapes[0]).setLine(0F, height/2, width, height/2);
          ((Line2D.Float)shapes[1]).setLine(0F, height/2, 0F, height/2);
          ((Line2D.Float)shapes[2]).setLine(0F, height/2, 0F, height/2);
        }
        else {
          ((Line2D.Float)shapes[0]).setLine(0F, 10F, width/2, 10F);
          ((Line2D.Float)shapes[1]).setLine(width/2, 10F, width/2, height - 10F);
          ((Line2D.Float)shapes[2]).setLine(width/2, height - 10F, width, height - 10F);
        }
      }
      else if ( 225.0 <= rotate && rotate < 315.0) {
        if ( width < 20) {
          ((Line2D.Float)shapes[0]).setLine(width/2, 0F, width/2, height);
          ((Line2D.Float)shapes[1]).setLine(width/2, 0F, width/2, 0F);
          ((Line2D.Float)shapes[2]).setLine(width/2, 0F, width/2, 0F);
        }
        else {
          ((Line2D.Float)shapes[0]).setLine(10F, 0F, 10F, height/2);
          ((Line2D.Float)shapes[1]).setLine(width - 10F, height/2, 10F, height/2);
          ((Line2D.Float)shapes[2]).setLine(width - 10F, height/2, width - 10F, height);
        }
      }
      else {
        if ( height < 20) {
          ((Line2D.Float)shapes[0]).setLine(0F, height/2, width, height/2);
          ((Line2D.Float)shapes[1]).setLine(0F, height/2, 0F, height/2);
          ((Line2D.Float)shapes[2]).setLine(0F, height/2, 0F, height/2);
        }
        else {
          ((Line2D.Float)shapes[0]).setLine(0F, height - 10F, width/2, height - 10F);
          ((Line2D.Float)shapes[1]).setLine(width/2, height - 10F, width/2, 10F);
          ((Line2D.Float)shapes[2]).setLine(width/2, 10F, width, 10F);
        }
      }

      widthOld = width;
      heightOld = height;
      rotateOld = rotate;
    }

    g.setStroke( new BasicStroke((float)lineWidth));
    g.setColor(GeColor.getColor(0, borderColor));
    g.draw( shapes[0]);
    g.draw( shapes[1]);
    g.draw( shapes[2]);
  }
  public Dimension getPreferredSize() { return size;}
  public Dimension getMinimumSize() { return size;}
}
