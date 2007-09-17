/* 
 * Proview   $Id: JopXYCurve.java,v 1.1 2007-09-17 15:35:28 claes Exp $
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
import java.awt.*;
import java.awt.geom.*;
import java.awt.image.*;
import java.awt.font.*;
import javax.swing.*;
import javax.swing.Timer;
import java.awt.event.*;
import jpwr.rt.*;

public class JopXYCurve extends JopTrend {

  public JopXYCurve( JopSession session)
  {
    super(session);  
  }

  public void dynamicOpen() {
    if ( en.isInstance())
      dd.setInstance( en.getInstance());
    dd.connect();
  }

  public void dynamicClose() {
    dd.disconnect();
  }

  public void dynamicUpdate( boolean animationOnly) {
    dd.scan();
  }

  public void paintComponent(Graphics g1) {
    int i, j;
    Graphics2D g = (Graphics2D) g1;
    float width = getWidth();
    float height = getHeight();
    AffineTransform save = g.getTransform();
    float delta;

    if ( shapes == null) {
      original_width = width;
      original_height = height;
      shapes = new Shape[1];
      shapes[0] = new Rectangle2D.Float(0F, 0F, width, height);
      if ( verticalLines > 0)
      {
        vLines = new Line2D.Float[verticalLines];
        delta = width / ( verticalLines + 1);
	for ( i = 0; i < verticalLines; i++)
	  vLines[i] = new Line2D.Float( delta * (i+1), 0F, delta * (i+1), height);
      }
      if ( horizontalLines > 0)
      {
        hLines = new Line2D.Float[horizontalLines];
        delta = height / ( horizontalLines + 1);
	for ( i = 0; i < horizontalLines; i++)
	  hLines[i] = new Line2D.Float( 0F, delta * (i+1), width, delta * (i+1));
      }
    }
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
    else {
      g.transform( AffineTransform.getScaleInstance( width/original_width,
						   height/original_height));
    }

    if ( drawFill == 1) {
      g.setColor(GeColor.getColor(22, fillColor));
      g.fill( shapes[0]);
    }

    AffineTransform save2 = g.getTransform();
    g.setTransform(save);
    Rectangle r = getBounds(null);
    if ( dd.elements.length > 0 && dd.elements[0] instanceof GeDynXYCurve &&
	 ((GeDynXYCurve)dd.elements[0]).fill_curve == 1) {
      for ( j = dd.elements.length - 1; j >= 0; j--) {
	if ( dd.elements[j] instanceof GeDynXYCurve) {
	  GeDynXYCurve elem = (GeDynXYCurve) dd.elements[j];
	  if ( elem.noOfPoints > 0 && elem.curveX != null && elem.curveY != null) {
	    int[] intY = new int[elem.noOfPoints+3];
	    int[] intX = new int[elem.noOfPoints+3];
	    for ( i = 0; i < elem.noOfPoints; i++) {
	      intY[i] = (int)(r.height - elem.curveY[i] * r.height);
	      intX[i] = (int)(elem.curveX[i] * r.width);
	    }
	    intY[elem.noOfPoints] = (int)(r.height);
	    intX[elem.noOfPoints] = intX[elem.noOfPoints-1];
	    intY[elem.noOfPoints+1] = (int)(r.height);
	    intX[elem.noOfPoints+1] = intX[0];
	    intY[elem.noOfPoints+2] = intY[0];
	    intX[elem.noOfPoints+2] = intX[0];

	    Polygon polygon = new Polygon( intX, intY, elem.noOfPoints+3);
	    g.setColor(GeColor.getColor(0, elem.fill_color));
	    g.fill( polygon);
	  }
	}
      }
    }
    g.setTransform(save2);

    g.setStroke( new BasicStroke((float)lineWidth));
    g.setColor(GeColor.getColor(0, borderColor));
    for ( i = 0; i < verticalLines; i++)
      g.draw( vLines[i]);
    for ( i = 0; i < horizontalLines; i++)
      g.draw( hLines[i]);

    g.setStroke( new BasicStroke((float)lineWidth));
    if ( drawBorder == 1 || drawFill == 0) {
      g.setColor(GeColor.getColor(0, borderColor));
      g.draw( shapes[0]);
    }

    g.setTransform(save);

    for ( j = 0; j < dd.elements.length; j++) {
      if ( dd.elements[j] instanceof GeDynXYCurve) {
        GeDynXYCurve elem = (GeDynXYCurve) dd.elements[j];

	if ( elem.curveX != null && elem.curveY != null) {
	  float pathX, pathY;
	  GeneralPath path = new GeneralPath(GeneralPath.WIND_EVEN_ODD);

	  path.reset();
	  pathY = r.height - elem.curveY[0] * r.height;
	  pathX = elem.curveX[0] * r.width;
	  path.moveTo( pathX, pathY);
	  int psize = elem.noOfPoints < elem.curveX.length ? elem.noOfPoints : elem.curveX.length;
	  psize = psize < elem.curveY.length ? psize : elem.curveY.length;
	  for ( i = 1; i < psize; i++) {
	    pathY = r.height - elem.curveY[i] * r.height;
	    pathX = elem.curveX[i] * r.width;
	    path.lineTo( pathX, pathY);
	  }

	  g.setColor(GeColor.getColor(0, elem.curve_color));
	  g.draw( path);
	}
      }
    }
  }
}
