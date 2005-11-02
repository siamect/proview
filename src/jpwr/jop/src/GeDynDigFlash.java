/* 
 * Proview   $Id: GeDynDigFlash.java,v 1.3 2005-11-02 14:02:18 claes Exp $
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

public class GeDynDigFlash extends GeDynElem {
  String attribute;
  int color;
  int color2;

  boolean attrFound;
  PwrtRefId subid;
  int p;
  boolean inverted;
  boolean oldValue;
  boolean firstScan = true;
  boolean on;

  public GeDynDigFlash( GeDyn dyn, String attribute, int color, int color2) {
    super( dyn, GeDyn.mDynType_DigFlash, GeDyn.mActionType_No);
    this.attribute = attribute;
    this.color = color;
    this.color2 = color2;
  }
  public void connect() {
    String attrName = dyn.getAttrName( attribute);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "DigColor: " + attrName);
      else {
	attrFound = true;
	p = ret.id;
	subid = ret.refid;
	inverted = GeDyndata.getAttrInverted( attribute);
      }
    }
  }
  public void disconnect() {
    if ( attrFound)
      dyn.en.gdh.unrefObjectInfo( subid);
  }
  public void scan() {
    if ( !attrFound || dyn.ignoreColor)
      return;
    boolean value = dyn.en.gdh.getObjectRefInfoBoolean( p);
    if ( !firstScan) {
      if ( value == oldValue && !dyn.resetColor) {
	// No change since last time
	if ( !((!inverted && value) || (inverted && !value)))
	  return;
      }
    }
    else
      firstScan = false;

    if ( (dyn.dynType & GeDyn.mDynType_Tone) != 0) {
      if ( (!inverted && value) || (inverted && !value)) {
	if ( on) {
	  if ( color <= GeColor.COLOR_TONE_MAX)
 	    dyn.comp.tsetColorTone( color);
	  else
 	    dyn.comp.tsetFillColor( color);
	  dyn.ignoreColor = true;
        }
	else {
	  dyn.comp.resetFillColor();
	  dyn.comp.resetColorTone();
	  dyn.resetColor = true;
	}
	on = !on;
      }
      else {
	dyn.comp.resetFillColor();
	dyn.comp.resetColorTone();
	dyn.resetColor = true;
      }
      dyn.repaintNow = true;
    }
    else {
      if ( (!inverted && value) || (inverted && !value)) {
	if ( on) {
	  dyn.comp.tsetFillColor( color);
	  dyn.ignoreColor = true;
        }
        else {
	  if ( color2 == GeColor.COLOR_INHERIT) {
	    dyn.comp.resetFillColor();
	    dyn.resetColor = true;
	  }
	  else {
	    dyn.comp.tsetFillColor( color2);
	    dyn.ignoreColor = true;
	  }
        }
	on = !on;
      }
      else {
	dyn.comp.resetFillColor();
        dyn.resetColor = true;
      }
      dyn.repaintNow = true;
    }
    oldValue = value;
  }
}
