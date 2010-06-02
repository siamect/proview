/* 
 * Proview   $Id$
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
import java.awt.event.*;

public class GeGradient {
    public static final int eGradient_No		= 0;
    public static final int eGradient_HorizontalUp	= 1;
    public static final int eGradient_HorizontalDown	= 2;
    public static final int eGradient_HorizontalTube1 	= 3;
    public static final int eGradient_HorizontalTube2	= 4;
    public static final int eGradient_VerticalLeft	= 5;
    public static final int eGradient_VerticalRight	= 6;
    public static final int eGradient_VerticalTube1	= 7;
    public static final int eGradient_VerticalTube2	= 8;
    public static final int eGradient_DiagonalUpperLeft = 9;
    public static final int eGradient_DiagonalLowerLeft = 10;
    public static final int eGradient_DiagonalUpperRight = 11;
    public static final int eGradient_DiagonalLowerRight = 12;
    public static final int eGradient_DiagonalUpTube 	= 13;
    public static final int eGradient_DiagonalDownTube 	= 14;
    public static final int eGradient_Globe 		= 15;
    public static final int eGradient_RadialCenter 	= 16;
    public static final int eGradient_RadialUpperLeft 	= 17;
    public static final int eGradient_RadialLowerLeft	= 18;
    public static final int eGradient_RadialUpperRight 	= 19;
    public static final int eGradient_RadialLowerRight	= 20;


    static public void paint( Graphics2D g, int gradient, int gradientContrast1, int gradientContrast2,
			      float x, float y, float w, float h,
			      boolean fixcolor, int fillDrawtype, int colorTone, int colorShift,
			      int colorIntensity, int colorInverse, int fillColor, 
			      boolean dimmed) {
	float gx0 = 0;
	float gy0 = 0;
	float gx1 = 0; 
	float gy1 = 0;
	float gr = 0;
	
	int fcolor = GeColor.getDrawtype(fillDrawtype, colorTone,colorShift,colorIntensity, 
					 0, colorInverse, fillColor, dimmed);
	if ( fcolor == GeColor.COLOR_RED || fcolor == GeColor.COLOR_LINERED || 
	     fcolor == GeColor.COLOR_YELLOW || fcolor == GeColor.COLOR_6) {
	  g.setColor( GeColor.getColor(fcolor));
	  return;
	}

	switch ( gradient) {
	case eGradient_HorizontalUp:
	    gx0 = x;
	    gy0 = y;
	    gx1 = x;
	    gy1 = y + h;
	    break;
	case eGradient_HorizontalDown:
	    gx0 = x;
	    gy0 = y + h;
	    gx1 = x;
	    gy1 = y;
	    break;
	case eGradient_HorizontalTube1:
	    gx0 = x;
	    gy0 = y + 0.5F * h;
	    gx1 = x;
	    gy1 = y;
	    break;
	case eGradient_HorizontalTube2:
	    gx0 = x;
	    gy0 = y + 0.3F * h;
	    gx1 = x;
	    gy1 = y + h;
	    break;
	case eGradient_VerticalLeft:
	    gx0 = x;
	    gy0 = y;
	    gx1 = x + w;
	    gy1 = y;
	    break;
	case eGradient_VerticalRight:
	    gx0 = x + w;
	    gy0 = y;
	    gx1 = x;
	    gy1 = y;
	    break;
	case eGradient_VerticalTube1:
	    gx0 = x + 0.5F * w;
	    gy0 = y;
	    gx1 = x;
	    gy1 = y;
	    break;
	case eGradient_VerticalTube2:
	    gx0 = x + 0.3F * w;
	    gy0 = y;
	    gx1 = x + w;
	    gy1 = y;
	    break;
	case eGradient_DiagonalUpperLeft:
	    gx0 = x;
	    gy0 = y;
	    gx1 = x + w;
	    gy1 = y + h;
	    break;
	case eGradient_DiagonalLowerLeft:
	    gx0 = x;
	    gy0 = y + h;
	    gx1 = x + w;
	    gy1 = y;
	    break;
	case eGradient_DiagonalUpperRight:
	    gx0 = x + w;
	    gy0 = y;
	    gx1 = x;
	    gy1 = y + h;
	    break;
	case eGradient_DiagonalLowerRight:
	    gx0 = x + w;
	    gy0 = y + h;
	    gx1 = x;
	    gy1 = y;
	    break;
	case eGradient_DiagonalUpTube:
	    gx0 = x + 0.5F * w;
	    gy0 = y + 0.5F * h;
	    gx1 = x + w;
	    gy1 = y + h;
	    break;
	case eGradient_DiagonalDownTube:
	    gx0 = x + 0.5F * w;
	    gy0 = y + 0.5F * h;
	    gx1 = x;
	    gy1 = y + h;
	    break;
	case eGradient_Globe:
	    gx0 = x + 0.3F * w;
	    gy0 = y + 0.3F * h;
	    gx1 = x + w;
	    gy1 = y + h;
	    gr = (float) Math.sqrt( w*w/4+h*h/4);
	    break;
	case eGradient_RadialCenter:
	    gx0 = x + 0.5F * w;
	    gy0 = y + 0.5F * h;
	    gx1 = x + w;
	    gy1 = y + h;
	    gr = (float) Math.sqrt( w*w/4F+h*h/4F);
	    break;
	case eGradient_RadialUpperLeft:
	    gx0 = x;
	    gy0 = y;
	    gx1 = x + w;
	    gy1 = y + h;
	    gr = (float) Math.sqrt( w*w+h*h);
	    break;
	case eGradient_RadialLowerLeft:
	    gx0 = x;
	    gy0 = y + h;
	    gx1 = x + w;
	    gy1 = y;
	    gr = (float) Math.sqrt( w*w+h*h);
	    break;
	case eGradient_RadialUpperRight: 
	    gx0 = x + w;
	    gy0 = y;
	    gx1 = x;
	    gy1 = y + h;
	    gr = (float) Math.sqrt( w*w+h*h);
	    break;
	case eGradient_RadialLowerRight:
	    gx0 = x + w;
	    gy0 = y + h;
	    gx1 = x;
	    gy1 = y;
	    gr = (float) Math.sqrt( w*w+h*h);
	    break;
	default: ;
	}

	switch ( gradient) {
	case eGradient_HorizontalUp:
	case eGradient_HorizontalDown:
	case eGradient_HorizontalTube1:
	case eGradient_HorizontalTube2:
	case eGradient_VerticalLeft:
	case eGradient_VerticalRight:
	case eGradient_VerticalTube1:
	case eGradient_VerticalTube2:
	case eGradient_DiagonalUpperLeft:
	case eGradient_DiagonalLowerLeft:
	case eGradient_DiagonalUpperRight:
	case eGradient_DiagonalLowerRight:
	case eGradient_DiagonalUpTube:
	case eGradient_DiagonalDownTube:	    
	    if ( fixcolor) {
		GradientPaint gp = new GradientPaint( gx0, gy0,
					GeColor.getColor(fillDrawtype, GeColor.NO_TONE, 0, 0, gradientContrast2, colorInverse, GeColor.NO_COLOR, dimmed),
					gx1, gy1 ,
					GeColor.getColor(fillDrawtype, GeColor.NO_TONE,0,0,gradientContrast1, colorInverse, GeColor.NO_COLOR, dimmed),true);
		g.setPaint(gp);
	    }
	    else {
		GradientPaint gp = new GradientPaint( gx0, gy0,
					GeColor.getColor(fillDrawtype ,colorTone,colorShift,colorIntensity, gradientContrast2, colorInverse, fillColor, dimmed),
					gx1, gy1, 
					GeColor.getColor(fillDrawtype, colorTone,colorShift,colorIntensity, gradientContrast1, colorInverse, fillColor, dimmed),true);
		g.setPaint(gp);
	    }
	    break;
	case eGradient_Globe:
	case eGradient_RadialCenter:
	case eGradient_RadialUpperLeft:
	case eGradient_RadialLowerLeft:
	case eGradient_RadialUpperRight: 
	case eGradient_RadialLowerRight:
	    if ( fixcolor) {
		RadialGradientPaint gp = new RadialGradientPaint( new Point2D.Float(gx0,gy0), gr, new float[] {0F,1F}, new Color[] { GeColor.getColor(fillDrawtype, GeColor.NO_TONE,0,0,gradientContrast2, colorInverse, GeColor.NO_COLOR, dimmed),GeColor.getColor(fillDrawtype,GeColor.NO_TONE,0,0,gradientContrast1, colorInverse, GeColor.NO_COLOR, dimmed)});
		g.setPaint(gp);
	    }
	    else {
		RadialGradientPaint gp = new RadialGradientPaint( new Point2D.Float(gx0,gy0),gr, new float[] {0F,1F}, new Color[] { GeColor.getColor(fillDrawtype,colorTone,colorShift,colorIntensity,gradientContrast2, colorInverse, fillColor, dimmed),GeColor.getColor(fillDrawtype,colorTone,colorShift,colorIntensity,gradientContrast1, colorInverse, fillColor, dimmed)});
		g.setPaint(gp);
	    }
	    break;
	default: ;
	}
    }	    
}
