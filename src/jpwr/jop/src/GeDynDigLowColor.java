/* 
 * Proview   $Id: GeDynDigLowColor.java,v 1.3 2005-09-01 14:57:50 claes Exp $
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

public class GeDynDigLowColor extends GeDynElem {
  String attribute;
  int color;

  boolean attrFound;
  PwrtRefId subid;
  int p;
  boolean inverted;
  boolean oldValue;
  boolean firstScan = true;
  boolean localDb = false;

  public GeDynDigLowColor( GeDyn dyn, String attribute, int color) {
    super( dyn, GeDyn.mDynType_DigLowColor, GeDyn.mActionType_No);
    this.attribute = attribute;
    this.color = color;
  }
  public void connect() {
    String attrName = dyn.getAttrName( attribute);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret;
      localDb = dyn.isLocalDb(attrName);
      if ( !localDb)
	ret = dyn.en.gdh.refObjectInfo( attrName);
      else
	  ret = dyn.en.ldb.refObjectInfo( dyn.comp.dynamicGetRoot(), attrName);
      if ( ret.evenSts())
	System.out.println( "DigLowColor: " + attrName);
      else {
	attrFound = true;
	p = ret.id;
	subid = ret.refid;
	inverted = GeDyndata.getAttrInverted( attribute);
      }
    }
  }
  public void disconnect() {
    if ( attrFound && !localDb)
      dyn.en.gdh.unrefObjectInfo( subid);
    System.out.println("Disconnecting: " + attribute);
  }
  public void scan() {
    if ( !attrFound || dyn.ignoreColor)
      return;
    boolean value;
    if ( !localDb)
      value = dyn.en.gdh.getObjectRefInfoBoolean( p);
    else
      value = dyn.en.ldb.getObjectRefInfoBoolean( p);
    if ( !firstScan) {
      if ( oldValue == value && !dyn.resetColor)
	return;
    }
    else
      firstScan = false;
	
    if ( (dyn.dynType & GeDyn.mDynType_Tone) != 0) {
      if ( (!inverted && !value) || (inverted && value)) {
	if ( color <= GeColor.COLOR_TONE_MAX)
	  dyn.comp.tsetColorTone( color);
	else
 	  dyn.comp.tsetFillColor( color);
      }
      else {
	dyn.comp.resetFillColor();
	dyn.comp.resetColorTone();
      }
      dyn.repaintNow = true;
    }
    else {
      if ( (!inverted && !value) || (inverted && value))
	dyn.comp.tsetFillColor( color);
      else
	dyn.comp.resetFillColor();
      dyn.repaintNow = true;
    }
    oldValue = value;
  }
}














