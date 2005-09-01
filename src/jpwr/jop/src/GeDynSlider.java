/* 
 * Proview   $Id: GeDynSlider.java,v 1.4 2005-09-01 14:57:50 claes Exp $
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
import java.awt.event.*;
import java.awt.*;
import javax.swing.*;


public class GeDynSlider extends GeDynElem {
  String attribute;
  public int direction;
  double minValue;
  double maxValue;
  double minPos;
  double maxPos;

  boolean attrFound;
  PwrtRefId subid;
  int p;
  public int typeId;
  float oldValueF;
  int oldValueI;
  boolean firstScan = true;
  boolean moveActive = false;
  Point offset = new Point();
  float original_width = 0;
  float original_height = 0;

  public GeDynSlider( GeDyn dyn, String attribute, double minValue, double maxValue,
		      int direction, double minPos, double maxPos) {
    super( dyn, GeDyn.mDynType_No, GeDyn.mActionType_Slider);
    this.attribute = attribute;
    this.minValue = minValue;
    this.maxValue = maxValue;
    this.direction = direction;
    this.minPos = minPos;
    this.maxPos = maxPos;
  }
  public void setMinValue( double minValue) {
    this.minValue = minValue;
  }
  public void setMaxValue( double maxValue) {
    this.maxValue = maxValue;
  }
  public void update() {
    if ( !attrFound || moveActive)
      return;

    switch ( typeId) {
    case Pwr.eType_Int32:
      oldValueI = -10000;
      break;
    default:
      oldValueF = -10000;
    }
  }
  public void connect() {
    String attrName = dyn.getAttrName( attribute);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "Slider: " + attrName);
      else {
	attrFound = true;
	p = ret.id;
	subid = ret.refid;
	typeId = ret.typeId;
      }
    }
  }
  public void disconnect() {
    if ( attrFound)
      dyn.en.gdh.unrefObjectInfo( subid);
  }
  public void scan() {
    if ( !attrFound || moveActive)
      return;

    float width = ((JComponent)dyn.comp).getParent().getWidth();
    float height = ((JComponent)dyn.comp).getParent().getHeight();
    if ( original_width == 0) {
      original_width = width;
      original_height = height;
    }

    float value;
    switch ( typeId) {
    case Pwr.eType_Int32:
      int ivalue = dyn.en.gdh.getObjectRefInfoInt( p);
      if ( oldValueI == ivalue && !firstScan)
	return;
      oldValueI = ivalue;
      value = (float) ivalue;
      break;
    default:
      value = dyn.en.gdh.getObjectRefInfoFloat( p);
      if ( oldValueF == value && !firstScan)
	return;
      oldValueF = value;
    }

    Point loc = ((JComponent)dyn.comp).getLocation();
    int pos;
	
    double minPos;
    double maxPos;
    switch ( direction) {
    case Ge.DIRECTION_RIGHT:
	minPos = this.minPos * width / original_width;
	maxPos = this.maxPos * width / original_width;
	pos = (int)((maxValue - value)/(maxValue - minValue) *
		    (maxPos - minPos) + minPos);
	if ( pos < minPos)
	  pos = (int)minPos;
	if ( pos > maxPos)
	  pos = (int)maxPos;
	loc.x = pos;
	break;
    case Ge.DIRECTION_LEFT:
	minPos = this.minPos * width / original_width;
	maxPos = this.maxPos * width / original_width;
	pos = (int)(value /(maxValue - minValue) *
		    (maxPos - minPos) + minPos);
	if ( pos < minPos)
	  pos = (int)minPos;
	if ( pos > maxPos)
	  pos = (int)maxPos;
	loc.x = pos;
	break;
    case Ge.DIRECTION_UP:
	minPos = this.minPos * height / original_height;
	maxPos = this.maxPos * height / original_height;
	pos = (int)((value - minValue)/(maxValue - minValue) *
		    (maxPos - minPos) + minPos);
	if ( pos < minPos)
	  pos = (int)minPos;
	if ( pos > maxPos)
	  pos = (int)maxPos;
	loc.y = pos;
	break;
    default:
	minPos = this.minPos * height / original_height;
	maxPos = this.maxPos * height / original_height;
	pos = (int)((maxValue - value)/(maxValue - minValue) *
		    (maxPos - minPos) + minPos);
	if ( pos < minPos)
	  pos = (int)minPos;
	if ( pos > maxPos)
	  pos = (int)maxPos;
	loc.y = pos;
    }
    ((JComponent)dyn.comp).setLocation( loc);
    ((JComponent)dyn.comp).repaint();

    if ( firstScan)
      firstScan = false;
  }

  public void action( int eventType, MouseEvent e) {
    switch ( eventType) {
    case GeDyn.eEvent_MB1Down:
      moveActive = true;
      offset = e.getPoint();
      // ((GeComponent)dyn.comp).colorInverse = 1;
      // dyn.repaintNow = true;
      break;
    case GeDyn.eEvent_MB1Up:
      moveActive = false;
      // ((GeComponent)dyn.comp).colorInverse = 0;
      // dyn.repaintNow = true;
      break;
    case GeDyn.eEvent_SliderMoved:
      float value;
      double minPos;
      double maxPos;
      PwrtStatus sts;
      Point new_loc = new Point();
      float width = ((JComponent)dyn.comp).getParent().getWidth();
      float height = ((JComponent)dyn.comp).getParent().getHeight();
      Point ePoint = e.getPoint();
//	    System.out.println("Mouse dragged: " + thisPoint.x + ", " +
//	     thisPoint.y);
      Point loc = ((GeComponent)dyn.comp).getLocation();
      if ( original_width == 0)
        return;
      switch ( direction) {
      case Ge.DIRECTION_RIGHT:
	minPos = this.minPos * width / original_width;
	maxPos = this.maxPos * width / original_width;
	new_loc.x = loc.x + ePoint.x - offset.x;
	new_loc.y = loc.y;
	if ( new_loc.x > maxPos)
	  new_loc.x = (int) maxPos;
	if ( new_loc.x < minPos)
	  new_loc.x = (int) minPos;
	value = (float)((maxPos - new_loc.x) / (maxPos - minPos) *
	        	(maxValue - minValue) + minValue); 
	break;
      case Ge.DIRECTION_LEFT:
	minPos = this.minPos * width / original_width;
	maxPos = this.maxPos * width / original_width;
	new_loc.x = loc.x + ePoint.x - offset.x;
	new_loc.y = loc.y;
	if ( new_loc.x > maxPos)
	  new_loc.x = (int) maxPos;
	if ( new_loc.x < minPos)
	  new_loc.x = (int) minPos;
	value = (float)((new_loc.x - minPos) / (maxPos - minPos) * 
	        	(maxValue - minValue) + minValue);
	break;
      case Ge.DIRECTION_UP:
	minPos = this.minPos * height / original_height;
	maxPos = this.maxPos * height / original_height;
	new_loc.y = loc.y + ePoint.y - offset.y;
	new_loc.x = loc.x;
	if ( new_loc.y > maxPos)
	  new_loc.y = (int) maxPos;
	if ( new_loc.y < minPos)
          new_loc.y = (int) minPos;
	value = (float)((new_loc.y - minPos) / (maxPos - minPos) *
			(maxValue - minValue) + minValue);
	// System.out.println("old_y: " + ePoint.y + " new_y: " + new_loc.y + "v: " + value);
	break;
      default:
	minPos = this.minPos * height / original_height;
	maxPos = this.maxPos * height / original_height;
	  new_loc.y = loc.y + ePoint.y - offset.y;
	  // System.out.println( "loc.y " + loc.y + " eP.y " + ePoint.y + " offset.y " + offset.y + " new_loc.y " + new_loc.y + " maxPos " + maxPos + " minPos " + minPos);
	  new_loc.x = loc.x;
	  if ( new_loc.y > maxPos)
	    new_loc.y = (int) maxPos;
	  if ( new_loc.y < minPos)
	    new_loc.y = (int) minPos;
	  value = (float)((maxPos - new_loc.y) / (maxPos - minPos) *
			  (maxValue - minValue) + minValue);
	  // System.out.println("old_y: " + ePoint.y + " new_y: " + new_loc.y + "v: " + value);
      }
      ((JComponent)dyn.comp).setLocation(new_loc);
	
      String attrName = dyn.getAttrName(attribute);
      switch ( typeId) {
        case Pwr.eType_Int32:	 
	  sts = dyn.en.gdh.setObjectInfo( attrName, (int) value);
	  break;
        default:
	  sts = dyn.en.gdh.setObjectInfo( attrName, value);
      }
      if ( sts.evenSts())
	System.out.println( "GeSlider: " + sts);        
      break;
    }
  }
}













