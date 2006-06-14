/* 
 * Proview   $Id: GeDynSlider.java,v 1.7 2006-06-14 05:09:35 claes Exp $
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
  String minValueAttr;
  String maxValueAttr;
  String insensitiveAttr;
  boolean insensitiveLocalDb = false;

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
  int minValueP;
  int maxValueP;
  int insensitiveP;
  PwrtRefId minValueSubid;
  PwrtRefId maxValueSubid;
  PwrtRefId insensitiveSubid;
  boolean insensitiveInverted;

  public GeDynSlider( GeDyn dyn, String attribute, double minValue, double maxValue,
		      int direction, double minPos, double maxPos, String minValueAttr, 
		      String maxValueAttr, String insensitiveAttr) {
    super( dyn, GeDyn.mDynType_No, GeDyn.mActionType_Slider);
    this.attribute = attribute;
    this.minValue = minValue;
    this.maxValue = maxValue;
    this.direction = direction;
    this.minPos = minPos;
    this.maxPos = maxPos;
    this.minValueAttr = minValueAttr;
    this.maxValueAttr = maxValueAttr;
    this.insensitiveAttr = insensitiveAttr;
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
    minValueP = 0;
    if ( minValueAttr != null) {
      attrName = dyn.getAttrName( minValueAttr);
      if ( attrName.compareTo("") != 0) {
	GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
	if ( ret.evenSts())
	  System.out.println( "Slider: " + attrName);
	else {
	  minValueP = ret.id;
	  minValueSubid = ret.refid;
	}
      }
    }
    maxValueP = 0;
    if ( maxValueAttr != null) {
      attrName = dyn.getAttrName( maxValueAttr);
      if ( attrName.compareTo("") != 0) {
	GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
	if ( ret.evenSts())
	  System.out.println( "Slider: " + attrName);
	else {
	  maxValueP = ret.id;
	  maxValueSubid = ret.refid;
	}
      }
    }
    insensitiveP = 0;
    if ( insensitiveAttr != null) {
      attrName = dyn.getAttrName( insensitiveAttr);
      if ( attrName.compareTo("") != 0) {
	GdhrRefObjectInfo ret;
	insensitiveLocalDb = dyn.isLocalDb( attrName);
	if ( !insensitiveLocalDb)
	  ret = dyn.en.gdh.refObjectInfo( attrName);
	else
	  ret = dyn.en.ldb.refObjectInfo( dyn.comp.dynamicGetRoot(), attrName);
	if ( ret.evenSts())
	  System.out.println( "Slider: " + attrName);
	else {
	  insensitiveP = ret.id;
	  insensitiveSubid = ret.refid;
	  insensitiveInverted = GeDyndata.getAttrInverted( insensitiveAttr);
	}
      }
    }
  }
  public void disconnect() {
    if ( attrFound)
      dyn.en.gdh.unrefObjectInfo( subid);
    if ( minValueP != 0)
      dyn.en.gdh.unrefObjectInfo( minValueSubid);
    if ( maxValueP != 0)
      dyn.en.gdh.unrefObjectInfo( maxValueSubid);
    if ( insensitiveP != 0 && !insensitiveLocalDb)
      dyn.en.gdh.unrefObjectInfo( insensitiveSubid);
  }
  public void scan() {
    if ( !attrFound || moveActive)
      return;

    if ( minValueP != 0 && maxValueP != 0) {
      double minVal = dyn.en.gdh.getObjectRefInfoFloat( minValueP);
      double maxVal = dyn.en.gdh.getObjectRefInfoFloat( maxValueP);
      if ( (minVal != minValue || maxVal != maxValue) && maxVal != minVal) {
	minValue = minVal;
	maxValue = maxVal;
	firstScan = true;
      }
    }

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
	
    double minPosit;
    double maxPosit;
    switch ( direction) {
    case Ge.DIRECTION_RIGHT:
	minPosit = this.minPos * width / original_width;
	maxPosit = this.maxPos * width / original_width;
	pos = (int)((maxValue - value)/(maxValue - minValue) *
		    (maxPosit - minPosit) + minPosit);
	if ( pos < minPosit)
	  pos = (int)minPosit;
	if ( pos > maxPosit)
	  pos = (int)maxPosit;
	loc.x = pos;
	break;
    case Ge.DIRECTION_LEFT:
	minPosit = this.minPos * width / original_width;
	maxPosit = this.maxPos * width / original_width;
	pos = (int)(value /(maxValue - minValue) *
		    (maxPosit - minPosit) + minPosit);
	if ( pos < minPosit)
	  pos = (int)minPosit;
	if ( pos > maxPosit)
	  pos = (int)maxPosit;
	loc.x = pos;
	break;
    case Ge.DIRECTION_UP:
	minPosit = this.minPos * height / original_height;
	maxPosit = this.maxPos * height / original_height;
	pos = (int)((value - minValue)/(maxValue - minValue) *
		    (maxPosit - minPosit) + minPosit);
	if ( pos < minPosit)
	  pos = (int)minPosit;
	if ( pos > maxPosit)
	  pos = (int)maxPosit;
	loc.y = pos;
	break;
    default:
	minPosit = this.minPos * height / original_height;
	maxPosit = this.maxPos * height / original_height;
	pos = (int)((maxValue - value)/(maxValue - minValue) *
		    (maxPosit - minPosit) + minPosit);
	if ( pos < minPosit)
	  pos = (int)minPosit;
	if ( pos > maxPosit)
	  pos = (int)maxPosit;
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
      if ( insensitiveP != 0) {	  
	boolean insensitive;
	if ( !insensitiveLocalDb)
	  insensitive = dyn.en.gdh.getObjectRefInfoBoolean( insensitiveP);
	else
	  insensitive = dyn.en.ldb.getObjectRefInfoBoolean( insensitiveP);
      if ( (insensitiveInverted && !insensitive) || (!insensitiveInverted && insensitive))
	  return;
      }
      float value;
      double minPosit;
      double maxPosit;
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
	minPosit = this.minPos * width / original_width;
	maxPosit = this.maxPos * width / original_width;
	new_loc.x = loc.x + ePoint.x - offset.x;
	new_loc.y = loc.y;
	if ( new_loc.x > maxPosit)
	  new_loc.x = (int) maxPosit;
	if ( new_loc.x < minPosit)
	  new_loc.x = (int) minPosit;
	value = (float)((maxPosit - new_loc.x) / (maxPosit - minPosit) *
	        	(maxValue - minValue) + minValue); 
	break;
      case Ge.DIRECTION_LEFT:
	minPosit = this.minPos * width / original_width;
	maxPosit = this.maxPos * width / original_width;
	new_loc.x = loc.x + ePoint.x - offset.x;
	new_loc.y = loc.y;
	if ( new_loc.x > maxPosit)
	  new_loc.x = (int) maxPosit;
	if ( new_loc.x < minPosit)
	  new_loc.x = (int) minPosit;
	value = (float)((new_loc.x - minPosit) / (maxPosit - minPosit) * 
	        	(maxValue - minValue) + minValue);
	break;
      case Ge.DIRECTION_UP:
	minPosit = this.minPos * height / original_height;
	maxPosit = this.maxPos * height / original_height;
	new_loc.y = loc.y + ePoint.y - offset.y;
	new_loc.x = loc.x;
	if ( new_loc.y > maxPosit)
	  new_loc.y = (int) maxPosit;
	if ( new_loc.y < minPosit)
          new_loc.y = (int) minPosit;
	value = (float)((new_loc.y - minPosit) / (maxPosit - minPosit) *
			(maxValue - minValue) + minValue);
	// System.out.println("old_y: " + ePoint.y + " new_y: " + new_loc.y + "v: " + value);
	break;
      default:
	minPosit = this.minPos * height / original_height;
	maxPosit = this.maxPos * height / original_height;
	  new_loc.y = loc.y + ePoint.y - offset.y;
	  // System.out.println( "loc.y " + loc.y + " eP.y " + ePoint.y + " offset.y " + offset.y + " new_loc.y " + new_loc.y + " maxPos " + maxPosit + " minPos " + minPosit);
	  new_loc.x = loc.x;
	  if ( new_loc.y > maxPosit)
	    new_loc.y = (int) maxPosit;
	  if ( new_loc.y < minPosit)
	    new_loc.y = (int) minPosit;
	  value = (float)((maxPosit - new_loc.y) / (maxPosit - minPosit) *
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













