/* 
 * Proview   $Id: JopSlider.java,v 1.2 2005-09-01 14:57:50 claes Exp $
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
import java.awt.event.*;
import jpwr.rt.*;

public class JopSlider extends JComponent implements JopDynamic, ActionListener{
  Timer timer = new Timer(500, this);
  JopEngine en;
  Point offset = new Point();
  public JopSlider()
  {
    try {
      jbInit();
    }
    catch(Exception e) {
      e.printStackTrace();
    }
  }

  private void jbInit() throws Exception {
    timer.start();
    this.addMouseListener(new MouseAdapter() {
      public void mouseClicked(MouseEvent e) {
      }
      public void mousePressed(MouseEvent e) {
        moveActive = true;
//        colorInverse = 1;
//	repaint();
        offset = e.getPoint();
      }
      public void mouseReleased(MouseEvent e) {
        moveActive = false;
//        colorInverse = 0;
//	repaint();
      }
    });

    this.addMouseMotionListener( new MouseMotionAdapter() {
      public void mouseDragged(MouseEvent e) {
	float value;
	PwrtStatus sts;
	Point new_loc = new Point();
        Point ePoint = e.getPoint();
//	System.out.println("Mouse dragged: " + thisPoint.x + ", " +
//	   thisPoint.y);
        Point loc = getLocation();
        switch ( direction) {
	  case Ge.DIRECTION_LEFT:
	    new_loc.x = loc.x + ePoint.x - offset.x;
            new_loc.y = loc.y;
            if ( new_loc.x > maxPos)
	      new_loc.x = (int) maxPos;
	    if ( new_loc.x < minPos)
	      new_loc.x = (int) minPos;
            value = (float)((maxPos - new_loc.x) / (maxPos - minPos) *
	    	(maxValue - minValue) + minValue); 
            break;
	  case Ge.DIRECTION_RIGHT:
	    new_loc.x = loc.x + ePoint.x - offset.x;
	    new_loc.y = loc.y;
            if ( new_loc.x > maxPos)
	      new_loc.x = (int) maxPos;
	    if ( new_loc.x < minPos)
	      new_loc.x = (int) minPos;
            value = (float)((new_loc.x - minPos) / (maxPos - minPos) * 
	    	(maxValue - minValue) + minValue);
            break;
	  case Ge.DIRECTION_DOWN:
	    new_loc.y = loc.y + ePoint.y - offset.y;
	    new_loc.x = loc.x;
            if ( new_loc.y > maxPos)
	      new_loc.y = (int) maxPos;
	    if ( new_loc.y < minPos)
	      new_loc.y = (int) minPos;
	    value = (float)((new_loc.y - minPos) / (maxPos - minPos) *
	    	(maxValue - minValue) + minValue);
            break;
	  default:
	    new_loc.y = loc.y + ePoint.y - offset.y;
	    new_loc.x = loc.x;
            if ( new_loc.y > maxPos)
	      new_loc.y = (int) maxPos;
	    if ( new_loc.y < minPos)
	      new_loc.y = (int) minPos;
            value = (float)((maxPos - new_loc.y) / (maxPos - minPos) *
	    	(maxValue - minValue) + minValue);
        }
        setLocation(new_loc);
	
        sts = en.gdh.setObjectInfo( pwrAttribute, value);
        if ( sts.evenSts())
          System.out.println( "setObjectInfoError " + sts);        
      }
      public void mouseMoved(MouseEvent e) {}
    });
  }
  public void actionPerformed(ActionEvent e) {
    boolean engine_found = false;
    Container parent = getParent();
    while ( parent != null) {
      if ( parent instanceof JopFrame) {
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
  String pwrAttribute = new String();
  public void setPwrAttribute( String pwrAttribute) { this.pwrAttribute = pwrAttribute;}
  public String getPwrAttribute() { return pwrAttribute;}
  float minValue = 0;
  float maxValue = 100;
  double minPos = 0;
  double maxPos = 100;
  int direction = Ge.DIRECTION_UP;
  public void setMinValue( float minValue) {
    this.minValue = minValue;
  }
  public float getMinValue() {
    return minValue;
  }
  public void setMaxValue( float maxValue) {
    this.maxValue = maxValue;
  }
  public float getMaxValue() {
    return maxValue;
  }
  public void setMinPos( float minPos) {
    this.minPos = minPos;
  }
  public double getMinPos() {
    return minPos;
  }
  public void setMaxPos( float maxPos) {
    this.maxPos = maxPos;
  }
  public double getMaxPos() {
    return maxPos;
  }
  public void setDirection( int direction) {
    this.direction = direction;
  }
  public int getDirection() {
    return direction;
  }
  boolean moveActive = false;
  float valueOld;
  float value;
  boolean firstScan = true;
  GdhrRefObjectInfo retValue = null;
  boolean attrFound = false;
  public void dynamicOpen() {
    if ( pwrAttribute.compareTo("") != 0) {
      retValue = en.gdh.refObjectInfo( pwrAttribute);
      if ( retValue.evenSts())
        System.out.println( "refObjectInfoError retColor");
      else
        attrFound = true;
    }
  }
  public void dynamicClose() {
    if ( attrFound)
      en.gdh.unrefObjectInfo( retValue.refid);
  }
  public void dynamicUpdate( boolean animationOnly) {
    if ( animationOnly)
      return;
    if ( attrFound && !moveActive) {
      value = en.gdh.getObjectRefInfoFloat( retValue.id);
      if ( valueOld != value || firstScan) {
        Point loc = getLocation();
        int pos;
	
	switch ( direction) {
	  case Ge.DIRECTION_LEFT:
            pos = (int)((maxValue - value)/(maxValue - minValue) *
		(maxPos - minPos) + minPos);
            loc.x = pos;
	    break;
	  case Ge.DIRECTION_RIGHT:
            pos = (int)(value /(maxValue - minValue) *
		(maxPos - minPos) + minPos);
            loc.x = pos;
	    break;
	  case Ge.DIRECTION_DOWN:
            pos = (int)((value - minValue)/(maxValue - minValue) *
		(maxPos - minPos) + minPos);
            loc.y = pos;
	    break;
	  default:
            pos = (int)((maxValue - value)/(maxValue - minValue) *
		(maxPos - minPos) + minPos);
	    loc.y = pos;
	}
	setLocation( loc);
        repaint();
      }
      valueOld = value;
    }
    if ( firstScan)
      firstScan = false;
  }
}
