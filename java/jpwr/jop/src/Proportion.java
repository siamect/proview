/* 
 * Proview   $Id: Proportion.java,v 1.2 2005-09-01 14:57:51 claes Exp $
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

/* The Proportion class is used when using the RatioLayout layoutmanager. It 
   makes sure that the object associated with it  keeps its proportions 
   when the frame is resized. */
public class Proportion {
    
    //The ratios in x,y position and width, height
    public double rx,ry,rw,rh;	
    public Dimension previous;
    public boolean firstDraw =true;

    // constructor. area= the size and place of an object. framesize
    //is the size of the frame in which it is placed.
    public Proportion(Rectangle area, Dimension framesize){
	rx= (double) (1.0*area.x/framesize.width);
	ry= (double) (1.0*area.y/framesize.height);
	rw= (double) (1.0*area.width/framesize.width);
	rh= (double) (1.0*area.height/framesize.height);
	previous= new Dimension(1,1);
    }

    // For debuging purposes...
    public String toString(){
	return ""+rx+" "+ry+" "+rw+" "+rh+" "+previous+" "+firstDraw;
    }
}
