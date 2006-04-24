/* 
 * Proview   $Id: GeFrameThin.java,v 1.6 2006-04-24 13:21:46 claes Exp $
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

package jpwr.jop;
import jpwr.rt.*;
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
public class GeFrameThin extends GeComponent {
  Dimension dsize;
  public GeFrameThin( JopSession session)
  {
    super( session);
    dsize = new Dimension( 144, 84);
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
  float original_width = 0;
  float original_height = 0;
  public void paintComponent(Graphics g1) {
    Graphics2D g = (Graphics2D) g1;
    float width = getWidth();
    float height = getHeight();
    if ( original_width == 0)
      original_width = width;
    if ( original_height == 0)
      original_height = height;
    AffineTransform save = g.getTransform();
    AffineTransform save_tmp;
    g.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);

    if ( shapes[0] == null) {
      shapes[0] = new Line2D.Float(width - 2F, 2F, 2F, 2F);
      shapes[1] = new Line2D.Float(2F, height - 2F, 2F, 2F);
      shapes[2] = new Line2D.Float(width - 2F, height - 2F, 2F, height - 2F);
      shapes[3] = new Line2D.Float(width - 2F, height - 2F, width - 2F, 2F);
    }

    /*
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
    */
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

    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(78, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, false));
    g.draw( shapes[0]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(78, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, false));
    g.draw( shapes[1]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(40, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, false));
    g.draw( shapes[2]);
    g.setStroke( new BasicStroke(1F));
    g.setColor(GeColor.getColor(40, colorTone,
	 colorShift, colorIntensity, colorBrightness, colorInverse, borderColor, false));
    g.draw( shapes[3]);
    g.setTransform(save);
  }
  public Dimension getPreferredSize() { return dsize;}
  public Dimension getMinimumSize() { return dsize;}
}








