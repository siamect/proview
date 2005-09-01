/* 
 * Proview   $Id: AspectRatioListener.java,v 1.2 2005-09-01 14:57:50 claes Exp $
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
import java.awt.event.*;

/* This class is used as a ComponentListener for any Component for which
 the Aspect Ratio is important and needs to be retained. The constructor
needs to know what Component it is applied to and the original size of it.
From the size the aspect ratio is calculated and every time the Container
is resized. The size is changed so that the aspect ratio matches the, 
relatively, smaller one of the new width and height.
syntax : new AspectTarioListener(c)
   or    new AspectRatioListener(c,size)
where c is the component, and size is the original size of it. */

public class AspectRatioListener implements ComponentListener {
	

 
    private double ratio;
    private Dimension previous;
    private Container victim;


    public AspectRatioListener(Container c){
	victim = c;
	previous = (Dimension) c.getSize().clone();
	ratio = 1.0*c.getSize().width/c.getSize().height;
    }
	
    public AspectRatioListener(Container c, Dimension size){
	victim = c;
	previous = (Dimension) size.clone();
	ratio = 1.0*size.width/size.height;
    }
	
    public void componentResized(ComponentEvent e) {

	int width = victim.getWidth();
	int height = victim.getHeight();
	
	// check if the height or width should be adjusted.
	if (!(previous.equals(victim.getSize()))){
	
	    if ((width > previous.width) || 
		(height > previous.height)){
	    
		if ((0.1*height/previous.height)>(0.1*width/previous.width))
	            width = (int) (ratio*height);
		else 
		    height = (int) (width/ratio);	    
	    }
	    else{
		if ((0.1*width/previous.width)<(0.1*height/previous.height))
		    height = (int) (width/ratio);
	        else 
		    width = (int) (ratio*height);	
	    }
	    previous = new Dimension(width,height);
	    victim.setSize(width,height);
	}
	

         	
         
         

    }
    
    public void componentMoved(ComponentEvent e) {
    }

    public void componentShown(ComponentEvent e) {
    }

    public void componentHidden(ComponentEvent e) {
    }

}

