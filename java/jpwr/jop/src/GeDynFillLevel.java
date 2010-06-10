/* 
 * Proview   $Id: GeDynFillLevel.java,v 1.3 2005-11-02 14:00:10 claes Exp $
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
import javax.swing.*;

public class GeDynFillLevel extends GeDynElem {
  String attribute;
  int color;
  int direction;
  double minValue;
  double maxValue;
  double minPos;
  double maxPos;
  String minValueAttr;
  String maxValueAttr;

  boolean attrFound;
  PwrtRefId subid;
  int p;
  float oldValue;
  boolean firstScan = true;
  int minValueP;
  int maxValueP;
  PwrtRefId minValueSubid;
  PwrtRefId maxValueSubid;

  public GeDynFillLevel( GeDyn dyn, String attribute, int color, int direction,
			 double minValue, double maxValue, double minPos, double maxPos,
			 String minValueAttr, String maxValueAttr) {
    super( dyn, GeDyn.mDynType_FillLevel, GeDyn.mActionType_No);
    this.attribute = attribute;
    this.color = color;
    this.direction = direction;
    this.minValue = minValue;
    this.maxValue = maxValue;
    this.minPos = minPos;
    this.maxPos = maxPos;
    this.minValueAttr = minValueAttr;
    this.maxValueAttr = maxValueAttr;
  }
  public void connect() {
    if ( minValue == maxValue && minValueAttr == null && maxValueAttr == null)
      return;

    String attrName = dyn.getAttrName( attribute);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "FillLevel: " + attrName);
      else {
	attrFound = true;
	p = ret.id;
	subid = ret.refid;
	if ( maxPos == 0 && minPos == 0)
	  dyn.comp.setLevelDirection( direction);
	else
	  dyn.comp.setLevelDirection( Ge.DIRECTION_DOWN);
	if ( (dyn.dynType & GeDyn.mDynType_Tone) != 0) {
	  if ( color <= GeColor.COLOR_TONE_MAX)
	    dyn.comp.setLevelColorTone( color);
	  else
	    dyn.comp.setLevelFillColor( color);
	}
	else
	  dyn.comp.setLevelFillColor( color);
      }
    }
    minValueP = 0;
    if ( minValueAttr != null) {
      attrName = dyn.getAttrName( minValueAttr);
      if ( attrName.compareTo("") != 0) {
	GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
	if ( ret.evenSts())
	  System.out.println( "FillLevel: " + attrName);
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
	  System.out.println( "FillLevel: " + attrName);
	else {
	  maxValueP = ret.id;
	  maxValueSubid = ret.refid;
	}
      }
    }
    System.out.println("FillLevel minPos: " + minPos + " MaxPos: " + maxPos);
  }
  public void disconnect() {
    if ( attrFound)
      dyn.en.gdh.unrefObjectInfo( subid);
    if ( minValueP != 0)
      dyn.en.gdh.unrefObjectInfo( minValueSubid);
    if ( maxValueP != 0)
      dyn.en.gdh.unrefObjectInfo( maxValueSubid);
  }
  public void scan() {
    if ( !attrFound)
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

    float value = dyn.en.gdh.getObjectRefInfoFloat( p);
    if ( !firstScan) {
      if ( value == oldValue)
	// No change since last time
	return;
    }
    else
      firstScan = false;

    float level = 1;
    if ( maxPos == 0 && minPos == 0) {
      level = (float)((value - minValue) / (maxValue - minValue));
    }
    else {
      switch( direction) {
      case Ge.DIRECTION_LEFT:
      case Ge.DIRECTION_RIGHT:
	float width = ((JComponent)dyn.comp).getWidth();
	level = (float)(((value - minValue) / (maxValue - minValue) * ( maxPos - minPos)
			+ width - maxPos) / width);
	break;
      case Ge.DIRECTION_DOWN:
      case Ge.DIRECTION_UP:
	float height = ((JComponent)dyn.comp).getHeight();
	level = (float)(((value - minValue) / (maxValue - minValue) * ( maxPos - minPos)
			+ height - maxPos) / height);
	break;
      }
    }
    dyn.comp.setFillLevel( level);
    dyn.repaintNow = true;
    oldValue = value;
  }
}









