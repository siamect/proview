/* 
 * Proview   $Id: GeDynAnalogShift.java,v 1.2 2005-09-01 14:57:50 claes Exp $
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

public class GeDynAnalogShift extends GeDynElem {
  String attribute;

  boolean attrFound;
  PwrtRefId subid;
  int p;
  float oldValue;
  boolean firstScan = true;

  public GeDynAnalogShift( GeDyn dyn, String attribute) {
    super( dyn, GeDyn.mDynType_AnalogShift, GeDyn.mActionType_No);
    this.attribute = attribute;
  }
  public void connect() {
    String attrName = dyn.getAttrName( attribute);
    if ( attrName.compareTo("") != 0) {
      GdhrRefObjectInfo ret = dyn.en.gdh.refObjectInfo( attrName);
      if ( ret.evenSts())
	System.out.println( "AnalogShift: " + attrName);
      else {
	attrFound = true;
	p = ret.id;
	subid = ret.refid;
      }
    }
  }
  public void disconnect() {
    if ( attrFound)
      dyn.en.gdh.unrefObjectInfo( subid);
  }
  public void scan() {
    if ( !attrFound)
      return;

    float value0 = dyn.en.gdh.getObjectRefInfoFloat( p);
    if ( value0 != oldValue  || firstScan) {
      dyn.comp.setPage( (int)(value0 + 1.5));
      dyn.repaintNow = true;
      oldValue = value0;
    }
    if ( firstScan)
      firstScan = false;
  }
}







